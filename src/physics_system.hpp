#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"

// A simple physics system that moves rigid bodies and checks for collision
class PhysicsSystem
{
public:
	void step(float elapsed_ms, float window_width_px, float window_height_px);
	void step_freeRoam(float elapsed_ms, float window_width_px, float window_height_px);
	vec2 get_custom_position(Entity entity);
	vec2 get_custom_bounding_box(Entity entity);
	void showDebugBox();
	PhysicsSystem()
	{
	}

private:
	float xBorderLimitDist = 50;
	float yBorderLimitDist = 50;
	float playerBorderLimitDist = 25;
};