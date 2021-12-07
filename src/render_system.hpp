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
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::BACKGROUND_OBJ, mesh_path("basicEnemy.obj")),
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::SHIELD_MESH, mesh_path("necroBarrier.obj")),
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::ROCK_MESH, mesh_path("rock.obj")),
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::ARROW_MESH, mesh_path("arrow.obj")),
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::SIMPLIFIED_ROCK_MESH, mesh_path("rock_simplified.obj")),
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::SIMPLIFIED_ARROW_MESH, mesh_path("arrow_simplified.obj")),
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
			textures_path("arrowIcon.png"),
			textures_path("arrowIconSelected.png"),
			textures_path("rock.png"),
			textures_path("lightning.png"),
			textures_path("greenCross.png"),
			textures_path("meteor.png"),
			textures_path("charArrow.png"),
			textures_path("dot.png"),
			textures_path("particleBeamCharge.png"),
			textures_path("bleed.png"),
			textures_path("spike.png"),


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

			// Animation sheets
			textures_path("mage_anim.png"),
			textures_path("swordsman_idle.png"),
			textures_path("swordsman_walk.png"),
			textures_path("swordsman_melee.png"),
			textures_path("swordsman_taunt.png"),
			textures_path("swordsman_death.png"),
			textures_path("necro_one_idle.png"),
			textures_path("necro_one_casting.png"),
			textures_path("necro_one_summoning.png"),
			textures_path("necro_one_death_one.png"),
			textures_path("necro_one_death_two.png"),
			textures_path("necro_two_appear.png"),
			textures_path("necro_two_idle.png"),
			textures_path("necro_two_melee.png"),
			textures_path("necro_two_casting.png"),
			textures_path("necro_two_death.png"),
			textures_path("necro_minion_appear.png"),
			textures_path("necro_minion_idle.png"),
			textures_path("necro_minion_walk.png"),
			textures_path("necro_minion_melee.png"),
			textures_path("necro_minion_death.png"),
			textures_path("archerAnims.png"),
			textures_path("archerArrow.png"),
			textures_path("dragon_flying.png"),

			// Background layers

			textures_path("tutorialBackground1.png"),
			textures_path("tutorialBackground2.png"),
			textures_path("tutorialBackground3.png"),
			textures_path("tutorialBackground4.png"),
			textures_path("tutorialBackground5.png"),

			textures_path("levelOneBackground1.png"),
			textures_path("levelOneBackground2.png"),
			textures_path("levelOneBackground3.png"),
			textures_path("levelOneBackground4.png"),

			textures_path("freeRoamOneBackground1.png"),
			textures_path("freeRoamOneBackground2.png"),
			textures_path("freeRoamOneBackground3.png"),
			textures_path("freeRoamOneBackground4.png"),
			textures_path("freeRoamOneBackground5.png"),

			textures_path("levelTwoBackground1.png"),
			textures_path("levelTwoBackground2.png"),
			textures_path("levelTwoBackground3.png"),
			textures_path("levelTwoBackground4.png"),

			textures_path("freeRoamTwoBackground1.png"),
			textures_path("freeRoamTwoBackground2.png"),
			textures_path("freeRoamTwoBackground3.png"),
			textures_path("freeRoamTwoBackground4.png"),
			textures_path("freeRoamTwoBackground5.png"),
			textures_path("freeRoamTwoBackground6.png"),

			textures_path("levelThreeBackground1.png"),

			// Tutorial text boxes
			textures_path("tutorial_one.png"),
			textures_path("tutorial_two.png"),
			textures_path("tutorial_three.png"),
			textures_path("tutorial_four.png"),
			textures_path("tutorial_five.png"),
			textures_path("tutorial_six.png"),
			textures_path("tutorial_seven.png"),
			textures_path("tutorial_eight.png"),

			// Start screen & Pause menu buttons
			textures_path("new_game.png"),
			textures_path("new_game_hover.png"),
			textures_path("load_game.png"),
			textures_path("load_game_hover.png"),
			textures_path("save_game.png"),
			textures_path("create_game.png"),
			textures_path("create_game_hover.png"),
			textures_path("exit_game.png"),
			textures_path("exit_hover.png"),
			textures_path("game_title.png"),
			textures_path("open_menu.png"),
			textures_path("close_menu.png"),
			textures_path("empty_image.png"),
			textures_path("start_makeup.png"),
			textures_path("start_makeup_hover.png"),
			textures_path("reset_makeup.png"),
			textures_path("reset_makeup_hover.png"),

			//tootips
			textures_path("fireBallToolTip.png"),
			textures_path("iceShardToolTip.png"),
			textures_path("rockToolTip.png"),
			textures_path("meleeToolTip.png"),
			textures_path("tauntToolTip.png"),
			textures_path("healToolTip.png"),
			textures_path("arrowToolTip.png"),

			//storytelling background
			textures_path("battle.jpg"),
			textures_path("battleSub.jpg"),
			textures_path("room.jpg"),
			textures_path("whisper.png"),
			textures_path("storyBegin.png"),
			textures_path("startScreen.png"),
			textures_path("peaceful.jpg"),
			textures_path("celerbrate.jpg"),
			textures_path("dark.jpg"),
			textures_path("conclusionTheEnd.png"),
			textures_path("conclusionOne.png"),
			textures_path("conclusionTwo.png"),
			textures_path("conclusionThree.png"),
			textures_path("conclusionFour.png"),
			textures_path("conclusionFive.png"),
			textures_path("conclusionSix.png"),
			textures_path("conclusionSeven.png"),
				
			//background dialogue
			textures_path("backgroundOne.png"),
			textures_path("backgroundTwo.png"),
			textures_path("backgroundThree.png"),
			textures_path("backgroundFour.png"),
			textures_path("backgroundFive.png"),

			//Level One dialogue
			textures_path("missionOne.png"),
			textures_path("levelOneDialogueOne.png"),
			textures_path("levelOneDialogueTwo.png"),
			textures_path("levelOneDialogueThree.png"),
			textures_path("levelOneDialogueFour.png"),
			textures_path("levelOneDialogueFive.png"),
			textures_path("levelOneDialogueSix.png"),
			textures_path("levelOneDialogueSeven.png"),
			textures_path("levelOneDialogueEight.png"),
			textures_path("levelOneDialogueNine.png"),
			textures_path("levelOneDialogueTen.png"),

			//Level Two dialogue
			textures_path("missionTwo.png"),
			textures_path("levelTwoDialogueOne.png"),
			textures_path("levelTwoDialogueTwo.png"),
			textures_path("levelTwoDialogueThree.png"),
			textures_path("levelTwoDialogueFour.png"),
			textures_path("levelTwoDialogueFive.png"),
			textures_path("levelTwoDialogueSix.png"),
			
			//level Three dialogue
			//textures_path("levelThreeDialogueOne.png"),
			textures_path("levelThreeDialogueTwo.png"),
			textures_path("levelThreeDialogueThree.png"),
			textures_path("levelThreeDialogueFour.png"),
			textures_path("levelThreeDialogueFive.png"),
			textures_path("levelThreeDialogueSix.png"),
			textures_path("levelThreeDialogueSeven.png"),
			textures_path("levelThreeDialogueEight.png"),
			textures_path("missionThreeOne.png"),
			textures_path("missionThreeTwo.png"),
			textures_path("missionThreeThree.png"),
			textures_path("missionThreeFour.png"),

			//level Four dialogue
			textures_path("levelFourDialogueOne.png"),
			textures_path("levelFourDialogueTwo.png"),
			textures_path("levelFourDialogueThree.png"),
			textures_path("levelFourDialogueFour.png"),

			//Free roam level 1 dialogue
			textures_path("freeRoamLevelOneDialogueOne.png"),
			textures_path("freeRoamLevelOneDialogueTwo.png"),
			textures_path("freeRoamLevelOneDialogueThree.png"),
			textures_path("freeRoamLevelOneDialogueFour.png"),
			textures_path("freeRoamLevelOneDialogueFive.png"),

			//Free roam level 2 dialogue
			textures_path("freeRoamLevelTwoDialogueOne.png"),
			textures_path("freeRoamLevelTwoDialogueTwo.png"),
			textures_path("freeRoamLevelTwoDialogueThree.png"),

			textures_path("freeRoamTutorial.png"),
			textures_path("helper.png"),
				
			textures_path("particlered.png"),
			textures_path("firefly.png"),
			textures_path("platform.png"),
			textures_path("treasure_chest_sheet.png"),
			textures_path("smoke_particle.png"),
			textures_path("damage_increase_msg.png"),
			textures_path("health_increase_msg.png"),

			//Size Indicator
			textures_path("zero_out_of_four.png"),
			textures_path("one_out_of_four.png"),
			textures_path("two_out_of_four.png"),
			textures_path("three_out_of_four.png"),
			textures_path("four_out_of_four.png"),
			// charactor selectors

			textures_path("selectPanel.png"),

			textures_path("mage_anim_select.png"),
			textures_path("archerAnims_select.png"),
			textures_path("swordsman_select.png"),
			textures_path("necromancer_select.png"),
			textures_path("necro_two_select.png"),

			textures_path("new_makeup_options.png"),
			textures_path("yes_option.png"),
			textures_path("no_option.png")
  };
  
	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("pebble"),
		shader_path("textured"),
		shader_path("water"),
		shader_path("particle"),
		shader_path("basicEnemy"),
		shader_path("light")};	// NEW

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

	float NECRO_ONE_IDLE_FRAME_TIME = 200;
	float NECRO_ONE_CASTING_FRAME_TIME = 241.666666667;
	float NECRO_ONE_SUMMONING_FRAME_TIME = 550;
	float NECRO_ONE_DEATH_FRAME_TIME = 100;

	float NECRO_TWO_APPEAR_FRAME_TIME = 250;
	float NECRO_TWO_IDLE_FRAME_TIME = 200;
	float NECRO_TWO_MELEE_FRAME_TIME = 150;
	float NECRO_TWO_CASTING_FRAME_TIME = 150;
	float NECRO_TWO_DEATH_FRAME_TIME = 250;

	float NECRO_MINION_APPEAR_FRAME_TIME = 220;
	float NECRO_MINION_IDLE_FRAME_TIME = 300;
	float NECRO_MINION_WALK_FRAME_TIME = 100;
	float NECRO_MINION_MELEE_FRAME_TIME = 75;
	float NECRO_MINION_DEATH_FRAME_TIME = 200;

	float ARCHER_IDLE_FRAME_TIME = 250;
	float ARCHER_WALKING_FRAME_TIME = 100;
	float ARCHER_JUMPING_FRAME_TIME = 750;
	float ARCHER_ATTACKING_FRAME_TIME = 75;
	float ARCHER_TURN_ATTACKING_FRAME_TIME = 150;
	float ARCHER_DEAD_FRAME_TIME = 250;

	float DRAGON_FLYING_FRAME_TIME = 225;

	// pixel positions for the light balls in the background
	std::vector<float> lightBallsXcoords;
	std::vector<float> lightBallsYcoords;

	std::vector<float> fireFlyPosX;
	std::vector<float> fireFlyPosY;

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

	// Necromancer phase 1 frame stats
	const int NECRO_ONE_IDLE_FRAMES = 4;
	const GLfloat NECRO_ONE_IDLE_FRAME_WIDTH = 0.25;

	const int NECRO_ONE_CASTING_FRAMES = 6;
	const GLfloat NECRO_ONE_CASTING_FRAME_WIDTH = 0.16666666666;

	const int NECRO_ONE_SUMMONING_FRAMES = 4;
	const GLfloat NECRO_ONE_SUMMONING_FRAME_WIDTH = 0.25;

	const int NECRO_ONE_DEATH_ONE_FRAMES = 10;
	const GLfloat NECRO_ONE_DEATH_ONE_FRAME_WIDTH = 0.10;

	const int NECRO_ONE_DEATH_TWO_FRAMES = 10;
	const GLfloat NECRO_ONE_DEATH_TWO_FRAME_WIDTH = 0.10;

	// Necromancer phase 2 frame stats
	const int NECRO_TWO_APPEAR_FRAMES = 6;
	const GLfloat NECRO_TWO_APPEAR_FRAME_WIDTH = 0.16666666666;

	const int NECRO_TWO_IDLE_FRAMES = 8;
	const GLfloat NECRO_TWO_IDLE_FRAME_WIDTH = 0.125;

	const int NECRO_TWO_MELEE_FRAMES = 10;
	const GLfloat NECRO_TWO_MELEE_FRAME_WIDTH = 0.10;

	const int NECRO_TWO_CASTING_FRAMES = 8;
	const GLfloat NECRO_TWO_CASTING_FRAME_WIDTH = 0.125;

	const int NECRO_TWO_DEATH_FRAMES = 7;
	const GLfloat NECRO_TWO_DEATH_FRAME_WIDTH = 0.14285714285;

	// Necromancer minion frame stats
	const int NECRO_MINION_APPEAR_FRAMES = 10;
	const GLfloat NECRO_MINION_APPEAR_FRAME_WIDTH = 0.10;

	const int NECRO_MINION_IDLE_FRAMES = 5;
	const GLfloat NECRO_MINION_IDLE_FRAME_WIDTH = 0.2;

	const int NECRO_MINION_WALK_FRAMES = 8;
	const GLfloat NECRO_MINION_WALK_FRAME_WIDTH = 0.125;

	const int NECRO_MINION_MELEE_FRAMES = 10;
	const GLfloat NECRO_MINION_MELEE_FRAME_WIDTH = 0.10;

	const int NECRO_MINION_DEATH_FRAMES = 10;
	const GLfloat NECRO_MINION_DEATH_FRAME_WIDTH = 0.10;

	// Archer frame stats
	const GLfloat ARCHER_FRAME_WIDTH = 0.125;
	const int ARCHER_IDLE_FRAMES = 3;

	const int ARCHER_WALKING_FRAMES = 8;

	const int ARCHER_JUMPING_FRAMES = 2;

	const int ARCHER_ATTACKING_FRAMES = 7;

	const int ARCHER_DEAD_FRAMES = 8;

	// Dragon frame stats
	const GLfloat DRAGON_FLYING_FRAME_WIDTH = 0.11111111111;
	const int DRAGON_FLYING_FRAMES = 9;

	// Camera/scrolling constants
	float CAMERA_OFFSET_LEFT = 400;
	float CAMERA_OFFSET_TOP = 400;
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
	std::vector<vec3> splineControlPoints;
	int gameLevel = 1;
	int shouldDeform = 0;
	bool implode = false;

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
	void drawTexturedMesh(Entity entity, const mat3& projection, GLint& frame, GLfloat& frameWidth, float elapsed_ms);
	void drawDeathParticles(Entity entity, const mat3& projection);
	// void initParticlesBuffer();
	void drawToScreen();

	void drawLight(Entity entity);

	// Window handle
	GLFWwindow* window;
	float screen_scale;  // Screen to pixel coordinates scale factor (for apple
						 // retina display?)

	// Screen texture handles
	GLuint frame_buffer;
	GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

	Entity screen_state_entity;

	// GLuint particles_position_buffer;
	float deformTime = 0.f;

	//time 
	float time = 0;
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, std::string& gs_path, GLuint& out_program);