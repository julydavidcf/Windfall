#pragma once

#include <vector>
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "render_system.hpp"
#include "world_init.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

class SkillSystem
{
public:
	SkillSystem();
	~SkillSystem();
	

	// Music References
	Mix_Music* background_music;
	Mix_Chunk* salmon_dead_sound;
	Mix_Chunk* salmon_eat_sound;
	Mix_Chunk* hit_enemy_sound;
	Mix_Chunk* fireball_explosion_sound;
	Mix_Chunk* death_enemy_sound;
	Mix_Chunk* fire_spell_sound;
	Mix_Chunk* rock_spell_sound;
	Mix_Chunk* heal_spell_sound;
	Mix_Chunk* taunt_spell_sound;
	Mix_Chunk* melee_spell_sound;

	//timers
	const float animation_timer = 250.f;
	const float hit_position = 20.f;


	vec2 mousePos = { 0,0 };

	//WorldSystem ws;

	void startTauntAttack(Entity origin, Entity target);
	void startIceShardAttack(Entity origin, Entity target);
	void startFireballAttack(Entity origin);
	void startRockAttack(Entity origin, Entity target);
	void startHealAttack(Entity origin, Entity target);
	void startMeleeAttack(Entity origin, Entity target);


	Entity launchIceShard(vec2 startPos, vec2 ms_pos, RenderSystem* renderer);
	Entity launchFireball(vec2 startPos, vec2 ms_pos, RenderSystem* renderer);
	Entity launchRock(Entity target, RenderSystem* renderer);
	void launchTaunt(Entity target, RenderSystem* renderer);
	void launchHeal(Entity target, float amount, RenderSystem* renderer);
	void launchMelee(Entity target, RenderSystem* renderer);
	
	void removeTaunt(Entity target);
	

	//skill constants
	float FIREBALLSPEED = 900.f;
	float ARROWSPEED = 700.f;
	float ICESHARDSPEED = 100.f;

};