#pragma once

#include <array>
#include <utility>

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"

// System responsible for setting up OpenGL and for rendering all the
// visual entities in the game
class RenderSystem {
	/**
	 * The following arrays store the assets the game will use. They are loaded
	 * at initialization and are assumed to not be modified by the render loop.
	 *
	 * Whenever possible, add to these lists instead of creating dynamic state
	 * it is easier to debug and faster to execute for the computer.
	 */
	std::array<GLuint, texture_count> texture_gl_handles;
	std::array<ivec2, texture_count> texture_dimensions;

	// Make sure these paths remain in sync with the associated enumerators.
	// Associated id with .obj path
	const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
	{
		  //std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::BASICENEMY, mesh_path("basicEnemy.obj"))
		  // specify meshes of other assets here
	};

	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, texture_count> texture_paths = {
			textures_path("MagicalBarrier.png"),
			textures_path("fireball.png"),
			textures_path("fireballIcon.png"),
			textures_path("fireballIconSelected.png"),
			textures_path("healthbar.png"),
			textures_path("particle.png"),
			textures_path("playerTurn.png"),
			textures_path("enemyTurn.png"),
			textures_path("mage_anim.png"),
			textures_path("swordsman_idle.png"),
			textures_path("necromancer_idle.png"),
			textures_path("backgroundLayerOne.png"),
			textures_path("backgroundLayerTwo.png"),
			textures_path("backgroundLayerThree.png"),
			textures_path("backgroundLayerFour.png")
  };
  
	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("pebble"),
		shader_path("textured"),
		shader_path("water"),
		shader_path("particle") };

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;

	// pixel positions for the light balls in the background
	std::vector<float> lightBallsXcoords;
	std::vector<float> lightBallsYcoords;

	// Time per frame in ms
	float TIME_PER_FRAME = 100;
	
	// ---------------------------------- Frame stats for each character animation -----------------------------------

	// Mage frame stats
	const int MAGE_IDLE_FRAMES = 8;
	const GLfloat MAGE_IDLE_FRAME_WIDTH = 0.125;

	// Swordsman frame stats
	const int SWORDSMAN_IDLE_FRAMES = 16;
	const GLfloat SWORDSMAN_IDLE_FRAME_WIDTH = 0.0625;

	// Necromancer frame stats
	const int NECROMANCER_IDLE_FRAMES = 4;
	const GLfloat NECROMANCER_IDLE_FRAME_WIDTH = 0.25;

	// Adjust projectile camera width/height here
	float CAMERA_OFFSET_LEFT = 500;
	float CAMERA_OFFSET_TOP = 500;
	float CAMERA_OFFSET_RIGHT = 400;
	float CAMERA_OFFSET_BOTTOM = 200;
public:

	// Initialize the window
	bool init(int width, int height, GLFWwindow* window);

	template <class T>
	void bindVBOandIBO(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices);

	void initializeGlTextures();

	void initializeGlEffects();

	void initializeGlMeshes();
	Mesh& getMesh(GEOMETRY_BUFFER_ID id) { return meshes[(int)id]; };

	void initializeGlGeometryBuffers();
	// Initialize the screen texture used as intermediate render target
	// The draw loop first renders to this texture, then it is used for the water
	// shader
	bool initScreenTexture();

	// Destroy resources associated to one or all entities created by the system
	~RenderSystem();

	// Draw all entities
	void draw(float elapsed_ms);

	mat3 createProjectionMatrix();

	mat3 createCameraProjection(Motion& motion);

	void createRandomLightBallPosForBackground(int windowWidth, int windowHeight);

private:
	// Internal drawing functions for each entity type
	void drawTexturedMesh(Entity entity, const mat3& projection, GLint& frame, GLfloat& frameWidth);
	void drawDeathParticles(Entity entity, const mat3& projection);
	void drawToScreen();

	// Window handle
	GLFWwindow* window;
	float screen_scale;  // Screen to pixel coordinates scale factor (for apple
						 // retina display?)

	// Screen texture handles
	GLuint frame_buffer;
	GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

	Entity screen_state_entity;
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, GLuint& out_program);