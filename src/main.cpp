
#define GL3W_IMPLEMENTATION
#include <gl3w.h>

#if WIN32
#include <windows.h>
#else
#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

// stlib
#include <chrono>
#include <iostream>


// internal
#include "ai_system.hpp"
#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "skill_system.hpp"
#include "swarm_system.hpp"

using Clock = std::chrono::high_resolution_clock;
using namespace std;


int window_width_px = 1200;
int window_height_px = 750;

// Get the horizontal and vertical screen sizes in pixel
void getScreenResolution(unsigned int& width, unsigned int& height) {
#if WIN32
	width = (int)GetSystemMetrics(SM_CXSCREEN);
	height = (int)GetSystemMetrics(SM_CYSCREEN);
#else
	auto mainDisplayId = CGMainDisplayID();
	width = CGDisplayPixelsWide(mainDisplayId);
	height = CGDisplayPixelsHigh(mainDisplayId);
#endif
}

// Entry point
int main()
{
	// Global systems
	WorldSystem world;
	RenderSystem renderer;
	SkillSystem sk;
	PhysicsSystem physics;
	AISystem ai;
	SwarmSystem swarmSys;

	getScreenResolution(registry.horizontalResolution, registry.verticalResolution);
	getScreenResolution(sk.horizontalResolution, sk.verticalResolution);
	sk.initializeFireballSpeed();

	// Initializing window
	GLFWwindow* window = world.create_window(window_width_px, window_height_px);
	if (!window) {
		// Time to read the error message
		printf("Press any key to exit");
		getchar();
		return EXIT_FAILURE;
	}

	// initialize the main systems
	renderer.init(window_width_px, window_height_px, window);
	world.init(&renderer, &ai, &sk, &swarmSys);
	
	// variable timestep loop
	auto t = Clock::now();
	
	isFreeRoam = 0;

	while (!world.is_over()) {
		// Processes system messages, if this wasn't present the window would become
		// unresponsive
		glfwPollEvents();

		// Calculating elapsed times in milliseconds from the previous iteration
		auto now = Clock::now();
		float elapsed_ms =
			(float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
		t = now;

		if (world.canStep) {
			world.step(elapsed_ms);
			// ai.step(elapsed_ms);
			if(isFreeRoam){
				physics.step_freeRoam(elapsed_ms, window_width_px, window_height_px);
			} else {
				physics.step(elapsed_ms, window_width_px, window_height_px);
			}
			world.handle_collisions();
			world.handle_boundary_collision();
		}

		renderer.draw(elapsed_ms);
	}

	return EXIT_SUCCESS;
}
