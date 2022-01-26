#include <algorithm>
#include <iostream>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

#include "Video.hpp"

namespace fs = std::filesystem;

/**
 * Given a directory path and a file extension, find all files in the directory with that extension
 * and return a list of their file paths, sorted by ascending file path asc.
 */
std::vector<std::string> list_files_of_extension(std::string path, std::string extension)
{
    // Get all files with the correct extension and add their to a set (which will order them)
    std::set<std::string> sortedFilePaths;
    for (const auto& file : fs::directory_iterator(path)) {
        if (file.path().extension() == extension)
            sortedFilePaths.insert(file.path());
    }

    // Print them in order
    std::cout << "Listing " << extension << " files in '" << path << "'" << std::endl;
    for (const auto path : sortedFilePaths) {
        std::cout << "  > " << path << std::endl;
    }
    std::cout << std::endl;
    
    // Copy over to a vector to return
    // (more efficient to do in the above for loop but it's nice to try new things like std::copy)
    std::vector<std::string> filePathsVec(sortedFilePaths.size());
    std::copy(sortedFilePaths.begin(), sortedFilePaths.end(), filePathsVec.begin());

    return filePathsVec;
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

    // Open the first file and copy it to the CWD
    auto video = Video(mp4Files[0]);
    video.init();
    video.write_to("./out.mp4");

    return 0;
}
