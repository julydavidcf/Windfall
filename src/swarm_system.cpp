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

// generate pseudo random values from the range [0 , 1)
float r() {
	return (float) (rand() % 1000) / 1000;
}

void SwarmSystem::updateSwarm() {
	auto& fireflyEntities = registry.fireflySwarm.entities;

	float N = NUM_SWARM_PARTICLES;

	float SD_X = 0.f;
	float mu_X = 0.f;
	float sumSquaresX = 0.f;
	float maxX = registry.motions.get(fireflyEntities[0]).position.x;
	float minX = registry.motions.get(fireflyEntities[0]).position.x;
	
	float SD_Y = 0.f;
	float mu_Y = 0.f;
	float sumSquaresY = 0.f;
	float maxY = registry.motions.get(fireflyEntities[0]).position.y;
	float minY = registry.motions.get(fireflyEntities[0]).position.y;

	for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {
		Motion& particleMotion = registry.motions.get(fireflyEntities[i]);

		// Set the initial velocity for current step
		float randomSeedX = -1 + 2 * ((float)rand()) / RAND_MAX; // From -1 to 1
		float randomSeedY = -1 + 2 * ((float)rand()) / RAND_MAX; // From -1 to 1
		float initialVelocityX = INITIAL_BASE_VELOCITY * randomSeedX;
		float initialVelocityY = INITIAL_BASE_VELOCITY * randomSeedY;

		particleMotion.velocity = vec2(initialVelocityX, initialVelocityY);

		// Add to mu_X for current particle
		mu_X += particleMotion.position.x;
		
		if (particleMotion.position.x < minX) {
			minX = particleMotion.position.x;
		}
		if (particleMotion.position.x > maxX) {
			maxX = particleMotion.position.x;
		}

		// Add to mu_Y for current particle
		mu_Y += particleMotion.position.y;
		
		if (particleMotion.position.y < minY) {
			minY = particleMotion.position.y;
		}
		if (particleMotion.position.y > maxY) {
			maxY = particleMotion.position.y;
		}
	}

	for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {
		Motion& particleMotion = registry.motions.get(fireflyEntities[i]);

		// Add to sumSquaresX for current particle
		sumSquaresX += ((particleMotion.position.x - mu_X) * (particleMotion.position.x - mu_X));
		// Add to sumSquaresY for current particle
		sumSquaresY += ((particleMotion.position.y - mu_Y) * (particleMotion.position.y - mu_Y));
	}

	// Compute the standard deviation for all particle pos.x
	SD_X = sqrt(sumSquaresX / N);
	SD_Y = sqrt(sumSquaresY / N);
	
	// Case: X-spread is too large
	if (SD_X > 60000) {

		float midpointX = (minX + maxX) / 2;

		for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {
			Motion& particleMotion = registry.motions.get(fireflyEntities[i]);

			// Particle is closer to the left edge than center, move right
			if (abs(particleMotion.position.x - midpointX) >= abs(particleMotion.position.x - minX)) {
				particleMotion.velocity.x = abs(particleMotion.velocity.x);
			}
			// Particle is closer to the right edge than center, move left
			else if ((abs(particleMotion.position.x - midpointX) >= abs(particleMotion.position.x - maxX))){
				particleMotion.velocity.x = -abs(particleMotion.velocity.x);
			}

		}
	}

	// Case: Y-spread is too large
	if (SD_Y > 60000) {

		float midpointY = (minY + maxY) / 2;

		for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {
			Motion& particleMotion = registry.motions.get(fireflyEntities[i]);

			// Particle is closer to the top edge than center, move down
			if (abs(particleMotion.position.y - midpointY) >= abs(particleMotion.position.y - minY)) {
				particleMotion.velocity.y = abs(particleMotion.velocity.y);
			}
			// Particle is closer to the bottom edge than center, move up
			else if ((abs(particleMotion.position.y - midpointY) >= abs(particleMotion.position.y - maxY))) {
				particleMotion.velocity.y = -abs(particleMotion.velocity.y);
			}

		}
	}
}