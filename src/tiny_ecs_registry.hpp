#pragma once
#include <vector>

#include "tiny_ecs.hpp"
#include "components.hpp"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

class ECSRegistry
{
	// Callbacks to remove a particular or all entities in the system
	std::vector<ContainerInterface*> registry_list;

public:
	// Manually created list of all components this game has
	ComponentContainer<BackgroundObj> deformableEntities;
	ComponentContainer<ParticlePool> particlePools;
	ComponentContainer<SwarmParticle> fireflySwarm;
	ComponentContainer<DeathTimer> deathTimers;
	ComponentContainer<Motion> motions;
	ComponentContainer<Collision> collisions;
	ComponentContainer<Companion> companions;
	ComponentContainer<Mesh*> meshPtrs;
	ComponentContainer<BackgroundLayer> backgroundLayers;
	ComponentContainer<RenderRequest> renderRequests;
	ComponentContainer<ScreenState> screenStates;
	ComponentContainer<Projectile> projectiles;
	ComponentContainer<Enemy> enemies;
	ComponentContainer<Damage> damages;
	ComponentContainer<Silenced> silenced;
	ComponentContainer<Statistics> stats;
	ComponentContainer<DebugComponent> debugComponents;
	ComponentContainer<vec3> colors;
	ComponentContainer<ButtonItem> buttons;
	ComponentContainer<HitTimer> hit_timer;
	ComponentContainer<Reflect> reflects;
	ComponentContainer<TurnIndicator> turnIndicators;
	ComponentContainer<Attack> attackers;
	ComponentContainer<RunTowards> runners;
	ComponentContainer<Gravity> gravities;
	ComponentContainer<Taunt> taunts;
	ComponentContainer<StatIndicator> statsindicators;
	ComponentContainer<CheckRoundTimer> checkRoundTimer;
	ComponentContainer<toolTip> toolTip;
	ComponentContainer<CharIndicator> charIndicator;
	ComponentContainer<UIButton> uiButtons;
	ComponentContainer<Ultimate> ultimate;
	ComponentContainer<Shield> shield;
	ComponentContainer<Dot> dots;
	ComponentContainer<storyTellingBackground> storyTellingBackgrounds;
	ComponentContainer<Bleed> bleeds;
	ComponentContainer<BleedIndicator> bleedIndicators;
	ComponentContainer<ShieldIcon> shieldIcons;

	// Sounds
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
	Mix_Chunk* button_hover_sound;
	Mix_Chunk* turning_sound;
	Mix_Chunk* charge_spell_sound;
	Mix_Chunk* beam_spell_sound;
	Mix_Chunk* minion_spawn_sound;
	Mix_Chunk* error_sound;
	Mix_Chunk* gesture_heal_sound;
	Mix_Chunk* gesture_aoe_sound;
	Mix_Chunk* gesture_turn_sound;

	// Values for swarm behaviour calculations
	float convergenceValue = 0.7; float c1 = 1; float c2 = 3;

	// constructor that adds all containers for looping over them
	// IMPORTANT: Don't forget to add any newly added containers!
	ECSRegistry()
	{
		registry_list.push_back(&deformableEntities);
		registry_list.push_back(&particlePools);
		registry_list.push_back(&deathTimers);
		registry_list.push_back(&motions);
		registry_list.push_back(&collisions);
		registry_list.push_back(&companions);
		registry_list.push_back(&meshPtrs);
		registry_list.push_back(&renderRequests);
		registry_list.push_back(&screenStates);
		registry_list.push_back(&projectiles);
		registry_list.push_back(&silenced);
		registry_list.push_back(&enemies);
		registry_list.push_back(&debugComponents);
		registry_list.push_back(&colors);
		registry_list.push_back(&buttons);
		registry_list.push_back(&hit_timer);
		registry_list.push_back(&reflects);
		registry_list.push_back(&turnIndicators);
		registry_list.push_back(&attackers);
		registry_list.push_back(&gravities);
		registry_list.push_back(&taunts);
		registry_list.push_back(&runners);
		registry_list.push_back(&checkRoundTimer);
		registry_list.push_back(&statsindicators);
		registry_list.push_back(&toolTip);
		registry_list.push_back(&charIndicator);
		registry_list.push_back(&uiButtons);
		registry_list.push_back(&ultimate);
		registry_list.push_back(&shield);
		registry_list.push_back(&dots);
		registry_list.push_back(&storyTellingBackgrounds);
		registry_list.push_back(&bleeds);
		registry_list.push_back(&bleedIndicators);
		registry_list.push_back(&shieldIcons);
	}

	void clear_all_components() {
		for (ContainerInterface* reg : registry_list)
			reg->clear();
	}

	void list_all_components() {
		printf("Debug info on all registry entries:\n");
		for (ContainerInterface* reg : registry_list)
			if (reg->size() > 0)
				printf("%4d components of type %s\n", (int)reg->size(), typeid(*reg).name());
	}

	void list_all_components_of(Entity e) {
		printf("Debug info on components of entity %u:\n", (unsigned int)e);
		for (ContainerInterface* reg : registry_list)
			if (reg->has(e))
				printf("type %s\n", typeid(*reg).name());
	}

	void remove_all_components_of(Entity e) {
		for (ContainerInterface* reg : registry_list)
			reg->remove(e);
	}
};

extern ECSRegistry registry;