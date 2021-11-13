#include "json_loader.hpp"
using json = nlohmann::json;

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

// stlib
#include <cassert>
#include <sstream>
#include <iostream>
#include <filesystem>

using namespace std;

JSONLoader::JSONLoader() {

}


string get_path(string file_name){
    // https://www.tutorialspoint.com/find-out-the-current-working-directory-in-c-cplusplus
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    // https://java2blog.com/check-if-string-contains-substring-cpp/
    char* pos = buff;
    int index = 0;
    while((pos[0]!='0')&&(pos[1]!='8')){
        pos++;
        index++;
    }
    pos = pos + 2;
    index = index + 2;
    string src =  "/src/";
    string add_string = src.append(file_name);
    string new_path = "";
    for(int i = 0; i<index; i++){
        new_path = new_path+buff[i];
    }
    new_path = new_path.append(add_string);
    return new_path;
}

void JSONLoader::get_level(){
    string path = get_path("small_example.json");
    std::ifstream jsonFile(path);
    cout << "Current working directory: " << path << endl;
    if (jsonFile.is_open()){
        //std::cout << jsonFile.rdbuf();
	}
	else {printf("not open?\n");}
    json j = json::parse(jsonFile);
    std::cout << j << std::endl;
}