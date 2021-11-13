#include "json_loader.hpp"
using json = nlohmann::json;

#ifdef _WIN64
#include <direct.h>
#define GetCurrentDir _getcwd
#elif _WIN32
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


std::ifstream get_file(string file_name){
    // https://www.tutorialspoint.com/find-out-the-current-working-directory-in-c-cplusplus
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );

    // Convert the char array to a string
    string curr_path;
    for(int i = 0; i<FILENAME_MAX; i++){
        if(buff[i] == '\0'){
            break;
        }
        curr_path = curr_path + buff[i];
    }

    // Get the full path to the json file
    string iterate_path = curr_path;
    while(true){
        string try_file = iterate_path;
        #ifdef _WIN64
            try_file.append("\\..\\src\\").append(file_name);
        #elif _WIN32
            try_file.append("\\..\\src\\").append(file_name);
        #else
            try_file.append("/../src/").append(file_name);
        #endif
        std::ifstream jsonFile(try_file);
        if (jsonFile) {
            return jsonFile;
        }
        #ifdef _WIN64
            iterate_path = iterate_path.append("\\..").append(file_name);
        #elif _WIN32
            iterate_path = iterate_path.append("\\..").append(file_name);
        #else
            iterate_path = iterate_path.append("/..").append(file_name);
        #endif
    }
}

void JSONLoader::get_level(){
    std::ifstream jsonFile = get_file("small_example.json");
    if (jsonFile.is_open()){
        // std::cout << "PRINTING"<< endl;
        // std::cout << jsonFile.rdbuf();
	}
	else {printf("not open?\n");}
    get_file("small_example.json");
    json j = json::parse(jsonFile);
    std::cout << j << std::endl;
}