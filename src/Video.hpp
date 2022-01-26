#pragma once

#include <iostream>
#include <string>

// Prevent C++ name-mangling - unfortunately, FFMpeg libraries don't use an extern "C" {} themselves
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

class Video {
public:
    Video(std::string inputFilePath);
    ~Video();
    
    int init();

    void print_file_name();
    int write_to(std::string outputFileName);

private:
    const std::string inputFilePath;
    AVFormatContext *inputFormatContext = NULL;
    AVFormatContext *outputFormatContext = NULL;
    AVStream *inputVideoStream = NULL;
};
