#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

const std::string COMMITS_DIR = ".vcs/commits";
const std::string BLOBS_DIR = ".vcs/blobs";
const std::string TREES_DIR = ".vcs/trees";

// Function to list all versions of a specific file
std::unordered_map<std::string, std::string> list_file_versions(const std::string& filename) {
    std::unordered_map<std::string, std::string> versions; // maps commit hash to blob hash
    std::ifstream metadata_file(COMMITS_DIR + "/metadata.txt");

    // Check if the metadata file opened successfully
    if (!metadata_file) {
        std::cerr << "Error: Unable to open metadata file." << std::endl;
        return versions; // Return empty map on error
    }

    std::string line;
    while (std::getline(metadata_file, line)) {
        std::istringstream iss(line);
        std::string commit_hash, tree_hash, message;

        // Extract commit hash and tree hash from the metadata
        iss >> commit_hash; // Read the commit hash
        iss >> message;     // Skip '-'
        iss >> message;     // Skip 'Tree:'
        iss >> tree_hash;   // Read the tree hash

        // Construct the path to the tree file
        std::string tree_file_path = TREES_DIR + "/" + tree_hash;
        std::ifstream tree_file(tree_file_path);

        // Check if the tree file opened successfully
        if (!tree_file) {
            std::cerr << "Error: Unable to open tree file: " << tree_file_path << std::endl;
            continue; // Skip this commit if the tree file is not accessible
        }

        std::string tree_line;
        // Read the tree file line by line
        while (std::getline(tree_file, tree_line)) {
            std::istringstream tree_iss(tree_line);
            std::string blob_hash;

            // Read the blob hash from the tree file
            tree_iss >> blob_hash;

            // Now go to the corresponding blob file using the blob hash
            std::ifstream blob_file(BLOBS_DIR + "/" + blob_hash);
            if (!blob_file) {
                std::cerr << "Error: Unable to open blob file: " << BLOBS_DIR + "/" + blob_hash << std::endl;
                continue;
            }

            std::string blob_line;
            std::string blob_filename;
            
            // Read the blob file to extract the filename
            if (std::getline(blob_file, blob_line)) {
                std::istringstream blob_iss(blob_line);
                std::string actual_blob_hash;
                blob_iss >> actual_blob_hash;  // Extract the actual blob hash
                blob_filename = blob_line.substr(actual_blob_hash.size() + 1);  // Extract the filename
                
                // If the file name matches the filename we're looking for
                if (blob_filename == filename) {
                    // Store the commit hash and blob hash
                    versions[commit_hash] = actual_blob_hash; // Correctly store the actual blob hash
                    break; // No need to continue once we find the matching file
                }
            }
            blob_file.close();
        }
        tree_file.close();
    }

    return versions; // Return the map of versions
}

// Function to restore a file from a specific version
void restore_file_version(const std::string& blob_hash, const std::string& filename) {
    // Open the blob file that contains the file content
    std::ifstream blob_file(BLOBS_DIR + "/" + blob_hash, std::ios::binary);
    if (!blob_file) {
        std::cerr << "Error: Unable to open blob file for hash: " << blob_hash << std::endl;
        return;
    }

    // Restore the file content
    std::ofstream output_file(filename, std::ios::binary);
    output_file << blob_file.rdbuf(); // Write the content of the blob to the file

    std::cout << "Restored " << filename << " to version with blob hash " << blob_hash << "." << std::endl;

    blob_file.close();
    output_file.close();
}

// Function to prompt the user to restore a specific file version
void restore_file(const std::string& filename) {
    auto versions = list_file_versions(filename);

    if (versions.empty()) {
        std::cerr << "No versions found for " << filename << "!" << std::endl;
        return;
    }

    // List all versions of the file
    std::cout << "Available versions for " << filename << ":" << std::endl;
    for (const auto& [commit_hash, blob_hash] : versions) {
        std::cout << "- Commit Hash: " << commit_hash << ", Blob Hash: " << blob_hash << std::endl;
    }

    // Ask user for which version to restore
    std::cout << "Enter the commit hash to restore: ";
    std::string selected_commit_hash;
    std::cin >> selected_commit_hash;

    // Restore the selected version
    if (versions.find(selected_commit_hash) != versions.end()) {
        restore_file_version(versions[selected_commit_hash], filename); // Use the blob hash associated with the commit hash
    } else {
        std::cerr << "Error: Invalid commit hash selected." << std::endl;
    }
}
