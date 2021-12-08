// internal
#include "render_system.hpp"
#include <SDL.h>

#include <string>
#include <sstream>

#include "tiny_ecs_registry.hpp"

void RenderSystem::drawLight(Entity entity)
{
	auto& entityPos = registry.motions.get(entity).position;

	glUseProgram(effects[(GLuint)EFFECT_ASSET_ID::LIGHT]);
	gl_has_errors();

	const GLuint light_program = effects[(GLuint)EFFECT_ASSET_ID::LIGHT];

	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(1.f, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_has_errors();
	// Enabling alpha channel for textures
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	// Draw the screen texture on the quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]);
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]); // Note, GL_ELEMENT_ARRAY_BUFFER associates
																	 // indices to the bound GL_ARRAY_BUFFER
	gl_has_errors();

	GLuint resoltion_x_loc = glGetUniformLocation(light_program, "resolutionX");
	GLuint resoltion_y_loc = glGetUniformLocation(light_program, "resolutionY");
	glUniform1f(resoltion_x_loc, (float)w);
	glUniform1f(resoltion_y_loc, (float)h);

	glUniform2f(glGetUniformLocation(light_program, "lightSourcePos"), entityPos.x, (float)h - entityPos.y);

	float lightPercent = max(0.25f * cos(0.02f*time),0.f);
	glUniform1f(glGetUniformLocation(light_program, "randLight"), lightPercent);

	if (registry.projectiles.has(entity) && registry.projectiles.get(entity).empoweredArrow == 1) {
		glUniform1f(glGetUniformLocation(light_program, "collidesWithFirefly"), 1.f);
	}
	else {
		
		glUniform1f(glGetUniformLocation(light_program, "collidesWithFirefly"), 0.f);
	}
	
	if (!registry.fireflySwarm.has(entity)) {
		glUniform1f(glGetUniformLocation(light_program, "arrow"), 1.f);
	}
	else {
		glUniform1f(glGetUniformLocation(light_program, "arrow"), 0.f);
	}

	for (int i = 0; i < fireFlyPosX.size(); i++) {
		std::string s1("thingie.xCoordinates[");
		std::string s2("thingie.yCoordinates[");
		std::string iInS = std::to_string(i);
		s1 += iInS + "]";
		s2 += iInS + "]";

		GLuint locX = glGetUniformLocation(light_program, s1.c_str());
		GLuint locY = glGetUniformLocation(light_program, s2.c_str());
		glUniform1f(locX, fireFlyPosX[i]);
		glUniform1f(locY, (float) h - fireFlyPosY[i]);
	}
	gl_has_errors();

	// Set the vertex position and vertex texture coordinates (both stored in the
	// same VBO)
	GLint in_position_loc = glGetAttribLocation(light_program, "in_position");
	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	gl_has_errors();
	// Draw
	glDrawElements(
		GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
				  // no offset from the bound index buffer
	gl_has_errors();
}

void RenderSystem::drawDeathParticles(Entity entity, const mat3& projection)
{
	auto& pool = registry.particlePools.get(entity);
	if (!pool.faded) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		const GLuint used_effect_enum = (GLuint)EFFECT_ASSET_ID::PARTICLE;
		const GLuint program = (GLuint)effects[used_effect_enum];

		// Setting shaders
		glUseProgram(program);
		gl_has_errors();

		const GLuint vbo = vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];
		const GLuint ibo = index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];

		// Setting vertex and index buffers
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		gl_has_errors();

		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(vec3)); // note the stride to skip the preceeding vertex position
		gl_has_errors();

		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, pool.particles_position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, pool.particles.size() * 3 * sizeof(float), pool.positions);
		gl_has_errors();

		glBindBuffer(GL_ARRAY_BUFFER, pool.particles_position_buffer);
		glVertexAttribPointer(
			4, // attribute. must match the layout in the shader.
			3, // size : x + y + z + size => 4
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
		);
		gl_has_errors();

		glVertexAttribDivisor(4, 1);
		gl_has_errors();

		// textures
		GLint blueTexLoc = glGetUniformLocation(program, "particleTextureBlue");
		GLint redTexLoc = glGetUniformLocation(program, "particleTextureRed");
		GLint smokeTexLoc = glGetUniformLocation(program, "particleTextureSmoke");
		glUniform1i(blueTexLoc, 0);
		glUniform1i(redTexLoc, 1);
		glUniform1i(smokeTexLoc, 2);
		gl_has_errors();

		glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
		glBindTexture(GL_TEXTURE_2D, texture_gl_handles[(GLuint)TEXTURE_ASSET_ID::DEATH_PARTICLE]);
		gl_has_errors();

		glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
		glBindTexture(GL_TEXTURE_2D, texture_gl_handles[(GLuint)TEXTURE_ASSET_ID::RED_PARTICLE]);
		gl_has_errors();

		glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 2
		glBindTexture(GL_TEXTURE_2D, texture_gl_handles[(GLuint)TEXTURE_ASSET_ID::SMOKE_PARTICLE]);
		gl_has_errors();

		GLint currProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
		GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
		glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float*)&projection);
		gl_has_errors();

		//particle type
		if (pool.areTypeDeath) {
			glUniform1f(glGetUniformLocation(currProgram, "particleType"), 1.f);
		}
		else if (pool.areTypeSmoke) {
			glUniform1f(glGetUniformLocation(currProgram, "particleType"), 3.f);
		}
		else {
			glUniform1f(glGetUniformLocation(currProgram, "particleType"), 2.f);
		}
		
		// particle scales
		glUniform2f(glGetUniformLocation(currProgram, "scale"), pool.motion.scale.x, pool.motion.scale.y);
		gl_has_errors();

		// particle angle
		auto& angle = pool.angle;
		angle += 0.5;
		if (angle >= (2 * M_PI)) {
			angle = 0;
		}
		glUniform1f(glGetUniformLocation(currProgram, "angle"), angle);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, pool.particles.size());
		gl_has_errors();

		glDisable(GL_BLEND);
	}
}

void RenderSystem::drawTexturedMesh(Entity entity,
									const mat3 &projection,
								    GLint& frame,
									GLfloat& frameWidth,
									float elapsed_ms)
{
	Motion &motion = registry.motions.get(entity);
	// Transformation code, see Rendering and Transformation in the template
	// specification for more info Incrementally updates transformation matrix,
	// thus ORDER IS IMPORTANT
	Transform transform;
	transform.translate(motion.position);
	transform.rotate(motion.angle);
	transform.scale(motion.scale);


	assert(registry.renderRequests.has(entity));
	const RenderRequest &render_request = registry.renderRequests.get(entity);

	const GLuint used_effect_enum = (GLuint)render_request.used_effect;
	assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	assert(render_request.used_geometry != GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
	const GLuint vbo = vertex_buffers[(GLuint)render_request.used_geometry];
	const GLuint ibo = index_buffers[(GLuint)render_request.used_geometry];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	// Input data location as in the vertex buffer
	if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED)
	{
		
		GLint silenced_uloc = glGetUniformLocation(program, "silenced");
		gl_has_errors();
		assert(silenced_uloc >= 0);
		if(registry.silenced.has(entity)){
			glUniform1i(silenced_uloc, 1);
		} else {
			glUniform1i(silenced_uloc, 0);
		}

		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
							  sizeof(TexturedVertex), (void *)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void *)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position
		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();

		GLint frame_uloc = glGetUniformLocation(program, "frame");
		GLfloat frame_width_uloc = glGetUniformLocation(program, "frameWidth");
		glUniform1i(frame_uloc, frame);
		glUniform1f(frame_width_uloc, frameWidth);
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::BACKGROUND_OBJ || render_request.used_effect == EFFECT_ASSET_ID::PEBBLE)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_color_loc = glGetAttribLocation(program, "in_color");
		if (render_request.used_effect == EFFECT_ASSET_ID::BACKGROUND_OBJ) {
			if (registry.deformableEntities.has(entity)) {
				auto& backgroundObj = registry.deformableEntities.get(entity);
				glUniform1f(glGetUniformLocation(program, "time"), deformTime);
				if (backgroundObj.shouldDeform) {
					if (!implode) {
						deformTime += elapsed_ms;
					}
					if (deformTime >= 2200 && !implode) {
						implode = true;
						// shouldDeform = 0;
						// deformTime = 0;
					}

					if (implode) {
						deformTime -= elapsed_ms;
					}

					if (implode && deformTime <= 0) {
						implode = false;
						// keeping this variable since it can help for testing all deformations at once.
						shouldDeform = 0.;
						backgroundObj.shouldDeform = false;
						deformTime = 0;
					}
				}
				glUniform1i(glGetUniformLocation(program, "shouldDeform"), backgroundObj.shouldDeform);
				glUniform1i(glGetUniformLocation(program, "deformType2"), backgroundObj.deformType2);
				gl_has_errors();
			}
			else {
				// reaching here implies that we've got a render request but 
				// the entity is missing from the background objects container.
				// Indicates asynchronous behavior. Not very serious, so just log warning.
				glUniform1i(glGetUniformLocation(program, "shouldDeform"), false);
				glUniform1f(glGetUniformLocation(program, "time"), 0.f);
			}
		}
		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(ColoredVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_color_loc);
		glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(ColoredVertex), (void*)sizeof(vec3));
		gl_has_errors();
	}
	else
	{
		assert(false && "Type of render request not supported");
	}

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	const vec3 color = registry.colors.has(entity) ? registry.colors.get(entity) : vec3(1);
	glUniform3fv(color_uloc, 1, (float *)&color);
	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float *)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float *)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

// draw the intermediate texture to the screen, with some distortion to simulate
// water
void RenderSystem::drawToScreen()
{
	// Setting shaders
	// get the water texture, sprite mesh, and program
	glUseProgram(effects[(GLuint)EFFECT_ASSET_ID::WATER]);
	gl_has_errors();
	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(1.f, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_has_errors();
	// Enabling alpha channel for textures
	glDisable(GL_BLEND);
	// glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);

	// Draw the screen texture on the quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]);
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]); // Note, GL_ELEMENT_ARRAY_BUFFER associates
																	 // indices to the bound GL_ARRAY_BUFFER
	gl_has_errors();
	const GLuint water_program = effects[(GLuint)EFFECT_ASSET_ID::WATER];
	// Set clock
	GLuint time_uloc = glGetUniformLocation(water_program, "time");
	GLuint dead_timer_uloc = glGetUniformLocation(water_program, "darken_screen_factor");
	glUniform1f(time_uloc, (float)(glfwGetTime()));
	ScreenState &screen = registry.screenStates.get(screen_state_entity);
	glUniform1f(dead_timer_uloc, screen.darken_screen_factor);

	GLuint resoltion_x_loc = glGetUniformLocation(water_program, "resolutionX");
	GLuint resoltion_y_loc = glGetUniformLocation(water_program, "resolutionY");
	glUniform1f(resoltion_x_loc, (float)w);
	glUniform1f(resoltion_y_loc, (float)h);

	// Think this part causes some lag. Hence the number 10.
	if (isFreeRoam && freeRoamLevel == 10) {
		glUniform1i(glGetUniformLocation(water_program, "enableSpline"), true);
		for (int i = 0; i < splineControlPoints.size(); i++) {
			std::string s1("spline.xCoordinates[");
			std::string s2("spline.yCoordinates[");
			std::string iInS = std::to_string(i);
			s1 += iInS + "]";
			s2 += iInS + "]";

			// printf("%f %f\n", lightBallsXcoords[i], lightBallsYcoords[i]);
			GLuint locX = glGetUniformLocation(water_program, s1.c_str());
			GLuint locY = glGetUniformLocation(water_program, s2.c_str());
			glUniform1f(locX, splineControlPoints[i].x);
			glUniform1f(locY, (float)h - splineControlPoints[i].y);
		}
	}
	else {
		glUniform1i(glGetUniformLocation(water_program, "nextLevelTransition"), false);
	}

	glUniform1i(glGetUniformLocation(water_program, "gameLevel"), gameLevel);

	if (transitioningToNextLevel) {
		glUniform1i(glGetUniformLocation(water_program, "nextLevelTransition"), true);
		glUniform1f(glGetUniformLocation(water_program, "dimScreenFactor"), dimScreenFactor);
		glUniform1f(glGetUniformLocation(water_program, "fogFactor"), fogFactor);
		if (dimScreenFactor >= -0.1) {
			dimScreenFactor -= 0.02;
		}
		if (dimScreenFactor <= 0) {
			fogFactor += 0.01;
		}
	} else {
		dimScreenFactor = 1.f;
		fogFactor = 0.3;
		glUniform1i(glGetUniformLocation(water_program, "nextLevelTransition"), false);
	}

	for (int i = 0; i < lightBallsXcoords.size(); i++) {
		std::string s1("thingie.xCoordinates[");
		std::string s2("thingie.yCoordinates[");
		std::string iInS = std::to_string(i);
		s1 += iInS + "]";
		s2 += iInS + "]";

		// printf("%f %f\n", lightBallsXcoords[i], lightBallsYcoords[i]);
		GLuint locX = glGetUniformLocation(water_program, s1.c_str());
		GLuint locY = glGetUniformLocation(water_program, s2.c_str());
		glUniform1f(locX, lightBallsXcoords[i]);
		glUniform1f(locY, lightBallsYcoords[i]);
	}

	gl_has_errors();
	// Set the vertex position and vertex texture coordinates (both stored in the
	// same VBO)
	GLint in_position_loc = glGetAttribLocation(water_program, "in_position");
	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	gl_has_errors();
	// Draw
	glDrawElements(
		GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
				  // no offset from the bound index buffer
	gl_has_errors();
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void RenderSystem::draw(float elapsed_ms)
{
	// Getting size of window
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	// set time
	time +=1.f;
	//printf("time is  % f", time);

	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();
	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);

	// Background color
	glClearColor(0.54509803921, 0.f, 0.54509803921, 1);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST); // native OpenGL does not work with a depth buffer
							  // and alpha blending, one would have to sort
							  // sprites back to front
	gl_has_errors();
	mat3 projection_2D = createProjectionMatrix();
	std::vector<Entity> needParticleEffects;
	int hasTravellingProjectile = 0;
	std::vector<Entity> dialogues;

	mat3 projectionMat = createProjectionMatrix();
	for (Entity entity : registry.renderRequests.entities) {

		// DISABLE CAMERA FOR NOW

		// Handle camera focus only on swordsman melee
		//if ((registry.runners.has(entity) && !(registry.enemies.has(entity) && registry.enemies.get(entity).enemyType == NECROMANCER_TWO)
		//	|| (registry.companions.has(entity) && registry.companions.get(entity).companionType == SWORDSMAN && registry.companions.get(entity).curr_anim_type == ATTACKING)
		//	|| (registry.enemies.has(entity) 
		//		&& (registry.enemies.get(entity).enemyType == SWORDSMAN || registry.enemies.get(entity).enemyType == NECROMANCER_MINION)
		//		&& registry.enemies.get(entity).curr_anim_type == ATTACKING))) {
		//	Motion& motion = registry.motions.get(entity);

		//	if (registry.companions.has(entity)) {
		//		projectionMat = createCameraProjection(motion);
		//	}
		//	else if (registry.enemies.has(entity)) {
		//		projectionMat = createCameraProjection(motion);
		//	}
		//	
		//}
		// Add only dialogue entities
		if (registry.uiButtons.has(entity) && registry.uiButtons.get(entity).isDialogue) {
			dialogues.push_back(entity);
		}
	}

	// drawLight();

	// Draw all textured meshes that have a position and size component
	for (Entity entity : registry.renderRequests.entities)
	{

		// Skip the dialogue boxes to be rendered later
		if (registry.uiButtons.has(entity) && registry.uiButtons.get(entity).isDialogue) {
			continue;
		}

		mat3 projectionToUse = projectionMat;
		if (!registry.motions.has(entity))
			continue;

		//if (registry.light.has(entity)) {
		//	drawLight();	// only transparency effect??
		//}

		if (registry.particlePools.has(entity)) {
			needParticleEffects.push_back(entity);
		}
		if (registry.particlePools.has(entity) && registry.particlePools.get(entity).areTypeDeath) {
			// if an entity has particles of type "death", that means the 
			// entity is dead. So, no need to render the dead entity.
			// (Do nothing inside this conditional)
		}
		else {
			// Note, its not very efficient to access elements indirectly via the entity
			// albeit iterating through all Sprites in sequence. A good point to optimize

			// Only provide frames if the entity is supposed to be animated
			GLint curr_frame = 0;
			GLfloat frame_width = 0;
			int numFrames = 0;
			int timePerFrame = 0;

			// Handle animation frames
			if (registry.companions.has(entity) || registry.enemies.has(entity)) {

				//// Get the frame counter for current character and decrement it
				float* counter = registry.companions.has(entity) ? &registry.companions.get(entity).frame_counter_ms
					: &registry.enemies.get(entity).frame_counter_ms;

				*counter -= elapsed_ms;

				// Get the type of character (MAGE, SWORDSMAN, ARCHER, HEALER, NECROMANCER)
				int charType = registry.companions.has(entity) ? registry.companions.get(entity).companionType
					: registry.enemies.get(entity).enemyType;
				// Get the type of animation (IDLE, ATTACKING, DEAD)
				int animType = registry.companions.has(entity) ? registry.companions.get(entity).curr_anim_type
					: registry.enemies.get(entity).curr_anim_type;
				// Get the current texture to alter
				TEXTURE_ASSET_ID& currTexture = registry.renderRequests.get(entity).used_texture;
				// Get the current geometry to alter
				GEOMETRY_BUFFER_ID& currGeometry = registry.renderRequests.get(entity).used_geometry;
				// Get the current frame
				int* currFrame = registry.companions.has(entity) ? &registry.companions.get(entity).curr_frame
					: &registry.enemies.get(entity).curr_frame;

				switch (charType) {
					case MAGE: {
						switch (animType) {
							case IDLE: {
								if (currGeometry != GEOMETRY_BUFFER_ID::MAGE_IDLE) {
									currGeometry = GEOMETRY_BUFFER_ID::MAGE_IDLE;
									*currFrame = 0;
								}
								numFrames = MAGE_IDLE_FRAMES; frame_width = MAGE_IDLE_FRAME_WIDTH; timePerFrame = MAGE_IDLE_FRAME_TIME; break;
							}
							case ATTACKING: {
								if (currGeometry != GEOMETRY_BUFFER_ID::MAGE_CASTING) {
									currGeometry = GEOMETRY_BUFFER_ID::MAGE_CASTING;
									*currFrame = 0;
								}
								numFrames = MAGE_CASTING_FRAMES; frame_width = MAGE_CASTING_FRAME_WIDTH; timePerFrame = MAGE_ATTACK_FRAME_TIME; break;
							}
							case DEAD: {
								if (currGeometry != GEOMETRY_BUFFER_ID::MAGE_DEATH) {
									currGeometry = GEOMETRY_BUFFER_ID::MAGE_DEATH;
									*currFrame = 0;
								}
								numFrames = MAGE_DEATH_FRAMES; frame_width = MAGE_DEATH_FRAME_WIDTH; timePerFrame = MAGE_DEATH_FRAME_TIME; break;
							}
							default: break;
						}
						break;
					}
					case SWORDSMAN: {
						switch (animType) {
							case IDLE: {
								if (currGeometry != GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE) {
									*currFrame = 0;
								}
								currTexture = TEXTURE_ASSET_ID::SWORDSMAN_IDLE;
								currGeometry = GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE;
								numFrames = SWORDSMAN_IDLE_FRAMES; frame_width = SWORDSMAN_IDLE_FRAME_WIDTH; timePerFrame = SWORDSMAN_IDLE_FRAME_TIME; break;
							}
							case ATTACKING: {
								switch (registry.attackers.get(entity).attack_type) {
									case MELEE: {
										if (currGeometry != GEOMETRY_BUFFER_ID::SWORDSMAN_MELEE) {
											*currFrame = 0;
										}
										currTexture = TEXTURE_ASSET_ID::SWORDSMAN_MELEE;
										currGeometry = GEOMETRY_BUFFER_ID::SWORDSMAN_MELEE;
										numFrames = SWORDSMAN_MELEE_FRAMES; frame_width = SWORDSMAN_MELEE_FRAME_WIDTH; timePerFrame = SWORDSMAN_MELEE_FRAME_TIME; break;
									}
									case TAUNT: {
										if (currGeometry != GEOMETRY_BUFFER_ID::SWORDSMAN_TAUNT) {
											*currFrame = 0;
										}
										currTexture = TEXTURE_ASSET_ID::SWORDSMAN_TAUNT;
										currGeometry = GEOMETRY_BUFFER_ID::SWORDSMAN_TAUNT;
										numFrames = SWORDSMAN_TAUNT_FRAMES; frame_width = SWORDSMAN_TAUNT_FRAME_WIDTH; timePerFrame = SWORDSMAN_TAUNT_FRAME_TIME; break;
									}
									case FIREBALL: {
										if (currGeometry != GEOMETRY_BUFFER_ID::SWORDSMAN_TAUNT) {
											*currFrame = 0;
										}
										currTexture = TEXTURE_ASSET_ID::SWORDSMAN_TAUNT;
										currGeometry = GEOMETRY_BUFFER_ID::SWORDSMAN_TAUNT;
										numFrames = SWORDSMAN_TAUNT_FRAMES; frame_width = SWORDSMAN_TAUNT_FRAME_WIDTH; timePerFrame = SWORDSMAN_TAUNT_FRAME_TIME; break;
									}
									default: break;
								}
							    break;
							}
							case WALKING: {
								if (currGeometry != GEOMETRY_BUFFER_ID::SWORDSMAN_WALK) {
									currTexture = TEXTURE_ASSET_ID::SWORDSMAN_WALK;
									currGeometry = GEOMETRY_BUFFER_ID::SWORDSMAN_WALK;
									*currFrame = 0;
								}
								numFrames = SWORDSMAN_WALK_FRAMES; frame_width = SWORDSMAN_WALK_FRAME_WIDTH; timePerFrame = SWORDSMAN_WALK_FRAME_TIME; break;
							}
							case DEAD: {
								if (currGeometry != GEOMETRY_BUFFER_ID::SWORDSMAN_DEATH) {
									currTexture = TEXTURE_ASSET_ID::SWORDSMAN_DEATH;
									currGeometry = GEOMETRY_BUFFER_ID::SWORDSMAN_DEATH;
									*currFrame = 0;
								}
								numFrames = SWORDSMAN_DEATH_FRAMES; frame_width = SWORDSMAN_DEATH_FRAME_WIDTH; timePerFrame = SWORDSMAN_DEATH_FRAME_TIME; break;
							}
							default: break;
						}
						break;
				}
				case NECROMANCER_ONE: {
					switch (animType) {
						case IDLE: {
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_ONE_IDLE) {
								*currFrame = 0;
							}
							currTexture = TEXTURE_ASSET_ID::NECRO_ONE_IDLE;
							currGeometry = GEOMETRY_BUFFER_ID::NECRO_ONE_IDLE;
							numFrames = NECRO_ONE_IDLE_FRAMES; frame_width = NECRO_ONE_IDLE_FRAME_WIDTH; timePerFrame = NECRO_ONE_IDLE_FRAME_TIME; break;
						}
						case ATTACKING: {
							switch (registry.attackers.get(entity).attack_type) {
								/*case SUMMONING: {
									if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_ONE_SUMMONING) {
										*currFrame = 0;
									}	
									currTexture = TEXTURE_ASSET_ID::NECRO_ONE_SUMMONING;
									currGeometry = GEOMETRY_BUFFER_ID::NECRO_ONE_SUMMONING;
									numFrames = NECRO_ONE_SUMMONING_FRAMES; frame_width = NECRO_ONE_SUMMONING_FRAME_WIDTH; timePerFrame = NECRO_ONE_SUMMONING_FRAME_TIME; break;
								}*/
								case SUMMONING: {
									if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_ONE_SUMMONING) {
										*currFrame = 0;
									}
									currTexture = TEXTURE_ASSET_ID::NECRO_ONE_SUMMONING;
									currGeometry = GEOMETRY_BUFFER_ID::NECRO_ONE_SUMMONING;
									numFrames = NECRO_ONE_SUMMONING_FRAMES; frame_width = NECRO_ONE_SUMMONING_FRAME_WIDTH; timePerFrame = NECRO_ONE_SUMMONING_FRAME_TIME;

									if (*currFrame == NECRO_ONE_SUMMONING_FRAMES - 1) {
										registry.enemies.get(entity).curr_anim_type = IDLE;
										currTexture = TEXTURE_ASSET_ID::NECRO_ONE_IDLE;
										currGeometry = GEOMETRY_BUFFER_ID::NECRO_ONE_IDLE;
										numFrames = NECRO_ONE_IDLE_FRAMES; frame_width = NECRO_ONE_IDLE_FRAME_WIDTH; timePerFrame = NECRO_ONE_IDLE_FRAME_TIME; break;
									}
									break;
								}
								default: {
									if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_ONE_CASTING) {
										*currFrame = 0;
									}
									currTexture = TEXTURE_ASSET_ID::NECRO_ONE_CASTING;
									currGeometry = GEOMETRY_BUFFER_ID::NECRO_ONE_CASTING;
									numFrames = NECRO_ONE_CASTING_FRAMES; frame_width = NECRO_ONE_CASTING_FRAME_WIDTH; timePerFrame = NECRO_ONE_CASTING_FRAME_TIME; break;
								}
							}
							break;
						}
						case DEAD: {
							// Starting to die
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_ONE && currGeometry != GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_TWO) {
								*currFrame = 0;
								currTexture = TEXTURE_ASSET_ID::NECRO_ONE_DEATH_ONE;
								currGeometry = GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_ONE;
								numFrames = NECRO_ONE_DEATH_ONE_FRAMES; frame_width = NECRO_ONE_DEATH_ONE_FRAME_WIDTH;
							}
							else if (currGeometry == GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_ONE) {
								currTexture = TEXTURE_ASSET_ID::NECRO_ONE_DEATH_ONE;
								currGeometry = GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_ONE;
								numFrames = NECRO_ONE_DEATH_ONE_FRAMES; frame_width = NECRO_ONE_DEATH_ONE_FRAME_WIDTH;

								// Half-dead, switch to second death anim
								if (*currFrame == NECRO_ONE_DEATH_ONE_FRAMES - 1) {
									*currFrame = 0;
									currTexture = TEXTURE_ASSET_ID::NECRO_ONE_DEATH_TWO;
									currGeometry = GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_TWO;
									numFrames = NECRO_ONE_DEATH_TWO_FRAMES; frame_width = NECRO_ONE_DEATH_TWO_FRAME_WIDTH;
								}
							}
							else if (currGeometry == GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_TWO) {
								currTexture = TEXTURE_ASSET_ID::NECRO_ONE_DEATH_TWO;
								currGeometry = GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_TWO;
								numFrames = NECRO_ONE_DEATH_TWO_FRAMES; frame_width = NECRO_ONE_DEATH_TWO_FRAME_WIDTH;
							}
							timePerFrame = NECRO_ONE_DEATH_FRAME_TIME; break;
						}
						default: break;
					}
					break;
				}
				case NECROMANCER_TWO: {
					switch (animType) {
						case APPEARING: {
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_TWO_APPEAR) {
								*currFrame = 0;
							}
							currTexture = TEXTURE_ASSET_ID::NECRO_TWO_APPEAR;
							currGeometry = GEOMETRY_BUFFER_ID::NECRO_TWO_APPEAR;
							numFrames = NECRO_TWO_APPEAR_FRAMES; frame_width = NECRO_TWO_APPEAR_FRAME_WIDTH; timePerFrame = NECRO_TWO_APPEAR_FRAME_TIME; break;
						}
						case IDLE: {
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_TWO_IDLE) {
								*currFrame = 0;
							}
							currTexture = TEXTURE_ASSET_ID::NECRO_TWO_IDLE;
							currGeometry = GEOMETRY_BUFFER_ID::NECRO_TWO_IDLE;
							numFrames = NECRO_TWO_IDLE_FRAMES; frame_width = NECRO_TWO_IDLE_FRAME_WIDTH; timePerFrame = NECRO_TWO_IDLE_FRAME_TIME; break;
						}
						case ATTACKING: {
							switch (registry.attackers.get(entity).attack_type) {
								case BLEEDMELEE: {
									if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_TWO_MELEE) {
									*currFrame = 0;
								}
								currTexture = TEXTURE_ASSET_ID::NECRO_TWO_MELEE;
								currGeometry = GEOMETRY_BUFFER_ID::NECRO_TWO_MELEE;
								numFrames = NECRO_TWO_MELEE_FRAMES; frame_width = NECRO_TWO_MELEE_FRAME_WIDTH; timePerFrame = NECRO_TWO_MELEE_FRAME_TIME; break;
								}
								case AOEMELEE: {
									if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_TWO_MELEE) {
										*currFrame = 0;
									}
									currTexture = TEXTURE_ASSET_ID::NECRO_TWO_MELEE;
									currGeometry = GEOMETRY_BUFFER_ID::NECRO_TWO_MELEE;
									numFrames = NECRO_TWO_MELEE_FRAMES; frame_width = NECRO_TWO_MELEE_FRAME_WIDTH; timePerFrame = NECRO_TWO_MELEE_FRAME_TIME; break;
								}
								case CHARGING: {
									if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_TWO_CASTING) {
										*currFrame = 0;
									}
									currTexture = TEXTURE_ASSET_ID::NECRO_TWO_CASTING;
									currGeometry = GEOMETRY_BUFFER_ID::NECRO_TWO_CASTING;
									numFrames = NECRO_TWO_CASTING_FRAMES; frame_width = NECRO_TWO_CASTING_FRAME_WIDTH; timePerFrame = NECRO_TWO_CASTING_FRAME_TIME; break;
								}
								case ULTI: {
									if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_TWO_DEATH) {
										*currFrame = 0;
									}
									currTexture = TEXTURE_ASSET_ID::NECRO_TWO_DEATH;
									currGeometry = GEOMETRY_BUFFER_ID::NECRO_TWO_DEATH;
									// Use first three frames of death anim for particle beam ult
									numFrames = 3; frame_width = NECRO_TWO_DEATH_FRAME_WIDTH; timePerFrame = 550; break;
								}
								default: {
									if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_TWO_CASTING) {
										*currFrame = 0;
									}
									currTexture = TEXTURE_ASSET_ID::NECRO_TWO_CASTING;
									currGeometry = GEOMETRY_BUFFER_ID::NECRO_TWO_CASTING;
									numFrames = NECRO_TWO_CASTING_FRAMES; frame_width = NECRO_TWO_CASTING_FRAME_WIDTH; timePerFrame = NECRO_TWO_CASTING_FRAME_TIME; break;
								}
								// Todo: Add more spell cases later
							}
							break;
						}
						case WALKING: {
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_TWO_MELEE) {
								*currFrame = 0;
							}
							currTexture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
							currGeometry = GEOMETRY_BUFFER_ID::NECRO_TWO_MELEE;
							numFrames = NECRO_TWO_MELEE_FRAMES; frame_width = NECRO_TWO_MELEE_FRAME_WIDTH; timePerFrame = NECRO_TWO_MELEE_FRAME_TIME; break;
						}
						case DEAD: {
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_TWO_DEATH) {
								*currFrame = 0;
							}
							currTexture = TEXTURE_ASSET_ID::NECRO_TWO_DEATH;
							currGeometry = GEOMETRY_BUFFER_ID::NECRO_TWO_DEATH;
							numFrames = NECRO_TWO_DEATH_FRAMES; frame_width = NECRO_TWO_DEATH_FRAME_WIDTH; timePerFrame = NECRO_TWO_DEATH_FRAME_TIME; break;
						}
						default: break;
						}
					break;
				}
				case NECROMANCER_MINION: {
					switch (animType) {
						case APPEARING: {
							// Finished appearing, switch to idle anim
							if (*currFrame == NECRO_MINION_APPEAR_FRAMES - 1) {
								*currFrame = 0;
								// Change state here
								registry.enemies.get(entity).curr_anim_type = IDLE;
								currTexture = TEXTURE_ASSET_ID::NECRO_MINION_IDLE;
								currGeometry = GEOMETRY_BUFFER_ID::NECRO_MINION_IDLE;
								numFrames = NECRO_MINION_IDLE_FRAMES; frame_width = NECRO_MINION_IDLE_FRAME_WIDTH; timePerFrame = NECRO_MINION_IDLE_FRAME_TIME;
							}
							else {
								currTexture = TEXTURE_ASSET_ID::NECRO_MINION_APPEAR;
								currGeometry = GEOMETRY_BUFFER_ID::NECRO_MINION_APPEAR;
								numFrames = NECRO_MINION_APPEAR_FRAMES; frame_width = NECRO_MINION_APPEAR_FRAME_WIDTH; timePerFrame = NECRO_MINION_APPEAR_FRAME_TIME;
							}
							break;
							
						}
						case IDLE: {
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_MINION_IDLE) {
								*currFrame = 0;
							}
							currTexture = TEXTURE_ASSET_ID::NECRO_MINION_IDLE;
							currGeometry = GEOMETRY_BUFFER_ID::NECRO_MINION_IDLE;
							numFrames = NECRO_MINION_IDLE_FRAMES; frame_width = NECRO_MINION_IDLE_FRAME_WIDTH; timePerFrame = NECRO_MINION_IDLE_FRAME_TIME; break;
						}
						case ATTACKING: {
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_MINION_MELEE) {
								*currFrame = 0;
							}
							currTexture = TEXTURE_ASSET_ID::NECRO_MINION_MELEE;
							currGeometry = GEOMETRY_BUFFER_ID::NECRO_MINION_MELEE;
							numFrames = NECRO_MINION_MELEE_FRAMES; frame_width = NECRO_MINION_MELEE_FRAME_WIDTH; timePerFrame = NECRO_MINION_MELEE_FRAME_TIME; break;
						}
						case WALKING: {
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_MINION_WALK) {
								currTexture = TEXTURE_ASSET_ID::NECRO_MINION_WALK;
								currGeometry = GEOMETRY_BUFFER_ID::NECRO_MINION_WALK;
								*currFrame = 0;
							}
							numFrames = NECRO_MINION_WALK_FRAMES; frame_width = NECRO_MINION_WALK_FRAME_WIDTH; timePerFrame = NECRO_MINION_WALK_FRAME_TIME; break;
						}
						case DEAD: {
							if (currGeometry != GEOMETRY_BUFFER_ID::NECRO_MINION_DEATH) {
								currTexture = TEXTURE_ASSET_ID::NECRO_MINION_DEATH;
								currGeometry = GEOMETRY_BUFFER_ID::NECRO_MINION_DEATH;
								*currFrame = 0;
							}
							numFrames = NECRO_MINION_DEATH_FRAMES; frame_width = NECRO_MINION_DEATH_FRAME_WIDTH; timePerFrame = NECRO_MINION_DEATH_FRAME_TIME; break;
						}
						default: break;
					}
					break;
				}
				case ARCHER: {
					frame_width = ARCHER_FRAME_WIDTH;
					switch (animType) {
						case IDLE: {
							if (currGeometry != GEOMETRY_BUFFER_ID::ARCHER_IDLE) {
								*currFrame = 0;
							}
							currGeometry = GEOMETRY_BUFFER_ID::ARCHER_IDLE;
							numFrames = ARCHER_IDLE_FRAMES; timePerFrame = ARCHER_IDLE_FRAME_TIME; break;
						}
						case WALKING: {
							if (currGeometry != GEOMETRY_BUFFER_ID::ARCHER_WALKING) {
								*currFrame = 0;
							}
							currGeometry = GEOMETRY_BUFFER_ID::ARCHER_WALKING;
							numFrames = ARCHER_WALKING_FRAMES; timePerFrame = ARCHER_WALKING_FRAME_TIME; break;
						}
						case JUMPING: {
							timePerFrame = ARCHER_JUMPING_FRAME_TIME; numFrames = ARCHER_JUMPING_FRAMES;
							if (currGeometry != GEOMETRY_BUFFER_ID::ARCHER_JUMPING) {
								*currFrame = 0;
							}
							currGeometry = GEOMETRY_BUFFER_ID::ARCHER_JUMPING;

							// Only one frame for falling
							if (registry.motions.get(entity).velocity.y > 0.f) {
								numFrames = 1;
							}
							break;
						}
						case ATTACKING: {
							numFrames = ARCHER_ATTACKING_FRAMES; timePerFrame = ARCHER_ATTACKING_FRAME_TIME;

							if (registry.motions.get(entity).scale.x == 160.f) {
								// Slower attack frames for turn-based battle
								timePerFrame = ARCHER_TURN_ATTACKING_FRAME_TIME;
								if (currGeometry != GEOMETRY_BUFFER_ID::ARCHER_ATTACKING) {
									currGeometry = GEOMETRY_BUFFER_ID::ARCHER_ATTACKING;
									*currFrame = 0;
								}
								break;
							}

							if (currGeometry != GEOMETRY_BUFFER_ID::ARCHER_ATTACKING) {
								currGeometry = GEOMETRY_BUFFER_ID::ARCHER_ATTACKING;
								*currFrame = 0;
							}
							// Switch to IDLE after one full cycle of shooting anim
							else if (currGeometry == GEOMETRY_BUFFER_ID::ARCHER_ATTACKING && *currFrame == ARCHER_ATTACKING_FRAMES - 1) {
								currGeometry = GEOMETRY_BUFFER_ID::ARCHER_IDLE;
								registry.companions.get(entity).curr_anim_type = IDLE;
								*currFrame = 0;
								numFrames = ARCHER_IDLE_FRAMES; timePerFrame = ARCHER_IDLE_FRAME_TIME;
							}
							break;
						}
						case WALK_ATTACKING: {
							numFrames = ARCHER_ATTACKING_FRAMES; timePerFrame = ARCHER_ATTACKING_FRAME_TIME;
							if (currGeometry != GEOMETRY_BUFFER_ID::ARCHER_WALK_ATTACKING) {
								currGeometry = GEOMETRY_BUFFER_ID::ARCHER_WALK_ATTACKING;
								*currFrame = 0;
							}
							// Switch to IDLE after one full cycle of shooting anim
							else if (currGeometry == GEOMETRY_BUFFER_ID::ARCHER_WALK_ATTACKING && *currFrame == ARCHER_ATTACKING_FRAMES - 1) {
								currGeometry = GEOMETRY_BUFFER_ID::ARCHER_IDLE;
								registry.companions.get(entity).curr_anim_type = IDLE;
								*currFrame = 0;
								numFrames = ARCHER_IDLE_FRAMES; timePerFrame = ARCHER_IDLE_FRAME_TIME;
							}
							break;
						}
						case DEAD: {
 							if (currGeometry != GEOMETRY_BUFFER_ID::ARCHER_DEAD) {
 								currGeometry = GEOMETRY_BUFFER_ID::ARCHER_DEAD;
 								*currFrame = 0;
 							}
 							numFrames = ARCHER_DEAD_FRAMES; 
							timePerFrame = ARCHER_DEAD_FRAME_TIME; 
							break;
 						}
						default: break;
					}
					break;
				}
				case DRAGON: {
					switch (animType) {
					case IDLE: {
						if (currGeometry != GEOMETRY_BUFFER_ID::DRAGON_FLYING) {
							*currFrame = 0;
						}
						currGeometry = GEOMETRY_BUFFER_ID::DRAGON_FLYING;
						numFrames = DRAGON_FLYING_FRAMES;  frame_width = DRAGON_FLYING_FRAME_WIDTH; timePerFrame = DRAGON_FLYING_FRAME_TIME; 
						
						// Flip dragon when reaching edge
						if (registry.bird.components[0].birdNextPostionTracker == 163) {
							registry.motions.get(entity).scale.x = abs(registry.motions.get(entity).scale.x);
						}
						else if (registry.bird.components[0].birdNextPostionTracker == 1) {
							registry.motions.get(entity).scale.x = -abs(registry.motions.get(entity).scale.x);
						}

						break;
					}
					default: break;
					}
					break;
				}
				default: break;
			}

				if (*counter <= 0) {
					
					if (registry.companions.has(entity) || registry.enemies.has(entity)) {
						curr_frame = *currFrame;
						// Increment frame count
						curr_frame += 1;
						*currFrame = curr_frame % numFrames;
					}
					// Reset frame timer
					*counter = timePerFrame;
				}
				else {
					curr_frame = *currFrame;
				}
			}

			// Handle the background layers and scrolling
			if (registry.backgroundLayers.has(entity)) {
				if (registry.backgroundLayers.get(entity).isAutoScroll) {
					registry.backgroundLayers.get(entity).scrollX += AUTOSCROLL_RATE;
					curr_frame = registry.backgroundLayers.get(entity).scrollX;
					frame_width = 0.01;
				}
				else if (registry.backgroundLayers.get(entity).isCameraScrollOne && hasTravellingProjectile) {
					if (hasTravellingProjectile == 1) {
						registry.backgroundLayers.get(entity).scrollX += CAMERA_SCROLL_RATE_ONE;
					}
					else {
						registry.backgroundLayers.get(entity).scrollX -= CAMERA_SCROLL_RATE_ONE;
					}
					curr_frame = registry.backgroundLayers.get(entity).scrollX;
					frame_width = 0.001;
				}
				else if (registry.backgroundLayers.get(entity).isCameraScrollTwo && hasTravellingProjectile) {
					if (hasTravellingProjectile == 1) {
						registry.backgroundLayers.get(entity).scrollX += CAMERA_SCROLL_RATE_TWO;
					}
					else {
						registry.backgroundLayers.get(entity).scrollX -= CAMERA_SCROLL_RATE_TWO;
					}
					curr_frame = registry.backgroundLayers.get(entity).scrollX;
					frame_width = 0.001;
				}

			}
			// UI-related entities should remain in constant position on screen
			if (registry.buttons.has(entity) || registry.turnIndicators.has(entity) || registry.uiButtons.has(entity)) projectionToUse = projection_2D;

			if (registry.enemies.has(entity)) {
				deferredRenderingEntities.emplace(registry.enemies.get(entity).healthbar, entity);
			}
			if (transitioningToNextLevel && registry.enemies.has(entity)) {
				// delay rendering of enemies when transitioning to next level
			} else if (transitioningToNextLevel && deferredRenderingEntities.count(entity) > 0) {
				// delay rendering of enemy healthbar when transitioning to next level
			} else {
				drawTexturedMesh(entity, projectionToUse, curr_frame, frame_width, elapsed_ms);
			}
		}

	}
	// Truely render to the screen

	drawToScreen();

	if (isFreeRoam && (freeRoamLevel == 2)) {
		for (int i = 0; i < registry.motions.components.size(); i++) {
			Entity e = registry.motions.entities[i];
			if (registry.light.has(e)) {
				if (registry.fireflySwarm.has(e)) {
					// update positions of fireflies in the buffer
					for (Entity e : registry.fireflySwarm.entities) {
						if (registry.motions.has(e)) {
							auto& fireFlyMotion = registry.motions.get(e);
							fireFlyPosX.push_back(fireFlyMotion.position.x);
							fireFlyPosY.push_back(fireFlyMotion.position.y);
						}
					}
				}
				drawLight(e);
			}
		}
	}

	// render particles at the end
	for (auto& entity : needParticleEffects) {
		drawDeathParticles(entity, projection_2D);
	}

	// Render the dialogue boxes
	for (auto& entity : dialogues) {
		GLint curr_frame = 0;
		GLfloat frame_width = 0;
		drawTexturedMesh(entity, projection_2D, curr_frame, frame_width, elapsed_ms);
	}
	
	// flicker-free display with a double buffer
	glfwSwapBuffers(window);
	gl_has_errors();

	fireFlyPosX.clear();
	fireFlyPosY.clear();
}

mat3 RenderSystem::createProjectionMatrix()
{
	// Fake projection matrix, scales with respect to window coordinates
	float left = 0.f;
	float top = 0.f;

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	gl_has_errors();
	float right = (float)w / screen_scale;
	float bottom = (float)h / screen_scale;

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	return {{sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx , ty, 1.f}};
}

mat3 RenderSystem::createCameraProjection(Motion& motion)
{
	float left = (motion.position.x - abs(motion.scale.x / 2)) - CAMERA_OFFSET_LEFT;
	float top = (motion.position.y - abs(motion.scale.y / 2)) - CAMERA_OFFSET_TOP;

	float right = (motion.position.x + abs(motion.scale.x / 2)) + CAMERA_OFFSET_RIGHT;
	float bottom = (motion.position.y + abs(motion.scale.y / 2)) + CAMERA_OFFSET_BOTTOM;

	float sx = 2.f / abs(right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	return { {sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f} };
}