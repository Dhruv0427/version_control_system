#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "my_vcs.h"

const std::string STAGING_DIR = ".vcs/staging";
const std::string BLOBS_DIR = ".vcs/blobs";
const std::string INDEX_FILE = STAGING_DIR + "/index.txt";  



void add(const std::string& filename) {
    if (!std::filesystem::exists(STAGING_DIR)) {
        std::filesystem::create_directory(STAGING_DIR);
    }
    if (!std::filesystem::exists(BLOBS_DIR)) {
        std::filesystem::create_directory(BLOBS_DIR);
    }

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "File " << filename << " does not exist or cannot be opened!" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_content = buffer.str();

    std::string file_hash = calculate_sha1_hash(file_content);
    std::string blob_path = BLOBS_DIR + "/" + file_hash;

    if (!std::filesystem::exists(blob_path)) {
        std::ofstream blob_file(blob_path, std::ios::binary);
        blob_file << file_content;
        blob_file.close();
        std::cout << "Blob created for file: " << filename << " with hash: " << file_hash << std::endl;
    }

    std::ofstream index_file(INDEX_FILE, std::ios::app);
    if (!index_file) {
        std::cerr << "Error opening index file in the staging area!" << std::endl;
        return;
    }

    index_file << file_hash << " " << filename << std::endl;
    std::cout << "File " << filename << " is staged with blob hash: " << file_hash << std::endl;

    index_file.close();
    file.close();
}
