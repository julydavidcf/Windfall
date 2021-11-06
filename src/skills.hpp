#include <vector>
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_system.hpp"

class Skills
{
public:
	Skills();
	~Skills();
	

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

	WorldSystem ws;

	void startTauntAttack(Entity origin, Entity target);
	void startIceShardAttack(Entity origin, Entity target);
	void startFireballAttack(Entity origin);
	void startRockAttack(Entity origin, Entity target);
	void startHealAttack(Entity origin, Entity target);
	void startMeleeAttack(Entity origin, Entity target);
};