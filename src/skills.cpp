#include "skills.hpp"
#include "world_system.hpp"



Skills::Skills() {



}

Skills::~Skills() {

}



void Skills::startTauntAttack(Entity origin, Entity target) {
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


void Skills::startHealAttack(Entity origin, Entity target) {
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

void Skills::startIceShardAttack(Entity origin, Entity target) {
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

void Skills::startFireballAttack(Entity origin) {
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

void Skills::startRockAttack(Entity origin, Entity target) {
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


void Skills::startMeleeAttack(Entity origin, Entity target) {
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

		playerUseMelee = 1;
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

		playerUseMelee = 1;
	}
}