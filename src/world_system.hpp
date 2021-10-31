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

	// create attacks
	void iceShardAttack(Entity currPlayer);
	void rockAttack(Entity target);
	void healSkill(Entity target, float amount);
	void meleeSkill(Entity target);
	void tauntSkill(Entity target);

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

	// check if mouse in entity
	bool inEntity(const Entity entity);

	// deselect current button (after using ability)
	void deselectButton();

	// reset all selected buttons if there are any (when a button is clicked)
	void deselectButtons();

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

	// Death particle activation
	void activate_deathParticles(Entity entity);

	//Skills Function
	Entity WorldSystem::launchIceShard(vec2 startPos);
	Entity WorldSystem::launchFireball(vec2 startPos);
	Entity WorldSystem::launchRock(Entity target);
	void WorldSystem::launchMelee(Entity origin, Entity target);
	void WorldSystem::launchTaunt(Entity target);
	void WorldSystem::removeTaunt(Entity target);
	void WorldSystem::healTarget(Entity target, float amount);
	void WorldSystem::damageTarget(Entity target, float amount);

	bool WorldSystem::canUseSkill(Entity user, int skill);
	void WorldSystem::showCorrectSkills();

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
	Entity silence_icon;

	Entity iceShard;

	//icons
	Entity melee_icon;
	Entity iceShard_icon;
	Entity fireBall_icon;
	Entity taunt_icon;
	Entity heal_icon;
	Entity rock_icon;

	// music references
	Mix_Music* background_music;
	Mix_Chunk* salmon_dead_sound;
	Mix_Chunk* salmon_eat_sound;
	Mix_Chunk* hit_enemy_sound;
	Mix_Chunk* fireball_explosion_sound;
	Mix_Chunk* death_enemy_sound;
	Mix_Chunk* fire_spell_sound;
	Mix_Chunk* rock_spell_sound;
	Mix_Chunk* heal_spell_sound;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniform_dist; // number between 0..1

	// Camera Control constants
	float CAM_OFFSET_XMAX = 0.4;
	float CAM_OFFSET_YMAX = 0.2;

	//skill constants
	float ICESHARDSPEED = 100.f;
	float FIREBALLSPEED = 700.f;

	std::vector<std::vector<bool>> skill_character_aviability = {
		// ice  fire  rock  heal  taunt  melee
//mage
		{ true, true, true, true, false, false},
//swordsman
		{ false, false, false, false, true, true}
	};


};
// Can't use diretly somehow so just for reference
enum class SKILL_ID {
	SK_ICESHARD = 0,
	SK_FIREBALL = SK_ICESHARD + 1, //1
	SK_ROCK = SK_FIREBALL + 1, //2
	SK_HEAL = SK_ROCK + 1, //3
	SK_TAUNT = SK_HEAL+1,//4
	SK_MELEE = SK_TAUNT +1,//5



	SKILL_COUNT = SK_MELEE + 1,
};
