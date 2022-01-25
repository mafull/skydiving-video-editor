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
    Video(std::string filePath);
    ~Video();
    
    int init();

    void print_file_name();

private:
    const std::string filePath;
    AVFormatContext *fileContext = NULL;
    AVStream *videoStream = NULL;
};
