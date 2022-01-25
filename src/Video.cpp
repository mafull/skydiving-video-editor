#include "Video.hpp"

Video::Video(std::string filePath):
    filePath(filePath)
{
    std::cout << "Creating video instance for file: " << filePath << std::endl;
}

Video::~Video()
{
    avformat_close_input(&fileContext);
    std::cout << "Deleting video instance for file: " << filePath << std::endl;
}

int Video::init()
{
    if (avformat_open_input(&fileContext, std::string("file:" + filePath).c_str(), NULL, NULL) != 0)
    {
        fprintf(stderr, "Failed to open input file %s\n", filePath.c_str());
        return -1;
    }
    if (avformat_find_stream_info(fileContext, NULL) < 0)
    {
        fprintf(stderr, "could not find stream info\n");
        return -1;
    }

    for (unsigned i=0; i<fileContext->nb_streams; i++)
        if (fileContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) // TODO: ->codec is deprecated
        {
            videoStream = fileContext->streams[i];
            break;
        }

    if (videoStream == NULL)
    {
        fprintf(stderr, "didn't find any video stream\n");
        return -1;
    }

    return 0;
}

void Video::print_file_name()
{
    std::cout << filePath << std::endl;
}