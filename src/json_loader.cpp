#include "json_loader.hpp"
#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

using json = nlohmann::json;

// Determines the file to be included
// depending on the OS
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

// Find and open the given file eg: "level_1"
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
        // Add the path depending on the OS
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

// Load the level from the given json
void load_level(json j){
    for(json entity: j["entities"]){

        // ---------------------------- COMPANIONS ----------------------------
        if(entity["type"] == "Companion"){
            // Mage
            if(entity["skill"] == "Mage"){
                printf("Loading the player mage\n");
                player_mage = createPlayerMage(renderer, vec2(entity["position"]["x"], 
                                                                entity["position"]["y"]));
            } 
            // Swordsman
            else if(entity["skill"] == "Swordsman"){
                printf("Loading the player swordsman\n");
                player_swordsman = createPlayerSwordsman(renderer, vec2(entity["position"]["x"], 
                                                                        entity["position"]["y"]));
            }
            else {
                printf("Given companion does not exist\n");
            }
        } 
        
        // ---------------------------- ENEMIES ----------------------------
        else if(entity["type"] == "Enemy"){
            // Mage
            if(entity["skill"] == "Mage"){
                printf("Loading the enemy mage\n");
                enemy_mage = createEnemyMage(renderer, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
                for(json component: entity["components"]){
                    if(component["type"] == "colors"){
                        registry.colors.insert(enemy_mage, vec3(component["vec"]["x"],
                                                                component["vec"]["y"],
                                                                component["vec"]["z"]));
                                                                
                    }
                }
            } 
            // Swordsman
            else if(entity["skill"] == "Swordsman"){
                printf("Creating a swordsman\n");
                enemy_swordsman = createEnemySwordsman(renderer, vec2(entity["position"]["x"], 
                                                                        entity["position"]["y"]));
                for(json component: entity["components"]){
                    if(component["type"] == "colors"){
                        registry.colors.insert(enemy_swordsman, vec3(component["vec"]["x"],
                                                                    component["vec"]["y"],
                                                                    component["vec"]["z"]));
                                                                
                    }
                }
            }
            else {
                printf("Given enemy does not exist\n");
            }
        }
        
        // ---------------------------- ICONS ----------------------------
        else if(entity["type"] == "Icon"){
            // Fireball
            if(entity["skill"] == "Fireball"){
                printf("Loading the fireball icon\n");
                fireBall_icon = createFireballIcon(renderer, vec2(entity["position"]["x"], 
                                                                    entity["position"]["y"]));
            }
            // Taunt
            else if(entity["skill"] == "Taunt"){
                printf("Loading the taunt icon\n");
                taunt_icon = createTauntIcon(renderer, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
            }
            // Heal
            else if(entity["skill"] == "Heal"){
                printf("Loading the heal icon\n");
                heal_icon = createHealIcon(renderer, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
            }
            // Melee
            else if(entity["skill"] == "Melee"){
                printf("Loading the melee icon\n");
                melee_icon = createMeleeIcon(renderer, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
            }
            // Ice Shard
            else if(entity["skill"] == "Iceshard"){
                printf("Loading the ice shard icon\n");
                iceShard_icon = createIceShardIcon(renderer, vec2(entity["position"]["x"], 
                                                                    entity["position"]["y"]));
            }
            // Rock
            else if(entity["skill"] == "Rock"){
                printf("Loading the rock icon\n");
                rock_icon = createRockIcon(renderer, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
            } 
            else {
                printf("Given icon does not exist\n");
            }
        }
        else {
            printf("Given entity type does not exist\n");
        }
    }

}

// Finds the level file and loads it
void JSONLoader::get_level(string file_name){
    std::ifstream jsonFile = get_file(file_name);
    if (jsonFile.is_open()){
        json j = json::parse(jsonFile);
        std::cout << j << std::endl;
        load_level(j);
	}
	else {printf("File not found\n");}
}