#include <iostream>
#include <time.h>
#include "swarm_system.hpp"
using namespace std;

void SwarmSystem::initializeSwarmEntities(RenderSystem* renderer) {
	float initialXPos = window_width_px / 2;
	float initialYPos = window_height_px / 2;
	float posGap = FIREFLY_WIDTH + 10;

	// Max 6 fireflies per column
	int maxYPerColumn = 5;
	int xRowCounter = 0; int yRowCounter = 0;
	for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {

		float xPos = initialXPos + posGap * xRowCounter;
		float yPos = initialYPos + posGap * yRowCounter;

		createFirefly(renderer, vec2(xPos, yPos));
		
		yRowCounter++;
		if (yRowCounter == maxYPerColumn) {
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
	//std::default_random_engine generator(std::random_device{}());
	//std::uniform_int_distribution<int> distribution(0, 1);
	//return distribution(generator);
	return (float) (rand() % 1000) / 1000;
}

// main program function
// Reference: Particle Swarm Optimization (PSO) algorithm from Swarm Intelligence Algorithms: A Tutorial: Slowik, Adam
int SwarmSystem::startSwarm() {
	// Random number generator
	srand(time(NULL));
	// Pmin and Pmax determine the closeness between fireflies, lower tightness value = tighter swarm behaviour
	const int tightness = 20;

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

void SwarmSystem::updateSwarm() {

	// update the fireflies (particles) here
	TheBest = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < D; j++) {
			// for each particle compute the velocity vector
			V[i][j] = V[i][j] + c1 * r() * (Pbest[i][j] - P[i][j]) + c2 * r() * (
				Gbest[j] - P[i][j]);

			auto& fireflyComponent = registry.fireflySwarm.components[i];
			//// flip x-velocity to avoid going out of border
			//if (j == 0 && fireflyComponent.shouldFlipVelocityX) {
			//	V[i][j] = -V[i][j];
			//	fireflyComponent.shouldFlipVelocityX = 0;
			//}
			//// flip y-velocity to avoid going out of border
			//if (j == 1 && fireflyComponent.shouldFlipVelocityY) {
			//	V[i][j] = -V[i][j];
			//	fireflyComponent.shouldFlipVelocityY = 0;
			//}

			// update the corresponding firefly velocity
			Motion& fireflyMotion = registry.motions.get(registry.fireflySwarm.entities[i]);
			if (j == 0) {
				// Update the firefly's x-velocity
				fireflyMotion.velocity.x = V[i][j];
				// Give it a boost to fly faster in that direction
				//if (V[i][j] < 0) {
				//	fireflyMotion.velocity.x -= velocityBoost;
				//}
				//else if (V[i][j] > 0) {
				//	fireflyMotion.velocity.x += velocityBoost;
				//}
			}
			else {
				// Update the firefly's y-velocity
				fireflyMotion.velocity.y = V[i][j];
				// Give it a boost to fly faster in that direction
				//if (V[i][j] < 0) {
				//	fireflyMotion.velocity.y -= velocityBoost;
				//}
				//else if (V[i][j] > 0) {
				//	fireflyMotion.velocity.y += velocityBoost;
				//}
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