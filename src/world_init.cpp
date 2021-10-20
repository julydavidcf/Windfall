#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

Entity createPlayerMage(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MAGE_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ MAGE_WIDTH, MAGE_HEIGHT });

	// Give hp to companion
	registry.healthPoints.emplace(entity);

	// Add a healthbar
	Companion& companion = registry.companions.emplace(entity);
	companion.healthbar = createHealthBar(renderer, pos);
	companion.companionType = MAGE;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MAGE_ANIM,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::MAGE_IDLE });

	return entity;
}

Entity createEnemyMage(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MAGE_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { -MAGE_WIDTH, MAGE_HEIGHT };

	// Give hp to enemy
	registry.healthPoints.emplace(entity);

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, pos);
	enemy.enemyType = MAGE;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MAGE_ANIM,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::MAGE_IDLE });

	return entity;
}

Entity createPlayerSwordsman(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ SWORDSMAN_WIDTH, SWORDSMAN_HEIGHT });

	// Give hp to enemy
	registry.healthPoints.emplace(entity);

	// Add a healthbar
	Companion& companion = registry.companions.emplace(entity);
	companion.healthbar = createHealthBar(renderer, pos);
	companion.companionType = SWORDSMAN;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SWORDSMAN_IDLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE });

	return entity;
}

Entity createEnemySwordsman(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ -SWORDSMAN_WIDTH, SWORDSMAN_HEIGHT });

	// Give hp to enemy
	registry.healthPoints.emplace(entity);

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, pos);
	enemy.enemyType = SWORDSMAN;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SWORDSMAN_IDLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE });

	return entity;
}

Entity createNecromancer(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::NECROMANCER_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ -NECROMANCER_WIDTH, NECROMANCER_HEIGHT });

	// Give hp to enemy
	registry.healthPoints.emplace(entity);

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, pos);
	enemy.enemyType = NECROMANCER;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::NECROMANCER_IDLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::NECROMANCER_IDLE });

	return entity;
}

Entity createFireball(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, int isFriendly)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.velocity = velocity;
	motion.position = position;


	motion.scale = vec2({ FIREBALL_WIDTH, FIREBALL_HEIGHT });


	// Set damage here--------------------------------
	Damage& damage = registry.damages.emplace(entity);
	damage.isFriendly = isFriendly;
	damage.minDamage = 30;
	damage.range = 10;
	//------------------------------------------------


	registry.projectiles.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::FIREBALL,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createFireballIcon(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	motion.scale = vec2({ FIREBALL_ICON_WIDTH, FIREBALL_ICON_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::FIREBALLICON,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createFireballIconSelected(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh o	bject (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	motion.scale = vec2({ FIREBALL_ICON_WIDTH, FIREBALL_ICON_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::FIREBALLICONSELECTED,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}


//turn indicators
Entity createPlayerTurn(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.turnIndicators.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = position;

	motion.scale = vec2({ PLAYERTURN_WIDTH, PLAYERTURN_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER_TURN,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createEnemyTurn(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.turnIndicators.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = position;

	motion.scale = vec2({ ENEMYTURN_WIDTH, ENEMYTURN_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ENEMY_TURN,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}


// create barrier
Entity createBarrier(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.reflects.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { -500.f, 0.f };
	motion.acceleration = { 300.f, 0.f };
	motion.position = position;

	motion.scale = vec2({ BARRIER_WIDTH, BARRIER_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BARRIER,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createHealthBar(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	position[1] -= 75;
	motion.position = position;

	motion.scale = vec2({ HEALTHBAR_WIDTH, HEALTHBAR_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::HEALTHBAR,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createLine(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::PEBBLE,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE });

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	registry.debugComponents.emplace(entity);
	return entity;
}

Entity createPebble(vec2 pos, vec2 size)
{
	auto entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = size;

	// Create and (empty) Salmon component to be able to refer to all turtles
	registry.companions.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::PEBBLE,
			GEOMETRY_BUFFER_ID::PEBBLE });

	return entity;
}