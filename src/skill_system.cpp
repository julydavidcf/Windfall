#include "skill_system.hpp"

#include <random>

namespace {
	void initParticlesBuffer(ParticlePool& pool) {
		GLuint particles_position_buffer;
		glGenBuffers(1, &particles_position_buffer);
		pool.particles_position_buffer = particles_position_buffer;
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, (pool.size * 3 * sizeof(GLfloat)), NULL, GL_STREAM_DRAW);
		// pool.positions = new float[1000 * 3];
		// int a = 5;
	}
}

SkillSystem::SkillSystem() { }

SkillSystem::~SkillSystem() { }

void SkillSystem::startTauntAttack(Entity origin, Entity target) {
	printf("Started the taunt attack\n");
	if (registry.enemies.has(origin)) {
		Mix_PlayChannel(-1, registry.taunt_spell_sound, 0);
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
		Mix_PlayChannel(-1, registry.taunt_spell_sound, 0);

		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = TAUNT;
		attack.target = target;
		attack.counter_ms = 1500.f;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + 1500;
		}
	}
}

void SkillSystem::startSilenceAttack(Entity origin, Entity target) {
	printf("Started the silence attack\n");
	if (registry.enemies.has(origin)) {
		Mix_PlayChannel(-1, registry.silence_spell_sound, 0);
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
		Mix_PlayChannel(-1, registry.silence_spell_sound, 0);
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = SILENCE;
		attack.target = target;
		attack.counter_ms = 1500.f;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + 1500;
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
			timer.counter_ms = attack.counter_ms + 1500;
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
		attack.counter_ms += 50.f;	// attack.counter_ms is 250, animation_timer is 500
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer; // adjust this value to delay enemy attack
		}
	}
	else if (registry.companions.has(origin)) {
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = ICESHARD;
		attack.old_pos = mousePos;
		attack.counter_ms += 50.f;	// attack.counter_ms is 250, animation_timer is 500
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + 3000; // adjust this value to delay enemy attack
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
		attack.counter_ms += 50.f;	// attack.counter_ms is 250, animation_timer is 500
		attack.old_pos = mousePos;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + 3000; // adjust this value to delay enemy attack
		}
	}
}

void SkillSystem::startArrowAttack(Entity origin) {
	printf("Started the arrow attack\n");
	if (registry.enemies.has(origin)) {
		//TODO
	}
	else if (registry.companions.has(origin)) {
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = BATTLE_ARROW;
		attack.counter_ms += 400.f;	// attack.counter_ms is 250, animation_timer is 500
		attack.old_pos = mousePos;
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + 3000; // adjust this value to delay enemy attack
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
		attack.counter_ms += 50.f;	// attack.counter_ms is 250, animation_timer is 500
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer; // adjust this value to delay enemy attack
		}
	}
	else if (registry.companions.has(origin)) {
		Companion& companion = registry.companions.get(origin);
		companion.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = ROCK;
		attack.target = target;
		attack.counter_ms += 50.f;	// attack.counter_ms is 250, animation_timer is 500
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + 3000; // adjust this value to delay enemy attack
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
			timer.counter_ms = attack.counter_ms + 1500;
		}
	}
}

void SkillSystem::startMeleeAttack(Entity origin, Entity target, int bleedOrAOE) {
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

		if (enemy.enemyType == NECROMANCER_TWO) {
			rt.bleedOrAOE = bleedOrAOE;
			rt.counter_ms = 500.f;
		}

		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);

			if (enemy.enemyType == SWORDSMAN) {
				timer.counter_ms = rt.counter_ms + 3000.f + animation_timer;
			}
			else if (enemy.enemyType == NECROMANCER_MINION) {
				timer.counter_ms = rt.counter_ms + 3000.f + animation_timer;
			}
			else if (enemy.enemyType == NECROMANCER_TWO) {
				timer.counter_ms = rt.counter_ms + 3000.f + animation_timer;
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
			timer.counter_ms = rt.counter_ms + 2500.f + 1500;
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
		Mix_PlayChannel(-1, registry.beam_spell_sound, 0);

		Motion motion = registry.motions.get(origin);
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = ULTI;
		attack.target = target;
		attack.counter_ms = 2000.f;

		if (!registry.particlePools.has(origin)) {
			ParticlePool particlePool(1000);
			// particlePool.size = 1000;
			initParticlesBuffer(particlePool);
			particlePool.poolLife = 2500.f;
			particlePool.areTypeDeath = false;
			particlePool.motion.scale = vec2(9.f, 9.f);

			for (int p = 0; p < particlePool.size; p++) {
				auto originMotion = registry.motions.get(origin);
				originMotion.position.y += 150.f;
				Particle particle;
				particle.Life = particlePool.poolLife;

				std::random_device rdx; // obtain a random number from hardware
				std::mt19937 genX(rdx()); // seed the generator
				std::uniform_int_distribution<> distrX((int)originMotion.position.x - 50, (int)originMotion.position.x - 40); // define the range

				std::random_device rdy;
				std::mt19937 genY(rdy());
				std::uniform_int_distribution<> distrY((int)originMotion.position.y - 20, (int)originMotion.position.y + 20);

				float rColor = 0.5f + ((rand() % 100) / 100.0f);
				particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);

				particle.motion.position = vec2(distrX(genX), distrY(genY));

				std::uniform_int_distribution<> distrVelX(0, 1200);
				std::uniform_int_distribution<> distrVelY(-60, 60);
				particle.motion.velocity.x = (float)distrVelX(genX);
				particle.motion.velocity.y = (float)distrVelY(genY);

				particlePool.particles.push_back(particle);
				particlePool.positions[p * 3 + 0] = particle.motion.position.x;
				particlePool.positions[p * 3 + 1] = particle.motion.position.y;
				particlePool.positions[p * 3 + 2] = particle.Life / particlePool.poolLife;
			}
			registry.particlePools.insert(origin, particlePool);
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
		Mix_PlayChannel(-1, registry.charge_spell_sound, 0);

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


void SkillSystem::startShieldAttack(Entity origin) {
	if (registry.enemies.has(origin)) {
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = SHIELD;
		attack.counter_ms = 1000.f;

		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + 1500;
		}
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

void SkillSystem::launchNecroBarrier(Entity target, RenderSystem* renderer) {
	vec2 targetp = registry.motions.get(target).position;
	createBarrier(renderer, { targetp.x - 300 , targetp.y + 75 });

	if (!registry.shield.has(target)) {
		registry.shield.emplace(target);
		Shield* sh = &registry.shield.get(target);
		sh->shieldDuration = 3;
	}
	else {
		Shield* sh = &registry.shield.get(target);
		sh->shieldDuration = 3;
	}
}

void SkillSystem::luanchCompanionTeamHeal( float amount, RenderSystem* renderer) {
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
	for (Entity em : registry.enemies.entities) {
		vec2 targetp = registry.motions.get(em).position;
		createMeteorShower(renderer, { 1400, 0  }, 0);
		Mix_PlayChannel(-1, registry.gesture_aoe_sound, 0);
		if (registry.stats.has(em)) {
			Statistics* tStats = &registry.stats.get(em);
				tStats->health -= amount;
		}
	}

}

void SkillSystem::luanchNecroCompanionTeamBleed(RenderSystem* renderer) {
	for (Entity cp : registry.companions.entities) {
		launchBleed(cp, renderer);
	}
}

void SkillSystem::initializeFireballSpeed() {
	fireball_speed_x = horizontalResolution / 2.048;
	fireball_speed_y = horizontalResolution / 2.048;
}

Entity SkillSystem::launchFireball(vec2 startPos, vec2 ms_pos, RenderSystem* renderer) {

	float proj_x = startPos.x + 50;
	float proj_y = startPos.y;
	float mouse_x = ms_pos.x;
	float mouse_y = ms_pos.y;

	float dx = mouse_x - proj_x;
	float dy = mouse_y - proj_y;
	float dxdy = sqrt((dx * dx) + (dy * dy));
	float vx = fireball_speed_x * dx / dxdy;
	float vy = fireball_speed_y * dy / dxdy;

	float angle = atan(dy / dx);
	if (dx < 0) {
		angle += M_PI;
	}
	Entity resultEntity = createFireBall(renderer, { startPos.x + 50, startPos.y }, angle, { vx,vy }, 1);
	Motion* arrowacc = &registry.motions.get(resultEntity);
	arrowacc->acceleration = vec2(200 * vx / fireball_speed_x, 200 * vy / fireball_speed_y);

	return  resultEntity;
}

Entity SkillSystem::launchArrow(Entity start, vec2 ms_pos, RenderSystem* renderer, int isFreeRoam) {

	vec2 startPos = registry.motions.get(start).position;

	float proj_x = startPos.x + 50;
	float proj_y = startPos.y;
	float mouse_x = ms_pos.x;
	float mouse_y = ms_pos.y;

	float dx = mouse_x - proj_x;
	float dy = mouse_y - proj_y;
	float dxdy = sqrt((dx * dx) + (dy * dy));
	float vx = isFreeRoam ? (ARROWSPEED / 1.5) * dx / dxdy : ARROWSPEED * dx / dxdy;
	float vy = isFreeRoam ? (ARROWSPEED / 1.5) * dy / dxdy : ARROWSPEED * dy / dxdy;

	float angle = atan(dy / dx);
	if (dx < 0) {
		angle += M_PI;
	}
	Entity resultEntity = createArrow(renderer, { startPos.x, startPos.y }, angle, { vx,vy }, 1, isFreeRoam);
	Motion* arrowacc = &registry.motions.get(resultEntity);
	arrowacc->acceleration = vec2(200 * vx / ARROWSPEED, 200 * vy / ARROWSPEED);

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

Entity SkillSystem::launchSpike(Entity target, RenderSystem* renderer) {
	int isFriendly = 1;
	vec2 targetp = registry.motions.get(target).position;
	if (registry.companions.has(target)) {
		isFriendly = 0;
	}
	Entity resultEntity = createSpike(renderer, { targetp.x, targetp.y - 500 }, isFriendly);
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

void SkillSystem::launchBleed(Entity target, RenderSystem* renderer) {
	if (!registry.bleeds.has(target)) {
		registry.bleeds.emplace(target);
		Bleed* t = &registry.bleeds.get(target);
		t->duration = 3;	// making it 4 so that it last one more turn when checkRound decrements it on the next turn
		createBleedIndicator(renderer, target);
		printf("bleed!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
	else {
		Bleed* t = &registry.bleeds.get(target);
		t->duration = 3;	// making it 4 so that it last one more turn when checkRound decrements it on the next turn
		printf("bleed extended!\n");
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

void SkillSystem::removeBleed(Entity target) {
	if (registry.bleeds.has(target)) {
		registry.bleeds.remove(target);
		for (int j = 0; j < registry.bleedIndicators.components.size(); j++) {
			if (registry.bleedIndicators.components[j].owner == target) {
				registry.remove_all_components_of(registry.bleedIndicators.entities[j]);
			}
		}
		printf("bleed removed!!!!!!!!!!!!!!!!!!!!!!!\n");
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


void SkillSystem::launchBleedDMG(Entity target, RenderSystem* renderer) {
	printf("creating a bleed skill\n");
	Motion enemy = registry.motions.get(target);
	if (registry.companions.has(target)) {
		Entity resultEntity = createBleedDMG(renderer, { enemy.position.x, enemy.position.y }, 0);
	}
	else {
		Entity resultEntity = createBleedDMG(renderer, { enemy.position.x, enemy.position.y }, 1);
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
		printf("silence removed!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
}

void SkillSystem::launchSummon(RenderSystem* renderer) {
	Mix_PlayChannel(-1, registry.minion_spawn_sound, 0);

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
	if (registry.particlePools.has(origin)) {
		// auto& originMotion = registry.motions.get(origin);
		ParticlePool& pool = registry.particlePools.get(origin);
		for (int i = 0; i < pool.particles.size(); i++) {
			auto& particle = pool.particles[i];
			particle.Life -= elapsed_ms_since_last_update * 0.5;

			if (particle.Life <= 0.f) {
				pool.fadedParticles++;
			}

			float step_seconds = 1.0f * (elapsed_ms_since_last_update / 1000.f);
			particle.motion.position.x -= particle.motion.velocity.x * step_seconds;
			if (particle.motion.position.x <= w / 1.50) {
				particle.motion.position.y -= particle.motion.velocity.y * step_seconds * (float)0.8;
			}
			if (!updateHealthSignals.first) {
				for (auto& companion : registry.companions.entities) {
					auto& companionMotion = registry.motions.get(companion);
					if (abs(companionMotion.position.x - particle.motion.position.x) <= 2) {
						updateHealthSignals.first = true;
						break;
					}
				}
			}
			if (particle.Life / pool.poolLife < 0.8) {	// adjust 0.8 to change damage, up the value for lower damage
				updateHealthSignals.second = true;
			}

			pool.positions[i * 3 + 0] = particle.motion.position.x;
			pool.positions[i * 3 + 1] = particle.motion.position.y;
			pool.positions[i * 3 + 2] = particle.Life/ pool.poolLife;
		}

		if (pool.fadedParticles == pool.size) {
			delete[] pool.positions;
			pool.faded = true;
			registry.particlePools.remove(origin);
		}
	}
	return updateHealthSignals;
}

void SkillSystem::removeUltimate(Entity target) {
	if (registry.ultimate.has(target)) {
		registry.ultimate.remove(target);
	}
}

Entity SkillSystem::launchParticleBeamCharge(Entity target, RenderSystem* renderer) {
	printf("target is %g \n", float(registry.stats.get(target).speed));
	vec2 targetp = registry.motions.get(target).position;
	Entity resultEntity = createParticleBeamCharge(renderer, { targetp.x, targetp.y });
	registry.ultimate.emplace(resultEntity);
	return  resultEntity;
}

void SkillSystem::removeShield(Entity target) {
	if (registry.shield.has(target)) {
		registry.shield.remove(target);
	}
}