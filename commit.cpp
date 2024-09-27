#include<iostream>
#include<ctime>
#include<string>
#include<fstream>
#include<filesystem>
#include "my_vcs.h"

const std::string STAGING_DIR = ".vcs/staging";
const std::string METADATA_FILE = ".vcs/commits/metadata.txt";


void commit(const std::string& message){
  std::filesystem::path vcsDir(".vcs/commits");

  if(!std::filesystem::exists(vcsDir)){
    std::filesystem::create_directory(vcsDir);
  }

  std::ofstream metadata(METADATA_FILE, std::ios::app);
    if (!metadata) {
        std::cerr << "Error opening metadata file!" << std::endl;
        return;
    }

  for(const auto& entry : std::filesystem::directory_iterator(STAGING_DIR)){
    std::string filename = entry.path().filename().string();

    std::time_t now = std::time(nullptr);

    std::string version_name = std::to_string(now)+"_"+filename;

    std::filesystem::copy_file(entry.path(),vcsDir / version_name,std::filesystem::copy_options::overwrite_existing);

    std::cout<<"commited: "<<version_name<<std::endl;

        // Write the commit message to the metadata file
    if (!message.empty()) {
            metadata << version_name << " - Message: " << message << std::endl;
            std::cout << " - Message: " << message;
    }
    else {
            metadata << version_name << " - Message: [No message]" << std::endl;
    }

   std::cout << std::endl; // Move to the next line


  }

  std::filesystem::remove_all(STAGING_DIR);
  std::filesystem::create_directory(STAGING_DIR);

  metadata.close();
  
  return;
}
 

