#include <iostream>
#include <filesystem>
#include <fstream>  // Include for file handling
#include "my_vcs.h"

void init() {
    // Check if the .vcs directory exists
    if (!std::filesystem::exists(".vcs")) {
        // Create the .vcs directory and its subdirectories
        std::filesystem::create_directory(".vcs");

        std::cout << "Directory structure '.vcs' created in the directory: " 
                  << std::filesystem::current_path() << std::endl;
        std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
    } else {
        std::cout << "'.vcs' directory already exists." << std::endl;
    }

    return;
}
