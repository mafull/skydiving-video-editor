#include "Video.hpp"

#include <libavutil/timestamp.h>


Video::Video(std::string inputFilePath):
    inputFilePath(inputFilePath)
{
    std::cout << "Constructing video instance for file: " << inputFilePath << std::endl;
}

Video::~Video()
{
    std::cout << "Destroying video instance for file: " << inputFilePath << std::endl;
    
    // Clean up FFmpeg allocations/open file handles
    avformat_close_input(&inputFormatContext);
    if (outputFormatContext && !(outputFormatContext->oformat->flags & AVFMT_NOFILE))
        avio_close(outputFormatContext->pb);
    avformat_free_context(outputFormatContext);
}

int Video::init()
{
    // Attempt to open the input file, read its header, and extract information about the file format
    // Note: This allocates the context for us ("format context" essentially means "file info")
    if (avformat_open_input(&inputFormatContext, std::string("file:" + inputFilePath).c_str(), NULL, NULL) < 0) {
        fprintf(stderr, "Failed to open input file %s\n", inputFilePath.c_str());
        return -1;
    }
    // Try to read and decode a few frames to fill in any gaps in the stream info in the file header
    if (avformat_find_stream_info(inputFormatContext, NULL) < 0) {
        fprintf(stderr, "Failed to find stream info\n");
        return -1;
    }

    // Print some information about the file to aid debugging
    av_dump_format(inputFormatContext, 0, inputFilePath.c_str(), 0);

    // Check that the file contains a video stream
    AVStream *videoStream = NULL;
    for (unsigned int i = 0; i < inputFormatContext->nb_streams; i++) {
        if (inputFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = inputFormatContext->streams[i];
            break;
        }
    }
    if (videoStream == NULL) {
        fprintf(stderr, "Failed to find a video stream\n");
        return -1;
    }

    return 0;
}

void Video::print_file_name()
{
    std::cout << inputFilePath << std::endl;
}

/**
 * Log a video data packet, for debug purposes.
 * 
 * Copied from http://www.ffmpeg.org/doxygen/trunk/doc_2examples_2remuxing_8c-example.html
 */
static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

int Video::write_to(std::string outputUrl)
{
    /*
     * Approach largely copied from https://ffmpeg.org/doxygen/4.1/remuxing_8c-example.html
     * Personally documented with code comments for greater clarity and understanding.
     */

    std::cout << "Writing video out to: " << outputUrl << std::endl;

    // Allocate output file struct and copy file context over from the input file
    if (avformat_alloc_output_context2(&outputFormatContext, NULL, NULL, outputUrl.c_str()) < 0) {
        fprintf(stderr, "Failed to allocate output context\n");
        return -1;
    }

    // For each input file stream, create a new stream in the output file context and copy over the codec parameters
    for (unsigned int i = 0; i < inputFormatContext->nb_streams; i++) {
        AVStream *outputStream = avformat_new_stream(outputFormatContext, NULL);
        if (!outputStream) {
            fprintf(stderr, "Failed to allocate output stream\n");
            return -1;
        }

        AVStream *inputStream = inputFormatContext->streams[i];
        if (avcodec_parameters_copy(outputStream->codecpar, inputStream->codecpar) < 0) {
            fprintf(stderr, "Failed to copy codec parameters\n");
            return -1;
        }
    }

    // Print some information about the output file format to aid debugging
    av_dump_format(outputFormatContext, 0, outputUrl.c_str(), 1);

    // If the output file already exists, attempt to open it, setting up the IO context for writing
    if (!(outputFormatContext->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&outputFormatContext->pb, outputUrl.c_str(), AVIO_FLAG_WRITE) < 0) {
            fprintf(stderr, "Failed to open output file '%s'", outputUrl.c_str());
            return -1;
        }
    }

    // Write the output file header
    if (avformat_write_header(outputFormatContext, NULL) < 0) {
        fprintf(stderr, "Failed to write output file header\n");
        return -1;
    }

    // Copy across the file data, one packet at a time
    AVPacket pkt;
    while (1) {
        AVStream *inputStream, *outputStream;

        // Get the next frame from the input file
        if (av_read_frame(inputFormatContext, &pkt) < 0) break;
        // log_packet(inputFormatContext, &pkt, "in ");

        // Get the relevant input and output streams
        inputStream  = inputFormatContext->streams[pkt.stream_index];
        outputStream = outputFormatContext->streams[pkt.stream_index];

        // Ensure timing properties pts, dts, and duration are scaled appropriately for the output time base
        pkt.pts = av_rescale_q_rnd(pkt.pts, inputStream->time_base, outputStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, inputStream->time_base, outputStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, inputStream->time_base, outputStream->time_base);
        pkt.pos = -1; // Unknown byte position in the stream
        // log_packet(outputFormatContext, &pkt, "out");

        // Write the frame to the output file
        if (av_interleaved_write_frame(outputFormatContext, &pkt) < 0) {
            fprintf(stderr, "Failed to write packet to output file\n");
            break;
        }

        // Finally, free the packet struct used for copying
        av_packet_unref(&pkt);
    }
    std::cout << std::endl;

    // Flush any buffered packets and finalise the output file
    av_write_trailer(outputFormatContext);

    // Note: File stream is closed and muxing context cleared in the destructor - this catches any failures above

    return 0;
}