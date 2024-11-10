#include <iostream>
#include <cstring>
#include <filesystem>
#include "my_vcs.h"

const std::string VCS_DIR = ".vcs";

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "No command provided. Start program by using the 'init' command." << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "init") {
        std::cout << "---------------------------------------------------------------------" << std::endl;
        std::cout << "                Initializing version control system ...               " << std::endl;
        std::cout << "---------------------------------------------------------------------" << std::endl;
        init();
        return 0;
    }

    if (!std::filesystem::exists(VCS_DIR)) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Error: Version control system not initialized." << std::endl;
        std::cout << "Please run 'init' first." << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return 1;
    }

    if (command == "add") {
        if (argc < 3) {
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "Please enter a correct filename." << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            return 1;
        }

        for (int i = 2; i < argc; i++) {
            std::string filename = argv[i];
            add(filename);
        }
    }

    else if (command == "commit") {
        std::string message;
        if (argc > 3 && std::string(argv[2]) == "-m") {
            message = argv[3];
        } else if (argc == 2 && std::string(argv[1]) == "commit") {
            message = "";
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

    else if (command == "restore") {
        restore();
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
