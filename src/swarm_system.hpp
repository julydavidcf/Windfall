#pragma once

#include <vector>
#include <random>
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"
#include "render_system.hpp"

class SwarmSystem {

public:
	void initializeSwarmEntities(RenderSystem* renderer);
	void updateSwarm();
	void resetSwarm();

	int swarmInitialized = 0;

private:
	float INITIAL_BASE_VELOCITY = 75;
};