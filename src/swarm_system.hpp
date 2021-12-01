#pragma once

#include <vector>
#include <random>
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"
#include "render_system.hpp"

class SwarmSystem {

public:
	int startSwarm();
	void updateSwarm();
	void initializeSwarmEntities(RenderSystem* renderer);

private:

	// Initialization of the parameters:
	// N = # particles, MAX = # iterations, D = # dimensions (should be 2 for x-axis & y-axis)
	const int N = NUM_SWARM_PARTICLES; const int D = 2;
	int i, j, k;
	int TheBest = 0;
	float Pmin[2]; float Pmax[2];


	// all data structures which are needed by PSO
	float P[NUM_SWARM_PARTICLES][2]; float Pbest[NUM_SWARM_PARTICLES][2];
	float V[NUM_SWARM_PARTICLES][2]; float Eval[NUM_SWARM_PARTICLES]; float EvalPbest[NUM_SWARM_PARTICLES];
	float Gbest[2]; float EvalGbest;

	float velocityBoost = 50;
};