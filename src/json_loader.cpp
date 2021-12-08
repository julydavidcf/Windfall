#include "json_loader.hpp"
#include "world_init.hpp"
#include "world_system.hpp"
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

RenderSystem* renderer_load;

JSONLoader::JSONLoader() {

}

void JSONLoader::init(RenderSystem* renderer_arg){
    renderer_load = renderer_arg;
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

void balanceHealthNumbers(int16 levelNum) {
    switch (levelNum) {
    case 0: {
        registry.player_archer_hp = 175;
        registry.enemy_mage_hp = 10;
        break;
    }
    case 1: {
        registry.enemy_mage_hp = 30;
        break;
    }
    case 2: {
        registry.player_mage_hp = 70;
        registry.enemy_swordsman_hp = 150;
        break;
    }
    case 3: {
        registry.player_swordsman_hp = 250;
        registry.necro_minion_health = 10;
        registry.necro_1_health = 160;
        registry.necro_2_health = 200;
        break;
    }
    default: break;
    }

    if (registry.HPBuff > 0) {
        for (int i = 0; i < registry.HPBuff; i++) {
            registry.player_archer_hp += 10;
            registry.player_mage_hp += 5;
            registry.swordsmanHPBuff++;
        }
        registry.HPBuff = 0;
    }

    if (registry.swordsmanHPBuff > 0 && levelNum == 3) {
        for (int i = 0; i < registry.swordsmanHPBuff; i++) {
            registry.player_swordsman_hp += 20;
        }
        registry.swordsmanHPBuff = 0;
    }

    if (registry.HPDebuff > 0) {
        if (levelNum == 2) {
            registry.player_archer_hp = registry.player_archer_hp - 15;
            registry.player_mage_hp = registry.player_mage_hp - 5;
        }
        else if (levelNum == 3) {
            registry.player_swordsman_hp = registry.player_swordsman_hp - 20;
        }
    }

}

// Load the level from the given json
void load_level(json j){
    int roundNum = 0;
    printf("Loading\n");
    Entity tempRound[10];
    if(!j["roundSize"].is_null()){
        printf("Loading round size\n");
        std::cout << j["roundSize"] << std::endl;
        std::vector<Entity> roundVec(j["roundSize"]);
    }
    if(!j["story"].is_null()){
        printf("Loading story\n");
        story = j["story"];
    }
    if(!j["isFreeRoam"].is_null()){
        printf("Loading free roam status\n");
        isFreeRoam = j["isFreeRoam"];
    }
    if(!j["freeRoamLevel"].is_null()){
        printf("Loading free roam level\n");
        freeRoamLevel = j["freeRoamLevel"];
    }
    if(!j["HPDebuff"].is_null()){
        printf("Loading hp decrease\n");
        registry.HPDebuff = j["HPDebuff"];
    }
    if(!j["HPBuff"].is_null()){
        printf("Loading hp increase\n");
        registry.HPBuff = j["HPBuff"];
    }
    if(!j["swordsmanHPBuff"].is_null()){
        printf("Loading swordsman hp buff\n");
        registry.swordsmanHPBuff = j["swordsmanHPBuff"];
    }

    if(!j["level"].is_null()){
        printf("Loading level\n");
        loadedLevel = j["level"];
        if(loadedLevel == 0){
            createBackground(renderer_load, { window_width_px / 2, window_height_px / 2 }, TUTORIAL);
        } else if(loadedLevel == 1){
            createBackground(renderer_load, { window_width_px / 2, window_height_px / 2 }, LEVEL_ONE);
        } else if(loadedLevel == 2){
            createBackground(renderer_load, { window_width_px / 2, window_height_px / 2 }, LEVEL_TWO);
        } else if(loadedLevel == 3){
            createBackground(renderer_load, { window_width_px / 2, window_height_px / 2 }, LEVEL_THREE);
        }
        balanceHealthNumbers(loadedLevel);
    }

    for(json entity: j["entities"]){
        Entity create_entity;
        // ---------------------------- COMPANIONS ----------------------------
        if(entity["type"] == "Companion"){
            // Mage
            if(entity["skill"] == "Mage"){
                printf("Loading the player mage\n");
                player_mage = createPlayerMage(renderer_load, vec2(entity["position"]["x"], 
                                                                entity["position"]["y"]));
                create_entity = player_mage;
            } 
            // Swordsman
            else if(entity["skill"] == "Swordsman"){
                printf("Loading the player swordsman\n");
                player_swordsman = createPlayerSwordsman(renderer_load, vec2(entity["position"]["x"], 
                                                                        entity["position"]["y"]));
                create_entity = player_swordsman;
            }
            // Archer
            else if(entity["skill"] == "Archer"){
                printf("Loading the player archer\n");
                player_archer = createPlayerArcher(renderer_load, vec2(entity["position"]["x"], 
                                                                        entity["position"]["y"]),
                                                                        isFreeRoam);
                create_entity = player_archer;
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
                enemy_mage = createEnemyMage(renderer_load, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
                create_entity = enemy_mage;

            } 
            // Swordsman
            else if(entity["skill"] == "Swordsman"){
                printf("Loading the enemy swordsman\n");
                enemy_swordsman = createEnemySwordsman(renderer_load, vec2(entity["position"]["x"], 
                                                                        entity["position"]["y"]));
                create_entity = enemy_swordsman;
                }
            // Necromancer1
            else if(entity["skill"] == "Necromancer1"){
                printf("Loading the phase 1 necromancer\n");
                necromancer_phase_one = createNecromancerPhaseOne(renderer_load, vec2(entity["position"]["x"], 
                                                                        entity["position"]["y"]));
                create_entity = necromancer_phase_one;
                }
            // Necromancer2
            else if(entity["skill"] == "Necromancer2"){
                printf("Loading the phase 2 necromancer\n");
                necromancer_phase_two = createNecromancerPhaseTwo(renderer_load, vec2(entity["position"]["x"], 
                                                                        entity["position"]["y"]));
                create_entity = necromancer_phase_two;
                if(registry.enemies.has(create_entity)){
                    printf("is an enemy\n");
                }if(registry.motions.has(create_entity)){
                    printf("has motion\n");
                }
                }
            
            // Minion
            else if(entity["skill"] == "Minion"){
                printf("Loading the necromancer minion\n");
                necromancer_minion = createNecromancerMinion(renderer_load, vec2(entity["position"]["x"], 
                                                                        entity["position"]["y"]));
                create_entity = necromancer_minion;
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
                fireBall_icon = createFireballIcon(renderer_load, vec2(entity["position"]["x"], 
                                                                    entity["position"]["y"]));
                create_entity = fireBall_icon;
            }
            // Taunt
            else if(entity["skill"] == "Taunt"){
                printf("Loading the taunt icon\n");
                taunt_icon = createTauntIcon(renderer_load, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
                create_entity = taunt_icon;
            }
            // Heal
            else if(entity["skill"] == "Heal"){
                printf("Loading the heal icon\n");
                heal_icon = createHealIcon(renderer_load, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
                create_entity = heal_icon;
            }
            // Melee
            else if(entity["skill"] == "Melee"){
                printf("Loading the melee icon\n");
                melee_icon = createMeleeIcon(renderer_load, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
                create_entity = melee_icon;
            }
            // Ice Shard
            else if(entity["skill"] == "Iceshard"){
                printf("Loading the ice shard icon\n");
                iceShard_icon = createIceShardIcon(renderer_load, vec2(entity["position"]["x"], 
                                                                    entity["position"]["y"]));
                create_entity = iceShard_icon;
            }
            // Rock
            else if(entity["skill"] == "Rock"){
                printf("Loading the rock icon\n");
                rock_icon = createRockIcon(renderer_load, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
                create_entity = rock_icon;
            } 
            // Arrow
            else if(entity["skill"] == "Arrow"){
                printf("Loading the arrow icon\n");
                arrow_icon = createArrowIcon(renderer_load, vec2(entity["position"]["x"], 
                                                            entity["position"]["y"]));
                create_entity = arrow_icon;
            } 
            else {
                printf("Given icon does not exist\n");
            }
        }
        else {
            printf("Given entity type does not exist\n");
        }
        // ---------------------------- COMPONENTS ----------------------------
        for(json component: entity["components"]){
            if(component["type"] == "colors"){
                printf("Emplacing entity in colors\n");
                registry.colors.insert(create_entity, vec3(component["vec"]["x"],
                                                    component["vec"]["y"],
                                                    component["vec"]["z"]));
                                                                
            } else if(component["type"] == "stat"){
                printf("Emplacing entity in stats\n");
                Statistics& stat = registry.stats.get(create_entity);
                stat.health = component["health"];
                stat.speed = component["speed"];
            } else if(component["type"] == "silenced"){
                printf("Emplacing entity in silenced\n");
                Silenced& silenced = registry.silenced.emplace(create_entity);
                silenced.turns = component["turn"];
            } else if(component["type"] == "taunt"){
                printf("Emplacing entity in taunt\n");
                Taunt& taunt = registry.taunts.emplace(create_entity);
                taunt.duration = component["turn"];
                createTauntIndicator(renderer_load, create_entity);
            } else if(component["type"] == "deathTimer"){
                printf("Emplacing entity in death timer\n");
                DeathTimer& dt = registry.deathTimers.emplace(create_entity);
                dt.counter_ms = component["time"];
            } else if(component["type"] == "bleed"){
                printf("Emplacing entity in bleed\n");
                Bleed& bleed = registry.bleeds.emplace(create_entity);
                bleed.duration = component["turn"];
                createBleedIndicator(renderer_load, create_entity);
            }
        }
        // ---------------------------- STATUS ----------------------------
        if(!entity["status"].is_null()){
            printf("Loading entity status\n");
            if(entity["status"] == "Current"){
                printf("Is current\n");
                currPlayer = create_entity;
            } else if(entity["status"] == "Previous"){
                printf("Is previous\n");
                prevPlayer = create_entity;
            } else if(entity["status"] == "Round"){
                 std::cout << "In round: "<< entity["round_num"] << std::endl;
                tempRound[entity["round_num"]] = create_entity;
                roundNum++;
            } 
        }
        if(!entity["round_status"].is_null()){
            std::cout << "In round: "<< entity["round_nums"] << std::endl;
            for(int round: entity["round_nums"]){
                tempRound[round] = create_entity;
                roundNum++;
            }
        } 
        printf("Loaded the entity\n");
    }
    if(j["roundSize"] != roundNum) {
        roundVec.clear();
    } else {
        for(int i = 0; i<roundNum; i++){
            roundVec.push_back(tempRound[i]);
        }
    }
    

}

int get_round(Entity entity){
    int round = -1;
    for(int i = 0; i<roundVec.size(); i++){
        if(entity == roundVec[i]){
            round = i;
        }
    }
    return round;
}

json get_entity(Entity entity){
    json j;
    int comp_num = 0;
    if(entity == player_mage){
        printf("Saving player mage\n");
        j["type"] = "Companion";
        j["skill"] = "Mage";
    } else if(entity == player_swordsman){
        printf("Saving player swordsman\n");
        j["type"] = "Companion";
        j["skill"] = "Swordsman";
    } else if(entity == player_archer){
        printf("Saving player archer\n");
        j["type"] = "Companion";
        j["skill"] = "Archer";
    } else if(entity == enemy_mage){
        printf("Saving enemy mage\n");
        j["type"] = "Enemy";
        j["skill"] = "Mage";
        j["components"][comp_num]["type"] = "colors";
        j["components"][comp_num]["vec"]["x"] = 0;
        j["components"][comp_num]["vec"]["y"] = 1;
        j["components"][comp_num]["vec"]["z"] = 1;
        comp_num++;

    } else if(entity == enemy_swordsman){
        printf("Saving enemy swordsman");
        j["type"] = "Enemy";
        j["skill"] = "Swordsman";
        j["components"][comp_num]["type"] = "colors";
        j["components"][comp_num]["vec"]["x"] = 0;
        j["components"][comp_num]["vec"]["y"] = 1;
        j["components"][comp_num]["vec"]["z"] = 1;
        comp_num++;
    } else if(entity == necromancer_phase_one){
        printf("Saving enemy necromancer phase 1");
        j["type"] = "Enemy";
        j["skill"] = "Necromancer1";
    } else if(entity == necromancer_phase_two){
        printf("Saving enemy necromancer phase 2");
        j["type"] = "Enemy";
        j["skill"] = "Necromancer2";
    } else if(entity == necromancer_minion){
        printf("Saving the necromancer minion");
        j["type"] = "Enemy";
        j["skill"] = "Minion";
    } 
    if(registry.motions.has(entity)){
        Motion motion = registry.motions.get(entity);
        j["position"]["x"] = motion.position.x;
        j["position"]["y"] = motion.position.y;
    }
    if (registry.stats.has(entity)){
        Statistics stat = registry.stats.get(entity);
         j["components"][comp_num]["type"] = "stat";
         j["components"][comp_num]["health"] = stat.health;
         j["components"][comp_num]["speed"] = stat.speed;
         comp_num++;
    } 
    if (registry.silenced.has(entity)){
        Silenced silenced = registry.silenced.get(entity);
         j["components"][comp_num]["type"] = "silenced";
         j["components"][comp_num]["turn"] = silenced.turns;
         comp_num++;
    } 
    if (registry.taunts.has(entity)){
        Taunt taunt = registry.taunts.get(entity);
         j["components"][comp_num]["type"] = "taunt";
         j["components"][comp_num]["turn"] = taunt.duration;
         comp_num++;
    } 
    if (registry.deathTimers.has(entity)){
        DeathTimer dt = registry.deathTimers.get(entity);
         j["components"][comp_num]["type"] = "deathTimer";
         j["components"][comp_num]["time"] = dt.counter_ms;
         comp_num++;
    } 
    if (registry.bleeds.has(entity)){
        Bleed bleed = registry.bleeds.get(entity);
        j["components"][comp_num]["type"] = "bleed";
        j["components"][comp_num]["turn"] = bleed.duration;
        comp_num++;
    } 
    if(entity == currPlayer){
        j["status"] = "Current";
    } 
    else if(entity == prevPlayer){
        j["status"] = "Previous";
    } 
    if(entity == necromancer_phase_two){

    }
    int round_val = get_round(entity);
    if(round_val > (-1)){
        if(entity == necromancer_phase_two){
            j["round_status"] = "Round";
            int ind = 0;
            for(int i = 0; i<roundVec.size(); i++){
                if(entity == roundVec[i]){
                    j["round_nums"][ind] = i;
                    ind++;
                }
            }
        } else {
            j["status"] = "Round";
            j["round_num"] = round_val;
        }
    }
    return j;
}


void JSONLoader::save_game(){
    ofstream save_file;
    save_file.open ("save_file.json");
    json j;
    j["level"] = gameLevel;
    j["roundSize"] = roundVec.size();
    j["story"] = story;
    j["isFreeRoam"] = isFreeRoam;
    j["freeRoamLevel"] = freeRoamLevel;
    j["HPDebuff"] = registry.HPDebuff;
    j["HPBuff"] = registry.HPBuff;
    j["swordsmanHPBuff"] = registry.swordsmanHPBuff;
    
    int entity = 0;
    for(Entity companion: registry.companions.entities){
        j["entities"][entity] = get_entity(companion);
        entity++;
    }
    for(Entity enemy: registry.enemies.entities){
        j["entities"][entity] = get_entity(enemy);
        entity++;
    }
    j["entities"][entity]["type"] = "Icon";
    j["entities"][entity]["skill"] = "Arrow";
    j["entities"][entity]["position"]["x"] = 200;
    j["entities"][entity]["position"]["y"] = 700;
    entity++;

    j["entities"][entity]["type"] = "Icon";
    j["entities"][entity]["skill"] = "Taunt";
    j["entities"][entity]["position"]["x"] = 300;
    j["entities"][entity]["position"]["y"] = 700;
    entity++;

    j["entities"][entity]["type"] = "Icon";
    j["entities"][entity]["skill"] = "Heal";
    j["entities"][entity]["position"]["x"] = 400;
    j["entities"][entity]["position"]["y"] = 700;
    entity++;

    j["entities"][entity]["type"] = "Icon";
    j["entities"][entity]["skill"] = "Melee";
    j["entities"][entity]["position"]["x"] = 500;
    j["entities"][entity]["position"]["y"] = 700;
    entity++;

    j["entities"][entity]["type"] = "Icon";
    j["entities"][entity]["skill"] = "Iceshard";
    j["entities"][entity]["position"]["x"] = 600;
    j["entities"][entity]["position"]["y"] = 700;
    entity++;

    j["entities"][entity]["type"] = "Icon";
    j["entities"][entity]["skill"] = "Fireball";
    j["entities"][entity]["position"]["x"] = 700;
    j["entities"][entity]["position"]["y"] = 700;
    entity++;

    j["entities"][entity]["type"] = "Icon";
    j["entities"][entity]["skill"] = "Rock";
    j["entities"][entity]["position"]["x"] = 800;
    j["entities"][entity]["position"]["y"] = 700;
    entity++;

    save_file << j;
    save_file.close();
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

bool JSONLoader::get_save_file(){
    std::ifstream jsonFile("./save_file.json");
    if (jsonFile.is_open()){
        printf("Found the save file\n");
        json j = json::parse(jsonFile);
        std::cout << j << std::endl;
        load_level(j);
        return true;
	}
	else {
        printf("File not found\n");
        return false;
    }
}