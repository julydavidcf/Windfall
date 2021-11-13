#pragma once

#include <array>
#include <utility>

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"
#include <map>

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
			textures_path("silenceIcon.png"),
			textures_path("silenceIconSelected.png"),
			textures_path("silencebubble.png"),
			textures_path("fireballIconDisable.png"),
			textures_path("healthbar.png"),
			textures_path("particle.png"),
			textures_path("playerTurn.png"),
			textures_path("enemyTurn.png"),
			textures_path("arrow.png"),
			textures_path("rock.png"),
			textures_path("greenCross.png"),
			textures_path("iceShard.png"),
			textures_path("iceShardIcon.png"),
			textures_path("iceShardIconSelected.png"),
			textures_path("iceShardIconDisable.png"),
			textures_path("rockIcon.png"),
			textures_path("rockIconSelected.png"),
			textures_path("rockIconDisable.png"),
			textures_path("healIcon.png"),
			textures_path("healIconSelected.png"),
			textures_path("healIconDisable.png"),
			textures_path("meleeIcon.png"),
			textures_path("meleeIconSelected.png"),
			textures_path("meleeIconDisable.png"),
			textures_path("taunt.png"),
			textures_path("tauntIcon.png"),
			textures_path("tauntIconSelected.png"),
			textures_path("tauntIconDisable.png"),


			textures_path("mage_anim.png"),
			textures_path("swordsman_idle.png"),
			textures_path("swordsman_walk.png"),
			textures_path("swordsman_melee.png"),
			textures_path("swordsman_taunt.png"),
			textures_path("swordsman_death.png"),
			textures_path("necromancer_idle.png"),
			textures_path("backgroundLayerOne.png"),
			textures_path("backgroundLayerTwo.png"),
			textures_path("backgroundLayerThree.png"),
			textures_path("backgroundLayerFour.png"),

			//tootips
			textures_path("fireBallToolTip.png"),
			textures_path("iceShardToolTip.png"),
			textures_path("rockToolTip.png"),
			textures_path("meleeToolTip.png"),
			textures_path("tauntToolTip.png"),
			textures_path("healToolTip.png"),

			textures_path("particlered.png")
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


	// Time per frame in ms, for each action
	float MAGE_IDLE_FRAME_TIME = 175;
	float MAGE_ATTACK_FRAME_TIME = 150;
	float MAGE_DEATH_FRAME_TIME = 280;

	float SWORDSMAN_IDLE_FRAME_TIME = 150;
	float SWORDSMAN_MELEE_FRAME_TIME = 50;
	float SWORDSMAN_WALK_FRAME_TIME = 100;
	float SWORDSMAN_TAUNT_FRAME_TIME = 90;
	float SWORDSMAN_DEATH_FRAME_TIME = 80;

	// pixel positions for the light balls in the background
	std::vector<float> lightBallsXcoords;
	std::vector<float> lightBallsYcoords;

	// Time per frame in ms
	float TIME_PER_FRAME = 100;
	
	// ---------------------------------- Frame stats for each character animation -----------------------------------

	// Mage frame stats
	const int MAGE_IDLE_FRAMES = 8;
	const GLfloat MAGE_IDLE_FRAME_WIDTH = 0.125;

	const int MAGE_CASTING_FRAMES = 4;
	const GLfloat MAGE_CASTING_FRAME_WIDTH = 0.250;

	const int MAGE_DEATH_FRAMES = 8;
	const GLfloat MAGE_DEATH_FRAME_WIDTH = 0.125;

	// Swordsman frame stats
	const int SWORDSMAN_IDLE_FRAMES = 16;
	const GLfloat SWORDSMAN_IDLE_FRAME_WIDTH = 0.0625;

	const int SWORDSMAN_WALK_FRAMES = 8;
	const GLfloat SWORDSMAN_WALK_FRAME_WIDTH = 0.125;

	const int SWORDSMAN_MELEE_FRAMES = 30;
	const GLfloat SWORDSMAN_MELEE_FRAME_WIDTH = 0.03333333333;

	const int SWORDSMAN_TAUNT_FRAMES = 18;
	const GLfloat SWORDSMAN_TAUNT_FRAME_WIDTH = 0.05555555555;

	const int SWORDSMAN_DEATH_FRAMES = 40;
	const GLfloat SWORDSMAN_DEATH_FRAME_WIDTH = 0.025;

	// Necromancer frame stats
	const int NECROMANCER_IDLE_FRAMES = 4;
	const GLfloat NECROMANCER_IDLE_FRAME_WIDTH = 0.25;

	// Camera/scrolling constants
	float CAMERA_OFFSET_LEFT = 500;
	float CAMERA_OFFSET_TOP = 500;
	float CAMERA_OFFSET_RIGHT = 400;
	float CAMERA_OFFSET_BOTTOM = 200;

	float AUTOSCROLL_RATE = 0.25;
	float CAMERA_SCROLL_RATE_ONE = 0.50;
	float CAMERA_SCROLL_RATE_TWO = 3.0;

public:

	const float DEFAULT_GAME_LEVEL_TRANSITION_PERIOD_MS = 4500.f;
	bool transitioningToNextLevel = false;
	float nextLevelTranistionPeriod_ms = DEFAULT_GAME_LEVEL_TRANSITION_PERIOD_MS;
	float dimScreenFactor = 0.4f;
	float fogFactor = 0.2;
	std::map<int, int> deferredRenderingEntities = {};
	int gameLevel = 1;

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
	void initParticlesBuffer();
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

	GLuint particles_position_buffer;
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, GLuint& out_program);