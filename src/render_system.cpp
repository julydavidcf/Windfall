// internal
#include "render_system.hpp"
#include <SDL.h>

#include <string>
#include <sstream>

#include "tiny_ecs_registry.hpp"

void RenderSystem::drawDeathParticles(Entity entity, const mat3& projection)
{
	auto& particles = registry.deathParticles.get(entity).deathParticles;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for (auto& particle : particles) {
		if (particle.Life > 0.f) {
			Transform transform;
			transform.translate(particle.motion.position);
			transform.rotate(particle.motion.angle);
			transform.scale(particle.motion.scale);

			const GLuint used_effect_enum = (GLuint)EFFECT_ASSET_ID::PARTICLE;
			// assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
			const GLuint program = (GLuint)effects[used_effect_enum];

			// Setting shaders
			glUseProgram(program);
			gl_has_errors();

			// assert(render_request.used_geometry != GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
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

			// Enabling and binding texture to slot 0
			glActiveTexture(GL_TEXTURE0);
			gl_has_errors();
			GLuint texture_id = texture_gl_handles[(GLuint)TEXTURE_ASSET_ID::DEATH_PARTICLE];
			glBindTexture(GL_TEXTURE_2D, texture_id);
			gl_has_errors();

			// Getting uniform locations for glUniform* calls
			GLint color_uloc = glGetUniformLocation(program, "deathParticleColor");
			// const vec3 color = registry.colors.has(entity) ? registry.colors.get(entity) : vec3(1);
			glUniform4fv(color_uloc, 1, (float*)&particle.Color);
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
			glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float*)&transform.mat);
			GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
			glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float*)&projection);
			gl_has_errors();
			// Drawing of num_indices/3 triangles specified in the index buffer
			glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
			gl_has_errors();
		}
	}
	glDisable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderSystem::drawTexturedMesh(Entity entity,
									const mat3 &projection,
								    GLint& frame,
									GLfloat& frameWidth)
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
	else if (render_request.used_effect == EFFECT_ASSET_ID::PEBBLE)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_color_loc = glGetAttribLocation(program, "in_color");
		gl_has_errors();

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

	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();
	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);

	// Background color
	glClearColor(0.2, 0.2, 0.2, 0.65);
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
	
	mat3 projectionMat = createProjectionMatrix();
	for (Entity entity : registry.renderRequests.entities) {
		// Handle camera focus on projectiles
		if (registry.projectiles.has(entity) && registry.projectiles.get(entity).flyingTimer > 0) {
			Motion& motion = registry.motions.get(entity);
			projectionMat = createCameraProjection(motion);
		}
	}

	// Draw all textured meshes that have a position and size component
	for (Entity entity : registry.renderRequests.entities)
	{
		if (!registry.motions.has(entity))
			continue;
		// if an entity has a deathParticle compononent, that means the 
		// entity is dead. So, no need to render the dead entity.
		if (registry.deathParticles.has(entity)) {
			needParticleEffects.push_back(entity);
		}
		else {
			// Note, its not very efficient to access elements indirectly via the entity
			// albeit iterating through all Sprites in sequence. A good point to optimize

			// Only provide frames if the entity is supposed to be animated
			GLint curr_frame = 0;
			GLfloat frame_width = 0;
			int numFrames = 0;

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

				switch (charType) {
					case MAGE: {
						switch (animType) {
							case IDLE: numFrames = MAGE_IDLE_FRAMES; frame_width = MAGE_IDLE_FRAME_WIDTH; break;
							case ATTACKING: numFrames = MAGE_IDLE_FRAMES; frame_width = MAGE_IDLE_FRAME_WIDTH; break;
							case DEAD: numFrames = MAGE_IDLE_FRAMES; frame_width = MAGE_IDLE_FRAME_WIDTH; break;
							default: break;
							}
						break;
					}
					case SWORDSMAN: {
						switch (animType) {
							case IDLE: numFrames = SWORDSMAN_IDLE_FRAMES; frame_width = SWORDSMAN_IDLE_FRAME_WIDTH; break;
							case ATTACKING: numFrames = SWORDSMAN_IDLE_FRAMES; frame_width = SWORDSMAN_IDLE_FRAME_WIDTH; break;
							case DEAD: numFrames = SWORDSMAN_IDLE_FRAMES; frame_width = SWORDSMAN_IDLE_FRAME_WIDTH; break;
							default: break;
							}
						break;
					case NECROMANCER: {
						switch (animType) {
						case IDLE: numFrames = NECROMANCER_IDLE_FRAMES; frame_width = NECROMANCER_IDLE_FRAME_WIDTH; break;
						case ATTACKING: numFrames = NECROMANCER_IDLE_FRAMES; frame_width = NECROMANCER_IDLE_FRAMES; break;
						case DEAD: numFrames = NECROMANCER_IDLE_FRAMES; frame_width = NECROMANCER_IDLE_FRAMES; break;
						default: break;
						}
						break;
					}
				}
					// TODO: Implement healer, archer, necromancer cases later
				default: break;
				}

				if (*counter <= 0) {
					// Increment frame count
					if (registry.companions.has(entity)) {
						curr_frame = registry.companions.get(entity).curr_frame;
						curr_frame += 1;
						registry.companions.get(entity).curr_frame = curr_frame % numFrames;
					}
					else if (registry.enemies.has(entity)) {
						curr_frame = registry.enemies.get(entity).curr_frame;
						curr_frame += 1;
						registry.enemies.get(entity).curr_frame = curr_frame % numFrames;
					}
					// Reset frame timer
					*counter = TIME_PER_FRAME;
				}
				else {
					curr_frame = registry.companions.has(entity) ? registry.companions.get(entity).curr_frame
																 : registry.enemies.get(entity).curr_frame;
				}

			}
			drawTexturedMesh(entity, projectionMat, curr_frame, frame_width);
		}

	}

	// Truely render to the screen
	drawToScreen();

	// render particles at the end
	for (auto& entity : needParticleEffects) {
		drawDeathParticles(entity, projection_2D);
	}
	
	// flicker-free display with a double buffer
	glfwSwapBuffers(window);
	gl_has_errors();
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
	float left = (motion.position.x - motion.scale.x / 2) - CAMERA_OFFSET_LEFT;
	float top = (motion.position.y - motion.scale.y / 2) - CAMERA_OFFSET_TOP;

	float right = (motion.position.x + motion.scale.x / 2) + CAMERA_OFFSET_RIGHT;
	float bottom = (motion.position.y + motion.scale.y / 2) + CAMERA_OFFSET_BOTTOM;

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	return { {sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f} };
}