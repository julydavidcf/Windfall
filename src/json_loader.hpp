#pragma once

#include "../ext/nlohmann/json.hpp"
#include "common.hpp"
#include "render_system.hpp"
#include "tiny_ecs.hpp"

using namespace std;

class JSONLoader 
{
    public: 
        JSONLoader();
        void get_level(std::string file_name);
        void init(RenderSystem* renderer);

};