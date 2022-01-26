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
    
    avformat_close_input(&inputFormatContext);

    if (outputFormatContext && !(outputFormatContext->oformat->flags & AVFMT_NOFILE))
        avio_close(outputFormatContext->pb);
    avformat_free_context(outputFormatContext);
}

int Video::init()
{
    if (avformat_open_input(&inputFormatContext, std::string("file:" + inputFilePath).c_str(), NULL, NULL) != 0)
    {
        fprintf(stderr, "Failed to open input file %s\n", inputFilePath.c_str());
        return -1;
    }
    if (avformat_find_stream_info(inputFormatContext, NULL) < 0)
    {
        fprintf(stderr, "Failed to find stream info\n");
        return -1;
    }

    av_dump_format(inputFormatContext, 0, inputFilePath.c_str(), 0);

    for (unsigned i=0; i<inputFormatContext->nb_streams; i++)
        if (inputFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) // TODO: ->codec is deprecated
        {
            inputVideoStream = inputFormatContext->streams[i];
            break;
        }

    if (inputVideoStream == NULL)
    {
        fprintf(stderr, "Failed to find a video stream\n");
        return -1;
    }

    return 0;
}

void Video::print_file_name()
{
    std::cout << inputFilePath << std::endl;
}

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

int Video::write_to(std::string outputFilePath)
{
    // http://www.ffmpeg.org/doxygen/trunk/doc_2examples_2remuxing_8c-example.html with adjustments following deprecations

    std::cout << "Writing video out to: " << outputFilePath << std::endl;

    avformat_alloc_output_context2(&outputFormatContext, NULL, NULL, outputFilePath.c_str());
    if (!outputFormatContext) {
        fprintf(stderr, "Failed to allocate output context\n");
        return -1;
    }


    for (unsigned i = 0; i < inputFormatContext->nb_streams; i++) {
        AVStream *inputStream = inputFormatContext->streams[i];
        AVStream *outputStream = avformat_new_stream(outputFormatContext, inputStream->codec->codec);
        if (!outputStream) {
            fprintf(stderr, "Failed to create output stream\n");
            return -1;
        }

        AVCodecParameters *codecParams = avcodec_parameters_alloc();
        if (
            avcodec_parameters_from_context(codecParams, inputStream->codec) < 0 ||
            avcodec_parameters_to_context(outputStream->codec, codecParams) < 0
        ) {
            fprintf(stderr, "Failed to copy codec parameters\n");
            avcodec_parameters_free(&codecParams);
            return -1;
        }
        avcodec_parameters_free(&codecParams);

        if (outputFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
            outputStream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    av_dump_format(outputFormatContext, 0, outputFilePath.c_str(), 1);

    if (!(outputFormatContext->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&outputFormatContext->pb, outputFilePath.c_str(), AVIO_FLAG_WRITE) < 0) {
            fprintf(stderr, "Failed to open output file '%s'", outputFilePath.c_str());
            return -1;
        }
    }

    if (avformat_write_header(outputFormatContext, NULL) < 0) {
        fprintf(stderr, "Failed to write output file header\n");
        return -1;
    }

    AVPacket pkt;
    while (1) {
        AVStream *inputStream, *outputStream;
        if (av_read_frame(inputFormatContext, &pkt) < 0)
            break;
        inputStream  = inputFormatContext->streams[pkt.stream_index];
        outputStream = outputFormatContext->streams[pkt.stream_index];
        // log_packet(inputFormatContext, &pkt, "in ");
        /* copy packet */
        pkt.pts = av_rescale_q_rnd(pkt.pts, inputStream->time_base, outputStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, inputStream->time_base, outputStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, inputStream->time_base, outputStream->time_base);
        pkt.pos = -1;
        // log_packet(outputFormatContext, &pkt, "out");
        if (av_interleaved_write_frame(outputFormatContext, &pkt) < 0) {
            fprintf(stderr, "Failed to write packet to output file\n");
            break;
        }
        av_free_packet(&pkt);
    }
    std::cout << std::endl;
    av_write_trailer(outputFormatContext);

    return 0;
}