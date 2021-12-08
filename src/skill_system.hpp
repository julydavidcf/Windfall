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
	void startSummonAttack(Entity origin);
	void startParticleBeamAttack(Entity origin, Entity target);
	void startParticleBeamCharge(Entity origin, Entity target);
	void startMeleeAttack(Entity origin, Entity target, int bleedOrAOE);
	void startShieldAttack(Entity origin);
	void startArrowAttack(Entity origin);

	// launch skills
	Entity launchIceShard(vec2 startPos, vec2 ms_pos, RenderSystem* renderer);
	Entity launchFireball(vec2 startPos, vec2 ms_pos, RenderSystem* renderer);
	Entity launchArrow(Entity start, vec2 ms_pos, RenderSystem* renderer, int isFreeRoam);
	Entity launchRock(Entity target, RenderSystem* renderer);
	Entity launchSpike(Entity target, RenderSystem* renderer);
	Entity launchLightning(Entity target, RenderSystem* renderer);
	Entity launchParticleBeamCharge(Entity target, RenderSystem* render);
	void launchTaunt(Entity target, RenderSystem* renderer);
	void launchBleed(Entity target, RenderSystem* renderer);
	void launchBleedDMG(Entity target, RenderSystem* renderer);
	void launchHeal(Entity target, float amount, RenderSystem* renderer);
	void launchMelee(Entity target, RenderSystem* renderer);
	void launchParticleBeam(Entity target);

	void launchSilence(Entity target, RenderSystem* renderer);
	void launchSummon(RenderSystem* renderer);
	void luanchCompanionTeamHeal(float amount, RenderSystem* renderer);
	void luanchEnemyTeamDamage(float amount, RenderSystem* renderer);

	std::pair<bool, bool> updateParticleBeam(Entity& origin, float elapsed_ms_since_last_update, float width, float height);

	void luanchNecroCompanionTeamBleed(RenderSystem* renderer);
	void launchNecroBarrier(Entity target, RenderSystem* renderer);
	
	void removeTaunt(Entity target);
	void removeBleed(Entity target);
	void removeSilence(Entity target);
	void removeUltimate(Entity target);
	void removeShield(Entity target);

	void initializeFireballSpeed();

	//skill constants
	float fireball_speed_x = 0.f;
	float fireball_speed_y = 0.f;

	unsigned int horizontalResolution = 0.f;
	unsigned int verticalResolution = 0.f;

	float ARROWSPEED = 1400.f;
	float ICESHARDSPEED = 100.f;

};