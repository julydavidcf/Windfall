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
	Statistics& stat = registry.stats.emplace(entity);
	stat.health = player_swordsman_hp;
	stat.speed = 14;
	stat.classID = 0;
	

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
	motion.angle = 0;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { -MAGE_WIDTH, MAGE_HEIGHT };

	// Give hp to enemy
	Statistics& stat = registry.stats.emplace(entity);
	stat.max_health = enemy_mage_hp;
	stat.speed = 13;

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

Entity createBackgroundObject(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::BACKGROUND_OBJ);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { -BACKGROUND_OBJ_WIDTH, -BACKGROUND_OBJ_WIDTH };


	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::BACKGROUND_OBJ,
			GEOMETRY_BUFFER_ID::BACKGROUND_OBJ });

	registry.backgroundObjects.insert(entity, {});
	// renderer->enemyMage = entity;
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
	Statistics& stat = registry.stats.emplace(entity);
	stat.health = player_swordsman_hp;
	stat.speed = 12;
	stat.classID = 1;

	// Add a healthbar
	Companion& companion = registry.companions.emplace(entity);
	companion.healthbar = createHealthBar(renderer, pos);
	companion.companionType = SWORDSMAN;

	auto& abc = registry.renderRequests.insert(
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
	Statistics& stat = registry.stats.emplace(entity);
	stat.health = enemy_swordsman_hp;
	stat.speed = 11;

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
	Statistics& stat = registry.stats.emplace(entity);
	stat.health = 100;
	stat.speed = 100;

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

Entity createFireBall(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, int isFriendly)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	auto & gravity = registry.gravities.emplace(entity);
	gravity.gravity = 30;

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.velocity = velocity;
	motion.position = position;


	motion.scale = vec2({ FIREBALL_WIDTH, FIREBALL_HEIGHT });


	// Set damage here--------------------------------
	Damage& damage = registry.damages.emplace(entity);
	damage.isFriendly = isFriendly;
	damage.minDamage = fireball_dmg;
	damage.range = 10;
	//------------------------------------------------


	registry.projectiles.emplace(entity);
	registry.renderRequests.insert(
		entity,
		//currently using fireball
		{ TEXTURE_ASSET_ID::FIREBALL,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createIceShard(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, int isFriendly)
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


	motion.scale = vec2({ ICESHARD_WIDTH, ICESHARD_HEIGHT });


	// Set damage here--------------------------------
	Damage& damage = registry.damages.emplace(entity);
	damage.isFriendly = isFriendly;
	damage.minDamage = iceshard_dmg;
	damage.range = 10;
	//------------------------------------------------


	registry.projectiles.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ICESHARD,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createIceShardIcon(RenderSystem* renderer, vec2 position)
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

	motion.scale = vec2({ ICON_WIDTH, ICON_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ICESHARDICON,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createFireballIcon(RenderSystem* renderer, vec2 position)
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

	motion.scale = vec2({ ICON_WIDTH, ICON_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::FIREBALLICON,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

//melee icon

Entity createMeleeIcon(RenderSystem* renderer, vec2 position)
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

	motion.scale = vec2({ ICON_WIDTH, ICON_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MELEEICON,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

//taunt icon

Entity createTauntIcon(RenderSystem* renderer, vec2 position)
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

	motion.scale = vec2({ ICON_WIDTH, ICON_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TAUNTICON,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

//heal icon

Entity createHealIcon(RenderSystem* renderer, vec2 position)
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

	motion.scale = vec2({ ICON_WIDTH, ICON_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::HEALICON,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

//rock icon

Entity createRockIcon(RenderSystem* renderer, vec2 position)
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

	motion.scale = vec2({ ICON_WIDTH, ICON_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ROCKICON,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createSilenceIcon(RenderSystem* renderer, vec2 position)
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

	motion.scale = vec2({ SILENCE_ICON_WIDTH, SILENCE_ICON_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SILENCEICON,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createSilenceIconSelected(RenderSystem* renderer, vec2 position)
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
		{ TEXTURE_ASSET_ID::SILENCEICONSELECTED,
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
Entity createGreenCross(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, -500.f };
	motion.acceleration = { 0.f, 0.f };
	motion.position = position;

	motion.scale = vec2({ GREENCROSS_WIDTH, GREENCROSS_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::GREENCROSS,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createTauntIndicator(RenderSystem* renderer, Entity owner)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	auto*statid =  &registry.statsindicators.emplace(entity);
	statid->owner = owner;

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.acceleration = { 0.f, 0.f };
	motion.position = {0.f,0.f};

	motion.scale = vec2({ GREENCROSS_WIDTH, GREENCROSS_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TAUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

// create rock
Entity createRock(RenderSystem* renderer, vec2 position, int isFriendly)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = {0.f,50};
	motion.acceleration = { 0.f,1000 };
	motion.position = position;


	motion.scale = vec2({ ROCK_WIDTH, ROCK_HEIGHT });

	registry.projectiles.emplace(entity);

	// Set damage here--------------------------------
	Damage& damage = registry.damages.emplace(entity);
	damage.isFriendly = isFriendly;
	damage.minDamage = rock_dmg;
	damage.range = 10;
	//------------------------------------------------

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ROCK,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

// create rock
Entity createMelee(RenderSystem* renderer, vec2 position, int isFriendly)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f,0 };
	motion.acceleration = { 0.f,0 };
	motion.position = position;


	motion.scale = vec2({ 1, 1 });


	// Set damage here--------------------------------
	Damage& damage = registry.damages.emplace(entity);
	damage.isFriendly = isFriendly;
	damage.minDamage = melee_dmg;
	damage.range = 10;
	//------------------------------------------------


	registry.projectiles.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ROCK,
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

Entity createSilenceBubble(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	position[1] -= 30;
	position[0] += 70;
	motion.position = position;

	motion.scale = vec2({ SILENCEBUBBLE_WIDTH, SILENCEBUBBLE_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SILENCEBUBBLE,
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

Entity createBackgroundLayerOne(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.scale = vec2({ BACKGROUND_WIDTH, BACKGROUND_HEIGHT });

	auto& backgroundLayer = registry.backgroundLayers.emplace(entity);
	backgroundLayer.isAutoScroll = 1;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BACKGROUNDLAYERONE,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createBackgroundLayerTwo(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.scale = vec2({ BACKGROUND_WIDTH, BACKGROUND_HEIGHT });

	auto& backgroundLayer = registry.backgroundLayers.emplace(entity);
	backgroundLayer.isCameraScrollOne = 1;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BACKGROUNDLAYERTWO,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createBackgroundLayerThree(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.scale = vec2({ BACKGROUND_WIDTH, BACKGROUND_HEIGHT });

	auto& backgroundLayer = registry.backgroundLayers.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BACKGROUNDLAYERTHREE,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createBackgroundLayerFour(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.scale = vec2({ BACKGROUND_WIDTH, BACKGROUND_HEIGHT });

	auto& backgroundLayer = registry.backgroundLayers.emplace(entity);
	backgroundLayer.isCameraScrollTwo = 1;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BACKGROUNDLAYERFOUR,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

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

Entity createTooltip(RenderSystem* renderer, vec2 position, std::string type) {
	auto entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	if (type == "FB") {
		motion.scale = { 550.f, 150.f };
		registry.toolTip.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::FIREBALLTOOLTIP,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == "IS") {
		motion.scale = { 550.f, 150.f };
		registry.toolTip.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::ICESHARDTOOLTIP,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == "RK") {
		motion.scale = { 550.f, 150.f };
		registry.toolTip.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::ROCKTOOLTIP,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == "HL") {
		motion.scale = { 550.f, 150.f };
		registry.toolTip.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::HEALTOOLTIP,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == "TT") {
		motion.scale = { 550.f, 150.f };
		registry.toolTip.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::TAUNTTOOLTIP,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == "ML") {
		motion.scale = { 550.f, 150.f };
		registry.toolTip.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MELEETOOLTIP,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}


	return entity;
}