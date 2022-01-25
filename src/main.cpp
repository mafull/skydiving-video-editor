#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

// Prevent C++ name-mangling - unfortunately, FFMpeg libraries don't use an extern "C" {} themselves
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

namespace fs = std::filesystem;

class Video {
private:
    const std::string filePath;
    AVFormatContext *i_fmt_ctx = NULL;
    AVStream *i_video_stream = NULL;

public:
    Video(std::string filePath):
        filePath(filePath)
    {
        std::cout << "Creating video instance for file: " << filePath << std::endl;
    }
    ~Video() {
        avformat_close_input(&i_fmt_ctx);
    }
    
    int init() {
        if (avformat_open_input(&i_fmt_ctx, std::string("file:" + filePath).c_str(), NULL, NULL) != 0)
        {
            fprintf(stderr, "Failed to open input file %s\n", filePath.c_str());
            return -1;
        }
        if (avformat_find_stream_info(i_fmt_ctx, NULL) < 0)
        {
            fprintf(stderr, "could not find stream info\n");
            return -1;
        }

        for (unsigned i=0; i<i_fmt_ctx->nb_streams; i++)
            if (i_fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                i_video_stream = i_fmt_ctx->streams[i];
                break;
            }

        if (i_video_stream == NULL)
        {
            fprintf(stderr, "didn't find any video stream\n");
            return -1;
        }

        return 0;
    }

    void print_file_name() {
        std::cout << filePath << std::endl;
    }
};

/**
 * Given a directory path and a file extension, find all files in the directory with that extension
 * and return a list of their file paths.
 */
std::vector<std::string> list_files_of_extension(std::string path, std::string extension)
{
    std::vector<std::string> files;
    std::cout << "Listing " << extension << " files in '" << path << "'" << std::endl;
    for (const auto& file : fs::directory_iterator(path)) {
        if (file.path().extension() == extension) {
            std::cout << "\t" << file.path() << std::endl;
            files.push_back(file.path());
        }
    }
    std::cout << std::endl;
    return files;
}

int main(int argc, const char* argv[])
{
    // Parse CLI arguments
    if (argc < 2) {
        std::cerr << "Usage: ./run <directory>" << std::endl;
        return -1;
    }
    const auto directory = std::string(argv[1]);

    // Get all .mp4 file names in the given directory
    auto mp4Files = list_files_of_extension(directory, ".mp4");
    if (mp4Files.size() < 1) {
        std::cerr << "No .mp4 files in '" << directory << "'" << std::endl;
        return -1;
    }

    // Open the first file
    auto video2 = Video(mp4Files[0]);
    
    // TODO: do stuff with it!

    return 0;
}
