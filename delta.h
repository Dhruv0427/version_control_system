#ifndef DELTA_H
#define DELTA_H

#include <string>
#include <vector>
#include <utility>
#include <filesystem>

namespace fs = std::filesystem;

void storeDelta(const std::string &fileName,  
                const std::vector<std::string> &added, 
                const std::vector<std::pair<int, std::pair<std::string, std::string>>> &modified,  
                const std::vector<std::string> &deleted);

void displayDelta(const std::string &deltaFile);

std::string getNextDeltaFileName(const std::string &fileName);

#endif
