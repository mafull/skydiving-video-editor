#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

#include "Video.hpp"

namespace fs = std::filesystem;

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
