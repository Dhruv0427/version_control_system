#include<filesystem>
#include<iostream>
#include<cstring>
#include "my_vcs.h"

const std::string STAGING_DIR = ".vcs/staging";

void add(const std::string& filename){

  if(!std::filesystem::exists(STAGING_DIR)){
      std::filesystem::create_directory(STAGING_DIR);
   }
   std::filesystem::path stagefile = STAGING_DIR + "/" + filename;

   try{
     std::filesystem::copy_file(filename,stagefile,std::filesystem::copy_options::overwrite_existing);
     std::cout<<"File "<<filename<<" is staged "<<std::endl;
   }
   catch(std::filesystem::filesystem_error &e){
     std::cout<<"File "<<filename<<" do not exist or following error has occured"<<e.what()<<std::endl;
   }

   return;
}
