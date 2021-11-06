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
		attack.old_pos = ws.getMouse();
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
		attack.old_pos = ws.getMouse();
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
