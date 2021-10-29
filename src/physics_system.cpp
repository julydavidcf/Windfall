// internal
#include "physics_system.hpp"
#include "world_init.hpp"

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
		} else if(type==NECROMANCER){
			return { 12*(abs(motion.scale.x)/10), 13*(abs(motion.scale.y)/10) };
		} else{
			return { abs(motion.scale.x), abs(motion.scale.y) };
		}

	} else {
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
		} else if(type==NECROMANCER){
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
	if (dist_squared < r_squared)
		return true;
	return false;
}

void PhysicsSystem::step(float elapsed_ms, float window_width_px, float window_height_px)
{
	auto& motion_registry = registry.motions;
	for(uint i = 0; i< motion_registry.size(); i++)
	{


		// !!! TODO: Calculate newest fireball position based on step_seconds and motion.velocity

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
		//printf("acc:%f %f\n", motion->acceleration.x, motion->acceleration.y);
		//printf("v:%f %f\n", motion->velocity.x, motion->velocity.y);

		// assume gravity effected object need to adjest angels

		if (registry.gravities.has(entity)) {
			float angle = atan(motion->velocity.y / motion->velocity.x);
			if (motion->velocity.x < 0) {
				angle += M_PI;
			}
			motion->angle = angle;

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
	if (debugging.in_debug_mode)
	{
		uint size_before_adding_new = (uint)motion_container.components.size();
		for (uint i = 0; i < size_before_adding_new; i++)
		{
			Motion& motion_i = motion_container.components[i];
			Entity entity_i = motion_container.entities[i];

			vec3 point0 	= {0.f, 0.f, 1.f};
			vec3 pointy 	= {0.f, 1.f, 1.f};
			vec3 pointx 	= {1.f, 0.f, 1.f};
			vec3 pointxy 	= {1.f, 1.f, 1.f};

			Transform transform;
			vec2 new_scale = get_custom_bounding_box(entity_i);
			vec2 custom_pos = get_custom_position(entity_i);
			
			vec2 offset = {((cos(motion_i.angle)*new_scale.x)/2-(sin(motion_i.angle)*new_scale.y)/2), 
								((sin(motion_i.angle)*new_scale.x)/2+(cos(motion_i.angle)*new_scale.y)/2)};
			vec2 new_pos = custom_pos - offset;
			transform.translate(new_pos);
			transform.rotate(motion_i.angle);
			transform.scale(new_scale);
			
			point0 	= transform.mat * point0;
			pointy 	= transform.mat * pointy;
			pointx 	= transform.mat * pointx;
			pointxy = transform.mat * pointxy;

			vec2 points[4] = {point0, pointx, pointy, pointxy};
			
			float min_x = point0.x;
			float max_x = pointx.x;
			float min_y = point0.y;
			float max_y = pointy.y;
			
			for(vec2 vec: points){
				if(vec.x<min_x){
					min_x = vec.x;
				}
				if(vec.x>max_x){
					max_x = vec.x;
				}
				if(vec.y<min_y){
					min_y = vec.y;
				}
				if(vec.y>max_y){
					max_y = vec.y;
				}
			}

			float line_thickness = 1.5f;

			vec2 line1_pos = {(max_x+min_x)/2, min_y};
			vec2 line2_pos = {(max_x+min_x)/2, max_y};
			vec2 line3_pos = {min_x, (max_y+min_y)/2};
			vec2 line4_pos = {max_x, (max_y+min_y)/2};

			vec2 line1_scale = {(max_x-min_x), line_thickness};
			vec2 line3_scale = {line_thickness, (max_y-min_y)};

			
			Entity line1 = createLine(line1_pos, line1_scale);
			Entity line2 = createLine(line2_pos, line1_scale);
			Entity line3 = createLine(line3_pos, line3_scale);
			Entity line4 = createLine(line4_pos, line3_scale);

			// center = pos
			Entity center = createLine(get_custom_position(entity_i), {5.f, 5.f});

		}
	}
}