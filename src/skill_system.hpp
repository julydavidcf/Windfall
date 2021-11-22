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
	Mix_Chunk* silence_spell_sound;
	Mix_Chunk* lightning_spell_sound;
	Mix_Chunk* ice_spell_sound;
	Mix_Chunk* summon_spell_sound;
	Mix_Chunk* charge_spell_sound;
	Mix_Chunk* beam_spell_sound;
	Mix_Chunk* minion_spawn_sound;
	Mix_Chunk* error_sound;
	Mix_Chunk* gesture_heal_sound;
	Mix_Chunk* gesture_aoe_sound;
	Mix_Chunk* gesture_turn_sound;

	//timers
	const float animation_timer = 500.f;
	const float hit_position = 20.f;


	vec2 mousePos = { 0,0 };

	//WorldSystem ws;
	// start skills
	void startTauntAttack(Entity origin, Entity target);
	void startSilenceAttack(Entity origin, Entity target);
	void startIceShardAttack(Entity origin, Entity target);
	void startFireballAttack(Entity origin);
	void startRockAttack(Entity origin, Entity target);
	void startLightningAttack(Entity origin, Entity target);
	void startHealAttack(Entity origin, Entity target);
	void startMeleeAttack(Entity origin, Entity target);
	void startSummonAttack(Entity origin);
	void startParticleBeamAttack(Entity origin, Entity target);
	void startParticleBeamCharge(Entity origin, Entity target);

	// launch skills
	Entity launchIceShard(vec2 startPos, vec2 ms_pos, RenderSystem* renderer);
	Entity launchFireball(vec2 startPos, vec2 ms_pos, RenderSystem* renderer);
	Entity launchRock(Entity target, RenderSystem* renderer);
	Entity launchLightning(Entity target, RenderSystem* renderer);
	Entity launchParticleBeamCharge(Entity target, RenderSystem* render);
	void launchTaunt(Entity target, RenderSystem* renderer);
	void launchHeal(Entity target, float amount, RenderSystem* renderer);
	void launchMelee(Entity target, RenderSystem* renderer);
	void launchParticleBeam(Entity target);

	void launchSilence(Entity target, RenderSystem* renderer);
	void launchSummon(RenderSystem* renderer);
	void luanchCompanionTeamHeal(float amount, RenderSystem* renderer);
	void luanchEnemyTeamDamage(float amount, RenderSystem* renderer);

	std::pair<bool, bool> updateParticleBeam(Entity& origin, float elapsed_ms_since_last_update, float width, float height);

	void removeTaunt(Entity target);
	void removeSilence(Entity target);
	void removeUltimate(Entity target);
	

	//skill constants
	float FIREBALLSPEED = 900.f;
	float ARROWSPEED = 700.f;
	float ICESHARDSPEED = 100.f;

};