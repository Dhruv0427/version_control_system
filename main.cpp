#include <iostream>
#include <cstring>
#include <filesystem>
#include "my_vcs.h"

const std::string VCS_DIR = ".vcs"; // Define the VCS directory to check if initialized

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "No command provided. Start program by using the 'init' command." << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return 1; // Exit the program
    }

    std::string command = argv[1];

    // Always allow the 'init' command to proceed
    if (command == "init") {
        std::cout << "---------------------------------------------------------------------" << std::endl;
        std::cout << "                Initializing version control system ...               " << std::endl;
        std::cout << "---------------------------------------------------------------------" << std::endl;
        init();
        return 0;
    }

    // For any other command, check if the VCS has been initialized
    if (!std::filesystem::exists(VCS_DIR)) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Error: Version control system not initialized." << std::endl;
        std::cout << "Please run 'init' first." << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return 1;
    }

    // Now check for other commands after initialization has been verified
    if (command == "add") {
        if (argc < 3) {
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "Please enter a correct filename." << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            return 1;
        }
        std::string filename = argv[2];
        add(filename);
    }
    else if (command == "commit") {
        std::string message;
        if (argc > 3 && std::string(argv[2]) == "-m") {
            message = argv[3]; // Take the message from the command-line argument
        } else if (argc == 2 && std::string(argv[1]) == "commit") {
            message = ""; // No message provided
        } else {
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "Usage: ./myvcs commit [-m \"commit message\"]" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            return 1;
        }
        commit(message);
    }
    else if (command == "log") {
        log();
    }
    else {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Invalid command." << std::endl;
        std::cout << "Please use 'init' to initialize VCS." << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return 1;
    }

    return 0;
}
