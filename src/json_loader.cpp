#include "json_loader.hpp"
#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

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

RenderSystem* renderer;

JSONLoader::JSONLoader() {

}

void JSONLoader::init(RenderSystem* renderer_arg){
    renderer = renderer_arg;
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
            try_file.append("\\..\\levels\\").append(file_name);
        #elif _WIN32
            try_file.append("\\..\\levels\\").append(file_name);
        #else
            try_file.append("/../levels/").append(file_name);
        #endif
        std::cout << try_file.c_str() << endl;
        std::ifstream jsonFile(try_file);
        if (jsonFile) {
            return jsonFile;
        }
        #ifdef _WIN64
            iterate_path.append("\\..");
        #elif _WIN32
            iterate_path.append("\\..");
        #else
            iterate_path.append("/..");
        #endif
    }
}

void load_level(json j){
    for(json entity: j["entities"]){
        if(entity["type"] == "Companion"){
            if(entity["skill"] == "Mage"){
                printf("Loading the player mage\n");
                player_mage = createPlayerMage(renderer, vec2(entity["position"]["x"], entity["position"]["y"]));
            } else if(entity["skill"] == "Swordsman"){
                printf("Loading the player swordsman\n");
                player_swordsman = createPlayerSwordsman(renderer, vec2(entity["position"]["x"], entity["position"]["y"]));
            }
        } else if(entity["type"] == "Enemy"){
            if(entity["skill"] == "Mage"){
                printf("Loading the enemy mage\n");
                enemy_mage = createEnemyMage(renderer, vec2(entity["position"]["x"], entity["position"]["y"]));
                for(json component: entity["components"]){
                    if(component["type"] == "colors"){
                        registry.colors.insert(enemy_mage, vec3(component["vec"]["x"],
                                                                component["vec"]["y"],
                                                                component["vec"]["z"]));
                                                                
                    }
                }
            } else if(entity["skill"] == "Swordsman"){
                printf("Creating a swordsman\n");
            }
        }
    }

}

void JSONLoader::get_level(string file_name){
    std::ifstream jsonFile = get_file(file_name);
    if (jsonFile.is_open()){
        json j = json::parse(jsonFile);
        std::cout << j << std::endl;
        load_level(j);
	}
	else {printf("File not found\n");}
}