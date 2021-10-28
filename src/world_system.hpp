#pragma once

// internal
#include "common.hpp"

// stlib
#include <vector>
#include <random>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

#include "render_system.hpp"

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class WorldSystem
{
public:
	WorldSystem();

	// Creates a window
	GLFWwindow* create_window(int width, int height);

	// starts the game
	void init(RenderSystem* renderer);

	// display player turn
	void displayPlayerTurn();

	// display enemy turn
	void displayEnemyTurn();

	// create temporary fireball attack
	void temporaryFireball(Entity currPlayer);

	// creates a round
	void createRound();

	// check a round
	void checkRound();

	// Releases all associated resources
	~WorldSystem();

	// Steps the game ahead by ms milliseconds
	bool step(float elapsed_ms);

	// Check for collisions
	void handle_collisions();

	void handle_boundary_collision();

	// Should the game be over ?
	bool is_over()const;

	int player_turn;

private:
	// Input callback functions
	void on_key(int key, int, int action, int mod);
	void on_mouse_move(vec2 pos);
	// handel mouse click
	void on_mouse_button( int button,int action, int mods);

	// check if mouse in button
	bool inButton(vec2 buttonPos, float buttonX, float buttonY);

	// deselect current button (after using ability)
	void deselectButton();

	// Helper function for updating health in collision
	void update_health(Entity entity, Entity other_entity);

	// Updates all healthbars
	void update_healthBars();

	// restart level
	void restart_game();

	// OpenGL window handle
	GLFWwindow* window;

	// Number of fish eaten by the salmon, displayed in the window title
	unsigned int points;

	//Skills Function
	Entity launchFireball(vec2 startPos);
	Entity WorldSystem::launchArrow(vec2 startPos);
	Entity WorldSystem::launchRock(Entity target);
	Entity WorldSystem::launchMelee(Entity target);
	void WorldSystem::healTarget(Entity target, float amount);
	void WorldSystem::damageTarget(Entity target, float amount);

	// Game state
	RenderSystem* renderer;
	float current_speed;
	Entity player_mage;
	Entity enemy_mage;
	Entity player_swordsman;
	Entity enemy_swordsman;
	Entity necromancer;
	Entity fireball;
	Entity fireball_icon;


	// music references
	Mix_Music* background_music;
	Mix_Chunk* salmon_dead_sound;
	Mix_Chunk* salmon_eat_sound;
	Mix_Chunk* hit_enemy_sound;
	Mix_Chunk* fireball_explosion_sound;
	Mix_Chunk* death_enemy_sound;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniform_dist; // number between 0..1

	//skill constants
	float FIREBALLSPEED = 100.f;
	float ARROWSPEED = 700.f;
};
