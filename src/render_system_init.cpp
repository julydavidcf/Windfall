// internal
#include "render_system.hpp"

#include <array>
#include <fstream>

#include "../ext/stb_image/stb_image.h"

// This creates circular header inclusion, that is quite bad.
#include "tiny_ecs_registry.hpp"

// stlib
#include <iostream>
#include <sstream>

namespace {
	float RandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}

	const int NUM_LIGHT_BALLS_BACKGROUND = 100;
};

// World initialization
bool RenderSystem::init(int width, int height, GLFWwindow* window_arg)
{
	this->window = window_arg;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // vsync

	// Load OpenGL function pointers
	const int is_fine = gl3w_init();
	assert(is_fine == 0);

	// Create a frame buffer
	frame_buffer = 0;
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();

	// For some high DPI displays (ex. Retina Display on Macbooks)
	// https://stackoverflow.com/questions/36672935/why-retina-screen-coordinate-value-is-twice-the-value-of-pixel-value
	int fb_width, fb_height;
	glfwGetFramebufferSize(window, &fb_width, &fb_height);
	screen_scale = static_cast<float>(fb_width) / width;
	(int)height; // dummy to avoid warning

	// ASK(Camilo): Setup error callback. This can not be done in mac os, so do not enable
	// it unless you are on Linux or Windows. You will need to change the window creation
	// code to use OpenGL 4.3 (not suported on mac) and add additional .h and .cpp
	// glDebugMessageCallback((GLDEBUGPROC)errorCallback, nullptr);

	// We are not really using VAO's but without at least one bound we will crash in
	// some systems.
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	gl_has_errors();

	initScreenTexture();
    initializeGlTextures();
	initializeGlEffects();
	initializeGlGeometryBuffers();
	createRandomLightBallPosForBackground(width, height);
	// initParticlesBuffer();

	return true;
}

void RenderSystem::initializeGlTextures()
{
    glGenTextures((GLsizei)texture_gl_handles.size(), texture_gl_handles.data());

    for(uint i = 0; i < texture_paths.size(); i++)
    {
        const std::string& path = texture_paths[i];
        ivec2& dimensions = texture_dimensions[i];

        stbi_uc* data;
        data  = stbi_load(path.c_str(), &dimensions.x, &dimensions.y, NULL, 4);

        if (data == NULL)
        {
			const std::string message = "Could not load the file " + path + ".";
            fprintf(stderr, "%s", message.c_str());
            assert(false);
        }
        glBindTexture(GL_TEXTURE_2D, texture_gl_handles[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl_has_errors();
        stbi_image_free(data);
    }
	gl_has_errors();
}

void RenderSystem::initializeGlEffects()
{
	for(uint i = 0; i < effect_paths.size(); i++)
	{
		const std::string vertex_shader_name = effect_paths[i] + ".vs.glsl";
		const std::string fragment_shader_name = effect_paths[i] + ".fs.glsl";
		std::string geometry_shader_name;
		if (effect_paths[i] == shader_path("basicEnemy")) {
			geometry_shader_name = effect_paths[i] + ".gs.glsl";
			
		}
		bool is_valid = loadEffectFromFile(vertex_shader_name, fragment_shader_name, geometry_shader_name, effects[i]);
		// bool is_valid = loadEffectFromFile(vertex_shader_name, fragment_shader_name, effects[i]);
		assert(is_valid && (GLuint)effects[i] != 0);
	}
}

// One could merge the following two functions as a template function...
template <class T>
void RenderSystem::bindVBOandIBO(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(uint)gid]);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	gl_has_errors();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffers[(uint)gid]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
	gl_has_errors();
}

void RenderSystem::initializeGlMeshes()
{
	for (uint i = 0; i < mesh_paths.size(); i++)
	{
		// Initialize meshes
		GEOMETRY_BUFFER_ID geom_index = mesh_paths[i].first;
		std::string name = mesh_paths[i].second;
		Mesh::loadFromOBJFile(name, 
			meshes[(int)geom_index].vertices,
			meshes[(int)geom_index].vertex_indices,
			meshes[(int)geom_index].original_size);

		bindVBOandIBO(geom_index,
			meshes[(int)geom_index].vertices, 
			meshes[(int)geom_index].vertex_indices);
	}
}

void RenderSystem::initializeGlGeometryBuffers()
{
	// Vertex Buffer creation.
	glGenBuffers((GLsizei)vertex_buffers.size(), vertex_buffers.data());
	// Index Buffer creation.
	glGenBuffers((GLsizei)index_buffers.size(), index_buffers.data());

	// Index and Vertex buffer data initialization.
	initializeGlMeshes();

	//////////////////////////
	// Initialize sprite
	// The position corresponds to the center of the texture.
	std::vector<TexturedVertex> textured_vertices(4);
	textured_vertices[0].position = { -1.f/2, +1.f/2, 0.f };
	textured_vertices[1].position = { +1.f/2, +1.f/2, 0.f };
	textured_vertices[2].position = { +1.f/2, -1.f/2, 0.f };
	textured_vertices[3].position = { -1.f/2, -1.f/2, 0.f };
	textured_vertices[0].texcoord = { 0.f, 1.f };
	textured_vertices[1].texcoord = { 1.f, 1.f };
	textured_vertices[2].texcoord = { 1.f, 0.f };
	textured_vertices[3].texcoord = { 0.f, 0.f };

	// Counterclockwise as it's the default opengl front winding direction.
	const std::vector<uint16_t> textured_indices = { 0, 3, 1, 1, 3, 2 };
	bindVBOandIBO(GEOMETRY_BUFFER_ID::SPRITE, textured_vertices, textured_indices);

	//------------------------------ SPRITE ROWS -------------------------------------

	// mage_idle sprite row
	std::vector<TexturedVertex> mage_idle_vertices(4);
	mage_idle_vertices[0].position = { -1.f, +1.f/2, 0.f };   
	mage_idle_vertices[1].position = { +1.f, +1.f/2, 0.f };   
	mage_idle_vertices[2].position = { +1.f, -1.f, 0.f };   
	mage_idle_vertices[3].position = { -1.f, -1.f, 0.f };   
	mage_idle_vertices[0].texcoord = { 0.f, 0.11111111111 };      // Bottom left
	mage_idle_vertices[1].texcoord = { 0.125, 0.11111111111 };    // Bottom right
	mage_idle_vertices[2].texcoord = { 0.125, 0.001 };            // Top right
	mage_idle_vertices[3].texcoord = { 0.f, 0.001 };              // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::MAGE_IDLE, mage_idle_vertices, textured_indices);

	// mage_casting sprite row
	std::vector<TexturedVertex> mage_casting_vertices(4);
	mage_casting_vertices[0].position = { -1.f, +1.f / 2, 0.f };
	mage_casting_vertices[1].position = { +1.f, +1.f / 2, 0.f };
	mage_casting_vertices[2].position = { +1.f, -1.f, 0.f };
	mage_casting_vertices[3].position = { -1.f, -1.f, 0.f };
	mage_casting_vertices[0].texcoord = { 0.f, 0.88888888888 };      // Bottom left
	mage_casting_vertices[1].texcoord = { 0.125, 0.88888888888 };    // Bottom right
	mage_casting_vertices[2].texcoord = { 0.125, 0.77777777777 };            // Top right
	mage_casting_vertices[3].texcoord = { 0.f, 0.77777777777 };              // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::MAGE_CASTING, mage_casting_vertices, textured_indices);

	// mage_death sprite row
	std::vector<TexturedVertex> mage_death_vertices(4);
	mage_death_vertices[0].position = { -1.f, +1.f / 2, 0.f };
	mage_death_vertices[1].position = { +1.f, +1.f / 2, 0.f };
	mage_death_vertices[2].position = { +1.f, -1.f, 0.f };
	mage_death_vertices[3].position = { -1.f, -1.f, 0.f };
	mage_death_vertices[0].texcoord = { 0.f, 0.99999999999 };      // Bottom left
	mage_death_vertices[1].texcoord = { 0.125, 0.99999999999 };    // Bottom right
	mage_death_vertices[2].texcoord = { 0.125, 0.88888888888 };            // Top right
	mage_death_vertices[3].texcoord = { 0.f, 0.88888888888 };              // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::MAGE_DEATH, mage_death_vertices, textured_indices);

	// swordsman_idle sprite row
	std::vector<TexturedVertex> swordsman_idle_vertices(4);
	swordsman_idle_vertices[0].position = { -1.f, +1.f, 0.f };   
	swordsman_idle_vertices[1].position = { +1.f, +1.f, 0.f };   
	swordsman_idle_vertices[2].position = { +1.f, -1.f, 0.f };   
	swordsman_idle_vertices[3].position = { -1.f, -1.f, 0.f };   
	swordsman_idle_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	swordsman_idle_vertices[1].texcoord = { 0.0625, 1.f };        // Bottom right
	swordsman_idle_vertices[2].texcoord = { 0.0625, 0.f };        // Top right
	swordsman_idle_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::SWORDSMAN_IDLE, swordsman_idle_vertices, textured_indices);

	// swordsman_walk sprite row
	std::vector<TexturedVertex> swordsman_walk_vertices(4);
	swordsman_walk_vertices[0].position = { -1.f, +1.f, 0.f };
	swordsman_walk_vertices[1].position = { +1.f, +1.f, 0.f };
	swordsman_walk_vertices[2].position = { +1.f, -1.f, 0.f };
	swordsman_walk_vertices[3].position = { -1.f, -1.f, 0.f };
	swordsman_walk_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	swordsman_walk_vertices[1].texcoord = { 0.125, 1.f };        // Bottom right
	swordsman_walk_vertices[2].texcoord = { 0.125, 0.f };        // Top right
	swordsman_walk_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::SWORDSMAN_WALK, swordsman_walk_vertices, textured_indices);

	// swordsman_melee sprite row
	std::vector<TexturedVertex> swordsman_melee_vertices(4);
	swordsman_melee_vertices[0].position = { -1.f, +1.f, 0.f };
	swordsman_melee_vertices[1].position = { +1.f, +1.f, 0.f };
	swordsman_melee_vertices[2].position = { +1.f, -1.f, 0.f };
	swordsman_melee_vertices[3].position = { -1.f, -1.f, 0.f };
	swordsman_melee_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	swordsman_melee_vertices[1].texcoord = { 0.03333333333, 1.f };        // Bottom right
	swordsman_melee_vertices[2].texcoord = { 0.03333333333, 0.f };        // Top right
	swordsman_melee_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::SWORDSMAN_MELEE, swordsman_melee_vertices, textured_indices);

	// swordsman_taunt sprite row
	std::vector<TexturedVertex> swordsman_taunt_vertices(4);
	swordsman_taunt_vertices[0].position = { -1.f, +1.f, 0.f };
	swordsman_taunt_vertices[1].position = { +1.f, +1.f, 0.f };
	swordsman_taunt_vertices[2].position = { +1.f, -1.f, 0.f };
	swordsman_taunt_vertices[3].position = { -1.f, -1.f, 0.f };
	swordsman_taunt_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	swordsman_taunt_vertices[1].texcoord = { 0.05555555555, 1.f };        // Bottom right
	swordsman_taunt_vertices[2].texcoord = { 0.05555555555, 0.f };        // Top right
	swordsman_taunt_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::SWORDSMAN_TAUNT, swordsman_idle_vertices, textured_indices);

	// swordsman_death sprite row
	std::vector<TexturedVertex> swordsman_death_vertices(4);
	swordsman_death_vertices[0].position = { -1.f, +1.f, 0.f };
	swordsman_death_vertices[1].position = { +1.f, +1.f, 0.f };
	swordsman_death_vertices[2].position = { +1.f, -1.f, 0.f };
	swordsman_death_vertices[3].position = { -1.f, -1.f, 0.f };
	swordsman_death_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	swordsman_death_vertices[1].texcoord = { 0.025, 1.f };        // Bottom right
	swordsman_death_vertices[2].texcoord = { 0.025, 0.f };        // Top right
	swordsman_death_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::SWORDSMAN_DEATH, swordsman_death_vertices, textured_indices);

	// necro_one_idle sprite row
	std::vector<TexturedVertex> necro_one_idle_vertices(4);
	necro_one_idle_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_one_idle_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_one_idle_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_one_idle_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_one_idle_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_one_idle_vertices[1].texcoord = { 0.25, 1.f };        // Bottom right
	necro_one_idle_vertices[2].texcoord = { 0.25, 0.f };        // Top right
	necro_one_idle_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_ONE_IDLE, necro_one_idle_vertices, textured_indices);

	// necro_one_casting sprite row
	std::vector<TexturedVertex> necro_one_casting_vertices(4);
	necro_one_casting_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_one_casting_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_one_casting_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_one_casting_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_one_casting_vertices[0].texcoord = { 0.f, 0.6666666666 };           // Bottom left
	necro_one_casting_vertices[1].texcoord = { 0.16666666666, 0.6666666666 };        // Bottom right
	necro_one_casting_vertices[2].texcoord = { 0.16666666666, 0.3333333333 };        // Top right
	necro_one_casting_vertices[3].texcoord = { 0.f, 0.3333333333 };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_ONE_CASTING, necro_one_casting_vertices, textured_indices);

	// necro_one_summoning sprite row
	std::vector<TexturedVertex> necro_one_summoning_vertices(4);
	necro_one_summoning_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_one_summoning_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_one_summoning_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_one_summoning_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_one_summoning_vertices[0].texcoord = { 0.f, 0.5 };           // Bottom left
	necro_one_summoning_vertices[1].texcoord = { 0.25, 0.5 };        // Bottom right
	necro_one_summoning_vertices[2].texcoord = { 0.25, 0.f };        // Top right
	necro_one_summoning_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_ONE_SUMMONING, necro_one_summoning_vertices, textured_indices);

	// necro_one_death_one sprite row
	std::vector<TexturedVertex> necro_one_death_one_vertices(4);
	necro_one_death_one_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_one_death_one_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_one_death_one_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_one_death_one_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_one_death_one_vertices[0].texcoord = { 0.f, 0.5 };           // Bottom left
	necro_one_death_one_vertices[1].texcoord = { 0.10, 0.5 };        // Bottom right
	necro_one_death_one_vertices[2].texcoord = { 0.10, 0.f };        // Top right
	necro_one_death_one_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_ONE, necro_one_death_one_vertices, textured_indices);

	// necro_one_death_two sprite row
	std::vector<TexturedVertex> necro_one_death_two_vertices(4);
	necro_one_death_two_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_one_death_two_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_one_death_two_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_one_death_two_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_one_death_two_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_one_death_two_vertices[1].texcoord = { 0.10, 1.f };        // Bottom right
	necro_one_death_two_vertices[2].texcoord = { 0.10, 0.5 };        // Top right
	necro_one_death_two_vertices[3].texcoord = { 0.f, 0.5 };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_ONE_DEATH_TWO, necro_one_death_two_vertices, textured_indices);

	// necro_two_appear sprite row
	std::vector<TexturedVertex> necro_two_appear_vertices(4);
	necro_two_appear_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_two_appear_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_two_appear_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_two_appear_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_two_appear_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_two_appear_vertices[1].texcoord = { 0.16666666666, 1.f };        // Bottom right
	necro_two_appear_vertices[2].texcoord = { 0.16666666666, 0.f };        // Top right
	necro_two_appear_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_TWO_APPEAR, necro_two_appear_vertices, textured_indices);

	// necro_two_idle sprite row
	std::vector<TexturedVertex> necro_two_idle_vertices(4);
	necro_two_idle_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_two_idle_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_two_idle_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_two_idle_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_two_idle_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_two_idle_vertices[1].texcoord = { 0.125, 1.f };        // Bottom right
	necro_two_idle_vertices[2].texcoord = { 0.125, 0.f };        // Top right
	necro_two_idle_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_TWO_IDLE, necro_two_idle_vertices, textured_indices);

	// necro_two_melee sprite row
	std::vector<TexturedVertex> necro_two_melee_vertices(4);
	necro_two_melee_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_two_melee_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_two_melee_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_two_melee_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_two_melee_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_two_melee_vertices[1].texcoord = { 0.10, 1.f };        // Bottom right
	necro_two_melee_vertices[2].texcoord = { 0.10, 0.f };        // Top right
	necro_two_melee_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_TWO_MELEE, necro_two_melee_vertices, textured_indices);

	// necro_two_casting sprite row
	std::vector<TexturedVertex> necro_two_casting_vertices(4);
	necro_two_casting_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_two_casting_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_two_casting_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_two_casting_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_two_casting_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_two_casting_vertices[1].texcoord = { 0.125, 1.f };        // Bottom right
	necro_two_casting_vertices[2].texcoord = { 0.125, 0.f };        // Top right
	necro_two_casting_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_TWO_CASTING, necro_two_casting_vertices, textured_indices);

	// necro_two_death sprite row
	std::vector<TexturedVertex> necro_two_death_vertices(4);
	necro_two_death_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_two_death_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_two_death_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_two_death_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_two_death_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_two_death_vertices[1].texcoord = { 0.14285714285, 1.f };        // Bottom right
	necro_two_death_vertices[2].texcoord = { 0.14285714285, 0.f };        // Top right
	necro_two_death_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_TWO_DEATH, necro_two_death_vertices, textured_indices);

	// necro_minion_appear sprite row
	std::vector<TexturedVertex> necro_minion_appear_ertices(4);
	necro_minion_appear_ertices[0].position = { -1.f, +1.f, 0.f };
	necro_minion_appear_ertices[1].position = { +1.f, +1.f, 0.f };
	necro_minion_appear_ertices[2].position = { +1.f, -1.f, 0.f };
	necro_minion_appear_ertices[3].position = { -1.f, -1.f, 0.f };
	necro_minion_appear_ertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_minion_appear_ertices[1].texcoord = { 0.10, 1.f };        // Bottom right
	necro_minion_appear_ertices[2].texcoord = { 0.10, 0.f };        // Top right
	necro_minion_appear_ertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_MINION_APPEAR, necro_minion_appear_ertices, textured_indices);

	// necro_minion_idle sprite row
	std::vector<TexturedVertex> necro_minion_idle_ertices(4);
	necro_minion_idle_ertices[0].position = { -1.f, +1.f, 0.f };
	necro_minion_idle_ertices[1].position = { +1.f, +1.f, 0.f };
	necro_minion_idle_ertices[2].position = { +1.f, -1.f, 0.f };
	necro_minion_idle_ertices[3].position = { -1.f, -1.f, 0.f };
	necro_minion_idle_ertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_minion_idle_ertices[1].texcoord = { 0.20, 1.f };        // Bottom right
	necro_minion_idle_ertices[2].texcoord = { 0.20, 0.f };        // Top right
	necro_minion_idle_ertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_MINION_IDLE, necro_minion_idle_ertices, textured_indices);

	// necro_minion_walk sprite row
	std::vector<TexturedVertex> necro_minion_walk_ertices(4);
	necro_minion_walk_ertices[0].position = { -1.f, +1.f, 0.f };
	necro_minion_walk_ertices[1].position = { +1.f, +1.f, 0.f };
	necro_minion_walk_ertices[2].position = { +1.f, -1.f, 0.f };
	necro_minion_walk_ertices[3].position = { -1.f, -1.f, 0.f };
	necro_minion_walk_ertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_minion_walk_ertices[1].texcoord = { 0.125, 1.f };        // Bottom right
	necro_minion_walk_ertices[2].texcoord = { 0.125, 0.f };        // Top right
	necro_minion_walk_ertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_MINION_WALK, necro_minion_walk_ertices, textured_indices);

	// necro_minion_melee sprite row
	std::vector<TexturedVertex> necro_minion_melee_vertices(4);
	necro_minion_melee_vertices[0].position = { -1.f, +1.f, 0.f };
	necro_minion_melee_vertices[1].position = { +1.f, +1.f, 0.f };
	necro_minion_melee_vertices[2].position = { +1.f, -1.f, 0.f };
	necro_minion_melee_vertices[3].position = { -1.f, -1.f, 0.f };
	necro_minion_melee_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_minion_melee_vertices[1].texcoord = { 0.10, 1.f };        // Bottom right
	necro_minion_melee_vertices[2].texcoord = { 0.10, 0.f };        // Top right
	necro_minion_melee_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_MINION_MELEE, necro_minion_melee_vertices, textured_indices);

	// necro_minion_death sprite row
	std::vector<TexturedVertex> necro_minion_death_ertices(4);
	necro_minion_death_ertices[0].position = { -1.f, +1.f, 0.f };
	necro_minion_death_ertices[1].position = { +1.f, +1.f, 0.f };
	necro_minion_death_ertices[2].position = { +1.f, -1.f, 0.f };
	necro_minion_death_ertices[3].position = { -1.f, -1.f, 0.f };
	necro_minion_death_ertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	necro_minion_death_ertices[1].texcoord = { 0.10, 1.f };        // Bottom right
	necro_minion_death_ertices[2].texcoord = { 0.10, 0.f };        // Top right
	necro_minion_death_ertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::NECRO_MINION_DEATH, necro_minion_death_ertices, textured_indices);

	// archer_idle sprite row
	std::vector<TexturedVertex> archer_idle_vertices(4);
	archer_idle_vertices[0].position = { -1.f, +1.f, 0.f };
	archer_idle_vertices[1].position = { +1.f, +1.f, 0.f };
	archer_idle_vertices[2].position = { +1.f, -1.f, 0.f };
	archer_idle_vertices[3].position = { -1.f, -1.f, 0.f };
	archer_idle_vertices[0].texcoord = { 0.f, 0.750 };           // Bottom left
	archer_idle_vertices[1].texcoord = { 0.125, 0.750 };        // Bottom right
	archer_idle_vertices[2].texcoord = { 0.125, 0.625 };        // Top right
	archer_idle_vertices[3].texcoord = { 0.f, 0.625 };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::ARCHER_IDLE, archer_idle_vertices, textured_indices);

	// archer_walking sprite row
	std::vector<TexturedVertex> archer_walking_vertices(4);
	archer_walking_vertices[0].position = { -1.f, +1.f, 0.f };
	archer_walking_vertices[1].position = { +1.f, +1.f, 0.f };
	archer_walking_vertices[2].position = { +1.f, -1.f, 0.f };
	archer_walking_vertices[3].position = { -1.f, -1.f, 0.f };
	archer_walking_vertices[0].texcoord = { 0.f, 0.125 };           // Bottom left
	archer_walking_vertices[1].texcoord = { 0.125, 0.125 };        // Bottom right
	archer_walking_vertices[2].texcoord = { 0.125, 0.f };        // Top right
	archer_walking_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::ARCHER_WALKING, archer_walking_vertices, textured_indices);

	// archer_jumping sprite row
	std::vector<TexturedVertex> archer_jumping_vertices(4);
	archer_jumping_vertices[0].position = { -1.f, +1.f, 0.f };
	archer_jumping_vertices[1].position = { +1.f, +1.f, 0.f };
	archer_jumping_vertices[2].position = { +1.f, -1.f, 0.f };
	archer_jumping_vertices[3].position = { -1.f, -1.f, 0.f };
	archer_jumping_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	archer_jumping_vertices[1].texcoord = { 0.125, 1.f };        // Bottom right
	archer_jumping_vertices[2].texcoord = { 0.125, 0.875 };        // Top right
	archer_jumping_vertices[3].texcoord = { 0.f, 0.875 };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::ARCHER_JUMPING, archer_jumping_vertices, textured_indices);

	// archer_attacking sprite row
	std::vector<TexturedVertex> archer_attacking_vertices(4);
	archer_attacking_vertices[0].position = { -1.f, +1.f, 0.f };
	archer_attacking_vertices[1].position = { +1.f, +1.f, 0.f };
	archer_attacking_vertices[2].position = { +1.f, -1.f, 0.f };
	archer_attacking_vertices[3].position = { -1.f, -1.f, 0.f };
	archer_attacking_vertices[0].texcoord = { 0.f, 0.500 };           // Bottom left
	archer_attacking_vertices[1].texcoord = { 0.125, 0.500 };        // Bottom right
	archer_attacking_vertices[2].texcoord = { 0.125, 0.375 };        // Top right
	archer_attacking_vertices[3].texcoord = { 0.f, 0.375 };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::ARCHER_ATTACKING, archer_attacking_vertices, textured_indices);

	// archer_walk_attacking sprite row
	std::vector<TexturedVertex> archer_walk_attacking_vertices(4);
	archer_walk_attacking_vertices[0].position = { -1.f, +1.f, 0.f };
	archer_walk_attacking_vertices[1].position = { +1.f, +1.f, 0.f };
	archer_walk_attacking_vertices[2].position = { +1.f, -1.f, 0.f };
	archer_walk_attacking_vertices[3].position = { -1.f, -1.f, 0.f };
	archer_walk_attacking_vertices[0].texcoord = { 0.f, 0.375 };           // Bottom left
	archer_walk_attacking_vertices[1].texcoord = { 0.125, 0.375 };        // Bottom right
	archer_walk_attacking_vertices[2].texcoord = { 0.125, 0.250 };        // Top right
	archer_walk_attacking_vertices[3].texcoord = { 0.f, 0.250 };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::ARCHER_WALK_ATTACKING, archer_walk_attacking_vertices, textured_indices);

	// archer_dead sprite row
 	std::vector<TexturedVertex> archer_dead_vertices(4);
 	archer_dead_vertices[0].position = { -1.f, +1.f, 0.f };
 	archer_dead_vertices[1].position = { +1.f, +1.f, 0.f };
 	archer_dead_vertices[2].position = { +1.f, -1.f, 0.f };
 	archer_dead_vertices[3].position = { -1.f, -1.f, 0.f };
 	archer_dead_vertices[0].texcoord = { 0.f, 0.250 };           // Bottom left
 	archer_dead_vertices[1].texcoord = { 0.125, 0.250 };        // Bottom right
 	archer_dead_vertices[2].texcoord = { 0.125, 0.125 };        // Top right
 	archer_dead_vertices[3].texcoord = { 0.f, 0.125 };           // Top left
 	bindVBOandIBO(GEOMETRY_BUFFER_ID::ARCHER_DEAD, archer_dead_vertices, textured_indices);

	// dragon_flying sprite row
	std::vector<TexturedVertex> dragon_flying_vertices(4);
	dragon_flying_vertices[0].position = { -1.f, +1.f, 0.f };
	dragon_flying_vertices[1].position = { +1.f, +1.f, 0.f };
	dragon_flying_vertices[2].position = { +1.f, -1.f, 0.f };
	dragon_flying_vertices[3].position = { -1.f, -1.f, 0.f };
	dragon_flying_vertices[0].texcoord = { 0.f, 1.f };           // Bottom left
	dragon_flying_vertices[1].texcoord = { 0.11111111111, 1.f };        // Bottom right
	dragon_flying_vertices[2].texcoord = { 0.11111111111, 0.f };        // Top right
	dragon_flying_vertices[3].texcoord = { 0.f, 0.f };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::DRAGON_FLYING, dragon_flying_vertices, textured_indices);

	// treasure chest closed
	std::vector<TexturedVertex> treasure_chest_closed_vertices(4);
	treasure_chest_closed_vertices[0].position = { -1.f, +1.f, 0.f };
	treasure_chest_closed_vertices[1].position = { +1.f, +1.f, 0.f };
	treasure_chest_closed_vertices[2].position = { +1.f, -1.f, 0.f };
	treasure_chest_closed_vertices[3].position = { -1.f, -1.f, 0.f };
	treasure_chest_closed_vertices[0].texcoord = { 0.60, 0.83333333333 };           // Bottom left
	treasure_chest_closed_vertices[1].texcoord = { 0.80, 0.83333333333 };        // Bottom right
	treasure_chest_closed_vertices[2].texcoord = { 0.80, 0.66666666666 };        // Top right
	treasure_chest_closed_vertices[3].texcoord = { 0.60, 0.66666666666 };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::TREASURE_CHEST_CLOSED, treasure_chest_closed_vertices, textured_indices);

	// treasure chest open
	std::vector<TexturedVertex> treasure_chest_open_vertices(4);
	treasure_chest_open_vertices[0].position = { -1.f, +1.f, 0.f };
	treasure_chest_open_vertices[1].position = { +1.f, +1.f, 0.f };
	treasure_chest_open_vertices[2].position = { +1.f, -1.f, 0.f };
	treasure_chest_open_vertices[3].position = { -1.f, -1.f, 0.f };
	treasure_chest_open_vertices[0].texcoord = { 0.60, 1.f };           // Bottom left
	treasure_chest_open_vertices[1].texcoord = { 0.80, 1.f };        // Bottom right
	treasure_chest_open_vertices[2].texcoord = { 0.80, 0.83333333333 };        // Top right
	treasure_chest_open_vertices[3].texcoord = { 0.60, 0.83333333333 };           // Top left
	bindVBOandIBO(GEOMETRY_BUFFER_ID::TREASURE_CHEST_OPEN, treasure_chest_open_vertices, textured_indices);

	////////////////////////
	// Initialize pebble
	std::vector<ColoredVertex> pebble_vertices;
	std::vector<uint16_t> pebble_indices;
	constexpr float z = -0.1f;
	constexpr int NUM_TRIANGLES = 62;

	for (int i = 0; i < NUM_TRIANGLES; i++) {
		const float t = float(i) * M_PI * 2.f / float(NUM_TRIANGLES - 1);
		pebble_vertices.push_back({});
		pebble_vertices.back().position = { 0.5 * cos(t), 0.5 * sin(t), z };
		pebble_vertices.back().color = { 0.8, 0.8, 0.8 };
	}
	pebble_vertices.push_back({});
	pebble_vertices.back().position = { 0, 0, 0 };
	pebble_vertices.back().color = { 0.8, 0.8, 0.8 };
	for (int i = 0; i < NUM_TRIANGLES; i++) {
		pebble_indices.push_back((uint16_t)i);
		pebble_indices.push_back((uint16_t)((i + 1) % NUM_TRIANGLES));
		pebble_indices.push_back((uint16_t)NUM_TRIANGLES);
	}
	int geom_index = (int)GEOMETRY_BUFFER_ID::PEBBLE;
	meshes[geom_index].vertices = pebble_vertices;
	meshes[geom_index].vertex_indices = pebble_indices;
	bindVBOandIBO(GEOMETRY_BUFFER_ID::PEBBLE, meshes[geom_index].vertices, meshes[geom_index].vertex_indices);

	//////////////////////////////////
	// Initialize debug line
	std::vector<ColoredVertex> line_vertices;
	std::vector<uint16_t> line_indices;

	constexpr float depth = 0.5f;
	constexpr vec3 red = { 0.8,0.1,0.1 };

	// Corner points
	line_vertices = {
		{{-0.5,-0.5, depth}, red},
		{{-0.5, 0.5, depth}, red},
		{{ 0.5, 0.5, depth}, red},
		{{ 0.5,-0.5, depth}, red},
	};

	// Two triangles
	line_indices = {0, 1, 3, 1, 2, 3};
	
	geom_index = (int)GEOMETRY_BUFFER_ID::DEBUG_LINE;
	meshes[geom_index].vertices = line_vertices;
	meshes[geom_index].vertex_indices = line_indices;
	bindVBOandIBO(GEOMETRY_BUFFER_ID::DEBUG_LINE, line_vertices, line_indices);

	///////////////////////////////////////////////////////
	// Initialize screen triangle (yes, triangle, not quad; its more efficient).
	std::vector<vec3> screen_vertices(3);
	screen_vertices[0] = { -1, -6, 0.f };
	screen_vertices[1] = { 6, -1, 0.f };
	screen_vertices[2] = { -1, 6, 0.f };

	// Counterclockwise as it's the default opengl front winding direction.
	const std::vector<uint16_t> screen_indices = { 0, 1, 2 };
	bindVBOandIBO(GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE, screen_vertices, screen_indices);
}

RenderSystem::~RenderSystem()
{
	// Don't need to free gl resources since they last for as long as the program,
	// but it's polite to clean after yourself.
	glDeleteBuffers((GLsizei)vertex_buffers.size(), vertex_buffers.data());
	glDeleteBuffers((GLsizei)index_buffers.size(), index_buffers.data());
	glDeleteTextures((GLsizei)texture_gl_handles.size(), texture_gl_handles.data());
	glDeleteTextures(1, &off_screen_render_buffer_color);
	glDeleteRenderbuffers(1, &off_screen_render_buffer_depth);
	gl_has_errors();

	for(uint i = 0; i < effect_count; i++) {
		glDeleteProgram(effects[i]);
	}
	// delete allocated resources
	glDeleteFramebuffers(1, &frame_buffer);
	gl_has_errors();

	// remove all entities created by the render system
	while (registry.renderRequests.entities.size() > 0)
	    registry.remove_all_components_of(registry.renderRequests.entities.back());
}

// Initialize the screen texture from a standard sprite
bool RenderSystem::initScreenTexture()
{
	registry.screenStates.emplace(screen_state_entity);

	int width, height;
	glfwGetFramebufferSize(const_cast<GLFWwindow*>(window), &width, &height);

	glGenTextures(1, &off_screen_render_buffer_color);
	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl_has_errors();

	glGenRenderbuffers(1, &off_screen_render_buffer_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, off_screen_render_buffer_depth);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, off_screen_render_buffer_color, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, off_screen_render_buffer_depth);
	gl_has_errors();

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	return true;
}

bool gl_compile_shader(GLuint shader)
{
	glCompileShader(shader);
	gl_has_errors();
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint log_len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
		std::vector<char> log(log_len);
		glGetShaderInfoLog(shader, log_len, &log_len, log.data());
		glDeleteShader(shader);

		gl_has_errors();

		fprintf(stderr, "GLSL: %s", log.data());
		return false;
	}

	return true;
}

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, std::string& gs_path, GLuint& out_program)
{
	// Opening files
	std::ifstream vs_is(vs_path);
	std::ifstream fs_is(fs_path);
	std::ifstream gs_is(gs_path);
	if (!gs_path.empty() && !gs_is.good()) {
		fprintf(stderr, "Failed to load geometry shader file %s\n", gs_path.c_str());
		assert(false);
		return false;
	}
	if (!vs_is.good() || !fs_is.good())
	{
		fprintf(stderr, "Failed to load shader files %s, %s", vs_path.c_str(), fs_path.c_str());
		assert(false);
		return false;
	}

	// Reading sources
	std::stringstream vs_ss, fs_ss, gs_ss;

	bool link_gs = false;
	GLuint geometry;
	if (!gs_path.empty()) {
		gs_ss << gs_is.rdbuf();
		std::string gs_str = gs_ss.str();
		const char* gs_src = gs_str.c_str();
		GLsizei gs_len = (GLsizei)gs_str.size();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gs_src, &gs_len);
		gl_has_errors();

		if (!gl_compile_shader(geometry))
		{
			fprintf(stderr, "geometry compilation failed");
			assert(false);
			return false;
		}
		link_gs = true;
	}

	vs_ss << vs_is.rdbuf();
	fs_ss << fs_is.rdbuf();
	std::string vs_str = vs_ss.str();
	std::string fs_str = fs_ss.str();
	const char* vs_src = vs_str.c_str();
	const char* fs_src = fs_str.c_str();
	GLsizei vs_len = (GLsizei)vs_str.size();
	GLsizei fs_len = (GLsizei)fs_str.size();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vs_src, &vs_len);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fs_src, &fs_len);
	gl_has_errors();

	// Compiling
	if (!gl_compile_shader(vertex))
	{
		fprintf(stderr, "Vertex compilation failed");
		assert(false);
		return false;
	}
	if (!gl_compile_shader(fragment))
	{
		fprintf(stderr, "fragment compilation failed");
		assert(false);
		return false;
	}

	// Linking
	out_program = glCreateProgram();
	glAttachShader(out_program, vertex);
	glAttachShader(out_program, fragment);
	if (link_gs) {
		glAttachShader(out_program, geometry);
	}
	glLinkProgram(out_program);
	gl_has_errors();


	{
		GLint is_linked = GL_FALSE;
		glGetProgramiv(out_program, GL_LINK_STATUS, &is_linked);
		if (is_linked == GL_FALSE)
		{
			GLint log_len;
			glGetProgramiv(out_program, GL_INFO_LOG_LENGTH, &log_len);
			std::vector<char> log(log_len);
			glGetProgramInfoLog(out_program, log_len, &log_len, log.data());
			gl_has_errors();

			fprintf(stderr, "Link error: %s", log.data());
			assert(false);
			return false;
		}
	}

	// No need to carry this around. Keeping these objects is only useful if we recycle
	// the same shaders over and over, which we don't, so no need and this is simpler.
	glDetachShader(out_program, vertex);
	glDetachShader(out_program, fragment);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (link_gs) {
		glDetachShader(out_program, geometry);
		glDeleteShader(geometry);
	}
	gl_has_errors();

	return true;
}

void RenderSystem::createRandomLightBallPosForBackground(int windowWidth, int windowHeight) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	for (int i = 0; i < NUM_LIGHT_BALLS_BACKGROUND; i++) {
		//RenderSystem::lightBallsXcoords.push_back(RandomFloat(-(float)windowWidth/ (float)windowHeight,
		// 													   (float)windowWidth / (float)windowHeight));
		RenderSystem::lightBallsXcoords.push_back(RandomFloat(-(float)w / (float)h, (float)w / (float)h));
		RenderSystem::lightBallsYcoords.push_back(RandomFloat(0.8, 1.));
	}

	/*for (int i = 0; i < lightBallsXcoords.size(); i++) {
		printf("%f, %f\n", lightBallsXcoords[i], lightBallsYcoords[i]);
	}*/
}

//void RenderSystem::initParticlesBuffer() {
//	GLuint particles_position_buffer;
//	glGenBuffers(1, &particles_position_buffer);
//	RenderSystem::particles_position_buffer = particles_position_buffer;
//	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
//	glBufferData(GL_ARRAY_BUFFER, 4000 * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
//}