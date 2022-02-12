#pragma once

#include <iostream>
#include <string>

// Prevent C++ name-mangling - unfortunately, FFmpeg libraries don't use an extern "C" { ... } themselves
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

class Video {
public:
    Video(std::string inputFilePath);
    ~Video();
    
    /**
     * Initialise the Video object, inspecting the file and extracting context for later use.
     * 
     * @returns 0 if successful, else -1
     */
    int init(); // TODO either remove explicit init() function or throw exceptions from other methods if not initialised

    /**
     * Print the file name provided in the constructor. 
     */
    void print_file_name();

    /**
     * Write the video out to a file.
     * 
     * If the file already exists, it will be overwritten; else it will be created before writing.
     */
    int write_to(std::string outputUrl);

private:
    const std::string inputFilePath;
    AVFormatContext *inputFormatContext = NULL;
    AVFormatContext *outputFormatContext = NULL;
};
