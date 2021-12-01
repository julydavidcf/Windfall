#include <iostream>
#include <time.h>
#include "swarm_system.hpp"
using namespace std;

void SwarmSystem::initializeSwarmEntities(RenderSystem* renderer) {
	float initialXPos = window_width_px / 2;
	float initialYPos = window_height_px / 2;
	float posGap = FIREFLY_WIDTH + 10;

	// Max fireflies per column - 1
	int maxYPerColumn = 9;
	int xRowCounter = 0; int yRowCounter = 0;
	for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {

		float xPos = initialXPos + posGap * xRowCounter;
		float yPos = initialYPos + posGap * yRowCounter;

		createFirefly(renderer, vec2(xPos, yPos));
		
		yRowCounter++;
		if (yRowCounter > maxYPerColumn) {
			yRowCounter = 0;
			xRowCounter++;
		}
	}
}

// definition of the objective function OF(.)
// Reference: Particle Swarm Optimization algorithm (PSO) from Swarm Intelligence Algorithms: A Tutorial: Slowik, Adam
float OF(float x[], int size_array) {
	float t = 0;
	for (int i = 0; i < size_array; i++) {
		t = t + x[i] * x[i];
	}
	return t;
}

// generate pseudo random values from the range [0 , 1)
float r() {
	return (float) (rand() % 1000) / 1000;
}

// main program function
// Reference: Particle Swarm Optimization (PSO) algorithm from Swarm Intelligence Algorithms: A Tutorial: Slowik, Adam
int SwarmSystem::startSwarm() {
	// Random number generator
	srand(time(NULL));
	// Pmin and Pmax determine the search space for the fireflies, in pixels
	const int tightness = 100;

	for (int j = 0; j < D; j++) {
		Pmin[j] = -tightness; Pmax[j] = tightness;
	}

	// randomly initialize swarm P and Pbest; initialize velocity vectors V as 0
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < D; j++) {
			P[i][j] = (Pmax[j] - Pmin[j]) * r() + Pmin[j];
			Pbest[i][j] = P[i][j];
			V[i][j] = 0;
		}
		// evaluate all particles in the swarm P using the objective function (OF)
		Eval[i] = OF(P[i], D);
		EvalPbest[i] = Eval[i];
		// find the best particle in the swarm P according to OF
		if (Eval[i] < Eval[TheBest]) TheBest = i;
	}

	// assign the best particle to Gbest
	for (j = 0; j < D; j++) Gbest[j] = P[TheBest][j];
	EvalGbest = Eval[TheBest];

	return 0;
}

void SwarmSystem::resetSwarm() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < D; j++) {
			P[i][j] = (Pmax[j] - Pmin[j]) * r() + Pmin[j];
			Pbest[i][j] = P[i][j];
		}
		//// evaluate all particles in the swarm P using the objective function (OF)
		//Eval[i] = OF(P[i], D);
		//EvalPbest[i] = Eval[i];
		//// find the best particle in the swarm P according to OF
		//if (Eval[i] < Eval[TheBest]) TheBest = i;
	}
}

void SwarmSystem::updateSwarm() {

	// update the fireflies (particles) here
	TheBest = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < D; j++) {

			if (!registry.motions.has(registry.fireflySwarm.entities[i])) continue;

			Motion& fireflyMotion = registry.motions.get(registry.fireflySwarm.entities[i]);

			// fetch the current velocity from motions
			float vel = (j == 0) ? fireflyMotion.velocity.x : fireflyMotion.velocity.y;

			// for each particle compute the velocity vector
			V[i][j] = registry.convergenceValue * vel + registry.c1 * r() * (Pbest[i][j] - P[i][j]) + registry.c2 * r() * (
				Gbest[j] - P[i][j]);

			auto& fireflyComponent = registry.fireflySwarm.components[i];


			if (j == 0) {
				// Update the firefly's x-velocity
				fireflyMotion.velocity.x = V[i][j];
			}
			else {
				// Update the firefly's y-velocity
				fireflyMotion.velocity.y = V[i][j];
			}

			// update each particle using velocity vector
			P[i][j] = P[i][j] + V[i][j];

			// check the constraints for each dimension
			if (P[i][j] < Pmin[j]) P[i][j] = Pmin[j];
			if (P[i][j] > Pmax[j]) P[i][j] = Pmax[j];
		}

		// evaluate each particle from the swarm P
		Eval[i] = OF(P[i], D);
		if (Eval[i] < EvalPbest[i]) {
			// for each particle update its PBest vector
			for (int j = 0; j < D; j++) Pbest[i][j] = P[i][j];
		}
		if (Eval[i] < Eval[TheBest]) TheBest = i;
	}

	// update Gbest vector if better particle is found
	if (Eval[TheBest] < EvalGbest) {
		for (int j = 0; j < D; j++) Gbest[j] = P[TheBest][j];
		EvalGbest = Eval[TheBest];
	}
	//cout << "EvalGbest = " << EvalGbest << endl;
}