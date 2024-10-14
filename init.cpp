#include <iostream>
#include <filesystem>
#include <fstream>  // Include for file handling
#include "my_vcs.h"

void init() {
    // Check if the .vcs directory exists
    if (!std::filesystem::exists(".vcs")) {
        // Create the .vcs directory and its subdirectories
        std::filesystem::create_directory(".vcs");
        std::filesystem::create_directory(".vcs/commits");
        std::filesystem::create_directory(".vcs/staging");
        std::filesystem::create_directory(".vcs/blobs");
        std::filesystem::create_directory(".vcs/trees");

        // Create the HEAD file and point it to the master branch
        std::ofstream head_file(".vcs/HEAD");
        head_file << "master";  // Set default branch to master
        head_file.close();

        std::cout << "Directory structure '.vcs' created in the directory: " 
                  << std::filesystem::current_path() << std::endl;
        std::cout << "HEAD file created, pointing to the default branch 'master'." << std::endl;
        std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
    } else {
        std::cout << "'.vcs' directory already exists." << std::endl;
    }

    return;
}
