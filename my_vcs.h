#ifndef MY_VCS_H
#define MY_VCS_H

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

// Function declarations
void init();
void add(const std::string& filename);
void log();
void diff(std::string fileName);
void compareFiles(const std::string& currentFile, const std::string& prevFile); 
std::string readFile(const std::string& filePath);
void storeDelta(const std::string& currentFile, 
                const std::vector<std::string>& added, 
                const std::vector<std::pair<int, std::pair<std::string, std::string>>>& modified,  
                const std::vector<std::string>& deleted);
void displayDelta(const std::string &deltaFile);
std::string getNextDeltaFileName(const std::string &fileName);
void commit(std::string message);
void updateVersionCount(const std::string &fileName) ;
void restore();

#endif // MY_VCS_H
