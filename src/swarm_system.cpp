#include <iostream>
#include <time.h>
#include "swarm_system.hpp"
using namespace std;

void SwarmSystem::initializeSwarmEntities(RenderSystem* renderer) {
	float initialXPos = window_width_px / 2;
	float initialYPos = window_height_px / 2;
	float posGap = FIREFLY_WIDTH + 10;
	swarmInitialized = 1;

	// Max fireflies per column - 1
	int maxYPerColumn = 9;
	int xRowCounter = 0; int yRowCounter = 0;
	for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {

		float xPos = initialXPos + posGap * xRowCounter;
		float yPos = initialYPos + posGap * yRowCounter;

		auto e = createFirefly(renderer, vec2(xPos, yPos));
		if (i == 0) {
			registry.light.emplace(e);
		}
		
		yRowCounter++;
		if (yRowCounter > maxYPerColumn) {
			yRowCounter = 0;
			xRowCounter++;
		}
	}
}

void SwarmSystem::resetSwarm() {
	float initialXPos = window_width_px / 2;
	float initialYPos = window_height_px / 2;
	float posGap = FIREFLY_WIDTH + 10;

	// Max fireflies per column - 1
	int maxYPerColumn = 9;
	int xRowCounter = 0; int yRowCounter = 0;
	for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {
		float xPos = initialXPos + posGap * xRowCounter;
		float yPos = initialYPos + posGap * yRowCounter;

		auto& motion = registry.motions.emplace(registry.fireflySwarm.entities[i]);
		motion.position = vec2(xPos, yPos);
		motion.velocity = vec2(0.f, 0.f);

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

	if (!registry.motions.has(fireflyEntities[0])) return;

	float maxX = registry.motions.get(fireflyEntities[0]).position.x;
	float minX = registry.motions.get(fireflyEntities[0]).position.x;

	float maxY = registry.motions.get(fireflyEntities[0]).position.y;
	float minY = registry.motions.get(fireflyEntities[0]).position.y;

	for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {
		if (!registry.motions.has(fireflyEntities[i])) continue;
		Motion& particleMotion = registry.motions.get(fireflyEntities[i]);

		// Check if any two fireflies are too close to each other
		for (int j = 0; j < NUM_SWARM_PARTICLES; j++) {
			if (!registry.motions.has(fireflyEntities[j])) continue;
			Motion& otherParticleMotion = registry.motions.get(fireflyEntities[j]);

			// x-pos is too close
			if (particleMotion.position.x < otherParticleMotion.position.x && particleMotion.position.x > otherParticleMotion.position.x - SEPARATION_DIST) {
				particleMotion.position.x -= SEPARATION_MOVE_VALUE;
				otherParticleMotion.position.x += SEPARATION_MOVE_VALUE;
			}
			else if (particleMotion.position.x > otherParticleMotion.position.x && particleMotion.position.x < otherParticleMotion.position.x + SEPARATION_DIST) {
				particleMotion.position.x += SEPARATION_MOVE_VALUE;
				otherParticleMotion.position.x -= SEPARATION_MOVE_VALUE;
			}

			// y-pos is too close
			if (particleMotion.position.y < otherParticleMotion.position.y && particleMotion.position.y > otherParticleMotion.position.y - SEPARATION_DIST) {
				particleMotion.position.y -= SEPARATION_MOVE_VALUE;
				otherParticleMotion.position.y += SEPARATION_MOVE_VALUE;
			}
			else if (particleMotion.position.y > otherParticleMotion.position.y && particleMotion.position.y < otherParticleMotion.position.y + SEPARATION_DIST) {
				particleMotion.position.y += SEPARATION_MOVE_VALUE;
				otherParticleMotion.position.y -= SEPARATION_MOVE_VALUE;
			}
		}

		if (!registry.fireflySwarm.components[i].isDodging) {
			// Set the initial velocity for current step
			float randomSeedX = -1 + 2 * ((float)rand()) / RAND_MAX; // From -1 to 1
			float randomSeedY = -1 + 2 * ((float)rand()) / RAND_MAX; // From -1 to 1
			float initialVelocityX = INITIAL_BASE_VELOCITY * randomSeedX;
			float initialVelocityY = INITIAL_BASE_VELOCITY * randomSeedY;

			if (i > 0) {
				vec2 leaderVelocity = registry.motions.get(fireflyEntities[0]).velocity;
				particleMotion.velocity = vec2(initialVelocityX + leaderVelocity.x, initialVelocityY + leaderVelocity.y);
			}
			else {
				particleMotion.velocity = vec2(initialVelocityX, initialVelocityY);
			}

		}
		
		if (particleMotion.position.x < minX) {
			minX = particleMotion.position.x;
		}
		if (particleMotion.position.x > maxX) {
			maxX = particleMotion.position.x;
		}
		
		if (particleMotion.position.y < minY) {
			minY = particleMotion.position.y;
		}
		if (particleMotion.position.y > maxY) {
			maxY = particleMotion.position.y;
		}
	}

	// Case: X-spread is too large
	if (maxX - minX > 200) {

		float midpointX = (minX + maxX) / 2;

		for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {
			if (!registry.motions.has(fireflyEntities[i]) || registry.fireflySwarm.components[i].isDodging) continue;
			Motion& particleMotion = registry.motions.get(fireflyEntities[i]);

			// Particle is closer to the left edge than center, move right
			if (abs(particleMotion.position.x - midpointX) >= abs(particleMotion.position.x - minX)) {
				particleMotion.velocity.x = abs(particleMotion.velocity.x);
			}
			// Particle is closer to the right edge than center, move left
			else if ((abs(particleMotion.position.x - midpointX) >= abs(particleMotion.position.x - maxX))) {
				particleMotion.velocity.x = -abs(particleMotion.velocity.x);
			}
			else particleMotion.velocity.x = particleMotion.velocity.x;

		}
	}

	// Case: Y-spread is too large
	if (maxY - minY > 200) {

		float midpointY = (minY + maxY) / 2;

		for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {
			if (!registry.motions.has(fireflyEntities[i]) || registry.fireflySwarm.components[i].isDodging) continue;
			Motion& particleMotion = registry.motions.get(fireflyEntities[i]);

			// Particle is closer to the top edge than center, move down
			if (abs(particleMotion.position.y - midpointY) >= abs(particleMotion.position.y - minY)) {
				particleMotion.velocity.y = abs(particleMotion.velocity.y);
			}
			// Particle is closer to the bottom edge than center, move up
			else if ((abs(particleMotion.position.y - midpointY) >= abs(particleMotion.position.y - maxY))) {
				particleMotion.velocity.y = -abs(particleMotion.velocity.y);
			}
			else particleMotion.velocity.y = particleMotion.velocity.y;

		}
	}
}