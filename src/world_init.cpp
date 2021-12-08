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
	stat.health = registry.player_mage_hp;
	stat.max_health = registry.player_mage_hp;
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

Entity createBird(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::DRAGON_FLYING);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ -DRAGON_WIDTH, DRAGON_HEIGHT });

	registry.bird.emplace(entity);

	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.enemyType = DRAGON;
	enemy.curr_anim_type = IDLE;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::DRAGON_FLYING,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::DRAGON_FLYING });

	return entity;
}

void createSpline(RenderSystem* renderer, std::vector<vec3> points)
{
	for (auto& point : points) {
		auto entity = Entity();

		Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
		registry.meshPtrs.emplace(entity, &mesh);

		Motion& motion = registry.motions.emplace(entity);
		motion.position = vec2(point.x, point.y);
		motion.angle = 0.f;
		motion.velocity = { 0.f, 0.f };
		motion.scale = vec2({ 10, 10 });

		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::RED_PARTICLE,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
	}
}

Entity createEnemyMage(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MAGE_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { -MAGE_WIDTH, MAGE_HEIGHT };

	// Give statistics to enemy mage
	Statistics& stat = registry.stats.emplace(entity);
	stat.max_health = registry.enemy_mage_hp;
	stat.health = registry.enemy_mage_hp;
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
	motion.scale = { -BACKGROUND_OBJ_WIDTH, -BACKGROUND_OBJ_WIDTH};


	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::BACKGROUND_OBJ,
			GEOMETRY_BUFFER_ID::BACKGROUND_OBJ });

	registry.deformableEntities.insert(entity, {});
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
	stat.health = registry.player_swordsman_hp;
	stat.max_health = registry.player_swordsman_hp;
	stat.speed = 11;
	stat.classID = 1;

	// Add a healthbar
	Companion& companion = registry.companions.emplace(entity);
	companion.healthbar = createHealthBar(renderer, { pos.x, pos.y - 20 });
	companion.companionType = SWORDSMAN;

	auto& abc = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SWORDSMAN_IDLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE });

	return entity;
}

Entity createPlayerArcher(RenderSystem* renderer, vec2 pos, int isFreeRoamArcher)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::ARCHER_IDLE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = isFreeRoamArcher ? vec2(ARCHER_WIDTH, window_height_px - ARCHER_HEIGHT) : pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = vec2({ ARCHER_WIDTH, ARCHER_HEIGHT });

	if (!isFreeRoamArcher) {
		// Add battle-system related stats
		Statistics& stat = registry.stats.emplace(entity);
		stat.health = registry.player_archer_hp;
		stat.max_health = registry.player_archer_hp;
		stat.speed = 12;
		stat.classID = 2;

		// Add a healthbar
		Companion& companion = registry.companions.emplace(entity);
		companion.healthbar = createHealthBar(renderer, { pos.x, pos.y - 20 });
		companion.companionType = ARCHER;
		companion.curr_anim_type = IDLE;
	}
	else {
		// No heathbar for free-roam archer
		Companion& companion = registry.companions.emplace(entity);
		companion.companionType = ARCHER;
		companion.curr_anim_type = IDLE;

		// Add gravity for jumping
		auto& gravity = registry.gravities.emplace(entity);
		gravity.gravity = 30;

		// Smaller
		motion.scale = vec2({ ARCHER_FREEROAM_WIDTH, ARCHER_FREEROAM_HEIGHT });
	}

	auto& abc = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ARCHER_ANIMS,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::ARCHER_IDLE });

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
	stat.health = registry.enemy_swordsman_hp;
	stat.max_health = registry.enemy_swordsman_hp;
	stat.speed = 10;

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, { pos.x, pos.y - 20 });
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
	stat.health = registry.necro_minion_health;
	stat.max_health = registry.necro_minion_health;
	stat.speed = 0;

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, {pos.x, pos.y - motion.scale.y/2});
	enemy.enemyType = NECROMANCER_MINION;

	// Minion should use appearing anim initially
	enemy.curr_anim_type = APPEARING;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::NECRO_MINION_APPEAR,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::NECRO_MINION_APPEAR });

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
	stat.health = registry.necro_1_health;
	stat.max_health = registry.necro_1_health;
	stat.speed = 1;

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, { pos.x, pos.y - motion.scale.y / 2 });
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
	stat.health = registry.necro_2_health;
	stat.max_health = registry.necro_2_health;
	stat.speed = 2;

	// Add a healthbar
	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.healthbar = createHealthBar(renderer, { pos.x, pos.y - motion.scale.y / 4 });
	enemy.enemyType = NECROMANCER_TWO;

	// Emplace ultimate component to delay ultimate attack
	Ultimate& u = registry.ultimate.emplace(entity);
	u.ultiDuration = 2;	// allows necro2 to use ultimate after one round

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
	damage.minDamage = registry.fireball_dmg;
	damage.range = 10;
	//------------------------------------------------

	registry.projectiles.emplace(entity);
	registry.FireBalls.emplace(entity);
	registry.renderRequests.insert(
		entity,
		//currently using fireball
		{ TEXTURE_ASSET_ID::FIREBALL,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createArrow(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, int isFriendly, int isFreeRoam)
{
	auto entity = Entity();

	Mesh& mesh = (isFreeRoam) ? renderer->getMesh(GEOMETRY_BUFFER_ID::SIMPLIFIED_ARROW_MESH) : renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	auto& gravity = registry.gravities.emplace(entity);
	gravity.gravity = 30;

	auto& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.velocity = velocity;
	motion.position = position;
	
	registry.projectiles.emplace(entity);
	registry.preciseColliders.emplace(entity);

	if (isFreeRoam) {
		motion.scale = vec2({ ARROW_MESH_WIDTH, ARROW_MESH_HEIGHT });
		registry.light.emplace(entity);

		registry.renderRequests.insert(
			entity,
			//currently using fireball
			{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
			 EFFECT_ASSET_ID::PEBBLE,
			 GEOMETRY_BUFFER_ID::ARROW_MESH });
	}
	else {
		motion.scale = vec2({ ARROW_WIDTH, ARROW_HEIGHT });

		// Set damage here--------------------------------
		Damage& damage = registry.damages.emplace(entity);
		damage.isFriendly = isFriendly;
		damage.minDamage = registry.arrow_dmg;
		damage.range = 10;
		//------------------------------------------------
		registry.bouncingArrows.emplace(entity);

		registry.renderRequests.insert(
			entity,
			//currently using fireball
			{ TEXTURE_ASSET_ID::ARROW,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}


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
	damage.minDamage = registry.iceshard_dmg;
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
//arrow icon
Entity createArrowIcon(RenderSystem* renderer, vec2 position)
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
		{ TEXTURE_ASSET_ID::ARROWICON,
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

Entity createCharIndicator(RenderSystem* renderer, vec2 position, Entity owner)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& charInd = registry.charIndicator.emplace(entity);
	charInd.owner = owner;

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

	registry.shieldIcons.emplace(entity);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0.f };
	motion.acceleration = { 0, 0.f };
	motion.position = position;
	motion.scale = vec2({ BARRIER_WIDTH, BARRIER_HEIGHT });

	/*registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BARRIER,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });*/

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::BACKGROUND_OBJ,
			GEOMETRY_BUFFER_ID::SHIELD_MESH });

	registry.deformableEntities.insert(entity, {});

	return entity;
}


Entity createFirefly(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.scale = vec2({ FIREFLY_WIDTH, -FIREFLY_HEIGHT });
	motion.velocity = vec2(0.f, 0.f);

	auto& firefly = registry.fireflySwarm.emplace(entity);
	// registry.light.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::FIREFLY_PNG,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createPlatform(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.scale = vec2({ PLATFORM_WIDTH, PLATFORM_HEIGHT });

	registry.platform.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLATFORM,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createRockMesh(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::ROCK_MESH);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.scale = vec2({ ROCK_MESH_WIDTH, -ROCK_MESH_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::PEBBLE,
			GEOMETRY_BUFFER_ID::ROCK_MESH });

	return entity;
}

Entity createTreasureChest(RenderSystem* renderer, vec2 position, int chestType)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::TREASURE_CHEST_CLOSED);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.scale = vec2({ TREASURE_CHEST_WIDTH, TREASURE_CHEST_HEIGHT });

	auto& chest = registry.chests.emplace(entity);
	chest.chestType = chestType;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TREASURE_CHEST_SHEET,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::TREASURE_CHEST_CLOSED });

	return entity;
}

Entity createGreenCross(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, -150.f };
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

Entity createBoostMessage(RenderSystem* renderer, vec2 position, int boostType)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, -150.f };
	motion.acceleration = { 0.f, 0.f };
	motion.position = position;
	motion.scale = vec2({ BOOST_MSG_WIDTH, BOOST_MSG_HEIGHT });

	registry.light.emplace(entity);

	TEXTURE_ASSET_ID id = (boostType == HEALTH_BOOST) ? TEXTURE_ASSET_ID::HEALTH_INCREASE : TEXTURE_ASSET_ID::DAMAGE_INCREASE;

	registry.renderRequests.insert(
		entity,
		{ id,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createMeteorShower(RenderSystem* renderer, vec2 position, int isFriendly)
{	
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.velocity = { -50.f, 50.f };
	motion.position = position;
	motion.scale = vec2({ METEOR_WIDTH, METEOR_HEIGHT });

	// Set damage here--------------------------------
	Damage& damage = registry.damages.emplace(entity);
	damage.isFriendly = isFriendly;
	damage.minDamage = 0;
	damage.range = 10;
	//------------------------------------------------

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::METEOR, //https://www.cleanpng.com/png-designer-tencent-tmall-golden-meteor-shower-99329/download-png.html
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;

}

Entity createTauntIndicator(RenderSystem* renderer, Entity owner)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	auto statid =  &registry.statsindicators.emplace(entity);
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
Entity createBleedIndicator(RenderSystem* renderer, Entity owner)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	auto statid = &registry.bleedIndicators.emplace(entity);
	statid->owner = owner;

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.acceleration = { 0.f, 0.f };
	motion.position = { 0.f,0.f };
	motion.scale = vec2({ GREENCROSS_WIDTH, GREENCROSS_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BLEED,
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
	damage.minDamage = registry.rock_dmg;
	damage.range = 10;
	//------------------------------------------------

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ROCK,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

// create cpike
Entity createSpike(RenderSystem* renderer, vec2 position, int isFriendly)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f,90 };
	motion.acceleration = { 0.f,1000 };
	motion.position = position;
	motion.scale = vec2({ SPIKE_WIDTH, SPIKE_HEIGHT });

	auto& proj = registry.projectiles.emplace(entity);
	proj.enableCameraTracking = 0;

	// Set damage here--------------------------------
	Damage& damage = registry.damages.emplace(entity);
	damage.isFriendly = isFriendly;
	damage.minDamage = registry.spike_dmg;
	damage.range = 10;
	//------------------------------------------------

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SPIKE,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createBoulder(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SIMPLIFIED_ROCK_MESH);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	registry.rollables.emplace(entity);
	registry.boulders.emplace(entity);
	registry.preciseColliders.emplace(entity);

	motion.scale = vec2({ ROCK_MESH_WIDTH, -ROCK_MESH_HEIGHT });
	motion.angle = 1.f;
	
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::PEBBLE,
			GEOMETRY_BUFFER_ID::ROCK_MESH });

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
	motion.velocity = { 0.f, 100 };
	motion.acceleration = { 0.f, 200 };
	motion.position = position;
	motion.scale = vec2({ LIGHTNING_WIDTH, LIGHTNING_HEIGHT });

	auto& proj = registry.projectiles.emplace(entity);
	proj.enableCameraTracking = 0;

	// Set damage here--------------------------------
	Damage& damage = registry.damages.emplace(entity);
	damage.isFriendly = isFriendly;
	damage.minDamage = registry.lightning_dmg;
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
	damage.minDamage = registry.melee_dmg;
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

Entity createBleedDMG(RenderSystem* renderer, vec2 position, int isFriendly)
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
	damage.minDamage = registry.bleed_dmg;
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
	motion.velocity = { 0.f, -150.f };
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

Entity createParticleBeamCharge(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion, probably can make it rotate/increase scale
	auto& motion = registry.motions.emplace(entity);
	position[1] += 100;
	motion.position = position;
	motion.scale = vec2({ PARTICLEBEAMCHARGE_WIDTH, PARTICLEBEAMCHARGE_HEIGHT });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PARTICLEBEAMCHARGE,	//https://pngtree.com/freepng/abstract-ring-purple-light-effect_5102994.html
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

Entity createDot(RenderSystem* renderer, vec2 position)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::DOT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = {DOT_WIDTH,DOT_HEIGHT};
	registry.dots.emplace(entity);
	return entity;
}


void createBackground(RenderSystem* renderer, vec2 pos, int levelNumber)
{

	std::vector<Entity> backgroundEntities;
	std::vector<TEXTURE_ASSET_ID> bgAssetIds;

	switch (levelNumber) {
		case TUTORIAL: {
			for (int i = 0; i < 5; i++) {
				backgroundEntities.push_back(Entity());
			}
			bgAssetIds.push_back(TEXTURE_ASSET_ID::TUTORIAL_BG_ONE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::TUTORIAL_BG_TWO);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::TUTORIAL_BG_THREE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::TUTORIAL_BG_FOUR);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::TUTORIAL_BG_FIVE); break;
		}
		case LEVEL_ONE: {
			for (int i = 0; i < 4; i++) {
				backgroundEntities.push_back(Entity());
			}
			bgAssetIds.push_back(TEXTURE_ASSET_ID::LEVEL_ONE_BG_ONE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::LEVEL_ONE_BG_TWO);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::LEVEL_ONE_BG_THREE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::LEVEL_ONE_BG_FOUR); break;
		}
		case FREE_ROAM_ONE: {
			for (int i = 0; i < 5; i++) {
				backgroundEntities.push_back(Entity());
			}
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_ONE_BG_ONE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_ONE_BG_TWO);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_ONE_BG_THREE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_ONE_BG_FOUR);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_ONE_BG_FIVE); break;
		}
		case LEVEL_TWO: {
			for (int i = 0; i < 4; i++) {
				backgroundEntities.push_back(Entity());
			}
			bgAssetIds.push_back(TEXTURE_ASSET_ID::LEVEL_TWO_BG_ONE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::LEVEL_TWO_BG_TWO);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::LEVEL_TWO_BG_THREE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::LEVEL_TWO_BG_FOUR); break;
		}
		case FREE_ROAM_TWO: {
			for (int i = 0; i < 6; i++) {
				backgroundEntities.push_back(Entity());
			}
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_TWO_BG_ONE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_TWO_BG_TWO);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_TWO_BG_THREE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_TWO_BG_FOUR);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_TWO_BG_FIVE);
			bgAssetIds.push_back(TEXTURE_ASSET_ID::FREE_ROAM_TWO_BG_SIX); break;
		}
		case LEVEL_THREE: {
			for (int i = 0; i < 1; i++) {
				backgroundEntities.push_back(Entity());
			}
			bgAssetIds.push_back(TEXTURE_ASSET_ID::LEVEL_THREE_BG_ONE); break;
		}
		default: break;
	}

	int numLayers = backgroundEntities.size();

	for (int i = 0; i < numLayers; i++) {
		Entity currEntity = backgroundEntities[i];
		auto& motion = registry.motions.emplace(currEntity);
		motion.angle = 0.f;
		motion.position = pos;

		// Shift moon towards top-left
		if (i == 1 && levelNumber == FREE_ROAM_TWO) {
			motion.position = vec2(pos.x - 100, pos.y - 100);
		}

		motion.scale = vec2({ BACKGROUND_WIDTH, BACKGROUND_HEIGHT });

		registry.backgroundLayers.emplace(currEntity);

		registry.renderRequests.insert(
			currEntity,
			{ bgAssetIds[i],
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}

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

Entity createTutorialBox(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { TUTORIAL_BOX_WIDTH, TUTORIAL_BOX_HEIGHT };

	TEXTURE_ASSET_ID tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_ONE;

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
	else if (type == "AR") {
		tooltipType = TEXTURE_ASSET_ID::ARROWTOOLTIP;
	}
	registry.renderRequests.insert(
		entity,
		{ tooltipType,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createUIButton(RenderSystem* renderer, vec2 position, int buttonType) {
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT };

	TEXTURE_ASSET_ID button_type = TEXTURE_ASSET_ID::GAME_TITLE;

	UIButton& button = registry.uiButtons.emplace(entity);

	switch (buttonType) {
		case 1: button_type = TEXTURE_ASSET_ID::NEW_GAME; button.button_type = 1; break;
		case 2: button_type = TEXTURE_ASSET_ID::LOAD_GAME; button.button_type = 2; break;
		case 3: button_type = TEXTURE_ASSET_ID::SAVE_GAME; motion.scale = { 200, 80 }; button.button_type = 3; break;
		case 4: button_type = TEXTURE_ASSET_ID::EXIT_GAME; button.button_type = 4; break;
		case 5: button_type = TEXTURE_ASSET_ID::GAME_TITLE; motion.scale = { TITLE_WIDTH, TITLE_HEIGHT }; button.button_type = 5; break;
		case 6: button_type = TEXTURE_ASSET_ID::OPEN_MENU; motion.scale = { 100, 100 }; button.button_type = 6; break;
		case 7: button_type = TEXTURE_ASSET_ID::CLOSE_MENU; motion.scale = { 100,100}; button.button_type = 7; break;
		case 8: button_type = TEXTURE_ASSET_ID::CREATE_GAME; button.button_type = 8; break;
		case 9: button_type = TEXTURE_ASSET_ID::START_MAKEUP; button.button_type = 9; break;
		case 10: button_type = TEXTURE_ASSET_ID::RESET_MAKEUP; button.button_type = 10; break;
		default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ button_type,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createStoryBackground(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { 1200, 800 };

	TEXTURE_ASSET_ID storyBackground = TEXTURE_ASSET_ID::GAME_TITLE;

	registry.storyTellingBackgrounds.emplace(entity);

	switch (number) {
	case 1: storyBackground = TEXTURE_ASSET_ID::BATTLE; break;
	case 2: storyBackground = TEXTURE_ASSET_ID::BATTLESUB; break;
	case 3: storyBackground = TEXTURE_ASSET_ID::ROOM; break;
	case 4: storyBackground = TEXTURE_ASSET_ID::WHISPER; break;
	case 5: storyBackground = TEXTURE_ASSET_ID::STORYBEGIN; break;
	case 6: storyBackground = TEXTURE_ASSET_ID::STARTSCREEN; break;
	case 7: storyBackground = TEXTURE_ASSET_ID::PEACEFUL; break;
	case 8: storyBackground = TEXTURE_ASSET_ID::CELERBRATE; break;
	case 9: storyBackground = TEXTURE_ASSET_ID::DARK; break;
	case 10: storyBackground = TEXTURE_ASSET_ID::CONCLUSIONTHEEND; break;

	case 11: storyBackground = TEXTURE_ASSET_ID::CONCLUSIONONE; break;
	case 12: storyBackground = TEXTURE_ASSET_ID::CONCLUSIONTWO; break;
	case 13: storyBackground = TEXTURE_ASSET_ID::CONCLUSIONTHREE; break;
	case 14: storyBackground = TEXTURE_ASSET_ID::CONCLUSIONFOUR; break;
	case 15: storyBackground = TEXTURE_ASSET_ID::CONCLUSIONFIVE; break;
	case 16: storyBackground = TEXTURE_ASSET_ID::CONCLUSIONSIX; break;
	case 17: storyBackground = TEXTURE_ASSET_ID::CONCLUSIONSEVEN; break;
	default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ storyBackground,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createBackgroundDiaogue(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { DIALOGUE_WIDTH, DIALOGUE_HEIGHT };

	TEXTURE_ASSET_ID dialogue = TEXTURE_ASSET_ID::GAME_TITLE;

	auto& ui = registry.uiButtons.emplace(entity);
	ui.isDialogue = 1;

	switch (number) {
	case 1: dialogue = TEXTURE_ASSET_ID::BACKGROUNDONE; break;
	case 2: dialogue = TEXTURE_ASSET_ID::BACKGROUNDTWO; break;
	case 3: dialogue = TEXTURE_ASSET_ID::BACKGROUNDTHREE; break;
	case 4: dialogue = TEXTURE_ASSET_ID::BACKGROUNDFOUR; break;
	case 5: dialogue = TEXTURE_ASSET_ID::BACKGROUNDFIVE; break;
	default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ dialogue,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}
Entity createLevelOneDiaogue(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { DIALOGUE_WIDTH, DIALOGUE_HEIGHT };

	TEXTURE_ASSET_ID dialogue = TEXTURE_ASSET_ID::GAME_TITLE;

	auto& ui = registry.uiButtons.emplace(entity);
	ui.isDialogue = 1;

	switch (number) {

	//level one
	case 1: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUEONE; break;
	case 2: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUETWO; break;
	case 3: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUETHREE; break;
	case 4: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUEFOUR; break;
	case 5: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUEFIVE; break;
	case 6: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUESIX; break;
	case 7: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUESEVEN; break;
	case 8: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUEEIGHT; break;
	case 9: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUENINE; break;
	case 10: dialogue = TEXTURE_ASSET_ID::LEVELONEDIALOGUETEN; break;
	case 11: dialogue = TEXTURE_ASSET_ID::MISSIONONE; break;
	default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ dialogue,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}
Entity createLevelTwoDiaogue(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { DIALOGUE_WIDTH, DIALOGUE_HEIGHT };

	TEXTURE_ASSET_ID dialogue = TEXTURE_ASSET_ID::GAME_TITLE;

	auto& ui = registry.uiButtons.emplace(entity);
	ui.isDialogue = 1;

	switch (number) {
	//level two
	case 1: dialogue = TEXTURE_ASSET_ID::LEVELTWODIALOGUEONE; break;
	case 2: dialogue = TEXTURE_ASSET_ID::LEVELTWODIALOGUETWO; break;
	case 3: dialogue = TEXTURE_ASSET_ID::LEVELTWODIALOGUETHREE; break;
	case 4: dialogue = TEXTURE_ASSET_ID::LEVELTWODIALOGUEFOUR; break;
	case 5: dialogue = TEXTURE_ASSET_ID::LEVELTWODIALOGUEFIVE; break;
	case 6: dialogue = TEXTURE_ASSET_ID::LEVELTWODIALOGUESIX; break;
	case 7: dialogue = TEXTURE_ASSET_ID::MISSIONTWO; break;

	default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ dialogue,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}
Entity createLevelThreeDiaogue(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { DIALOGUE_WIDTH, DIALOGUE_HEIGHT };

	TEXTURE_ASSET_ID dialogue = TEXTURE_ASSET_ID::GAME_TITLE;

	auto& ui = registry.uiButtons.emplace(entity);
	ui.isDialogue = 1;

	switch (number) {

		//level three
	//case 1: dialogue = TEXTURE_ASSET_ID::LEVELTHREEDIALOGUEONE; break;	// wrong place
	case 2: dialogue = TEXTURE_ASSET_ID::LEVELTHREEDIALOGUETWO; break;
	case 3: dialogue = TEXTURE_ASSET_ID::LEVELTHREEDIALOGUETHREE; break;
	case 4: dialogue = TEXTURE_ASSET_ID::LEVELTHREEDIALOGUEFOUR; break;
	case 5: dialogue = TEXTURE_ASSET_ID::LEVELTHREEDIALOGUEFIVE; break;
	case 6: dialogue = TEXTURE_ASSET_ID::LEVELTHREEDIALOGUESIX; break;
	case 7: dialogue = TEXTURE_ASSET_ID::LEVELTHREEDIALOGUESEVEN; break;
	case 8: dialogue = TEXTURE_ASSET_ID::LEVELTHREEDIALOGUEEIGHT; break;
	case 9: dialogue = TEXTURE_ASSET_ID::MISSIONTHREEONE; break;
	case 10: dialogue = TEXTURE_ASSET_ID::MISSIONTHREETWO; break;
	case 11: dialogue = TEXTURE_ASSET_ID::MISSIONTHREETHREE; break;
	case 12: dialogue = TEXTURE_ASSET_ID::MISSIONTHREEFOUR; break;
	default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ dialogue,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}
Entity createLevelFourDiaogue(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { DIALOGUE_WIDTH, DIALOGUE_HEIGHT };

	TEXTURE_ASSET_ID dialogue = TEXTURE_ASSET_ID::GAME_TITLE;

	auto& ui = registry.uiButtons.emplace(entity);
	ui.isDialogue = 1;

	switch (number) {
		//level Four
	case 1: dialogue = TEXTURE_ASSET_ID::LEVELFOURDIALOGUEONE; break;
	case 2: dialogue = TEXTURE_ASSET_ID::LEVELFOURDIALOGUETWO; break;
	case 3: dialogue = TEXTURE_ASSET_ID::LEVELFOURDIALOGUETHREE; break;
	case 4: dialogue = TEXTURE_ASSET_ID::LEVELFOURDIALOGUEFOUR; break;
	default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ dialogue,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}
Entity createFreeRoamLevelDiaogue(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { DIALOGUE_WIDTH, DIALOGUE_HEIGHT };

	TEXTURE_ASSET_ID dialogue = TEXTURE_ASSET_ID::GAME_TITLE;

	auto& ui = registry.uiButtons.emplace(entity);
	ui.isDialogue = 1;

	switch (number) {
		//level one
	case 1: dialogue = TEXTURE_ASSET_ID::FREEROAMLEVELONEDIALOGUEONE; break;
	case 2: dialogue = TEXTURE_ASSET_ID::FREEROAMLEVELONEDIALOGUETWO; break;
	case 3: dialogue = TEXTURE_ASSET_ID::FREEROAMLEVELONEDIALOGUETHREE; break;
	case 4: dialogue = TEXTURE_ASSET_ID::FREEROAMLEVELONEDIALOGUEFOUR; break;
	case 5: dialogue = TEXTURE_ASSET_ID::FREEROAMLEVELONEDIALOGUEFIVE; break;

		//level two
	case 6: dialogue = TEXTURE_ASSET_ID::FREEROAMLEVELTWODIALOGUEONE; break;
	case 7: dialogue = TEXTURE_ASSET_ID::FREEROAMLEVELTWODIALOGUETWO; break;
	case 8: dialogue = TEXTURE_ASSET_ID::FREEROAMLEVELTWODIALOGUETHREE; break;
	default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ dialogue,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createFreeRoamLevelTutorial(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { HELPER_BOX_WIDTH, HELPER_BOX_HEIGHT };

	TEXTURE_ASSET_ID tutorial_box = TEXTURE_ASSET_ID::FREEROAMTUTORIAL;

	registry.renderRequests.insert(
		entity,
		{ tutorial_box,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createSizeIndicator(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { SIZE_INDICATOR_WIDTH, SIZE_INDICATOR_HEIGHT };

	TEXTURE_ASSET_ID size_indicator = TEXTURE_ASSET_ID::EMPTY_IMAGE;


	switch (number) {
	case 1: size_indicator = TEXTURE_ASSET_ID::ZERO_OUT_OF_FOUR; break;
	case 2: size_indicator = TEXTURE_ASSET_ID::ONE_OUT_OF_FOUR; break;
	case 3: size_indicator = TEXTURE_ASSET_ID::TWO_OUT_OF_FOUR; break;
	case 4: size_indicator = TEXTURE_ASSET_ID::THREE_OUT_OF_FOUR; break;
	case 5: size_indicator = TEXTURE_ASSET_ID::FOUR_OUT_OF_FOUR; break;

	default: break;
	}
	registry.renderRequests.insert(
		entity,
		{ size_indicator,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createSelectPanel(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { SELECT_PANEL_WIDTH, SELECT_PANEL_HEIGHT };


	registry.renderRequests.insert(
		entity,
		{TEXTURE_ASSET_ID::SELECTPANEL,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createSelections(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { 100, 100 };

	TEXTURE_ASSET_ID selections = TEXTURE_ASSET_ID::EMPTY_IMAGE;


	switch (number) {
		//level one
	case 1: selections = TEXTURE_ASSET_ID::ARCHERANIMS_SELECT; break;
	case 2: selections = TEXTURE_ASSET_ID::MAGE_ANIM_SELECT; break;
	case 3: selections = TEXTURE_ASSET_ID::SWORDSMAN_SELECT; break;
	case 4: selections = TEXTURE_ASSET_ID::NECROMANCER_SELECT; break;
	case 5: selections = TEXTURE_ASSET_ID::NECRO_TWO_SELECT; break;
	default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ selections,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createFreeRoamLevelTutorialIndicator(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = { HELPER_INDICATOR_WIDTH, HELPER_INDICATOR_HEIGHT };

	TEXTURE_ASSET_ID tutorial_box = TEXTURE_ASSET_ID::FREEROAMTUTORIAL_HELPER;

	registry.renderRequests.insert(
		entity,
		{ tutorial_box,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createDebuffIndicator(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, -100.f };
	motion.scale = { GREENCROSS_WIDTH, GREENCROSS_HEIGHT };

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BLEED,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createFinishedOptions(RenderSystem* renderer, vec2 pos, int number)
{
	auto entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	TEXTURE_ASSET_ID options = TEXTURE_ASSET_ID::EMPTY_IMAGE;


	switch (number) {
	case 1: options = TEXTURE_ASSET_ID::NEW_MAKEUP_OPTIONS; motion.scale = { 500, 300 }; break;
	case 2: options = TEXTURE_ASSET_ID::YES_OPTION;  motion.scale = { 50, 30 }; break;
	case 3: options = TEXTURE_ASSET_ID::NO_OPTION;  motion.scale = { 100, 30 }; break;
	default: break;
	}

	registry.renderRequests.insert(
		entity,
		{ options,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}