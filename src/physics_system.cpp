// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include <iostream>

// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

vec2 PhysicsSystem::get_custom_bounding_box(Entity entity)
{
	Motion& motion = registry.motions.get(entity);
	if(registry.companions.has(entity) || registry.enemies.has(entity)){
		int type;
		
		if(registry.companions.has(entity)){
			Companion& companion = registry.companions.get(entity);
			type = companion.companionType;
		} else {
			Enemy& enemy = registry.enemies.get(entity);
			type = enemy.enemyType;
		}

		if(type==MAGE){
			return { abs(motion.scale.x), abs(motion.scale.y)/1.5 };
		} else if(type==SWORDSMAN){
			return { abs(motion.scale.x)/2, 4*(abs(motion.scale.y)/5) };
		} else if(type== NECROMANCER_ONE){
			return { 12*(abs(motion.scale.x)/10), 13*(abs(motion.scale.y)/10) };
		} else{
			return { abs(motion.scale.x), abs(motion.scale.y) };
		}

	}
	else if (registry.platform.has(entity)) {
		return { abs(motion.scale.x) + 50, abs(motion.scale.y) };
	}
	else if (registry.boulders.has(entity)) {
		return { abs(motion.scale.x)+50, abs(motion.scale.y)+50 };
	}
	else {
		return { abs(motion.scale.x), abs(motion.scale.y) };
	}
}

vec2 PhysicsSystem::get_custom_position(Entity entity)
{
	Motion& motion = registry.motions.get(entity);
	if(registry.companions.has(entity) || registry.enemies.has(entity)){
		int type;
		
		if(registry.companions.has(entity)){
			Companion& companion = registry.companions.get(entity);
			type = companion.companionType;
		} else {
			Enemy& enemy = registry.enemies.get(entity);
			type = enemy.enemyType;
		}

		if(type==MAGE){
			float offset = motion.scale.y/4;
			return {motion.position.x, motion.position.y+offset};
		} else if(type==SWORDSMAN){
			float offset_y = motion.scale.y/5;
			float offset_x = motion.scale.x/12;
			return {motion.position.x-offset_x, motion.position.y+offset_y};
		} else if(type==NECROMANCER_ONE){
			return motion.position;
		} else{
			return motion.position;
		}

	} else {
		return motion.position;
	}
}



// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection
bool collides(const Entity entity_i, const Entity entity_j)
{
	PhysicsSystem ps;
	vec2 custom_pos_i = ps.get_custom_position(entity_i);
	vec2 custom_pos_j = ps.get_custom_position(entity_j);
	vec2 dp = custom_pos_i - custom_pos_j;
	float dist_squared = dot(dp,dp);
	const vec2 other_bonding_box = ps.get_custom_bounding_box(entity_i) / 2.f;
	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
	const vec2 my_bonding_box = ps.get_custom_bounding_box(entity_j) / 2.f;
	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
	const float r_squared = max(other_r_squared, my_r_squared);

	if(registry.reflects.has(entity_i)){
		Motion motion_j = registry.motions.get(entity_j);
		int dir = 1;
		if(motion_j.velocity.x < 0){
			dir = -1;
		}
		int left_x_i = custom_pos_i.x - other_bonding_box.x;
		int right_x_i = custom_pos_i.x + other_bonding_box.x;

		int left_x_j = custom_pos_j.x - my_bonding_box.x;
		int right_x_j = custom_pos_j.x + my_bonding_box.x;

		if(dir == 1){
			if(right_x_j >= left_x_i){
				return true;
			}
			return false;
		} else {
			if(left_x_j <= right_x_i){
				return true;
			}
			return false;
		}
	}
	else if(registry.reflects.has(entity_j)){
		Motion motion_i = registry.motions.get(entity_i);
		int dir = 1;
		if(motion_i.velocity.x < 0){
			dir = -1;
		}
		int left_x_i = custom_pos_i.x - other_bonding_box.x;
		int right_x_i = custom_pos_i.x + other_bonding_box.x;

		int left_x_j = custom_pos_j.x - my_bonding_box.x;
		int right_x_j = custom_pos_j.x + my_bonding_box.x;

		if(dir == 1){
			if(right_x_i >= left_x_j){
				return true;
			}
			return false;
		} else {
			if(left_x_i <= right_x_j){
				return true;
			}
			return false;
		}
	}

	if (dist_squared < r_squared){
		return true;
	}
	return false;
}

bool fireflyCollides(const Entity entity_i, const Entity entity_j) {
	PhysicsSystem ps;
	vec2 custom_pos_i = ps.get_custom_position(entity_i);
	vec2 custom_pos_j = ps.get_custom_position(entity_j);
	vec2 dp = custom_pos_i - custom_pos_j;
	float dist_squared = dot(dp, dp);
	const vec2 other_bonding_box = ps.get_custom_bounding_box(entity_i) * 1.5f;
	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
	const vec2 my_bonding_box = ps.get_custom_bounding_box(entity_j) * 1.5f;
	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
	const float r_squared = max(other_r_squared, my_r_squared);

	if (dist_squared < r_squared) {
		return true;
	}
	return false;
}


// https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
float area(float x1, float y1, float x2, float y2, float x3, float y3)
{
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

// https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
bool isInside(vec3 point1, vec3 point2, vec3 point3, vec3 p)
{
	float A = area(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
	float A1 = area(p.x, p.y, point2.x, point2.y, point3.x, point3.y);
	float A2 = area(point1.x, point1.y, p.x, p.y, point3.x, point3.y);
	float A3 = area(point1.x, point1.y, point2.x, point2.y, p.x, p.y);
	return (A == (A1 + A2 + A3));
}


bool precise_collides(Entity entity1, Entity entity2)
{
	Motion& motion1 = registry.motions.get(entity1);
	Motion& motion2 = registry.motions.get(entity2);
	
	Mesh* mesh1 = registry.meshPtrs.get(entity1);
	Mesh* mesh2 = registry.meshPtrs.get(entity2);
	
	// Go over the first entity's faces
	for(int i = 0; i<mesh1->vertex_indices.size(); i=i+3){
		// vertex indices in a triangle face
		uint16_t ind1 = mesh1->vertex_indices[i];
		uint16_t ind2 = mesh1->vertex_indices[i+1];
		uint16_t ind3 = mesh1->vertex_indices[i+2];
		
		// vertices of the triangle
		std::vector<ColoredVertex> coloredVerteces = mesh1->vertices;
		vec3 point1 = {coloredVerteces[ind1].position.x, coloredVerteces[ind1].position.y, 1.f};
		vec3 point2 = {coloredVerteces[ind2].position.x, coloredVerteces[ind2].position.y, 1.f};
		vec3 point3 = {coloredVerteces[ind3].position.x, coloredVerteces[ind3].position.y, 1.f};
		
		// vertices of the triangle in world coor
		Transform transform1;
		transform1.translate(motion1.position);
		transform1.rotate(motion1.angle);
		transform1.scale(motion1.scale);

		point1 = transform1.mat*point1;
		point2 = transform1.mat*point2;
		point3 = transform1.mat*point3;
		
		// Go over the second entity's vertices
		for(ColoredVertex cv: mesh2->vertices){
			vec3 vertex = {cv.position.x, cv.position.y, 1.f};
			
			// vertex in world coor
			Transform transform2;
			transform2.translate(motion2.position);
			transform2.rotate(motion2.angle);
			transform2.scale(motion2.scale);

			vertex = transform2.mat*vertex;

			if(isInside(point1, point2, point3, vertex)){
				return true;
			}
		}
	}
	return false;	
}

void PhysicsSystem::step_freeRoam(float elapsed_ms, float window_width_px, float window_height_px)
{
	auto& motion_registry = registry.motions;
	Entity arrow_entity;
	int hasArrow = 0;

	for (int i = 0; i < registry.renderRequests.components.size(); i++) {
		if (registry.renderRequests.components[i].used_geometry == GEOMETRY_BUFFER_ID::ARROW_MESH) {
			arrow_entity = registry.renderRequests.entities[i];
			hasArrow = 1;
		}
	}

	Motion arrowMotion;
	float arrowPosX;
	float arrowPosY;
	float arrowScaleX;
	float arrowScaleY;
	vec2 topLeftPoint;
	vec2 topRightPoint;
	vec2 bottomLeftPoint;
	vec2 bottomRightPoint;
	if (hasArrow) {
		arrowMotion = registry.motions.get(arrow_entity);
		arrowPosX = arrowMotion.position.x;
		arrowPosY = arrowMotion.position.y;
		arrowScaleX = arrowMotion.scale.x;
		arrowScaleY = arrowMotion.scale.y;
		topLeftPoint = vec2(arrowPosX - arrowScaleX / 2 - 50, arrowPosY - arrowScaleY / 2 + 50);
		topRightPoint = vec2(arrowPosX + arrowScaleX / 2 - 50, arrowPosY - arrowScaleY / 2 + 50);
		bottomLeftPoint = vec2(arrowPosX - arrowScaleX / 2 - 50, arrowPosY + arrowScaleY / 2 + 50);
		bottomRightPoint = vec2(arrowPosX + arrowScaleX / 2 - 50, arrowPosY + arrowScaleY / 2 + 50);
	}

	for(uint i = 0; i< motion_registry.size(); i++)
	{
		Motion* motion = &motion_registry.components[i];
		Entity entity = motion_registry.entities[i];
		float step_seconds = 1.0f * (elapsed_ms / 1000.f);

		// Handle firefly movement:
		if (registry.fireflySwarm.has(entity)) {

			// 1. Avoidance movement: Separate from the incoming arrow
			if (hasArrow && fireflyCollides(entity, arrow_entity)) {
				vec2 fireflyPos = motion->position;
				float moveValue = 200;
				float timerValue = 100.f;
				auto& firefly = registry.fireflySwarm.get(entity);

				// Separate the arrow's collision box into four cases as below

				// First case: Within top-left collision box of arrow
				if (topLeftPoint.x <= fireflyPos.x
					&& fireflyPos.x <= arrowPosX
					&& topLeftPoint.y <= fireflyPos.y
					&& fireflyPos.y <= arrowPosY && !firefly.isDodging) {
					firefly.beforeDodgeVelX = motion->velocity.x;
					firefly.beforeDodgeVelY = motion->velocity.y;
					motion->velocity.x = moveValue;
					motion->velocity.y = moveValue;
					firefly.dodge_timer = timerValue;
					firefly.isDodging = 1;
					registry.projectiles.get(arrow_entity).empoweredArrow = 1;
				}

				// Second case: Within top-right collision box of arrow
				else if (arrowPosX < fireflyPos.x
					   && fireflyPos.x <= topRightPoint.x
					   && topRightPoint.y <= fireflyPos.y
					   && fireflyPos.y < arrowPosY && !firefly.isDodging) {
					firefly.beforeDodgeVelX = motion->velocity.x;
					firefly.beforeDodgeVelY = motion->velocity.y;
					motion->velocity.x = moveValue;
					motion->velocity.y = moveValue;
					firefly.dodge_timer = timerValue;
					firefly.isDodging = 1;
					registry.projectiles.get(arrow_entity).empoweredArrow = 1;
				}

				// Third case: Within bottom-left collision box of arrow
				else if (bottomLeftPoint.x <= fireflyPos.x
					   && fireflyPos.x < arrowPosX
					   && arrowPosY < fireflyPos.y
					   && fireflyPos.y <= bottomLeftPoint.y && !firefly.isDodging) {
					firefly.beforeDodgeVelX = motion->velocity.x;
					firefly.beforeDodgeVelY = motion->velocity.y;
					motion->velocity.x = moveValue;
					motion->velocity.y = moveValue;
					firefly.dodge_timer = timerValue;
					firefly.isDodging = 1;
					registry.projectiles.get(arrow_entity).empoweredArrow = 1;
				}

				// Fourth case: Within bottom-right collision box of arrow
				else if (arrowPosX <= fireflyPos.x
					   && fireflyPos.x <= bottomRightPoint.x
					   && arrowPosY <= fireflyPos.y
					   && fireflyPos.y <= bottomRightPoint.y && !firefly.isDodging) {
					firefly.beforeDodgeVelX = motion->velocity.x;
					firefly.beforeDodgeVelY = motion->velocity.y;
					motion->velocity.x = moveValue;
					motion->velocity.y = moveValue;
					firefly.dodge_timer = timerValue;
					firefly.isDodging = 1;
					registry.projectiles.get(arrow_entity).empoweredArrow = 1;
				}
			}

			// 2. Standard movement: Moves towards a boundary and bounces back after approaching
			float posX = motion->position.x;
			float posY = motion->position.y;
			auto& fireflyComoponent = registry.fireflySwarm.get(entity);

			// Bounce x
			if (fireflyComoponent.shouldFlipVelocityX == 1) {
				motion->velocity.x = abs(motion->velocity.x);
			}
			else if (fireflyComoponent.shouldFlipVelocityX == 2) {
				motion->velocity.x = -abs(motion->velocity.x);
			}

			// Bounce y
			if (fireflyComoponent.shouldFlipVelocityY == 1) {
				motion->velocity.y = abs(motion->velocity.y);
			}
			else if (fireflyComoponent.shouldFlipVelocityY == 2) {
				motion->velocity.y = -abs(motion->velocity.y);
			}

			if (posX - xBorderLimitDist < 0.f) {
				fireflyComoponent.shouldFlipVelocityX = 1;
			}
			else if (posX + xBorderLimitDist >= window_width_px) {
				fireflyComoponent.shouldFlipVelocityX = 2;
			}
			else {
				fireflyComoponent.shouldFlipVelocityX = 0;
			}
			if (posY - yBorderLimitDist < 0.f) {
				fireflyComoponent.shouldFlipVelocityY = 1;
			}
			else if (posY + yBorderLimitDist >= window_height_px) {
				fireflyComoponent.shouldFlipVelocityY = 2;
			}
			else {
				fireflyComoponent.shouldFlipVelocityY = 0;
			}
		}

		//normal movement
		motion->velocity.x += step_seconds * motion->acceleration.x;
		motion->velocity.y += step_seconds * motion->acceleration.y;
		motion->position.x += step_seconds * motion->velocity.x;
		motion->position.y += step_seconds * motion->velocity.y;

		//gravity effect
		if (registry.gravities.has(entity)) {

			if (registry.companions.has(entity) && registry.companions.get(entity).companionType == ARCHER) {
				if (motion->velocity.y != 0) {
					motion->acceleration.y += registry.gravities.get(entity).gravity;
				}
				continue;
			}
			motion->acceleration.y += registry.gravities.get(entity).gravity;
			float angle = atan(motion->velocity.y / motion->velocity.x);
			if (motion->velocity.x < 0) {
				angle += M_PI;
			}
			motion->angle = angle;
		}
	}

	// Check for collisions between all moving entities
	ComponentContainer<Motion>& motion_container = registry.motions;
	for (uint i = 0; i < motion_container.components.size(); i++)
	{
		Motion& motion_i = motion_container.components[i];
		Entity entity_i = motion_container.entities[i];
		for (uint j = 0; j < motion_container.components.size(); j++) // i+1
		{
			if (i == j)
				continue;

			Motion& motion_j = motion_container.components[j];
			Entity entity_j = motion_container.entities[j];

			// Handle charIndicator following the character here
			if (registry.charIndicator.has(entity_i) && registry.charIndicator.get(entity_i).owner == entity_j) {
				motion_i.position = vec2(motion_j.position.x, motion_i.position.y);
			}
			else if (registry.charIndicator.has(entity_j) && registry.charIndicator.get(entity_j).owner == entity_i) {
				motion_j.position = vec2(motion_i.position.x, motion_j.position.y);
			}
			
			if (collides(entity_i, entity_j))
			{
				if((registry.preciseColliders.has(entity_i) && registry.preciseColliders.has(entity_j))&&
					!(registry.boulders.has(entity_i) && registry.boulders.has(entity_j)))
				{
					if(precise_collides(entity_i, entity_j))
					{
						// Create a collisions event
						// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
						registry.collisions.emplace_with_duplicates(entity_i, entity_j);
						registry.collisions.emplace_with_duplicates(entity_j, entity_i);	
					}
				} else
				{
					// Create a collisions event
					// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
					registry.collisions.emplace_with_duplicates(entity_i, entity_j);
					registry.collisions.emplace_with_duplicates(entity_j, entity_i);
				}
			}
		}
	}

	// debugging of bounding boxes
	if (debugging.in_debug_mode) showDebugBox();
}

void PhysicsSystem::step(float elapsed_ms, float window_width_px, float window_height_px)
{


	auto& motion_registry = registry.motions;
	for(uint i = 0; i< motion_registry.size(); i++)
	{
		Motion* motion = &motion_registry.components[i];
		Entity entity = motion_registry.entities[i];
		float step_seconds = 1.0f * (elapsed_ms / 1000.f);

		//gravity effect
		if (registry.gravities.has(entity)){
			motion->acceleration.y += registry.gravities.get(entity).gravity;
		}

		//normal movement
		motion->velocity.x += step_seconds * motion->acceleration.x;
		motion->velocity.y += step_seconds * motion->acceleration.y;
		motion->position.x += step_seconds * motion->velocity.x;
		motion->position.y += step_seconds * motion->velocity.y;

		if (registry.gravities.has(entity)) {
			float angle = atan(motion->velocity.y / motion->velocity.x);
			if (motion->velocity.x < 0) {
				angle += M_PI;
			}
			motion->angle = angle;
		}
	}

	// make sure each stat indicator is with their owner
	ComponentContainer<StatIndicator>& statsid_container = registry.statsindicators;
	for (uint i = 0; i < statsid_container.components.size(); i++)
	{
		Motion* motion_i = &registry.motions.get(statsid_container.entities[i]);

		if (registry.motions.has(statsid_container.components[i].owner)) {
			Motion motion_o = registry.motions.get(statsid_container.components[i].owner);
			motion_i->position.x = motion_o.position.x + 20;
			motion_i->position.y = motion_o.position.y - 50;

			//lower it for mage 
			if (registry.enemies.has(statsid_container.components[i].owner) && registry.enemies.get(statsid_container.components[i].owner).enemyType == MAGE) {
				motion_i->position.y += 30;
			}
			if (registry.companions.has(statsid_container.components[i].owner) && registry.companions.get(statsid_container.components[i].owner).companionType == MAGE) {
				motion_i->position.y += 30;
			}
		}
	}

	// make sure each bleed indicator is with their owner
	ComponentContainer<BleedIndicator>& bleed_container = registry.bleedIndicators;
	for (uint i = 0; i < bleed_container.components.size(); i++)
	{
		Motion* motion_i = &registry.motions.get(bleed_container.entities[i]);

		if (registry.motions.has(bleed_container.components[i].owner)) {
			Motion motion_o = registry.motions.get(bleed_container.components[i].owner);
			motion_i->position.x = motion_o.position.x + 20;
			motion_i->position.y = motion_o.position.y - 30;

			//lower it for mage 
			if (registry.enemies.has(bleed_container.components[i].owner) && registry.enemies.get(bleed_container.components[i].owner).enemyType == MAGE) {
				motion_i->position.y += 30;
			}
			if (registry.companions.has(bleed_container.components[i].owner) && registry.companions.get(bleed_container.components[i].owner).companionType == MAGE) {
				motion_i->position.y += 30;
			}
		}
	}

	// Check for collisions between all moving entities
    ComponentContainer<Motion> &motion_container = registry.motions;
	for(uint i = 0; i<motion_container.components.size(); i++)
	{
		Motion& motion_i = motion_container.components[i];
		Entity entity_i = motion_container.entities[i];
		for(uint j = 0; j<motion_container.components.size(); j++) // i+1
		{
			if (i == j)
				continue;

			Motion& motion_j = motion_container.components[j];
			Entity entity_j = motion_container.entities[j];

			// Handle charIndicator following the character here
			if (registry.charIndicator.has(entity_i) && registry.charIndicator.get(entity_i).owner == entity_j) {
				motion_i.position = vec2(motion_j.position.x, motion_i.position.y);
			}
			else if (registry.charIndicator.has(entity_j) && registry.charIndicator.get(entity_j).owner == entity_i) {
				motion_j.position = vec2(motion_i.position.x, motion_j.position.y);
			}


			//if (collides(motion_i, motion_j))
			if(collides(entity_i, entity_j))
			{
				// Create a collisions event
				// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
				registry.collisions.emplace_with_duplicates(entity_i, entity_j);
				registry.collisions.emplace_with_duplicates(entity_j, entity_i);
			}
		}
	}

	// you may need the following quantities to compute wall positions
	(float)window_width_px; (float)window_height_px;


	// debugging of bounding boxes
	if (debugging.in_debug_mode) showDebugBox();
}

void PhysicsSystem::showDebugBox() {
	ComponentContainer<Motion>& motion_container = registry.motions;
	uint size_before_adding_new = (uint)motion_container.components.size();
	for (uint i = 0; i < size_before_adding_new; i++)
	{

		Motion& motion_i = motion_container.components[i];
		Entity entity_i = motion_container.entities[i];

		if (!registry.companions.has(entity_i) && !registry.enemies.has(entity_i) && !registry.projectiles.has(entity_i)
			&& !registry.bird.has(entity_i) && !registry.fireflySwarm.has(entity_i) && !registry.platform.has(entity_i)
			&& !registry.chests.has(entity_i)) {
			continue;
		}

		vec3 point0 = { 0.f, 0.f, 1.f };
		vec3 pointy = { 0.f, 1.f, 1.f };
		vec3 pointx = { 1.f, 0.f, 1.f };
		vec3 pointxy = { 1.f, 1.f, 1.f };

		Transform transform;
		vec2 new_scale = get_custom_bounding_box(entity_i);
		vec2 custom_pos = get_custom_position(entity_i);

		vec2 offset = { ((cos(motion_i.angle) * new_scale.x) / 2 - (sin(motion_i.angle) * new_scale.y) / 2),
							((sin(motion_i.angle) * new_scale.x) / 2 + (cos(motion_i.angle) * new_scale.y) / 2) };
		vec2 new_pos = custom_pos - offset;
		transform.translate(new_pos);
		transform.rotate(motion_i.angle);
		transform.scale(new_scale);

		point0 = transform.mat * point0;
		pointy = transform.mat * pointy;
		pointx = transform.mat * pointx;
		pointxy = transform.mat * pointxy;

		vec2 points[4] = { point0, pointx, pointy, pointxy };

		float min_x = point0.x;
		float max_x = pointx.x;
		float min_y = point0.y;
		float max_y = pointy.y;

		for (vec2 vec : points) {
			if (vec.x < min_x) {
				min_x = vec.x;
			}
			if (vec.x > max_x) {
				max_x = vec.x;
			}
			if (vec.y < min_y) {
				min_y = vec.y;
			}
			if (vec.y > max_y) {
				max_y = vec.y;
			}
		}

		float line_thickness = 6.f;

		vec2 line1_pos = { (max_x + min_x) / 2, min_y };
		vec2 line2_pos = { (max_x + min_x) / 2, max_y };
		vec2 line3_pos = { min_x, (max_y + min_y) / 2 };
		vec2 line4_pos = { max_x, (max_y + min_y) / 2 };

		vec2 line1_scale = { (max_x - min_x), line_thickness };
		vec2 line3_scale = { line_thickness, (max_y - min_y) };


		Entity line1 = createLine(line1_pos, line1_scale);
		Entity line2 = createLine(line2_pos, line1_scale);
		Entity line3 = createLine(line3_pos, line3_scale);
		Entity line4 = createLine(line4_pos, line3_scale);

		// center = pos
		Entity center = createLine(get_custom_position(entity_i), { 5.f, 5.f });
	}
}