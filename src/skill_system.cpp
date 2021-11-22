#include "skill_system.hpp"

#include <random>


SkillSystem::SkillSystem() { }

SkillSystem::~SkillSystem() { }

void SkillSystem::startTauntAttack(Entity origin, Entity target) {
	printf("Started the taunt attack\n");
	if (registry.enemies.has(origin)) {
		taunt_spell_sound = Mix_LoadWAV(audio_path("taunt_spell.wav").c_str());
		if (taunt_spell_sound != nullptr) {
			Mix_Volume(5, 32);
			Mix_PlayChannel(-1, taunt_spell_sound, 0);
		}
		else {
			printf("soundEff failed loading");
		}
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = TAUNT;
		attack.target = target;
		attack.counter_ms = 1500.f;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
		taunt_spell_sound = Mix_LoadWAV(audio_path("taunt_spell.wav").c_str());
		if (taunt_spell_sound != nullptr) {
			Mix_Volume(5, 32);
			Mix_PlayChannel(-1, taunt_spell_sound, 0);
		}
		else {
			printf("soundEff failed loading");
		}
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = TAUNT;
		attack.target = target;
		attack.counter_ms = 1500.f;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
}

void SkillSystem::startSilenceAttack(Entity origin, Entity target) {
	printf("Started the silence attack\n");
	if (registry.enemies.has(origin)) {
		silence_spell_sound = Mix_LoadWAV(audio_path("silence_spell.wav").c_str());
		if (silence_spell_sound != nullptr) {
			Mix_Volume(5, 32);
			Mix_PlayChannel(-1, silence_spell_sound, 0);
		}
		else {
			printf("soundEff failed loading");
		}
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = SILENCE;
		attack.target = target;
		attack.counter_ms = 1100.f;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
		silence_spell_sound = Mix_LoadWAV(audio_path("silence_spell.wav").c_str());
		if (silence_spell_sound != nullptr) {
			Mix_Volume(5, 32);
			Mix_PlayChannel(-1, silence_spell_sound, 0);
		}
		else {
			printf("soundEff failed loading");
		}
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = SILENCE;
		attack.target = target;
		attack.counter_ms = 1500.f;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
}

void SkillSystem::startHealAttack(Entity origin, Entity target) {
	printf("Started the heal attack\n");
	//prevPlayer = currPlayer;
	if (registry.enemies.has(origin)) {
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = HEAL;
		attack.target = target;

		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = HEAL;
		attack.target = target;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
}

void SkillSystem::startIceShardAttack(Entity origin, Entity target) {
	printf("Started the ice shard attack\n");
	if (registry.enemies.has(origin)) {
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = ICESHARD;
		attack.target = target;
		attack.counter_ms += 50.f;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = ICESHARD;
		attack.old_pos = mousePos;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
}

void SkillSystem::startFireballAttack(Entity origin) {
	printf("Started the fireball attack\n");
	if (registry.enemies.has(origin)) {
		//TODO
	}
	else if (registry.companions.has(origin)) {
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = FIREBALL;
		attack.counter_ms += 30.f;
		attack.old_pos = mousePos;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
}

void SkillSystem::startRockAttack(Entity origin, Entity target) {
	printf("Started the rock attack\n");
	if (registry.enemies.has(origin)) {
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = ROCK;
		attack.target = target;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = ROCK;
		attack.target = target;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
}

void SkillSystem::startLightningAttack(Entity origin, Entity target) {
	printf("Started the lightning attack\n");
	if (registry.enemies.has(origin)) {
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = LIGHTNING;
		attack.target = target;
		attack.counter_ms = 1100.f;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = LIGHTNING;
		attack.target = target;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
}

void SkillSystem::startMeleeAttack(Entity origin, Entity target) {
	printf("Started the melee attack\n");
	if (registry.enemies.has(origin)) {
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = WALKING;

		Motion& enemy_motion = registry.motions.get(origin);
		Motion target_motion = registry.motions.get(target);

		// Add enemy to the running component
		RunTowards& rt = registry.runners.emplace(origin);
		if (registry.hit_timer.has(origin)) {
			rt.old_pos = { enemy_motion.position.x - hit_position, enemy_motion.position.y };
		}
		else {
			rt.old_pos = enemy_motion.position;
		}

		rt.target = target;
		// Have some offset
		rt.target_position = { target_motion.position.x + 125, target_motion.position.y };

		// Change enemy's velocity
		float speed = 550.f;

		enemy_motion.velocity = { -speed,0.f };
		Motion& healthBar = registry.motions.get(enemy.healthbar);
		healthBar.velocity = enemy_motion.velocity;

		// Calculate the timer
		float time = (enemy_motion.position.x - rt.target_position.x) / speed;
		rt.counter_ms = time * 1000;

		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);

			if (enemy.enemyType == SWORDSMAN) {
				timer.counter_ms = rt.counter_ms + 1250.f + animation_timer;
			}
			else if (enemy.enemyType == NECROMANCER_MINION) {
				timer.counter_ms = rt.counter_ms + 800.f + animation_timer;
			}


		}

		//playerUseMelee = 1;
	}
	else if (registry.companions.has(origin)) {
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = WALKING;

		Motion& companion_motion = registry.motions.get(origin);
		Motion target_motion = registry.motions.get(target);

		// Add enemy to the running component
		RunTowards& rt = registry.runners.emplace(origin);
		if (registry.hit_timer.has(origin)) {
			rt.old_pos = { companion_motion.position.x + hit_position, companion_motion.position.y };
		}
		else {
			rt.old_pos = companion_motion.position;
		}

		rt.target = target;
		// Have some offset
		rt.target_position = { target_motion.position.x - 125, target_motion.position.y };

		// Change companion's velocity
		float speed = 550.f;

		companion_motion.velocity = { speed,0.f };
		Motion& healthBar = registry.motions.get(companion.healthbar);
		healthBar.velocity = companion_motion.velocity;

		// Calculate the timer
		float time = (rt.target_position.x - companion_motion.position.x) / speed;
		rt.counter_ms = time * 1000;

		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = rt.counter_ms + 1250.f + animation_timer;
		}

		//playerUseMelee = 1;
	}
}

void SkillSystem::startSummonAttack(Entity origin) {
	printf("Started the summon attack\n");
	if (registry.enemies.has(origin)) {
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = SUMMONING;
		attack.counter_ms = 1950.f;

		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
	}
}

void SkillSystem::startParticleBeamAttack(Entity origin, Entity target) {
	printf("Started the ultimate particle beam attack\n");
	if (registry.enemies.has(origin)) {
		beam_spell_sound = Mix_LoadWAV(audio_path("beam_spell.wav").c_str());
		if (beam_spell_sound != nullptr) {
			Mix_Volume(5, 32);
			Mix_PlayChannel(-1, beam_spell_sound, 0);
		}
		else {
			printf("soundEff failed loading");
		}

		Motion motion = registry.motions.get(origin);
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = ULTI;
		attack.target = target;
		attack.counter_ms = 2000.f;

		if (!registry.Particles.has(origin)) {
			Particle particleEffects;
			particleEffects.Life = 2500.f;
			particleEffects.areTypeDeath = false;
			particleEffects.motion.scale = vec2(9.f, 9.f);
			for (int p = 0; p < 4000; p++) {
				auto motion = registry.motions.get(origin);
				motion.position.y += 150.f;
				Particle particle;
				particle.Life = particleEffects.Life;
				// float random1 = ((rand() % 100) - 50) / 10.0f;
				// float random2 = ((rand() % 200) - 100) / 10.0f;

				std::random_device rdx; // obtain a random number from hardware
				std::mt19937 genX(rdx()); // seed the generator
				std::uniform_int_distribution<> distrX((int)motion.position.x - 50, (int)motion.position.x - 40); // define the range

				std::random_device rdy; // obtain a random number from hardware
				std::mt19937 genY(rdy()); // seed the generator
				std::uniform_int_distribution<> distrY((int)motion.position.y - 20, (int)motion.position.y + 20); // define the range


				float rColor = 0.5f + ((rand() % 100) / 100.0f);
				// particle.motion.position.x = motion.position.x + random1 + 20.f;
				// particle.motion.position.y = motion.position.y  + random2 - 40.f;
				particle.motion.position = vec2(distrX(genX), distrY(genY));

				particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
				// particle.motion.velocity.x = -100.f;

				std::uniform_int_distribution<> distrVelX(0, 1200); // define the range
				std::uniform_int_distribution<> distrVelY(-60, 60); // define the range
				particle.motion.velocity.x = (float)distrVelX(genX);
				particle.motion.velocity.y = (float)distrVelY(genY);
				// particle.motion.velocity.y = 0.f;
				// particle.angle = distrVelY(genY);

				// particle.motion.scale = vec2({ 10, 10 });
				particleEffects.deathParticles.push_back(particle);
				particleEffects.positions[p * 3 + 0] = particle.motion.position.x;
				particleEffects.positions[p * 3 + 1] = particle.motion.position.y;
				particleEffects.positions[p * 3 + 2] = particle.Life / particleEffects.Life;
			}
			registry.Particles.insert(origin, particleEffects);
		}

		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
	}
}

void SkillSystem::startParticleBeamCharge(Entity origin, Entity target) {
	printf("Started the particle beam charge\n");
	if (registry.enemies.has(origin)) {
		charge_spell_sound = Mix_LoadWAV(audio_path("charge_spell.wav").c_str());
		if (charge_spell_sound != nullptr) {
			Mix_Volume(5, 32);
			Mix_PlayChannel(-1, charge_spell_sound, 0);
		}
		else {
			printf("soundEff failed loading");
		}

		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = CHARGING;
		attack.target = target;
		attack.counter_ms = 3000.f;

		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
	}
}

Entity SkillSystem::launchIceShard(vec2 startPos, vec2 ms_pos, RenderSystem* renderer) {

	float proj_x = startPos.x + 50;
	float proj_y = startPos.y;
	float mouse_x = ms_pos.x;
	float mouse_y = ms_pos.y;

	float dx = mouse_x - proj_x;
	float dy = mouse_y - proj_y;
	float dxdy = sqrt((dx * dx) + (dy * dy));
	float vx = ICESHARDSPEED * dx / dxdy;
	float vy = ICESHARDSPEED * dy / dxdy;

	float angle = atan(dy / dx);
	if (dx < 0) {
		angle += M_PI;
	}
	//printf(" % f", angle);
	Entity resultEntity = createIceShard(renderer, { startPos.x + 50, startPos.y }, angle, { vx,vy }, 1);
	Motion* ballacc = &registry.motions.get(resultEntity);
	ballacc->acceleration = vec2(1000 * vx / ICESHARDSPEED, 1000 * vy / ICESHARDSPEED);
	Projectile* proj = &registry.projectiles.get(resultEntity);
	proj->flyingTimer = 2000.f;

	// ****temp**** enemy randomly spawn barrier REMOVED FOR NOW
	//int rng = rand() % 10;
	//if (rng >= 4) {
	//	createBarrier(renderer, registry.motions.get(enemy_mage).position);
	//}
	return  resultEntity;
}

void SkillSystem::launchHeal(Entity target, float amount,  RenderSystem* renderer) {
	vec2 targetp = registry.motions.get(target).position;
	createGreenCross(renderer, targetp);
	if (registry.stats.has(target)) {
		Statistics* tStats = &registry.stats.get(target);
		if (tStats->health + amount > tStats->max_health) {
			tStats->health = tStats->max_health;
		}
		else
		{
			tStats->health += amount;
		}
	}
	//update_healthBars();
}

void SkillSystem::luanchCompanionTeamHeal(float amount, RenderSystem* renderer) {
	for (Entity cp : registry.companions.entities) {
		vec2 targetp = registry.motions.get(cp).position;
		createGreenCross(renderer, targetp);
		if (registry.stats.has(cp)) {
			Statistics* tStats = &registry.stats.get(cp);
			if (tStats->health + amount > tStats->max_health) {
				tStats->health = tStats->max_health;
			}
			else
			{
				tStats->health += amount;
			}
		}
	}

}

void SkillSystem::luanchEnemyTeamDamage(float amount, RenderSystem* renderer) {
	for (Entity cp : registry.enemies.entities) {
		vec2 targetp = registry.motions.get(cp).position;
		createGreenCross(renderer, targetp);
		if (registry.stats.has(cp)) {
			Statistics* tStats = &registry.stats.get(cp);
			tStats->health -= amount;
		}
	}

}

Entity SkillSystem::launchFireball(vec2 startPos, vec2 ms_pos, RenderSystem* renderer) {

	float proj_x = startPos.x + 50;
	float proj_y = startPos.y;
	float mouse_x = ms_pos.x;
	float mouse_y = ms_pos.y;

	float dx = mouse_x - proj_x;
	float dy = mouse_y - proj_y;
	float dxdy = sqrt((dx * dx) + (dy * dy));
	float vx = FIREBALLSPEED * dx / dxdy;
	float vy = FIREBALLSPEED * dy / dxdy;

	float angle = atan(dy / dx);
	if (dx < 0) {
		angle += M_PI;
	}
	Entity resultEntity = createFireBall(renderer, { startPos.x + 50, startPos.y }, angle, { vx,vy }, 1);
	Motion* arrowacc = &registry.motions.get(resultEntity);
	arrowacc->acceleration = vec2(200 * vx / FIREBALLSPEED, 200 * vy / FIREBALLSPEED);

	return  resultEntity;
}

Entity SkillSystem::launchRock(Entity target, RenderSystem* renderer) {
	int isFriendly = 1;
	vec2 targetp = registry.motions.get(target).position;
	if (registry.companions.has(target)) {
		isFriendly = 0;
	}
	Entity resultEntity = createRock(renderer, { targetp.x, targetp.y - 300 }, isFriendly);
	Projectile* proj = &registry.projectiles.get(resultEntity);
	proj->flyingTimer = 2000.f;
	return  resultEntity;
}

Entity SkillSystem::launchLightning(Entity target, RenderSystem* renderer) {
	int isFriendly = 1;
	vec2 targetp = registry.motions.get(target).position;
	if (registry.companions.has(target)) {
		isFriendly = 0;
	}
	Entity resultEntity = createLightning(renderer, { targetp.x, targetp.y - 350 }, isFriendly);
	Projectile* proj = &registry.projectiles.get(resultEntity);
	proj->flyingTimer = 2000.f;
	return  resultEntity;
}

void SkillSystem::launchTaunt(Entity target, RenderSystem* renderer) {
	if (!registry.taunts.has(target)) {
		registry.taunts.emplace(target);
		Taunt* t = &registry.taunts.get(target);
		t->duration = 4;
		createTauntIndicator(renderer, target);
		printf("taunted!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
	else {
		Taunt* t = &registry.taunts.get(target);
		t->duration = 4;
		printf("taunt extended!\n");
	}
}

void SkillSystem::removeTaunt(Entity target) {
	if (registry.taunts.has(target)) {
		registry.taunts.remove(target);
		for (int j = 0; j < registry.statsindicators.components.size(); j++) {
			if (registry.statsindicators.components[j].owner == target) {
				registry.remove_all_components_of(registry.statsindicators.entities[j]);
			}
		}
		printf("taunt removed!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
}

void SkillSystem::launchMelee(Entity target, RenderSystem* renderer) {
	printf("creating a melee skill\n");
	Motion enemy = registry.motions.get(target);
	if (registry.companions.has(target)) {
		Entity resultEntity = createMelee(renderer, { enemy.position.x, enemy.position.y }, 0);
	}
	else {
		Entity resultEntity = createMelee(renderer, { enemy.position.x, enemy.position.y }, 1);
	}
}

void SkillSystem::launchSilence(Entity target, RenderSystem* renderer) {
	if (!registry.silenced.has(target)) {
		vec2 targetp = registry.motions.get(target).position;
		createSilenceBubble(renderer, targetp);
		registry.silenced.emplace(target);
		Silenced* s = &registry.silenced.get(target);
		s->turns = 2;	// making it 2 so that it last one more turn when checkRound decrements it on the next turn
		printf("silenced!!!!!!!!!!!!!!! \n");
	}
	else {
		Silenced* s = &registry.silenced.get(target);
		s->turns = 2;	// making it 2 so that it last one more turn when checkRound decrements it on the next turn
		printf("silence extended!\n");
	}
}

void SkillSystem::removeSilence(Entity target) {
	if (registry.silenced.has(target)) {
		registry.silenced.remove(target);
		for (int j = 0; j < registry.statsindicators.components.size(); j++) {
			if (registry.statsindicators.components[j].owner == target) {
				registry.remove_all_components_of(registry.statsindicators.entities[j]);
			}
		}
		printf("silence removed!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
}

void SkillSystem::launchSummon(RenderSystem* renderer) {
	minion_spawn_sound = Mix_LoadWAV(audio_path("minion_spawn.wav").c_str());
	if (minion_spawn_sound != nullptr) {
		Mix_Volume(5, 32);
		Mix_PlayChannel(-1, minion_spawn_sound, 0);
	}
	else {
		printf("soundEff failed loading");
	}
	createNecromancerMinion(renderer, { 750, 600 });
	printf("summoned\n");
}

void SkillSystem::launchParticleBeam(Entity target) {
	for (int i = 0; i < registry.ultimate.components.size(); i++) {
		Entity e = registry.ultimate.entities[i];
		if (registry.motions.get(e).scale.x == PARTICLEBEAMCHARGE_WIDTH) {
			registry.remove_all_components_of(e);
		}
	}
	if (!registry.ultimate.has(target)) {
		registry.ultimate.emplace(target);
		Ultimate* u = &registry.ultimate.get(target);
		u->ultiDuration = 4;
	}
	else {		
		Ultimate* u = &registry.ultimate.get(target);
		u->ultiDuration = 4;
	}
}

std::pair<bool, bool> SkillSystem::updateParticleBeam(Entity& origin, float elapsed_ms_since_last_update, float w, float h) {
	std::pair<bool, bool> updateHealthSignals = {false, false};
	if (registry.Particles.has(origin)) {
		auto& originMotion = registry.motions.get(origin);
		Particle& particles = registry.Particles.get(origin);
		for (int i = 0; i < particles.deathParticles.size(); i++) {
			auto& particle = particles.deathParticles[i];
			particle.Life -= elapsed_ms_since_last_update * 0.5;

			if (particle.Life <= 0.f) {
				particles.fadedParticles++;
				delete[] particle.positions;
			}
			float step_seconds = 1.0f * (elapsed_ms_since_last_update / 1000.f);
			particle.motion.position.x -= particle.motion.velocity.x * step_seconds;
			if (particle.motion.position.x <= w / 1.50) {
				particle.motion.position.y -= particle.motion.velocity.y * step_seconds * (float)0.8;
			}
			if (!updateHealthSignals.first) {
				for (auto& companion : registry.companions.entities) {
					auto& motion = registry.motions.get(companion);
					if (abs(motion.position.x - particle.motion.position.x) <= 2) {
						updateHealthSignals.first = true;
						break;
					}
				}
			}
			if (particle.Life / particles.Life < 0.85) {	// adjust 0.85 to change damage, up the value for lower damage
				updateHealthSignals.second = true;
			}

			particles.positions[i * 3 + 0] = particle.motion.position.x;
			particles.positions[i * 3 + 1] = particle.motion.position.y;
			particles.positions[i * 3 + 2] = particle.Life/ particles.Life;
		}

		if (particles.fadedParticles == 4000) {
			delete[] particles.positions;
			particles.faded = true;
			registry.Particles.remove(origin);
		}
	}
	return updateHealthSignals;
}

void SkillSystem::removeUltimate(Entity target) {
	if (registry.ultimate.has(target)) {
		registry.ultimate.remove(target);
		for (int j = 0; j < registry.statsindicators.components.size(); j++) {
			if (registry.statsindicators.components[j].owner == target) {
				registry.remove_all_components_of(registry.statsindicators.entities[j]);
			}
		}
	}
}

Entity SkillSystem::launchParticleBeamCharge(Entity target, RenderSystem* renderer) {
	printf("target is %g \n", float(registry.stats.get(target).speed));
	vec2 targetp = registry.motions.get(target).position;
	Entity resultEntity = createParticleBeamCharge(renderer, { targetp.x, targetp.y });
	registry.ultimate.emplace(resultEntity);
	return  resultEntity;
}