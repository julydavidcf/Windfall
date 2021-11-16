#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

Entity createPlayerMage(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MAGE_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ MAGE_WIDTH, MAGE_HEIGHT });

	// Give statistics to companion mage
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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MAGE_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { -MAGE_WIDTH, MAGE_HEIGHT };

	// Give statistics to enemy mage
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

Entity createPlayerSwordsman(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ SWORDSMAN_WIDTH, SWORDSMAN_HEIGHT });

	// Give statistics to companion swordsman
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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ -SWORDSMAN_WIDTH, SWORDSMAN_HEIGHT });

	// Give statistics to enemy swordsman
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

Entity createNecromancerMinion(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::NECRO_MINION_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ -NECRO_MINION_WIDTH, NECRO_MINION_HEIGHT });

	// Give statistics to necromancer minion
	Statistics& stat = registry.stats.emplace(entity);
	stat.health = 100;
	stat.speed = 1;

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, pos);
	enemy.enemyType = NECROMANCER_MINION;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::NECRO_MINION_IDLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::NECRO_MINION_IDLE });

	return entity;
}


Entity createNecromancerPhaseOne(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::NECRO_ONE_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ -NECRO_ONE_WIDTH, NECRO_ONE_HEIGHT });

	// Give statistics to necromancer phase one
	Statistics& stat = registry.stats.emplace(entity);
	stat.health = 100;
	stat.speed = 0;

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, pos);
	enemy.enemyType = NECROMANCER_ONE;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::NECRO_ONE_IDLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::NECRO_ONE_IDLE });

	return entity;
}

Entity createNecromancerPhaseTwo(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::NECRO_TWO_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ -NECRO_TWO_WIDTH, NECRO_TWO_HEIGHT });

	// Give statistics to necromancer phase two
	Statistics& stat = registry.stats.emplace(entity);
	stat.health = 100;
	stat.speed = 2;

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, pos);
	enemy.enemyType = NECROMANCER_TWO;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::NECRO_TWO_IDLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::NECRO_TWO_IDLE });

	return entity;
}

Entity createFireBall(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, int isFriendly)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	auto & gravity = registry.gravities.emplace(entity);
	gravity.gravity = 30;

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.buttons.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.turnIndicators.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.turnIndicators.emplace(entity);

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

Entity createCharIndicator(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.charIndicator.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	position[1] -= 150;
	motion.position = position;
	motion.scale = vec2({ CHARARROW_WIDTH, CHARARROW_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CHARARROW,	//https://pixelartmaker-data-78746291193.nyc3.digitaloceanspaces.com/image/db113ed0e206163.png
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

// create barrier
Entity createBarrier(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.reflects.emplace(entity);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	auto*statid =  &registry.statsindicators.emplace(entity);
	statid->owner = owner;

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

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = {0.f,50};
	motion.acceleration = { 0.f,1000 };
	motion.position = position;
	motion.scale = vec2({ ROCK_WIDTH, ROCK_HEIGHT });

	auto& proj = registry.projectiles.emplace(entity);
	proj.enableCameraTracking = 0;

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

//https://pngtree.com/freepng/lightning-design-lightning-effect-natural-phenomenon-lightning-source_3916935.html
// create lightning
Entity createLightning(RenderSystem* renderer, vec2 position, int isFriendly)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 50 };
	motion.acceleration = { 0.f, 200 };
	motion.position = position;
	motion.scale = vec2({ LIGHTNING_WIDTH, LIGHTNING_HEIGHT });

	auto& proj = registry.projectiles.emplace(entity);
	proj.enableCameraTracking = 0;

	// Set damage here--------------------------------
	Damage& damage = registry.damages.emplace(entity);
	damage.isFriendly = isFriendly;
	damage.minDamage = lightning_dmg;
	damage.range = 10;
	//------------------------------------------------

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::LIGHTNING,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}


Entity createMelee(RenderSystem* renderer, vec2 position, int isFriendly)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

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
	motion.velocity = { 0.f, -500.f };
	position[1] -= 30;
	position[0] += 70;
	motion.position = position;
	motion.scale = vec2({ SILENCEBUBBLE_WIDTH, SILENCEBUBBLE_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SILENCEBUBBLE,	//https://octopathtraveler.fandom.com/wiki/Silence?file=Status_Silence.png
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

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = size;

	registry.companions.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::PEBBLE,
			GEOMETRY_BUFFER_ID::PEBBLE });

	return entity;
}

Entity createTutorialBox(RenderSystem* renderer, vec2 position, int box_number) {
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { TUTORIAL_BOX_WIDTH, TUTORIAL_BOX_HEIGHT };

	TEXTURE_ASSET_ID tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_ONE;

	switch (box_number) {
		case 0: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_ONE; break;
		case 1: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_TWO; break;
		case 2: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_THREE; break;
		case 3: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_FOUR; break;
		case 4: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_FIVE; break;
		case 5: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_SIX; break;
		case 6: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_SEVEN; break;
		case 7: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_EIGHT; break;
		default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ tutorial_box_num,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createTooltip(RenderSystem* renderer, vec2 position, std::string type) {
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { 550.f, 150.f };
	registry.toolTip.emplace(entity);

	TEXTURE_ASSET_ID tooltipType = TEXTURE_ASSET_ID::FIREBALLTOOLTIP;

	if (type == "FB") {
		tooltipType = TEXTURE_ASSET_ID::FIREBALLTOOLTIP;
	}
	else if (type == "IS") {
		tooltipType = TEXTURE_ASSET_ID::ICESHARDTOOLTIP;
	}
	else if (type == "RK") {
		tooltipType = TEXTURE_ASSET_ID::ROCKTOOLTIP;
	}
	else if (type == "HL") {
		tooltipType = TEXTURE_ASSET_ID::HEALTOOLTIP;
	}
	else if (type == "TT") {
		tooltipType = TEXTURE_ASSET_ID::TAUNTTOOLTIP;
	}
	else if (type == "ML") {
		tooltipType = TEXTURE_ASSET_ID::MELEETOOLTIP;
	}

	registry.renderRequests.insert(
		entity,
		{ tooltipType,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}