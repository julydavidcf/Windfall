#pragma once

#include <vector>
#include <random>
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"
#include "render_system.hpp"

class SwarmSystem {

public:
	void updateSwarm();
	void initializeSwarmEntities(RenderSystem* renderer);

private:
	float INITIAL_BASE_VELOCITY = 75;
};