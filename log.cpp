#include <iostream>
#include <fstream>
#include <string>
#include "my_vcs.h"

const std::string METADATA_FILE = ".vcs/METADATA";

void log() {
    std::ifstream metadata(METADATA_FILE);

    if (!metadata) {
        std::cerr << "Error opening metadata file!" << std::endl;
        return;
    }

    std::string line;
    std::cout << "Commit History:\n";
    while (std::getline(metadata, line)) {
        std::cout << line << std::endl; // Output each line from the metadata file
    }

    metadata.close();
}

