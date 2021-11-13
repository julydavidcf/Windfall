#include "skill_system.hpp"



SkillSystem::SkillSystem() {
}

SkillSystem::~SkillSystem() {
}



void SkillSystem::startTauntAttack(Entity origin, Entity target) {
	printf("Started the taunt attack\n");
	if (registry.enemies.has(origin)) {
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
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = SILENCE;
		attack.target = target;
		attack.counter_ms = 500.f;	// TODO animation too quick for the timer, repeats on loop
		if (!registry.checkRoundTimer.has(currPlayer)) {
			auto& timer = registry.checkRoundTimer.emplace(currPlayer);
			timer.counter_ms = attack.counter_ms + animation_timer;
		}
	}
	else if (registry.companions.has(origin)) {
		silence_spell_sound = Mix_LoadWAV(audio_path("silence_spell.wav").c_str());
		if (silence_spell_sound != nullptr) {
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
	printf("Started the rock attack\n");
	if (registry.enemies.has(origin)) {
		Enemy& enemy = registry.enemies.get(origin);
		enemy.curr_anim_type = ATTACKING;
		Attack& attack = registry.attackers.emplace(origin);
		attack.attack_type = LIGHTNING;
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
			timer.counter_ms = rt.counter_ms + 1250.f + animation_timer;
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
		t->duration = 4;	// making it 4 so that it last one more turn when checkRound decrements it on the next turn
		createTauntIndicator(renderer, target);
		printf("taunted!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
	else {
		Taunt* t = &registry.taunts.get(target);
		t->duration = 4;	// making it 4 so that it last one more turn when checkRound decrements it on the next turn
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
