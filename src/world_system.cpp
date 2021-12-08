// Header
#include "world_system.hpp"
#include "world_init.hpp"
#include "physics_system.hpp"
#include "json_loader.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <iostream>

#include "ai_system.hpp"
#include "skill_system.hpp"
#include <queue>
#include "BFS.hpp"

// Game configuration
const size_t BOULDER_DELAY_MS = 1000 * 3;
const size_t BARRIER_DELAY = 4000;
const size_t ENEMY_TURN_TIME = 3000;
const vec2 TURN_INDICATOR_LOCATION = {600, 150};
const int NUM_DEATH_PARTICLES = 4000;
vec2 CURRPLAYER_LOCATION = {};

const float animation_timer = 250.f;
const float hit_position = 20.f;
//arrow BFS
vector<pair<int, int>> ArrowResult;
Entity currentArrow;

Entity player_archer;
Entity player_mage;
Entity enemy_mage;
Entity player_swordsman;
Entity enemy_swordsman;
Entity necromancer;
Entity necromancer_phase_one;
Entity necromancer_phase_two;
Entity necromancer_minion;
Entity silence_icon;

// icons
Entity melee_icon;
Entity iceShard_icon;
Entity fireBall_icon;
Entity taunt_icon;
Entity heal_icon;
Entity rock_icon;
Entity arrow_icon;

int16 gameLevel;
int16 loadedLevel;

Entity currPlayer;
// Entity target;
Entity prevPlayer;

// int playersDead = 0;
int playerUseMelee = 0;

vec2 msPos = vec2(0, 0);
bool is_ms_clicked = false;

float next_barrier_spawn = 1000;

float enemy_turn_timer = 1000;

int FIREBALLSELECTED = 0;
int SILENCESELECTED = 0;

bool loaded_game = false;

int selected_skill = -1;

// Free roam variables
bool isFreeRoam = false;
int freeRoamLevel = 1;


// Makeup Game
bool isMakeupGame = false;
bool isReady = false;
bool isReset = false;
bool isFInished = false;
int companion_size = 0;
int enemy_size = 0;

vec2 companionOnePos = {100, 300};
vec2 companionTwoPos = { 225, 300 };
vec2 companionThreePos = { 350, 300 };
vec2 companionFourPos = { 475, 300 };

vec2 enemyOnePos = { 1100, 300 };
vec2 enemyTwoPos = { 950, 300 };
vec2 enemyThreePos = { 800, 300 };
vec2 enemyFourPos = { 650, 300 };

// Makeup Game Speed:
int necro_one_speed = 1; 
int necro_two_speed = 2;

int enemy_swordsman_speed = 4; //[4-7]
int init_enemy_swordsman_speed = 4;

int player_swordsman_speed = 8; //[8-11]
int init_player_swordsman_speed = 8;

int player_archer_speed = 12; //[12-15]
int init_player_archer_speed = 12;

int enemy_mage_speed = 16; //[16-19]
int init_enemy_mage_speed = 16;

int player_mage_speed = 20; //[20-23]
int init_player_mage_speed = 20;


int beginning = 0;	// for beginning speech
int dragon = 0;		// for dragon speech

const size_t MAX_BOULDERS = 3;

// mouse gesture skills related=============
int startMousePosCollect = 0;
std::vector<vec2> mouseGestures;
int gestureSkillRemaining = 1;
int extraCompanionTurn = 1;
//===========================================
int hover_skill = -1;
// selected button
Entity selectedButton;

// current projectile
Entity currentProjectile;

int story = 0;

using namespace std;

WorldSystem::WorldSystem()
	: points(0)
	, next_boulder_spawn(0.f)
{
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

WorldSystem::~WorldSystem()
{
	// Destroy music components
	if (registry.background_music != nullptr)
		Mix_FreeMusic(registry.background_music);
	if (registry.salmon_dead_sound != nullptr)
		Mix_FreeChunk(registry.salmon_dead_sound);
	if (registry.salmon_eat_sound != nullptr)
		Mix_FreeChunk(registry.salmon_eat_sound);
	if (registry.hit_enemy_sound != nullptr)
		Mix_FreeChunk(registry.hit_enemy_sound);
	if (registry.fireball_explosion_sound != nullptr)
		Mix_FreeChunk(registry.fireball_explosion_sound);
	if (registry.death_enemy_sound != nullptr)
		Mix_FreeChunk(registry.death_enemy_sound);
	if (registry.fire_spell_sound != nullptr)
		Mix_FreeChunk(registry.fire_spell_sound);
	if (registry.rock_spell_sound != nullptr)
		Mix_FreeChunk(registry.rock_spell_sound);
	if (registry.heal_spell_sound != nullptr)
		Mix_FreeChunk(registry.heal_spell_sound);
	if (registry.taunt_spell_sound != nullptr)
		Mix_FreeChunk(registry.taunt_spell_sound);
	if (registry.melee_spell_sound != nullptr)
		Mix_FreeChunk(registry.melee_spell_sound);
	if (registry.silence_spell_sound != nullptr)
		Mix_FreeChunk(registry.silence_spell_sound);
	if (registry.lightning_spell_sound != nullptr)
		Mix_FreeChunk(registry.lightning_spell_sound);
	if (registry.ice_spell_sound != nullptr)
		Mix_FreeChunk(registry.ice_spell_sound);
	if (registry.summon_spell_sound != nullptr)
		Mix_FreeChunk(registry.summon_spell_sound);
	if (registry.button_hover_sound != nullptr)
		Mix_FreeChunk(registry.button_hover_sound);
	if (registry.turning_sound != nullptr)
		Mix_FreeChunk(registry.turning_sound);
	if (registry.charge_spell_sound != nullptr)
		Mix_FreeChunk(registry.charge_spell_sound);
	if (registry.beam_spell_sound != nullptr)
		Mix_FreeChunk(registry.beam_spell_sound);
	if (registry.minion_spawn_sound != nullptr)
		Mix_FreeChunk(registry.minion_spawn_sound);
	if (registry.error_sound != nullptr)
		Mix_FreeChunk(registry.error_sound);
	if (registry.gesture_heal_sound != nullptr)
		Mix_FreeChunk(registry.gesture_heal_sound);
	if (registry.gesture_aoe_sound != nullptr)
		Mix_FreeChunk(registry.gesture_aoe_sound);
	if (registry.gesture_turn_sound != nullptr)
		Mix_FreeChunk(registry.gesture_turn_sound);
	if (registry.menu_music != nullptr)
		Mix_FreeMusic(registry.menu_music);
	if (registry.wintervale_music != nullptr)
		Mix_FreeMusic(registry.wintervale_music);
	if (registry.cestershire_music != nullptr)
		Mix_FreeMusic(registry.cestershire_music);
	if (registry.boss_music != nullptr)
		Mix_FreeMusic(registry.boss_music);
	if (registry.crow_sound != nullptr)
		Mix_FreeChunk(registry.crow_sound);
	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

// Debugging
namespace
{
	void glfw_err_cb(int error, const char *desc)
	{
		fprintf(stderr, "%d: %s", error, desc);
	}

	void initParticlesBuffer(ParticlePool &pool)
	{
		GLuint particles_position_buffer;
		glGenBuffers(1, &particles_position_buffer);
		pool.particles_position_buffer = particles_position_buffer;
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, pool.size * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		pool.positions = new float[pool.size * 3];
	}

	std::vector<vec3> GenerateSpline(std::vector<vec3> points, int stepsPerCurve = 30, float tension = 1.5)
	{
		std::vector<vec3> result;

		for (int i = 0; i < points.size() - 1; i++)
		{
			vec3 prev = i == 0 ? points[i] : points[i - 1];
			vec3 currStart = points[i];
			vec3 currEnd = points[i + 1];
			vec3 next = i == points.size() - 2 ? points[i + 1] : points[i + 2];

			for (int step = 0; step <= stepsPerCurve; step++)
			{
				float t = (float)step / stepsPerCurve;
				float tSquared = t * t;
				float tCubed = tSquared * t;

				vec3 interpolatedPoint =
					(-.5f * tension * tCubed + tension * tSquared - .5f * tension * t) * prev +
					(1 + .5f * tSquared * (tension - 6) + .5f * tCubed * (4 - tension)) * currStart +
					(.5f * tCubed * (tension - 4) + .5f * tension * t - (tension - 3) * tSquared) * currEnd +
					(-.5f * tension * tSquared + .5f * tension * tCubed) * next;

				result.push_back(interpolatedPoint);
			}
		}

		return result;
	}
}

// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow *WorldSystem::create_window(int width, int height)
{
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW");
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// If you are on Linux or Windows, you can change these 2 numbers to 4 and 3 and
	// enable the glDebugMessageCallback to have OpenGL catch your mistakes for you.
	// GLFW / OGL Initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create the main window (for rendering, keyboard, and mouse input)
	window = glfwCreateWindow(width, height, "Windfall", nullptr, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow *wnd, int _0, int _1, int _2, int _3)
	{ ((WorldSystem *)glfwGetWindowUserPointer(wnd))->on_key(_0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow *wnd, double _0, double _1)
	{ ((WorldSystem *)glfwGetWindowUserPointer(wnd))->on_mouse_move({_0, _1}); };

	// handle mosue click
	auto mouse_button_callback = [](GLFWwindow *wnd, int _0, int _1, int _2)
	{ ((WorldSystem *)glfwGetWindowUserPointer(wnd))->on_mouse_button(_0, _1, _2); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//////////////////////////////////////
	// Loading music and sounds with SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		fprintf(stderr, "Failed to initialize SDL Audio");
		return nullptr;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		fprintf(stderr, "Failed to open audio device");
		return nullptr;
	}

	// TODO: Add different music and load sound effects later
	registry.background_music = Mix_LoadMUS(audio_path("combatMusic.wav").c_str());
	registry.salmon_dead_sound = Mix_LoadWAV(audio_path("salmon_dead.wav").c_str());
	registry.salmon_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav").c_str());
	registry.hit_enemy_sound = Mix_LoadWAV(audio_path("hit_enemy.wav").c_str());
	registry.fireball_explosion_sound = Mix_LoadWAV(audio_path("fireball_explosion_short.wav").c_str());
	registry.death_enemy_sound = Mix_LoadWAV(audio_path("death_enemy.wav").c_str());
	registry.fire_spell_sound = Mix_LoadWAV(audio_path("fireball_spell.wav").c_str());			// https://mixkit.co/free-sound-effects/spell/
	registry.rock_spell_sound = Mix_LoadWAV(audio_path("rock_spell.wav").c_str());				// https://mixkit.co/free-sound-effects/spell/
	registry.heal_spell_sound = Mix_LoadWAV(audio_path("heal_spell.wav").c_str());				// https://mixkit.co/free-sound-effects/spell/
	registry.taunt_spell_sound = Mix_LoadWAV(audio_path("taunt_spell.wav").c_str());			// https://mixkit.co/free-sound-effects/spell/
	registry.melee_spell_sound = Mix_LoadWAV(audio_path("melee_spell.wav").c_str());			// https://mixkit.co/free-sound-effects/spell/
	registry.silence_spell_sound = Mix_LoadWAV(audio_path("silence_spell.wav").c_str());		// https://freesound.org/people/Vicces1212/sounds/123757/
	registry.lightning_spell_sound = Mix_LoadWAV(audio_path("lightning_spell.wav").c_str());	// https://freesound.org/people/Puerta118m/sounds/471691/
	registry.ice_spell_sound = Mix_LoadWAV(audio_path("ice_spell.wav").c_str());				// https://freesound.org/people/EminYILDIRIM/sounds/550267/
	registry.summon_spell_sound = Mix_LoadWAV(audio_path("summon_spell.wav").c_str());			// https://freesound.org/people/alonsotm/sounds/396500/
	registry.button_hover_sound = Mix_LoadWAV(audio_path("button_hover.wav").c_str());			// https://freesound.org/people/wobesound/sounds/488382/
	registry.turning_sound = Mix_LoadWAV(audio_path("turning.wav").c_str());					// https://freesound.org/people/InspectorJ/sounds/416179/
	registry.charge_spell_sound = Mix_LoadWAV(audio_path("charge_spell.wav").c_str());			// https://freesound.org/people/18hiltc/sounds/186048/
	registry.beam_spell_sound = Mix_LoadWAV(audio_path("beam_spell.wav").c_str());				// https://freesound.org/people/MATRIXXX_/sounds/403297/
	registry.minion_spawn_sound = Mix_LoadWAV(audio_path("minion_spawn.wav").c_str());			// https://freesound.org/people/Breviceps/sounds/453391/
	registry.error_sound = Mix_LoadWAV(audio_path("error.wav").c_str());						// https://freesound.org/people/plasterbrain/sounds/423169/
	registry.gesture_heal_sound = Mix_LoadWAV(audio_path("gesture_heal.wav").c_str());			// https://freesound.org/people/SilverIllusionist/sounds/580814/
	registry.gesture_aoe_sound = Mix_LoadWAV(audio_path("gesture_aoe.wav").c_str());			// https://freesound.org/people/humanoide9000/sounds/329029/
	registry.gesture_turn_sound = Mix_LoadWAV(audio_path("gesture_turn.wav").c_str());			// https://freesound.org/people/Aleks41/sounds/406063/
	registry.menu_music = Mix_LoadMUS(audio_path("menuMusic.wav").c_str());						// https://downloads.khinsider.com/game-soundtracks/album/octopath-traveler-original-soundtrack-2018
	registry.wintervale_music = Mix_LoadMUS(audio_path("wintervaleMusic.wav").c_str());			// https://downloads.khinsider.com/game-soundtracks/album/octopath-traveler-original-soundtrack-2018
	registry.cestershire_music = Mix_LoadMUS(audio_path("cestershireMusic.wav").c_str());		// https://downloads.khinsider.com/game-soundtracks/album/octopath-traveler-original-soundtrack-2018
	registry.boss_music = Mix_LoadMUS(audio_path("bossMusic.wav").c_str());						// https://downloads.khinsider.com/game-soundtracks/album/darkest-dungeon-ost
	registry.crow_sound = Mix_LoadWAV(audio_path("crow.wav").c_str());							// https://freesound.org/people/vixuxx/sounds/9874/

	if (registry.background_music == nullptr || registry.salmon_dead_sound == nullptr || registry.salmon_eat_sound == nullptr || registry.hit_enemy_sound == nullptr || registry.fireball_explosion_sound == nullptr || registry.death_enemy_sound == nullptr || registry.fire_spell_sound == nullptr || registry.rock_spell_sound == nullptr || registry.heal_spell_sound == nullptr || registry.taunt_spell_sound == nullptr || registry.melee_spell_sound == nullptr || registry.silence_spell_sound == nullptr || registry.lightning_spell_sound == nullptr || registry.ice_spell_sound == nullptr || registry.summon_spell_sound == nullptr || registry.button_hover_sound == nullptr || registry.turning_sound == nullptr || registry.summon_spell_sound == nullptr || registry.charge_spell_sound == nullptr || registry.beam_spell_sound == nullptr || registry.minion_spawn_sound == nullptr || registry.error_sound == nullptr || registry.gesture_heal_sound == nullptr || registry.gesture_aoe_sound == nullptr || registry.gesture_turn_sound == nullptr || registry.menu_music == nullptr || registry.wintervale_music == nullptr || registry.cestershire_music == nullptr || registry.boss_music == nullptr || registry.crow_sound == nullptr)
	{
		//|| registry.menu_music == nullptr || registry.wintervale_music == nullptr || registry.cestershire_music == nullptr
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
				audio_path("combatMusic.wav").c_str(),
				audio_path("salmon_dead.wav").c_str(),
				audio_path("salmon_eat.wav").c_str(),
				audio_path("hit_enemy.wav").c_str(),
				audio_path("fireball_explosion_short.wav").c_str(),
				audio_path("fireball_spell.wav").c_str(),
				audio_path("rock_spell.wav").c_str(),
				audio_path("heal_spell.wav").c_str(),
				audio_path("taunt_spell.wav").c_str(),
				audio_path("melee_spell.wav").c_str(),
				audio_path("death_enemy.wav").c_str(),
				audio_path("silence_spell.wav").c_str(),
				audio_path("lightning_spell.wav").c_str(),
				audio_path("ice_spell.wav").c_str(),
				audio_path("summon_spell.wav").c_str(),
				audio_path("button_hover.wav").c_str(),
				audio_path("turning.wav").c_str(),
				audio_path("summon_spell.wav").c_str(),
				audio_path("charge_spell.wav").c_str(),
				audio_path("beam_spell.wav").c_str(),
				audio_path("minion_spawn.wav").c_str(),
				audio_path("error.wav").c_str(),
				audio_path("gesture_heal.wav").c_str(),
				audio_path("gesture_aoe.wav").c_str(),
				audio_path("gesture_turn.wav").c_str(),
				audio_path("menuMusic.wav").c_str(), 
				audio_path("wintervaleMusic.wav").c_str(), 
				audio_path("cestershireMusic.wav").c_str(), 
				audio_path("bossMusic.wav").c_str(),
				audio_path("crow.wav").c_str());
		return nullptr;
	}
	return window;
}

void WorldSystem::init(RenderSystem *renderer_arg, AISystem *ai_arg, SkillSystem *skill_arg, SwarmSystem *swarm_arg)
{
	this->renderer = renderer_arg;
	this->ai = ai_arg;
	this->sk = skill_arg;
	this->swarmSys = swarm_arg;

	Mix_VolumeMusic(MIX_MAX_VOLUME / 30);
	//Mix_FadeInMusic(registry.background_music, -1, 5000);
	Mix_VolumeChunk(registry.hit_enemy_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.fireball_explosion_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.death_enemy_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.fire_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.rock_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.heal_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.taunt_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.melee_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.silence_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.lightning_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.ice_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.summon_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.button_hover_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.charge_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.beam_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.minion_spawn_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.error_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(registry.gesture_heal_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.gesture_aoe_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(registry.gesture_turn_sound, MIX_MAX_VOLUME / 10);
	Mix_FadeInMusic(registry.menu_music, -1, 3000);
	//Mix_FadeInMusic(registry.wintervale_music, -1, 5000);
	//Mix_FadeInMusic(registry.cestershire_music, -1, 5000);

	fprintf(stderr, "Loaded music\n");

	// Start game with a start screen
	render_startscreen();
}

void WorldSystem::startMenuCleanUp()
{
	// GO BACK TO START MENU
	pauseMenuOpened = 0;
	canStep = 0;
	story = 0;
	isMakeupGame = false;
	tutorial_enabled = 0;

	companion_size = 0;
	enemy_size = 0;

	enemy_swordsman_speed = init_enemy_swordsman_speed;
	enemy_mage_speed = init_enemy_mage_speed;
	player_swordsman_speed = init_player_swordsman_speed;
	player_archer_speed = init_player_archer_speed;
	player_mage_speed = init_player_mage_speed;

	registry.HPDebuff = 0;
	registry.HPBuff = 0;
	registry.swordsmanHPBuff = 0;

	isReady = false;
	isReset = false;
	isFInished = false;

	isFreeRoam = false;
	freeRoamLevel = 1;

	renderer->gameLevel = 1;
				
	while (registry.motions.entities.size() > 0)
		registry.remove_all_components_of(registry.motions.entities.back());
	while (registry.renderRequests.entities.size() > 0)
		registry.remove_all_components_of(registry.renderRequests.entities.back());
	if (registry.charIndicator.components.size() != 0)
	{
		registry.remove_all_components_of(registry.charIndicator.entities[0]);
	}
	// Debugging for memory/component leaks
	registry.list_all_components();
	
	Mix_FadeInMusic(registry.menu_music, -1, 3000);
	render_startscreen();
}

void WorldSystem::render_startscreen()
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	createStoryBackground(renderer, {w / 2, h / 2}, 6);
	new_game_button = createUIButton(renderer, {850, 230}, NEW_GAME);
	load_game_button = createUIButton(renderer, {850, 330}, LOAD_GAME);
	makeup_game_button = createUIButton(renderer, {850, 430}, 8);
	exit_game_button = createUIButton(renderer, {850, 530}, EXIT_GAME);
	registry.motions.get(exit_game_button).scale = {150, 70};
}

void WorldSystem::displayPlayerTurn()
{
	displayTurnIndicator(1);
	
	if (registry.charIndicator.has(char_indicator)) {
		registry.motions.get(char_indicator).position = vec2(CURRPLAYER_LOCATION.x, CURRPLAYER_LOCATION.y - 150);
		registry.charIndicator.get(char_indicator).owner = currPlayer;
	}
	else {
		char_indicator = createCharIndicator(renderer, CURRPLAYER_LOCATION, currPlayer);
	}
}

void WorldSystem::displayEnemyTurn()
{
	displayTurnIndicator(0);
	if (registry.charIndicator.has(char_indicator)) {
		registry.motions.get(char_indicator).position = vec2(CURRPLAYER_LOCATION.x, CURRPLAYER_LOCATION.y - 150);
		registry.charIndicator.get(char_indicator).owner = currPlayer;
	}
	else {
		char_indicator = createCharIndicator(renderer, CURRPLAYER_LOCATION, currPlayer);
	}
}

void WorldSystem::displayTurnIndicator(int isPlayerTurn)
{
	if (registry.turnIndicators.components.size() == 0)
	{
		// Only initialize this entity once!
		turn_indicator = isPlayerTurn ? createPlayerTurn(renderer, TURN_INDICATOR_LOCATION) : createEnemyTurn(renderer, TURN_INDICATOR_LOCATION);
	}
	else
	{
		Motion &motion = registry.motions.get(turn_indicator);
		RenderRequest &renderRequest = registry.renderRequests.get(turn_indicator);
		motion.position = TURN_INDICATOR_LOCATION;
		renderRequest.used_texture = isPlayerTurn ? TEXTURE_ASSET_ID::PLAYER_TURN : TEXTURE_ASSET_ID::ENEMY_TURN;
	}
}

void WorldSystem::iceShardAttack(Entity currPlayer)
{
	// Mix_PlayChannel(-1, fire_spell_sound, 0); // added fire spell sound but doesnt work
	Motion enemy = registry.motions.get(currPlayer);
	if (!registry.deathTimers.has(currPlayer))
	{
		Motion enemy = registry.motions.get(currPlayer);
		Entity resultEntity = createIceShard(renderer, {enemy.position.x, enemy.position.y}, 3.14159, {-100, 0}, 0);
		Motion *ballacc = &registry.motions.get(resultEntity);
		ballacc->acceleration = vec2(1000 * -100 / ICESHARDSPEED, 1000 * 0 / ICESHARDSPEED);
		Projectile *proj = &registry.projectiles.get(resultEntity);
		proj->flyingTimer = 2000.f;
	}
}

std::vector<Entity> roundVec;
void WorldSystem::createRound()
{
	printf("Enemy size: %zu\n", registry.enemies.size());
	printf("Companion size: %zu\n", registry.companions.size());
	std::vector<int> speedVec;
	for (int i = 0; i < registry.enemies.components.size(); i++)
	{ // iterate through all enemies to get speed stats
		Entity &entity = registry.enemies.entities[i];

		// also decrement taunt duration if present
		if (registry.taunts.has(entity))
		{
			Taunt *t = &registry.taunts.get(entity);
			t->duration--;
		}
		if (registry.bleeds.has(entity))
		{
			Bleed *b = &registry.bleeds.get(entity);
			b->duration--;
			sk->launchBleedDMG(entity, renderer);
			update_healthBars();
		}
		// also decrement silence duration if present
		if (registry.silenced.has(entity))
		{
			Silenced *s = &registry.silenced.get(entity);
			s->turns--;
			if (s->turns <= 0)
			{ // remove silence to add speed stat later if turns <= 0
				sk->removeSilence(entity);
			}
		}
		// also decrement ultimate duration if present
		if (registry.ultimate.has(entity))
		{ // need to emplace ultimate onto necro2 for countdown when David implements the skill
			Ultimate *u = &registry.ultimate.get(entity);
			u->ultiDuration--;
			printf("ENTITY IS %g \n", float(registry.stats.get(entity).speed));
			printf("MY ULTIDURATION IS %g \n", float(u->ultiDuration));
			// need to remove the skill when duration <= 0
			if (u->ultiDuration <= 0)
			{
				sk->removeUltimate(entity);
			}
		}
		// also decrement shield duration if present
		if (registry.shield.has(entity))
		{ // need to emplace shield onto necro2 for countdown when David implements the skill
			Shield *sh = &registry.shield.get(entity);
			if (sh->shieldDuration > 0)
			{
				sh->shieldDuration--;
				printf("ENTITY IS %g \n", float(registry.stats.get(entity).speed));
				printf("MY SHIELD IS %g \n", float(sh->shieldDuration));
			}
			else
			{
				// registry.remove_all_components_of(entity);
				sk->removeShield(entity);
			}
		}

		if (!registry.silenced.has(entity))
		{
			Statistics &checkSpeed = registry.stats.get(entity);
			if (checkSpeed.speed == 2)
			{ // add extra turn for necromancer, change this if necromancer speed changes
				speedVec.push_back(checkSpeed.speed);
			}
			speedVec.push_back(checkSpeed.speed);
		}
	}

	for (int i = 0; i < registry.shieldIcons.components.size(); i++)
	{
		ShieldIcon &sh = registry.shieldIcons.components[i];
		sh.shieldDuration -= 1;
	}

	for (int i = 0; i < registry.companions.components.size(); i++)
	{ // iterate through all companions to get speed stats
		Entity &entity = registry.companions.entities[i];

		// also decrement taunt duration if present
		if (registry.taunts.has(entity))
		{
			Taunt *t = &registry.taunts.get(entity);
			t->duration--;
		}
		// also decrement bleed duration if present
		if (registry.bleeds.has(entity))
		{
			Bleed *b = &registry.bleeds.get(entity);
			b->duration--;
			sk->launchBleedDMG(entity, renderer);
			update_healthBars();
		}
		// also decrement silence duration if present
		if (registry.silenced.has(entity))
		{
			Silenced *s = &registry.silenced.get(entity);
			s->turns--;
			if (s->turns <= 0)
			{ // remove silence to add speed stat later if turns <= 0
				sk->removeSilence(entity);
			}
		}

		if (!registry.silenced.has(entity))
		{
			Statistics &checkSpeed = registry.stats.get(entity);
			if (extraCompanionTurn <= 0)
			{ // if extraCompanionTurn <= 0, add extra turn for all companions on screen
				speedVec.push_back(checkSpeed.speed);
			}
			speedVec.push_back(checkSpeed.speed);
		}
	}

	std::sort(speedVec.begin(), speedVec.end(), std::greater<int>()); // sorts in descending order

	for (int i = 0; i < speedVec.size(); i++)
	{
		for (int j = 0; j < registry.companions.components.size(); j++)
		{
			Entity &entity = registry.companions.entities[j]; // check companions stats
			Statistics &checkSpeed = registry.stats.get(entity);
			if (speedVec[i] == checkSpeed.speed)
			{
				roundVec.push_back(entity); // push to roundVec for use in checkRound
			}
		}
		for (int j = 0; j < registry.enemies.components.size(); j++)
		{
			Entity &entity = registry.enemies.entities[j]; // check enemies stats
			Statistics &checkSpeed = registry.stats.get(entity);
			if (speedVec[i] == checkSpeed.speed)
			{
				roundVec.push_back(entity); // push to roundVec for use in checkRound
			}
		}
	}

	if (extraCompanionTurn <= 0)
	{
		extraCompanionTurn = 1;
	}

	// print the sorted array
	for (int i = 0; i < roundVec.size(); i++)
	{
		printf("Round vec index: %d, %g \n",i, float(registry.stats.get(roundVec[i]).speed));
	}
}

void WorldSystem::checkRound()
{
	printf("am here at checkRound \n");

	if (roundVec.empty())
	{ // if empty, create new round
		printf("roundVec is empty, creating a new round \n");
		createRound();
	}

	Entity toPlay = roundVec[0]; // get first element
	printf("erase %g \n", float(registry.stats.get(roundVec[0]).speed));
	roundVec.erase(roundVec.begin()); // erase the first element

	if (registry.companions.has(toPlay) && registry.stats.get(toPlay).health > 0)
	{ // toPlay is companion, put to currPlayer to pass for fireball
		printf("its %g player turn \n", float(registry.stats.get(toPlay).speed));
		player_turn = 1;
		currPlayer = toPlay;
		// update skills display
		showCorrectSkills();
	}
	else if (registry.enemies.has(toPlay) && registry.stats.get(toPlay).health > 0)
	{ // toPlay is enemy, put to currPlayer to pass for fireball
		printf("its %g enemy turn \n", float(registry.stats.get(toPlay).speed));
		player_turn = 0;
		currPlayer = toPlay;
		// update skills display
		showCorrectSkills();
	}
	else
	{
		printf("no player or enemy, checking round now \n");
		prevPlayer = currPlayer;
		checkRound();
		// update skills display
		showCorrectSkills();
	}

	CURRPLAYER_LOCATION = registry.motions.get(currPlayer).position; // get currPlayer location

	printf("finished check round \n");
	// print the current round
	for (int i = 0; i < roundVec.size(); i++)
	{
		printf("CURRENT ROUND IS %g \n", float(registry.stats.get(roundVec[i]).speed));
	}
	printf("playerUseMelee is %g \n", float(playerUseMelee));
}

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update)
{
	// Get the screen dimensions
	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);

	if (renderer->transitioningToNextLevel)
	{
		renderer->nextLevelTranistionPeriod_ms -= elapsed_ms_since_last_update;
		if (renderer->nextLevelTranistionPeriod_ms <= 0.)
		{
			renderer->transitioningToNextLevel = false;
			renderer->nextLevelTranistionPeriod_ms = renderer->DEFAULT_GAME_LEVEL_TRANSITION_PERIOD_MS;
		}
	}

	// restart game if enemies or companions are 0
	if ((gameLevel != 3) && (registry.enemies.size() <= 0 || registry.companions.size() <= 0) && (registry.particlePools.size() <= 0) && (!isFreeRoam) && (!isMakeupGame))
	{
		if (registry.companions.size() <= 0){
			restart_game();
		}
		else if (story == 8 && gameLevel == 0)
		{
			restart_game();
		}
		else if (story == 8 && gameLevel == 1)
		{
			int w, h;
			glfwGetFramebufferSize(window, &w, &h);
			dialogue = createLevelOneDiaogue(renderer, {window_width_px / 2, window_height_px - window_height_px / 3}, 1);
			canStep = 0;
			story = 9;
		}
		else if (story == 19 && gameLevel == 2)
		{
			int w, h;
			glfwGetFramebufferSize(window, &w, &h);
			dialogue = createLevelTwoDiaogue(renderer, {window_width_px / 2, window_height_px - window_height_px / 3}, 1);
			canStep = 0;
			story = 20;
		}

		// restart_game();
	}
	else if ((gameLevel >= 3) && (registry.enemies.size() <= 0) && (registry.companions.size() > 0) && (!isFreeRoam) && (!isMakeupGame))
	{
		if (story == 26)
		{
			int w, h;
			glfwGetFramebufferSize(window, &w, &h);
			dialogue = createLevelThreeDiaogue(renderer, {window_width_px / 2, window_height_px - window_height_px / 3}, 2);
			canStep = 0;
			story = 27;
		}
		else if (story == 36)
		{
			int w, h;
			glfwGetFramebufferSize(window, &w, &h);
			dialogue = createLevelFourDiaogue(renderer, {window_width_px / 2, window_height_px - window_height_px / 3}, 1);
			canStep = 0;
			story = 37;
		}
	}
	else if ((gameLevel >= 3) && ((registry.enemies.size() <= 0) || (registry.companions.size() <= 0)) && (!isFreeRoam) && (!isMakeupGame))
	{
		restart_game();
	}

	if(isMakeupGame && ((registry.companions.size() <= 0) || (registry.enemies.size() <= 0)) && (registry.particlePools.size() <= 0)){
		// Custom game finished go back
		isFInished = true;
		optionPanel = createFinishedOptions(renderer, { 600, 300 }, 1);
		noOption = createFinishedOptions(renderer, { 700, 400 }, 3);
		yesOption = createFinishedOptions(renderer, { 500, 400 }, 2);
	}

	// Updating window title with volume control
	std::stringstream title_ss;
	title_ss << "Music volume (z-key , x-key): " << Mix_VolumeMusic(-1) << " ,   Effects volume (c-key , v-key): " << Mix_VolumeChunk(registry.death_enemy_sound, -1) << " ";
	glfwSetWindowTitle(window, title_ss.str().c_str());

	// Remove debug info from the last step
	while (registry.debugComponents.entities.size() > 0)
		registry.remove_all_components_of(registry.debugComponents.entities.back());

	// Removing out of screen entities
	auto &motions_registry = registry.motions;

	// Remove barrier
	auto &reflects_registry = registry.reflects;

	// Remove entities that leave the screen on the left side
	// Iterate backwards to be able to remove without unterfering with the next object to visit
	// (the containers exchange the last element with the current)
	for (int i = (int)motions_registry.components.size() - 1; i >= 0; --i)
	{
		Motion &motion = motions_registry.components[i];
		if (motion.position.x + abs(motion.scale.x) < 0.f)
		{
			registry.remove_all_components_of(motions_registry.entities[i]);
		}
	}

	// collect mouse gesture
	if (startMousePosCollect == 1)
	{
		mouseGestures.push_back(msPos);
		// creates dots
		createDot(renderer, msPos);
	}
	else
	{
		for (int j = 0; j < registry.dots.components.size(); j++)
		{
			registry.remove_all_components_of(registry.dots.entities[j]);
		}
	}

	// check taunt bleed and silence for enemy and companion
	for (int i = (int)registry.enemies.components.size() - 1; i >= 0; --i)
	{
		if (registry.taunts.has(registry.enemies.entities[i]))
		{
			if (registry.taunts.get(registry.enemies.entities[i]).duration <= 0)
			{
				sk->removeTaunt(registry.enemies.entities[i]);
			}
		}
		if (registry.bleeds.has(registry.enemies.entities[i]))
		{
			if (registry.bleeds.get(registry.enemies.entities[i]).duration <= 0)
			{
				sk->removeBleed(registry.enemies.entities[i]);
			}
		}
		if (registry.silenced.has(registry.enemies.entities[i]))
		{
			if (registry.silenced.get(registry.enemies.entities[i]).turns <= 0)
			{
				sk->removeSilence(registry.enemies.entities[i]);
			}
		}
	}
	for (int i = (int)registry.companions.components.size() - 1; i >= 0; --i)
	{
		if (registry.taunts.has(registry.companions.entities[i]))
		{
			if (registry.taunts.get(registry.companions.entities[i]).duration <= 0)
			{
				sk->removeTaunt(registry.companions.entities[i]);
			}
		}
		if (registry.bleeds.has(registry.companions.entities[i]))
		{
			if (registry.bleeds.get(registry.companions.entities[i]).duration <= 0)
			{
				sk->removeBleed(registry.companions.entities[i]);
			}
		}
		if (registry.silenced.has(registry.companions.entities[i]))
		{
			if (registry.silenced.get(registry.companions.entities[i]).turns <= 0)
			{
				sk->removeSilence(registry.companions.entities[i]);
			}
		}

		if (registry.stats.has(registry.companions.entities[i]) && registry.stats.get(registry.companions.entities[i]).health <= 0)
		{
			// get rid of dead entity's stats indicators
			sk->removeTaunt(registry.companions.entities[i]);
			sk->removeSilence(registry.companions.entities[i]);
			sk->removeBleed(registry.companions.entities[i]);
		}
	}

	for (int i = (int)registry.shield.components.size() - 1; i >= 0; --i)
	{
		if (registry.shield.has(registry.shield.entities[i]))
		{ // need to emplace shield onto necro2 for countdown when David implements the skill
			Shield *sh = &registry.shield.get(registry.shield.entities[i]);
			if (sh->shieldDuration < 0)
			{
				registry.remove_all_components_of(registry.shield.entities[i]);
			}
		}
	}

	for (int i = (int)registry.shieldIcons.components.size() - 1; i >= 0; --i)
	{

		ShieldIcon *sh = &registry.shieldIcons.get(registry.shieldIcons.entities[i]);
		if (sh->shieldDuration <= 0)
		{
			registry.remove_all_components_of(registry.shieldIcons.entities[i]);
		}
	}

	// maintain correct health
	for (int i = (int)registry.stats.components.size() - 1; i >= 0; --i)
	{
		if (registry.stats.components[i].health > registry.stats.components[i].max_health)
		{
			Statistics *stat = &registry.stats.components[i];
			stat->health = stat->max_health;
			update_healthBars();
		}
	}

	//printf("%f %f\n",msPos.x, msPos.y);

	//check bouncing arrow
	for (int i = (int)registry.bouncingArrows.components.size() - 1; i >= 0; --i) {
		BouncingArrow* ba = &registry.bouncingArrows.components[i];
		Motion* baM = &registry.motions.get(registry.bouncingArrows.entities[i]);
		float baXPos = baM->position.x;
		float baYPos = baM->position.y;
		//printf("arrowPos: %f %f\n", baXPos, baYPos);
		//check if collide side
		if (baXPos - baM->scale.x < 0 && baM->velocity.x <= 0 && baM->acceleration.x <=0 &&  ba->bounce_time>0) {

			baM->velocity.x = baM->velocity.x * -1;
			baM->acceleration.x = baM->acceleration.x * -1;
			baM->position.x = baXPos - baM->scale.x;
			ba->bounce_time--;
			//printf("Bouncing from left wall\n");
			//printf("bouncetime= %d\n", ba->bounce_time--);
		}
		if (baXPos + baM->scale.x > screen_width && baM->velocity.x >= 0 && baM->acceleration.x>=0 && ba->bounce_time>0) {

			baM->velocity.x = baM->velocity.x * -1;
			baM->acceleration.x = baM->acceleration.x * -1;
			baM->position.x = baXPos + baM->scale.x;
			ba->bounce_time--;
			//printf("Bouncing from right wall\n");
			//printf("bouncetime= %d\n", ba->bounce_time--);
		}
		if (baYPos - baM->scale.y < 0 && baM->velocity.y <= 0 && baM->acceleration.y >= 0 && ba->bounce_time>0) {

			baM->velocity.y = baM->velocity.y * -1;
			//baM->acceleration.y = baM->acceleration.y * -1;
			baM->position.y = baYPos - baM->scale.y;
			ba->bounce_time--;
			//printf("Bouncing from ceiling\n");
			//printf("bouncetime= %d\n", ba->bounce_time--);
		}
		if (baYPos + baM->scale.y > screen_height && baM->velocity.y >= 0 && baM->acceleration.y >= 0 && ba->bounce_time > 0) {

			baM->velocity.y = baM->velocity.y * -1;
			//baM->acceleration.y = baM->acceleration.y * -1;
			baM->position.y = baYPos + baM->scale.y;
			ba->bounce_time--;
			//printf("Bouncing from floor\n");
			//printf("bouncetime= %d\n", ba->bounce_time--);
		}

		//check if enemy exist
		if (registry.enemies.size() > 0) {
			//stops arrow when it have bounced

			if (ba->bounce_time <= 0 && ba->ai_runned == 0) {
				baM->velocity.x = baM->velocity.x * 0.9;
				baM->velocity.y = baM->velocity.y * 0.9;
				baM->acceleration.x = baM->acceleration.x * 0.9;
				baM->acceleration.y = baM->acceleration.y * 0.9;
				ba->ai_trigger = ba->ai_trigger - elapsed_ms_since_last_update;
			}

			////trigger BFS============================
			if (ba->ai_trigger <= 0 && ba->ai_runned == 0) {
				registry.gravities.remove(registry.bouncingArrows.entities[i]);
				baM->velocity.x = 0;
				baM->velocity.y = 0;
				baM->acceleration.x = 0;
				baM->acceleration.y = 0;
				ba->ai_runned = 1;
				//initilaze visited
				printf("initilizing grid\n");
				//printf("ai_runned is: %d\n", ba->ai_runned);
				vector<vector<bool>> visited;
				for (int i = 0; i < screen_width / 10; i++)
				{
					vector<bool> tempv;
					for (int j = 0; j < screen_height / 10; j++)
					{
						tempv.push_back(false);
					}
					visited.push_back(tempv);
				}

				//initilaze map
				vector<vector<int>> map;
				for (int i = 0; i < screen_width / 10; i++)
				{
					vector<int> tempv1;
					for (int j = 0; j < screen_height / 10; j++)
					{
						tempv1.push_back(1);
					}
					map.push_back(tempv1);
				}
				//if barrier exist
				if (registry.shield.size() != 0) {
					for (int i = 57 - 1 - 8; i <= 63 - 1 + 8; i++) {
						for (int j = 21 - 1 - 8; j <= 64 - 1; j++) {
							map[i][j] = -1;
							visited[i][j] = true;
						}
					}
				}
				// mark enemy
				Motion markedEnemyM = registry.motions.get(registry.enemies.entities[0]);
				pair<int, int> mark = make_pair(static_cast<int>(markedEnemyM.position.x)/10, static_cast<int>(markedEnemyM.position.y)/10);
				map[mark.first][mark.second]=9;

				//start BFS
				vec2 startPos = baM->position;
				vector<pair<int, int>> path;
				int newy = static_cast<int>(startPos.y) / 10;
				int newx = static_cast<int>(startPos.x) / 10;
				path.push_back(make_pair(std::max(0, std::min(screen_width / 10 - 1, newx)), std::max(0,std::min (screen_height / 10 -1, newy))));
				BFS bfs = BFS(std::max(0, std::min(screen_width / 10 - 1, newx)), std::max(0,std::min(screen_height/10 -1, newy)), 0, path);
				ArrowResult = bfs.arrowBFS(map, bfs, visited);
				currentArrow = registry.bouncingArrows.entities[i];
				printf("result path is:\n ");

				//print path
				for (int i = 0; i < ArrowResult.size(); i++) {
					printf("%d, %d\n", ArrowResult[i].first, ArrowResult[i].second);
				}
			}
			//======================================================================


		}
		
	}

	// move arrow
	if (!ArrowResult.empty()) {
		if (registry.motions.has(currentArrow)) {;
			Motion* arrowM = &registry.motions.get(currentArrow);
			arrowM->position = { ArrowResult[0].first*10,ArrowResult[0].second*10 };
			// assumes ArrowResult will never be empty because arrow disappears before path ends
			pair<int, int> currpos = ArrowResult[0];
			pair<int, int> nextpos = ArrowResult[2];
			ArrowResult.erase(ArrowResult.begin());
			ArrowResult.erase(ArrowResult.begin());
			// move right
			if (currpos.first < nextpos.first) {
				arrowM->angle = 0;
			}
			// move left
			if (currpos.first > nextpos.first) {
				arrowM->angle = M_PI;
			}
			if (currpos.second < nextpos.second) {
				arrowM->angle = M_PI/2;
			}
			// move left
			if (currpos.second > nextpos.second) {
				arrowM->angle = 3*M_PI/2;
			}

		}
	}





	// Walk
	for (Entity runner : registry.runners.entities)
	{
		RunTowards &run = registry.runners.get(runner);
		Motion &runner_motion = registry.motions.get(runner);
		run.counter_ms -= elapsed_ms_since_last_update;
		if (run.counter_ms <= 0.f)
		{
			printf("Reached destination\n");
			runner_motion.velocity = {0.f, 0.f};

			auto &anim_type = registry.companions.has(runner) ? registry.companions.get(runner).curr_anim_type
															  : registry.enemies.get(runner).curr_anim_type;

			auto &runner_type = registry.companions.has(runner) ? registry.companions.get(runner).companionType
																: registry.enemies.get(runner).enemyType;

			Entity healthbar = registry.companions.has(runner) ? registry.companions.get(runner).healthbar
															   : registry.enemies.get(runner).healthbar;
			Motion &healthbar_motion = registry.motions.get(healthbar);
			healthbar_motion.velocity = runner_motion.velocity;

			anim_type = ATTACKING;

			// Attack
			Attack &attack = registry.attackers.emplace(runner);
			attack.attack_type = MELEE;
			attack.old_pos = run.old_pos;
			attack.target = run.target;

			if (runner_type == SWORDSMAN)
			{
				attack.counter_ms = 1250.f;
			}
			else if (runner_type == NECROMANCER_MINION)
			{
				attack.counter_ms = 800.f;
			}
			else if (runner_type == NECROMANCER_TWO)
			{
				runner_motion.position = vec2(run.target_position.x - 25, runner_motion.position.y);
				if (run.bleedOrAOE == 0)
				{
					attack.attack_type = BLEEDMELEE;
				}
				else
				{
					attack.attack_type = AOEMELEE;
				}
				attack.counter_ms = 750.f;
			}

			registry.runners.remove(runner);

			// Replace with better melee sound effect
			// Mix_PlayChannel(-1, melee_spell_sound, 0);
		}
	}

	// Attack
	for (Entity attacker : registry.attackers.entities)
	{
		Attack &attack = registry.attackers.get(attacker);

		// Updating animation time
		attack.counter_ms -= elapsed_ms_since_last_update;

		if (!registry.deathTimers.has(attacker))
		{
			if (attack.counter_ms <= 0.f || attack.attack_type == SUMMON)
			{

				// Attack
				if (registry.companions.has(attacker))
				{
					printf("Companion is attacking\n");
					Companion &companion = registry.companions.get(attacker);
					Motion &companion_motion = registry.motions.get(attacker);
					switch (attack.attack_type)
					{
					case FIREBALL:
					{
						Mix_PlayChannel(-1, registry.fire_spell_sound, 0);
						printf("Fireball attack companion\n");
						currentProjectile = sk->launchFireball(companion_motion.position, attack.old_pos, renderer);
						Projectile *proj = &registry.projectiles.get(currentProjectile);
						proj->flyingTimer = 2000.f;
						break;
					}
					case TAUNT:
					{
						printf("Taunt attack companion\n");
						sk->launchTaunt(attack.target, renderer);
						// basiclly to have something hitting the boundary
						currentProjectile = sk->launchFireball({-20, -20}, {0, 0}, renderer); // TODO: change to new launch dummy projectile without sound
						Motion *projm = &registry.motions.get(currentProjectile);
						projm->velocity = {-100, 0};
						projm->acceleration = {-100, 0};
						break;
					}
					case ROCK:
					{
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, registry.rock_spell_sound, 0);
						currentProjectile = sk->launchRock(attack.target, renderer);
						break;
					}
					case MELEE:
					{
						companion_motion.position = attack.old_pos;
						Motion &healthbar_motion = registry.motions.get(companion.healthbar);
						healthbar_motion.position.x = attack.old_pos.x;
						sk->launchMelee(attack.target, renderer);
						break;
					}
					case ICESHARD:
					{
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, registry.ice_spell_sound, 0);
						currentProjectile = sk->launchIceShard(companion_motion.position, attack.old_pos, renderer);
						break;
					}
					case HEAL:
					{
						Mix_PlayChannel(-1, registry.heal_spell_sound, 0);
						// heal_amount
						sk->launchHeal(attack.target, 75, renderer);
						update_healthBars();
						// basiclly to have something hitting the boundary
						currentProjectile = sk->launchFireball({-20, -20}, {0, 0}, renderer);
						Motion *projm = &registry.motions.get(currentProjectile);
						projm->velocity = {-100, 0};
						projm->acceleration = {-100, 0};
						break;
					}
					case BATTLE_ARROW: {
						sk->launchArrow(attacker, attack.old_pos, renderer, 0);
						break;
					}
					case FREE_ROAM_ARROW: {
						// For free-roam archer arrow-shooting
						sk->launchArrow(player_archer, attack.old_pos, renderer, 1);
						break;
					}
					default:
						break;
					}
					if (attack.attack_type != FREE_ROAM_ARROW) {
						companion.curr_anim_type = IDLE;
					}
					printf("Not attacking anymore in idle\n");
					registry.attackers.remove(attacker);
				}
				else if (registry.enemies.has(attacker))
				{
					printf("Enemy is attacking\n");
					Enemy &enemy = registry.enemies.get(attacker);
					bool create_minion = false;
					switch (attack.attack_type)
					{
					case TAUNT:
					{
						// Mix_PlayChannel(-1, taunt_spell_sound, 0);	// sound moved to skill_system
						printf("taunt attack enemy\n");
						sk->launchTaunt(attack.target, renderer);
						break;
					}
					case SILENCE:
					{
						// Mix_PlayChannel(-1, silence_spell_sound, 0);	// sound moved to skill_system
						printf("taunt attack enemy\n");
						sk->launchSilence(attack.target, renderer);
						break;
					}
					case ROCK:
					{
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, registry.rock_spell_sound, 0);
						printf("Rock attack enemy\n");
						currentProjectile = sk->launchRock(attack.target, renderer);
						// sk->startParticleBeamAttack(attacker);
						break;
					}
					case LIGHTNING:
					{
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, registry.lightning_spell_sound, 0);
						printf("Lightning attack enemy\n");
						currentProjectile = sk->launchLightning(attack.target, renderer);
						break;
					}
					case MELEE:
					{
						printf("melee attack enemy\n");
						Motion &motion = registry.motions.get(attacker);
						motion.position = attack.old_pos;
						Motion &healthbar_motion = registry.motions.get(enemy.healthbar);
						healthbar_motion.position.x = attack.old_pos.x;
						sk->launchMelee(attack.target, renderer);
						break;
					}
					case ICESHARD:
					{
						printf("ice shard attack enemy\n");
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, registry.ice_spell_sound, 0);
						iceShardAttack(attack.target); // TODO
						break;
					}
					case HEAL:
					{
						Mix_PlayChannel(-1, registry.heal_spell_sound, 0);
						printf("heal attack enemy\n");

						int healValue = (gameLevel == 0 && !isMakeupGame) ? 0 : 5;

						sk->launchHeal(attack.target, healValue, renderer);
						update_healthBars();
						break;
					}
					case SUMMONING:
					{
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, registry.summon_spell_sound, 0);
						printf("summon necrominion \n");
						sk->launchSummon(renderer);
						create_minion = true;
						break;
					}
					case ULTI:
					{
						printf("ultimate attack enemy \n");
						sk->launchParticleBeam(attack.target);
						break;
					}
					case CHARGING:
					{
						printf("ultimate charge enemy \n");
						currentProjectile = sk->launchParticleBeamCharge(attack.target, renderer);
						break;
					}
					case BLEEDMELEE:
					{
						Motion &motion = registry.motions.get(attacker);
						motion.position = attack.old_pos;
						Motion &healthbar_motion = registry.motions.get(enemy.healthbar);
						healthbar_motion.position.x = attack.old_pos.x;
						sk->launchMelee(attack.target, renderer);
						sk->launchBleed(attack.target, renderer);
						break;
					}
					case AOEMELEE:
					{
						Motion &motion = registry.motions.get(attacker);
						motion.position = attack.old_pos;
						Motion &healthbar_motion = registry.motions.get(enemy.healthbar);
						healthbar_motion.position.x = attack.old_pos.x;

						for (Entity e : registry.companions.entities)
						{
							sk->launchMelee(e, renderer);
						}
						break;
					}
					case SHIELD:
					{
						sk->launchNecroBarrier(attacker, renderer);
						break;
					}
					default:
						break;
					}
					enemy.curr_anim_type = IDLE;
					printf("Not attacking anymore in idle\n");
					registry.attackers.remove(attacker);
					if (create_minion)
					{
						Motion& necro1_motion = registry.motions.get(attacker);
						necromancer_minion = createNecromancerMinion(renderer, {necro1_motion.position.x-abs(necro1_motion.scale.x), 575});
					}
				}
			}
		}
	}

	if (isFreeRoam && (freeRoamLevel == 2) && (!isMakeupGame)) {
		// Update swarm timer here, use fireflySwarm[0] to track time
		float& swarm_update_timer = registry.fireflySwarm.components[0].update_timer;
		if (swarm_update_timer < 0.f) {
			swarmSys->updateSwarm();
			registry.fireflySwarm.components[0].update_timer = 750.f;
		}
		else {
			swarm_update_timer -= elapsed_ms_since_last_update;
		}

		for (int i = 0; i < NUM_SWARM_PARTICLES; i++) {
			auto& timer = registry.fireflySwarm.components[i].dodge_timer;
			timer -= elapsed_ms_since_last_update;
			if (timer < 0.f) {
				registry.fireflySwarm.components[i].isDodging = 0;
			}
		}
	}

	if(!isFreeRoam){
		if (player_turn == 1)
		{
			displayPlayerTurn();
			prevPlayer = currPlayer;
		}

		// this area is to check for edge cases for enemy to attack during their turn
		if (player_turn == 0)
		{

			if ((registry.checkRoundTimer.size() <= 0) && (registry.companions.size() > 0))
			{
				displayEnemyTurn();
				if (registry.enemies.has(currPlayer))
				{ // check if enemies have currPlayer
					printf("Calling tree here\n");
					ai->callTree(currPlayer);
					prevPlayer = currPlayer; // added this line to progress the necromancer phase 2 turn 2 after the first turn's tree is called, not sure if it will affect other things, need more testing
				}
				else
				{
					if (roundVec.empty())
					{
						printf("roundVec is empty at enemy turn, createRound now \n");
						createRound();
					}
				}
			}
		}
	}

	// Processing the salmon state
	assert(registry.screenStates.components.size() <= 1);
	ScreenState &screen = registry.screenStates.components[0];

	// update state of particles
	for (Entity entity : registry.particlePools.entities)
	{
		ParticlePool &pool = registry.particlePools.get(entity);
		if (pool.areTypeDeath)
		{
			for (int i = 0; i < pool.particles.size(); i++)
			{
				auto &particle = pool.particles[i];
				particle.Life -= elapsed_ms_since_last_update;
				if (particle.Life <= 0)
				{
					pool.fadedParticles++;
				}
				particle.motion.position.x -= particle.motion.velocity.y * (rand() % 17) * 0.3f;
				particle.motion.position.y -= particle.motion.velocity.x * (rand() % 17) * 0.3f;
				particle.Color.a -= 0.05f * 0.01f;
				particle.motion.angle += 0.5;
				if (particle.motion.angle >= (2 * M_PI))
				{
					particle.motion.angle = 0;
				}
				pool.positions[i * 3 + 0] = particle.motion.position.x;
				pool.positions[i * 3 + 1] = particle.motion.position.y;
				pool.positions[i * 3 + 2] = particle.Life / pool.poolLife;
			}
			if (pool.fadedParticles == pool.size)
			{
				delete[] pool.positions;
				pool.faded = true;
				registry.particlePools.remove(entity);
				registry.remove_all_components_of(entity); // added back in, kinda works
			}
		}
		else if (pool.areTypeSmoke) {
			for (int i = 0; i < pool.particles.size(); i++)
			{
				auto& particle = pool.particles[i];
				particle.Life -= elapsed_ms_since_last_update;
				float g = 10.f;
				float initialSpeedX = particle.motion.velocity.x;
				float initialSpeedY = particle.motion.velocity.y;
				float finalSpeed = 100.f;
				float p1 = finalSpeed * initialSpeedY / g;
				float p2 = -g / initialSpeedY;
				float p3 = -finalSpeed * (initialSpeedY + finalSpeed) / g;
				float p5 = finalSpeed;
				if (particle.Life <= 0)
				{
					pool.fadedParticles++;
				}
				particle.motion.position.x = p1 * (1 - exp(p2 * particle.Life));
				particle.motion.position.y = p3 * (1 - exp(p2 * particle.Life)) - p5 * particle.Life;
				particle.Color.a -= 0.05f * 0.01f;
				//particle.motion.angle += 0.5;
				//if (particle.motion.angle >= (2 * M_PI))
				//{
				//	particle.motion.angle = 0;
				//}
				pool.positions[i * 3 + 0] = particle.motion.position.x;
				pool.positions[i * 3 + 1] = particle.motion.position.y;
				pool.positions[i * 3 + 2] = particle.Life / pool.poolLife;
			}
			if (pool.fadedParticles == pool.size)
			{
				delete[] pool.positions;
				pool.faded = true;
				registry.particlePools.remove(entity);
				registry.remove_all_components_of(entity); // added back in, kinda works
			}
		}
		else
		{
			int w, h;
			glfwGetFramebufferSize(window, &w, &h);
			auto maybeUpdateHealth = sk->updateParticleBeam(entity, elapsed_ms_since_last_update, (float)w, (float)h);
			if (maybeUpdateHealth.second == true)
			{
				// printf("life reached below 50%\n");
			}
			if (maybeUpdateHealth.first && !maybeUpdateHealth.second)
			{
				for (Entity entity : registry.companions.entities)
				{
					Statistics &stat = registry.stats.get(entity);
					stat.health -= 0.0001;
					if (stat.health > 0)
					{
						Companion &companion = registry.companions.get(entity);
						Entity healthbar = companion.healthbar;
						Motion &motion = registry.motions.get(healthbar);
						motion.scale = vec2({(HEALTHBAR_WIDTH * (stat.health / 100.f)), HEALTHBAR_HEIGHT});
					}
				}
			}
		}
	}

	if(isFreeRoam && (freeRoamLevel == 1) && (!isMakeupGame)){
		next_boulder_spawn -= elapsed_ms_since_last_update * current_speed;
		if (registry.boulders.components.size() <= MAX_BOULDERS && next_boulder_spawn < 0.f) {
			// Reset timer
			next_boulder_spawn = (BOULDER_DELAY_MS / 2) + uniform_dist(rng) * (BOULDER_DELAY_MS / 2);
			// Create boulder
			Entity entity = createBoulder(renderer, {window_width_px+50, window_height_px - ARCHER_FREEROAM_HEIGHT + 25});
			// Setting random initial position and constant velocity
			Motion& motion = registry.motions.get(entity);
			motion.acceleration = vec2(BOULDER_ACCELERATION, 0.f);
			motion.velocity = vec2(BOULDER_VELOCITY, 0.f);
		}
		// move it to physics
		for(Entity rollable: registry.rollables.entities){
			Motion& motion = registry.motions.get(rollable);
			motion.angle = motion.angle - 0.03;
		}
	}

	float min_counter_ms = 3000.f;
	for (Entity entity : registry.deathTimers.entities)
	{
		// progress timer
		DeathTimer &counter = registry.deathTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		if (counter.counter_ms < min_counter_ms)
		{
			min_counter_ms = counter.counter_ms;
		}

		// restart the game once the death timer expired
		if (counter.counter_ms < 0)
		{
			registry.deathTimers.remove(entity);
			if (registry.companions.has(entity))
			{
				registry.companions.remove(entity);
			}
			else if (registry.enemies.has(entity))
			{
				registry.enemies.remove(entity);
			}
			activate_deathParticles(entity);
			screen.darken_screen_factor = 0;
			// restart_game();
			return true;
		}
	}
	// Handle flying projectile timer
	for (Entity entity : registry.projectiles.entities)
	{
		Projectile& proj = registry.projectiles.get(entity);
		proj.flyingTimer -= elapsed_ms_since_last_update;
	}

	// update timer for enemy to return to its original position after being hit
	float min_counter_ms_2 = 500.f;
	for (Entity entity : registry.hit_timer.entities)
	{
		// progress timer
		HitTimer& hitCounter = registry.hit_timer.get(entity);
		hitCounter.counter_ms -= elapsed_ms_since_last_update;
		if (hitCounter.counter_ms < min_counter_ms_2)
		{
			min_counter_ms_2 = hitCounter.counter_ms;
		}

		if (hitCounter.counter_ms < 0)
		{
			registry.hit_timer.remove(entity);
			// check if entity is enemy or companion
			if (!registry.deathTimers.has(entity))
			{
				if (registry.companions.has(entity))
				{
					registry.motions.get(entity).position.x += hit_position;
				}
				else
				{
					registry.motions.get(entity).position.x -= hit_position;
				}
			}
			return true;
		}
	}

	// update timer for checking round
	float min_counter_ms_3 = 4000.f;
	for (Entity entity : registry.checkRoundTimer.entities)
	{
		// progress timer
		CheckRoundTimer& timerCounter = registry.checkRoundTimer.get(entity);
		timerCounter.counter_ms -= elapsed_ms_since_last_update;
		if (timerCounter.counter_ms < min_counter_ms_3)
		{
			min_counter_ms_3 = timerCounter.counter_ms;
		}

		if (timerCounter.counter_ms < 0)
		{
			registry.checkRoundTimer.remove(entity);
			printf("check round timer finished, checking round now \n");
			checkRound();
			return true;
		}
	}

	// update state of free_roam_bird
	if (registry.motions.has(free_roam_bird)) {
		auto& birdMotion = registry.motions.get(free_roam_bird);
		registry.bird.get(free_roam_bird).birdNextPostionTracker = birdNextPostionTracker;

		if (birdPositionDivisor == 1 || birdPositionDivisor == 2) {
			float offsetX = (spline[birdNextPostionTracker].x - spline[birdNextPostionTracker - 1].x) / 2.f;
			float offsetY = (spline[birdNextPostionTracker].y - spline[birdNextPostionTracker - 1].y) / 2.f;
			birdMotion.position += vec2(offsetX, offsetY);
			birdPositionDivisor++;
			if (birdNextPostionTracker >= 0 && birdNextPostionTracker < spline.size()) {
				if (spline[birdNextPostionTracker].x == 1070 && spline[birdNextPostionTracker].y == 180) {
					printf("birdIdx: %d\n", birdNextPostionTracker);
				}
			}
		}
		else {
			birdNextPostionTracker++;
			if (birdNextPostionTracker == spline.size()) {
				birdNextPostionTracker = 1;
			}

			float offsetX = (spline[birdNextPostionTracker].x - spline[birdNextPostionTracker - 1].x) / 2.f;
			float offsetY = (spline[birdNextPostionTracker].y - spline[birdNextPostionTracker - 1].y) / 2.f;
			birdMotion.position += vec2(offsetX, offsetY);
			birdPositionDivisor = 2;
		}
	}



	return true;
}

// Reset the world state to its initial state
void WorldSystem::restart_game(bool force_restart)
{
	JSONLoader json_loader;
	json_loader.init(renderer);

	if (registry.companions.size() > 0 && (registry.enemies.size() == 0 || (registry.enemies.size() != 0 && registry.enemies.has(free_roam_bird))))
	{
		if (gameLevel < 3)
		{
			//renderer->transitioningToNextLevel = true;
			renderer->gameLevel = gameLevel;
			if(!isFreeRoam){
				gameLevel++;
			}
			if (gameLevel == 1)
			{
				// render the beginning story
				renderBeginningStory();
				story = 8;
			}
			else if (gameLevel == 2)
			{
				// render the beginning story
				renderBeginningStory();
				story = 19;
			}
			else if (gameLevel == 3)
			{
				// render the beginning story
				renderBeginningStory();
				story = 26;
			}
			if (gameLevel > 0)
			{
				tutorial_enabled = 0;
			}
		}
		if(isFreeRoam){
			printf("incrementing free roam level\n");
			freeRoamLevel++;
		}
		if(gameLevel > 1){
			std::cout << "Old free roam " <<isFreeRoam << std::endl;
			isFreeRoam = !isFreeRoam;
			std::cout << "New free roam " <<isFreeRoam << std::endl;
		}
		printf("Updated game level %d\n", gameLevel);
	}
	if((registry.companions.size() <= 0) && (registry.enemies.size() > 0))
	{
		// GO BACK TO START MENU
		startMenuCleanUp();
		return;
	}
	if (gameLevel > MAX_GAME_LEVELS)
	{
		gameLevel = loadedLevel == -1 ? 1 : loadedLevel;
		renderer->gameLevel = gameLevel;
		if (gameLevel == 1)
		{			
			// render the beginning story
			renderBeginningStory();
			story = 8;
		}
		else if (gameLevel == 2)
		{
			// render the beginning story
			renderBeginningStory();
			story = 19;
		}
		else if (gameLevel == 3)
		{
			// render the beginning story
			renderBeginningStory();
			story = 26;
		}
	}
	if (registry.companions.size() == 0)
	{
		gameLevel = loadedLevel == -1 ? 1 : loadedLevel;
		renderer->gameLevel = gameLevel;
		if (gameLevel == 1)
		{
			// render the beginning story
			renderBeginningStory();
			story = 8;
		}
		else if (gameLevel == 2)
		{
			// render the beginning story
			renderBeginningStory();
			story = 19;
		}
		else if (gameLevel == 3)
		{
			// render the beginning story
			renderBeginningStory();
			story = 26;
		}
		// renderer->transitioningToNextLevel = true;
	}
	if (force_restart)
	{
		gameLevel = loadedLevel == -1 ? 1 : loadedLevel;
		renderer->gameLevel = gameLevel;
		if (gameLevel == 1)
		{
			// render the beginning story
			renderBeginningStory();
			story = 8;
		}
		else if (gameLevel == 2)
		{
			// render the beginning story
			renderBeginningStory();
			story = 19;
		}
		else if (gameLevel == 3)
		{
			// render the beginning story
			renderBeginningStory();
			story = 26;
		}
	}

	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Reset the game speed
	current_speed = 1.f;

	// Remove all entities that we created
	// All that have a motion, we could also iterate over all fish, turtles, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0)
		registry.remove_all_components_of(registry.motions.entities.back());
	while (registry.renderRequests.entities.size() > 0)
		registry.remove_all_components_of(registry.renderRequests.entities.back());

	// Debugging for memory/component leaks
	registry.list_all_components();

	int w, h;
	glfwGetWindowSize(window, &w, &h);
	// Render background before all else

	if (isFreeRoam)
	{
		if (freeRoamLevel == 1) {
			createBackground(renderer, { w / 2, h / 2 }, FREE_ROAM_ONE);
			initializeFreeRoamOne();
			renderBeginningStory();
		}

		else if (freeRoamLevel == 2) {
			createBackground(renderer, { w / 2, h / 2 }, FREE_ROAM_TWO);
			initializeFreeRoamTwo();
			renderBeginningStory();
		}

		open_menu_button = createUIButton(renderer, { 100, 100 }, OPEN_MENU);
		Motion openMenuMotion = registry.motions.get(open_menu_button);
		createFreeRoamLevelTutorialIndicator(renderer, vec2(openMenuMotion.position.x + openMenuMotion.scale.x + 50, openMenuMotion.position.y));
		
		player_archer = createPlayerArcher(renderer, { 700, window_height_px - ARCHER_FREEROAM_HEIGHT + 25 }, 1);
		printf("Loading free roam %d\n", freeRoamLevel);
		renderer->gameLevel = 1;
	}
	else
	{
		// Pause menu button
		bool hasSaveFile = false;
		if (loaded_game)
		{
			hasSaveFile = json_loader.get_save_file();
			if (hasSaveFile)
			{
				gameLevel = loadedLevel;
				renderer->gameLevel = gameLevel == 2 ? 2 : 1;
			}
			else
			{
				loadedLevel = 1;
				gameLevel = loadedLevel;
				renderer->gameLevel = gameLevel;
			}
			loaded_game = false;
			printf("Loaded game is now false\n");
		}
		if (!hasSaveFile)
		{
			printf("Loading a file\n");
			if (gameLevel == 0)
			{
			printf("Loading level 0\n");
			balanceHealthNumbers(0);
			renderer->gameLevel = 1;

			createBackground(renderer, { w / 2, h / 2 }, TUTORIAL);
			json_loader.get_level("level_0.json");

			registry.HPBuff = 0;
			registry.HPDebuff = 0;
			tutorial_enabled = 1;
		}
		else if(gameLevel == 1){
			printf("Loading level 1\n");
			balanceHealthNumbers(1);
			renderer->gameLevel = gameLevel;
			createBackground(renderer, { w / 2, h / 2 }, LEVEL_ONE);
			json_loader.get_level("level_1.json");

			// render the beginning story
			renderBeginningStory();

			story = 8;
		} else if(gameLevel == 2){
			printf("Loading level 2\n");
			balanceHealthNumbers(2);
			renderer->gameLevel = gameLevel;
			createBackground(renderer, { w / 2, h / 2 }, LEVEL_TWO);
			json_loader.get_level("level_2.json");

			// render the beginning story
			renderBeginningStory();

			story = 19;
		} else if(gameLevel == 3){
			printf("Loading level 3 phase 1\n");
			balanceHealthNumbers(3);
			renderer->gameLevel = 1;
			createBackground(renderer, { w / 2, h / 2 }, LEVEL_THREE);
			json_loader.get_level("level_3.json");

			// render the beginning story
			renderBeginningStory();

			story = 26;
		} else{
			printf("Incorrect level\n");
		}
		roundVec.clear();	// empty vector roundVec to create a new round
		createRound();
		checkRound();
	} else {
		loaded_game = false;
	}
		open_menu_button = createUIButton(renderer, {100, 100}, OPEN_MENU);
		// Create a tooltip
		tooltip;
		player_turn = 1;		   // player turn indicator
		gestureSkillRemaining = 1; // reset gesture skill remaining
		showCorrectSkills();
		displayPlayerTurn(); // display player turn when restart game
		update_healthBars();

		if (tutorial_enabled) {
			curr_tutorial_box = createTutorialBox(renderer, { 600, 300 });
			curr_tutorial_box_num = 0;
		}
	}

	if (!isFreeRoam) {
		switch (gameLevel) {
		case 0: Mix_FadeInMusic(registry.background_music, -1, 5000); break;
		case 1: Mix_FadeInMusic(registry.background_music, -1, 5000); break;
		case 2: Mix_FadeInMusic(registry.background_music, -1, 5000); break;
		case 3: Mix_FadeInMusic(registry.boss_music, -1, 5000); break;
		default: break;
		}
	}

	printf("done with restarting\n");
}

void WorldSystem::update_health(Entity entity, Entity other_entity)
{
	if (registry.projectiles.has(entity))
	{
		Damage &damage = registry.damages.get(entity);
		Statistics *hp = nullptr;
		Entity healthbar;
		if (damage.isFriendly)
		{
			if (registry.enemies.has(other_entity))
			{
				Enemy &enemy = registry.enemies.get(other_entity);
				healthbar = enemy.healthbar;
				hp = &registry.stats.get(other_entity);
			}
		}
		else
		{
			if (registry.companions.has(other_entity))
			{
				Companion &companion = registry.companions.get(other_entity);
				healthbar = companion.healthbar;
				hp = &registry.stats.get(other_entity);
			}
		}
		if (hp)
		{
			//hp->health = hp->health - (rand() % damage.range + damage.minDamage);
			// No randomness in damage
			hp->health = hp->health - damage.minDamage;
			Motion &motion = registry.motions.get(healthbar);
			if (registry.stats.get(currPlayer).health <= 0)
			{ // check if HP of currPlayer is 0, checkRound to skip this player
				if (!registry.deathTimers.has(other_entity))
				{
					auto &deathTimer = registry.deathTimers.emplace(other_entity);

					prevPlayer = currPlayer; // needed to allow checking of the edge case where the enemy dies and the next enemy goes

					if (!registry.checkRoundTimer.has(currPlayer))
					{
						auto &timer = registry.checkRoundTimer.emplace(currPlayer);
						timer.counter_ms = deathTimer.counter_ms + animation_timer;
					}
					if (registry.companions.has(other_entity))
					{
						printf("Companion is dead\n");
						Companion &companion = registry.companions.get(other_entity);
						companion.curr_anim_type = DEAD;
					}
					else if (registry.enemies.has(other_entity))
					{
						printf("Enemy is dead\n");
						Enemy &enemy = registry.enemies.get(other_entity);
						enemy.curr_anim_type = DEAD;
					}
				}
				motion.scale = vec2({(HEALTHBAR_WIDTH * (0.f / 100.f)), HEALTHBAR_HEIGHT});
			}
			else
			{
				if (hp->health <= 0)
				{
					if (!registry.deathTimers.has(other_entity))
					{
						auto &deathTimer = registry.deathTimers.emplace(other_entity);

						prevPlayer = currPlayer; // needed to allow checking of the edge case where the enemy dies and the next enemy goes

						if (!registry.checkRoundTimer.has(currPlayer))
						{
							auto &timer = registry.checkRoundTimer.emplace(currPlayer);
							timer.counter_ms = deathTimer.counter_ms + animation_timer;
						}
						if (registry.companions.has(other_entity))
						{
							printf("Companion is dead\n");
							Companion &companion = registry.companions.get(other_entity);
							companion.curr_anim_type = DEAD;
						}
						else if (registry.enemies.has(other_entity))
						{
							printf("Enemy is dead\n");
							Enemy &enemy = registry.enemies.get(other_entity);
							enemy.curr_anim_type = DEAD;
						}
					}
					motion.scale = vec2({(HEALTHBAR_WIDTH * (0.f / 100.f)), HEALTHBAR_HEIGHT});
				}
				else
				{
					motion.scale = vec2({(HEALTHBAR_WIDTH * (hp->health / 100.f)), HEALTHBAR_HEIGHT});
				}
			}
		}
	}
}

void WorldSystem::update_healthBars()
{
	for (Entity entity : registry.enemies.entities)
	{
		if (!registry.enemies.has(entity) || !registry.stats.has(entity)) continue;

		Enemy &enemy = registry.enemies.get(entity);
		Statistics &stat = registry.stats.get(entity);
		Entity healthbar = enemy.healthbar;

		if (!registry.motions.has(healthbar)) continue;

		Motion &motion = registry.motions.get(healthbar);
		motion.scale = vec2({(HEALTHBAR_WIDTH * (stat.health / 100.f)), HEALTHBAR_HEIGHT});
	}
	for (Entity entity : registry.companions.entities)
	{
		if (!registry.companions.has(entity) || !registry.stats.has(entity)) continue;

		Companion &enemy = registry.companions.get(entity);
		Statistics &stat = registry.stats.get(entity);
		Entity healthbar = enemy.healthbar;

		if (!registry.motions.has(healthbar)) continue;

		Motion &motion = registry.motions.get(healthbar);
		motion.scale = vec2({(HEALTHBAR_WIDTH * (stat.health / 100.f)), HEALTHBAR_HEIGHT});
	}
}

void WorldSystem::activate_deathParticles(Entity entity)
{
	ParticlePool pool(1000);
	pool.areTypeDeath = true;
	// pool.size = 1000;
	initParticlesBuffer(pool);
	pool.poolLife = 2000.f;
	pool.motion.scale = vec2(10.f, 10.f);

	for (int p = 0; p < pool.size; p++)
	{
		auto &motion = registry.motions.get(entity);
		Particle particle;
		float random1 = ((rand() % 100) - 50) / 10.0f;
		float random2 = ((rand() % 200) - 100) / 10.0f;
		float rColor = 0.5f + ((rand() % 100) / 100.0f);
		// particle.motion.position = motion.position + random + vec2({ 20,20 });
		particle.motion.position.x = motion.position.x + random1 + 20.f;
		particle.motion.position.y = motion.position.y + random2 + 40.f;
		particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
		particle.motion.velocity *= 0.1f;
		particle.motion.scale = vec2({10, 10});
		pool.particles.push_back(particle);
		pool.positions[p * 3 + 0] = particle.motion.position.x;
		pool.positions[p * 3 + 1] = particle.motion.position.y;
		pool.positions[p * 3 + 2] = particle.Life / pool.poolLife;
	}
	if (!registry.particlePools.has(entity))
	{
		registry.particlePools.insert(entity, pool);
	}
}
void WorldSystem::activate_smokeParticles(Entity entity)
{
	ParticlePool pool(1000);
	pool.areTypeSmoke = true;
	// pool.size = 1000;
	initParticlesBuffer(pool);
	pool.poolLife = 2000.f;
	pool.motion.scale = vec2(3.f, 3.f);

	for (int p = 0; p < pool.size; p++)
	{
		auto& motion = registry.motions.get(entity);
		Particle particle;
		float random1 = ((rand() % 100) - 50) / 10.0f;
		float random2 = ((rand() % 200) - 100) / 10.0f;

		float rColor = 0.5f + ((rand() % 100) / 100.0f);
		// particle.motion.position = motion.position + random + vec2({ 20,20 });
		particle.motion.position.x = motion.position.x + random1 + 20.f;
		particle.motion.position.y = motion.position.y + random2 + 40.f;
		particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
		particle.motion.velocity *= 0.1f;
		particle.motion.scale = vec2({ 10, 10 });
		pool.particles.push_back(particle);
		pool.positions[p * 3 + 0] = particle.motion.position.x;
		pool.positions[p * 3 + 1] = particle.motion.position.y;
		pool.positions[p * 3 + 2] = particle.Life / pool.poolLife;
	}
	if (!registry.particlePools.has(entity))
	{
		registry.particlePools.insert(entity, pool);
	}
}
// Compute collisions between entities
void WorldSystem::handle_collisions()
{
	// reduce turn
	// Loop over all collisions detected by the physics system


	// Reset the ceiling and floor (For free-roam archer physics)
	currCeilingPos = 0.f;
	currFloorPos = window_height_px - ARCHER_FREEROAM_HEIGHT + 25;

	auto &collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++)
	{
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		// deal with collisions in free roam
		if (isFreeRoam) {

			
			if (registry.companions.has(entity))
			{
				// Deal with archer - platform collisions
				if (registry.platform.has(entity_other))
				{
					Motion& platform_motion = registry.motions.get(entity_other);
					float platform_position_x = platform_motion.position.x;
					float platform_position_y = platform_motion.position.y;
					float platform_width = platform_motion.scale.x;
					float platform_height = platform_motion.scale.y;

					Motion& archer_motion = registry.motions.get(player_archer);
					float archer_pos_x = archer_motion.position.x;
					float archer_pos_y = archer_motion.position.y;

					int onTopOrBelow = 0;

					// Platform is on top of archer
					if (archer_motion.position.y > platform_position_y + platform_height / 2) {
						currCeilingPos = platform_position_y + platform_height;
						onTopOrBelow = 1;
					}
					// Archer is on top of a platform
					if (archer_motion.position.y < platform_position_y - platform_height / 2) {
						currFloorPos = platform_position_y - platform_height;
						onTopOrBelow = 1;
					}

					// Bounce back archer to the left, if not on top or directly below
					if (archer_pos_x > platform_position_x - platform_width && archer_pos_x < platform_position_x + 25 - ARCHER_FREEROAM_WIDTH
						&& !onTopOrBelow) {
						archer_motion.position.x -= 2;
					}
					// Bounce back archer to the right, if not on top or directly below
					if (archer_pos_x < platform_position_x + platform_width && archer_pos_x > platform_position_x - 25 + ARCHER_FREEROAM_WIDTH
						&& !onTopOrBelow) {
						archer_motion.position.x += 2;
					}
				}
				// Deal with archer - treasure chest collisions
				else if (registry.chests.has(entity_other)) {;
					TreasureChest chest = registry.chests.get(entity_other);
					RenderRequest& renderedChest = registry.renderRequests.get(entity_other);
					Motion chestMotion = registry.motions.get(entity_other);
					if (renderedChest.used_geometry != GEOMETRY_BUFFER_ID::TREASURE_CHEST_OPEN && chest.chestType == HEALTH_BOOST) {
						// Boost all companion HP permanently
						registry.HPBuff++;
						Mix_PlayChannel(-1, registry.heal_spell_sound, 0);
						createBoostMessage(renderer, chestMotion.position, HEALTH_BOOST);
					}
					else if (renderedChest.used_geometry != GEOMETRY_BUFFER_ID::TREASURE_CHEST_OPEN && chest.chestType == DAMAGE_BOOST) {
						// Boost all ability damages permanently
						registry.fireball_dmg *= 1.25;
						registry.arrow_dmg *= 1.25;
						Mix_PlayChannel(-1, registry.heal_spell_sound, 0);
						createBoostMessage(renderer, chestMotion.position, DAMAGE_BOOST);
					}
					// Switch to open chest image
					renderedChest.used_geometry = GEOMETRY_BUFFER_ID::TREASURE_CHEST_OPEN;
				} 
				// Rock archer collision
				else if (registry.boulders.has(entity_other) && !registry.particlePools.has(entity_other))
				{
					Motion& rollable_motion = registry.motions.get(entity_other);
					rollable_motion.velocity = { BOULDER_VELOCITY / 3, 0.f};

					float rock_pos_x = rollable_motion.position.x;
					float rock_pos_y = rollable_motion.position.y;
					float rock_width = rollable_motion.scale.x;
					float rock_height = abs(rollable_motion.scale.y);

					Motion& archer_motion = registry.motions.get(player_archer);
					float archer_pos_x = archer_motion.position.x;
					float archer_pos_y = archer_motion.position.y;

					// On top of rock
					if (archer_pos_y > rock_pos_y - rock_height
						&& !(archer_pos_x <= rock_pos_x - rock_width + 10)
						&& !(archer_pos_x >= rock_pos_x + rock_width - 25)) {
						currFloorPos = rock_pos_y - rock_height;
						registry.companions.get(player_archer).curr_anim_type = IDLE;
					}
					// Bounce archer to the left
					else if (archer_pos_x > rock_pos_x - rock_width && archer_pos_x < rock_pos_x
						&& !(archer_pos_y <= rock_pos_y - rock_height)) {
						archer_motion.position.x = rock_pos_x - rock_width;
					}
					// Bounce archer to the right
					else if (archer_pos_x < rock_pos_x + rock_width - 15 && archer_pos_x > rock_pos_x
						&& !(archer_pos_y <= rock_pos_y - rock_height)) {
						archer_motion.position.x = rock_pos_x + rock_width - 15;
					}
				}
			}

			if (registry.boulders.has(entity) && registry.boulders.has(entity_other)) {
				Motion& entity_motion = registry.motions.get(entity);
				Motion& entity_other_motion = registry.motions.get(entity_other);

				if (entity_motion.position.x < entity_other_motion.position.x) {
					entity_motion.velocity = vec2(BOULDER_VELOCITY * 1.5, 0.f);
					entity_other_motion.velocity = vec2(BOULDER_VELOCITY / 1.5, 0.f);
				}
				else {
					entity_motion.velocity = vec2(BOULDER_VELOCITY / 1.5, 0.f);
					entity_other_motion.velocity = vec2(BOULDER_VELOCITY * 1.5, 0.f);
				}
				
			}

			// Deal with arrow - bird collisions
			if (registry.projectiles.has(entity))
			{
				// Checking bird
				if (registry.bird.has(entity_other))
				{
					registry.remove_all_components_of(entity_other);
					registry.remove_all_components_of(entity);
					Mix_PlayChannel(-1, registry.crow_sound, 0);
				}
				// Arrow rock collision
				else if (registry.boulders.has(entity_other) && !registry.particlePools.has(entity_other)) {
					Mix_PlayChannel(-1, registry.fireball_explosion_sound, 0);
					activate_deathParticles(entity_other);
					registry.remove_all_components_of(entity);
				}
			}
		}		
		
		// deal with collisions in battles
		else {
			//printf("Not free world\n");
			// Deal with arrow - bird collisions
			if (registry.projectiles.has(entity))	// not working in free roam
			{
				// Checking bird
				if (registry.bird.has(entity_other))
				{
					registry.remove_all_components_of(entity_other);
					Mix_PlayChannel(-1, registry.crow_sound, 0);
				}
			}
			// Deal with fireball - Companion collisions
			if (registry.companions.has(entity))
			{
				// Checking Projectile - Companion collisions
				if (registry.projectiles.has(entity_other))
				{

					Damage& projDamage = registry.damages.get(entity_other);
					if (projDamage.isFriendly == 0)
					{ // check if isFriendly = 0 which hits companion
						// initiate death unless already dying
						if (!registry.deathTimers.has(entity))
						{
							if (!registry.buttons.has(entity))
							{
								update_health(entity_other, entity);
								registry.remove_all_components_of(entity_other);
								Mix_PlayChannel(-1, registry.fireball_explosion_sound, 0); // added fireball hit sound
								showCorrectSkills();
								if (registry.stats.has(entity) && registry.stats.get(entity).health <= 0)
								{
									Mix_PlayChannel(-1, registry.death_enemy_sound, 0); // added enemy death sound
								}
								else
								{
									Mix_PlayChannel(-1, registry.hit_enemy_sound, 0); // new enemy hit sound
								}
								// update only if hit_timer for entity does not already exist
								if (!registry.hit_timer.has(entity))
								{
									registry.motions.get(entity).position.x -= 20; // character shifts backwards
									registry.hit_timer.emplace(entity);			   // to move character back to original position
								}
								// displayPlayerTurn();	// displays player turn when enemy hits collide
							}
						}
					}
				}
				if (registry.stats.has(entity) && registry.stats.get(entity).health <= 0 && !registry.deathTimers.has(entity))
				{
					// get rid of dead entity's healthbar.
					Entity entityHealthbar = registry.companions.get(entity).healthbar;
					registry.motions.remove(entityHealthbar);
					registry.deathTimers.emplace(entity);
					if (registry.companions.has(entity))
					{
						printf("Companion is dead\n");
						Companion& companion = registry.companions.get(entity);
						companion.curr_anim_type = DEAD;
					}
					else if (registry.enemies.has(entity))
					{
						printf("Enemy is dead\n");
						Enemy& enemy = registry.enemies.get(entity);
						enemy.curr_anim_type = DEAD;
					}
				}
			}
			// Deal with fireball - Enemy collisions
			else if (registry.enemies.has(entity))
			{
				// Checking Projectile - Enemy collisions
				if (registry.projectiles.has(entity_other))
				{
					if (registry.FireBalls.has(entity_other)) {
						activate_smokeParticles(entity_other);
					}
					Damage& projDamage = registry.damages.get(entity_other);
					if (projDamage.isFriendly == 1)
					{ // check if isFriendly = 1 which hits enemy
						// initiate death unless already dying
						if (!registry.deathTimers.has(entity))
						{
							if (!registry.buttons.has(entity))
							{
								update_health(entity_other, entity);
								registry.remove_all_components_of(entity_other);
								Mix_PlayChannel(-1, registry.fireball_explosion_sound, 0); // added fireball hit sound
								if (registry.stats.has(entity) && registry.stats.get(entity).health <= 0)
								{
									// get rid of dead entity's stats indicators
									sk->removeTaunt(entity);
									sk->removeSilence(entity);
									sk->removeBleed(entity);
									Mix_PlayChannel(-1, registry.death_enemy_sound, 0); // added enemy death sound
								}
								else
								{
									Mix_PlayChannel(-1, registry.hit_enemy_sound, 0); // new enemy hit sound
								}
								// update only if hit_timer for entity does not already exist
								if (!registry.hit_timer.has(entity))
								{
									registry.motions.get(entity).position.x += 20; // character shifts backwards
									registry.hit_timer.emplace(entity);			   // to move character back to original position
								}
								// enemy turn start
							}
						}
					}
				}
				if (registry.stats.has(entity) && registry.stats.get(entity).health <= 0 && !registry.deathTimers.has(entity))
				{
					// get rid of dead entity's healthbar.
					Entity entityHealthbar = registry.enemies.get(entity).healthbar;
					registry.motions.remove(entityHealthbar);
					registry.deathTimers.emplace(entity);
					if (registry.companions.has(entity))
					{
						printf("Companion is dead\n");
						Companion& companion = registry.companions.get(entity);
						companion.curr_anim_type = DEAD;
					}
					else if (registry.enemies.has(entity))
					{
						printf("Enemy is dead\n");
						Enemy& enemy = registry.enemies.get(entity);
						enemy.curr_anim_type = DEAD;
					}
				}
			}
			// handle collisions with background objects
			if (registry.deformableEntities.has(entity) && registry.projectiles.has(entity_other) && !registry.reflects.has(entity))
			{
				auto& backgroundObj = registry.deformableEntities.get(entity);
				backgroundObj.shouldDeform = true;
				Mix_PlayChannel(-1, registry.fireball_explosion_sound, 0);
				registry.remove_all_components_of(entity_other);
				// enemy turn start
				if (player_turn == 0)
				{
					if (!registry.checkRoundTimer.has(currPlayer))
					{
						displayEnemyTurn();
						if (registry.enemies.has(currPlayer))
						{ // check if enemies have currPlayer
							ai->callTree(currPlayer);
						}
						else
						{
							if (roundVec.empty())
							{
								printf("roundVec is empty at enemy turn, createRound now \n");
								createRound();
							}
						}
					}
				}
			}
			// barrier collection
			else if (registry.projectiles.has(entity))
			{
				if (registry.reflects.has(entity_other))
				{
					// printf("colleds\n");
					// printf("%f\n", registry.motions.get(entity).velocity.x);
					if (registry.motions.get(entity).velocity.x > 0.f)
					{
						// printf("colleds1");
						auto& shieldMesh = registry.deformableEntities.get(entity_other);
						shieldMesh.shouldDeform = true;
						shieldMesh.deformType2 = true;

						Motion* reflectEM = &registry.motions.get(entity);

						reflectEM->velocity = vec2(-registry.motions.get(entity).velocity.x, reflectEM->velocity.y);
						reflectEM->acceleration = vec2(-registry.motions.get(entity).acceleration.x, reflectEM->acceleration.y);
						float reflectE = atan(registry.motions.get(entity).velocity.y / registry.motions.get(entity).velocity.x);
						if (registry.motions.get(entity).velocity.x < 0)
						{
							reflectE += M_PI;
						}
						reflectEM->angle = reflectE;
					}
				}
			}
		}		
	}

	if(isFreeRoam){

		Motion& archerMotion = registry.motions.get(player_archer);

		// Check if archer is above ceiling
		if (archerMotion.position.y < currCeilingPos) {
			archerMotion.position.y = currCeilingPos;
			archerMotion.velocity.y = 400.f;
			registry.companions.get(player_archer).curr_anim_type = JUMPING;
		}

		// Reset archer y-pos based on the current floor position, so archer doesn't fall through platform/ground
		if (archerMotion.position.y > currFloorPos) {
			archerMotion.velocity.y = 0.f;
			archerMotion.acceleration.y = 0.f;
			archerMotion.position.y = currFloorPos;

			if (registry.companions.get(player_archer).curr_anim_type == JUMPING) {
				if (archerMotion.velocity.x != 0) {
					registry.companions.get(player_archer).curr_anim_type = WALKING;
				}
				else registry.companions.get(player_archer).curr_anim_type = IDLE;
			}
		}
		// If archer is not on currFloorPos and falling from above (No jump)
		else if (archerMotion.position.y < currFloorPos && registry.companions.get(player_archer).curr_anim_type != JUMPING) {
			archerMotion.velocity.y = 400.f;
			registry.companions.get(player_archer).curr_anim_type = JUMPING;
		}
	}

	// Remove all collisions from this simulation step
	registry.collisions.clear();
}

void WorldSystem::handle_boundary_collision()
{
	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	auto &projectilesRegistry = registry.projectiles;
	for (uint i = 0; i < projectilesRegistry.components.size(); i++)
	{
		Entity entity = projectilesRegistry.entities[i];
		if (registry.motions.get(entity).position.x <= 0 - registry.motions.get(entity).scale.x - 20 ||
			registry.motions.get(entity).position.x >= screen_width + registry.motions.get(entity).scale.x + 20 ||
			registry.motions.get(entity).position.y <= 0 - registry.motions.get(entity).scale.y- 20 ||
			registry.motions.get(entity).position.y >= screen_height + registry.motions.get(entity).scale.y + 20)
		{
			if(registry.bouncingArrows.has(entity)){
				BouncingArrow& baM = registry.bouncingArrows.get(registry.bouncingArrows.entities[i]);
				if(baM.bounce_time>0){
					continue;
				} 
			}
			registry.remove_all_components_of(entity);
			registry.remove_all_components_of(entity);
			Mix_PlayChannel(-1, registry.fireball_explosion_sound, 0);
			// enemy turn start
		}
	}
	if(isFreeRoam){

		Motion& archerMotion = registry.motions.get(player_archer);

		// Check for left & right boundaries
		if (archerMotion.position.x < ARCHER_FREEROAM_WIDTH / 2) {
			archerMotion.position.x = ARCHER_FREEROAM_WIDTH / 2;
		}
		else if (archerMotion.position.x > window_width_px - ARCHER_FREEROAM_WIDTH) {
			// restart_game(false);
			renderDragonSpeech();
		}
	}
}

// Should the game be over ?
bool WorldSystem::is_over() const
{
	return bool(glfwWindowShouldClose(window)) || closeWindow;
}

// On key callback
void WorldSystem::on_key(int key, int, int action, int mod)
{
	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R)
	{
		int w, h;
		glfwGetWindowSize(window, &w, &h);

		restart_game(true);
	}

	if (isFreeRoam && !registry.renderRequests.has(dialogue)) {

		if (key == GLFW_KEY_H) {
			if (action == GLFW_RELEASE) {
				if (registry.renderRequests.has(free_roam_tutorial)) {
					registry.remove_all_components_of(free_roam_tutorial);
				}
				else {
					free_roam_tutorial = createFreeRoamLevelTutorial(renderer, vec2(window_width_px / 2, window_height_px / 2));
				}
			}
		}
	}


	if (isFreeRoam && beginning == 0 && dragon == 0) {
		// Move right
		if (key == GLFW_KEY_D) {
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
				Motion& motion = registry.motions.get(player_archer);
				motion.velocity.x = 400.f;
				// Turn archer right
				if (registry.companions.get(player_archer).curr_anim_type != ATTACKING
					&& registry.companions.get(player_archer).curr_anim_type != WALK_ATTACKING) {
					motion.scale.x = abs(motion.scale.x);
				}
				if (registry.companions.get(player_archer).curr_anim_type != JUMPING
					&& registry.companions.get(player_archer).curr_anim_type != ATTACKING
					&& registry.companions.get(player_archer).curr_anim_type != WALK_ATTACKING) {
					registry.companions.get(player_archer).curr_anim_type = WALKING;
				}
			}
			if (action == GLFW_RELEASE) {
				Motion& motion = registry.motions.get(player_archer);
				if (motion.velocity.x != -400.f) {
					motion.velocity.x = 0;
					if (registry.companions.get(player_archer).curr_anim_type != JUMPING
						&& registry.companions.get(player_archer).curr_anim_type != ATTACKING
						&& registry.companions.get(player_archer).curr_anim_type != WALK_ATTACKING) {
						registry.companions.get(player_archer).curr_anim_type = IDLE;
					}
				}
			}
		}
		// Move left
		if (key == GLFW_KEY_A) {
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
				Motion& motion = registry.motions.get(player_archer);
				motion.velocity.x = -400.f;
				// Turn archer left
				if (registry.companions.get(player_archer).curr_anim_type != ATTACKING
					&& registry.companions.get(player_archer).curr_anim_type != WALK_ATTACKING) {
					motion.scale.x = -abs(motion.scale.x);
				}
				if (registry.companions.get(player_archer).curr_anim_type != JUMPING
					&& registry.companions.get(player_archer).curr_anim_type != ATTACKING
					&& registry.companions.get(player_archer).curr_anim_type != WALK_ATTACKING) {
					registry.companions.get(player_archer).curr_anim_type = WALKING;
				}	
			}
			if (action == GLFW_RELEASE) {
				Motion& motion = registry.motions.get(player_archer);
				if (motion.velocity.x != 400.f) {
					motion.velocity.x = 0;
					if (registry.companions.get(player_archer).curr_anim_type != JUMPING
						&& registry.companions.get(player_archer).curr_anim_type != ATTACKING
						&& registry.companions.get(player_archer).curr_anim_type != WALK_ATTACKING) {
						registry.companions.get(player_archer).curr_anim_type = IDLE;
					}
				}
			}
		}
		// Jump up
		if (key == GLFW_KEY_W && registry.companions.get(player_archer).curr_anim_type != JUMPING
			&& registry.companions.get(player_archer).curr_anim_type != ATTACKING
			&& registry.companions.get(player_archer).curr_anim_type != WALK_ATTACKING) {
			if (action == GLFW_RELEASE) {
				Motion& motion = registry.motions.get(player_archer);
				motion.velocity.y = -(registry.verticalResolution / 2.25);
				registry.companions.get(player_archer).curr_anim_type = JUMPING;
			}
		}
	}


	// Debugging
	if (key == GLFW_KEY_B)
	{
		if (action == GLFW_RELEASE)
			debugging.in_debug_mode = false;
		else
			debugging.in_debug_mode = true;
	}

	// Volume control (z = Decrease BGM vol., x = Increase BGM vol., c = Decrease effects vol., v = Increase effects vol.)
	if (action == GLFW_RELEASE && key == GLFW_KEY_Z)
	{
		Mix_VolumeMusic(Mix_VolumeMusic(-1) - MIX_MAX_VOLUME / 30);
	}
	if (action == GLFW_RELEASE && key == GLFW_KEY_X)
	{
		Mix_VolumeMusic(Mix_VolumeMusic(-1) + MIX_MAX_VOLUME / 30);
	}
	if (action == GLFW_RELEASE && key == GLFW_KEY_C)
	{
		Mix_VolumeChunk(registry.hit_enemy_sound, Mix_VolumeChunk(registry.hit_enemy_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.fireball_explosion_sound, Mix_VolumeChunk(registry.fireball_explosion_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.death_enemy_sound, Mix_VolumeChunk(registry.death_enemy_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.fire_spell_sound, Mix_VolumeChunk(registry.fire_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.rock_spell_sound, Mix_VolumeChunk(registry.rock_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.heal_spell_sound, Mix_VolumeChunk(registry.heal_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.taunt_spell_sound, Mix_VolumeChunk(registry.taunt_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.melee_spell_sound, Mix_VolumeChunk(registry.melee_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.silence_spell_sound, Mix_VolumeChunk(registry.silence_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.lightning_spell_sound, Mix_VolumeChunk(registry.lightning_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.ice_spell_sound, Mix_VolumeChunk(registry.ice_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.summon_spell_sound, Mix_VolumeChunk(registry.summon_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.button_hover_sound, Mix_VolumeChunk(registry.button_hover_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.charge_spell_sound, Mix_VolumeChunk(registry.charge_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.beam_spell_sound, Mix_VolumeChunk(registry.beam_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.minion_spawn_sound, Mix_VolumeChunk(registry.minion_spawn_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.error_sound, Mix_VolumeChunk(registry.error_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.gesture_heal_sound, Mix_VolumeChunk(registry.gesture_heal_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.gesture_aoe_sound, Mix_VolumeChunk(registry.gesture_aoe_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.gesture_turn_sound, Mix_VolumeChunk(registry.gesture_turn_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.crow_sound, Mix_VolumeChunk(registry.crow_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.turning_sound, Mix_VolumeChunk(registry.turning_sound, -1) - MIX_MAX_VOLUME / 10);
	}
	if (action == GLFW_RELEASE && key == GLFW_KEY_V)
	{
		Mix_VolumeChunk(registry.hit_enemy_sound, Mix_VolumeChunk(registry.hit_enemy_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.fireball_explosion_sound, Mix_VolumeChunk(registry.fireball_explosion_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.death_enemy_sound, Mix_VolumeChunk(registry.death_enemy_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.fire_spell_sound, Mix_VolumeChunk(registry.fire_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.rock_spell_sound, Mix_VolumeChunk(registry.rock_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.heal_spell_sound, Mix_VolumeChunk(registry.heal_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.taunt_spell_sound, Mix_VolumeChunk(registry.taunt_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.melee_spell_sound, Mix_VolumeChunk(registry.melee_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.silence_spell_sound, Mix_VolumeChunk(registry.silence_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.lightning_spell_sound, Mix_VolumeChunk(registry.lightning_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.ice_spell_sound, Mix_VolumeChunk(registry.ice_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.summon_spell_sound, Mix_VolumeChunk(registry.summon_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.button_hover_sound, Mix_VolumeChunk(registry.button_hover_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.charge_spell_sound, Mix_VolumeChunk(registry.charge_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.beam_spell_sound, Mix_VolumeChunk(registry.beam_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.minion_spawn_sound, Mix_VolumeChunk(registry.minion_spawn_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.error_sound, Mix_VolumeChunk(registry.error_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.gesture_heal_sound, Mix_VolumeChunk(registry.gesture_heal_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.gesture_aoe_sound, Mix_VolumeChunk(registry.gesture_aoe_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.gesture_turn_sound, Mix_VolumeChunk(registry.gesture_turn_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.crow_sound, Mix_VolumeChunk(registry.crow_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(registry.turning_sound, Mix_VolumeChunk(registry.turning_sound, -1) + MIX_MAX_VOLUME / 10);
	}
}

void WorldSystem::createIcons(){
	fireBall_icon = createFireballIcon(renderer, {200, 700});
	taunt_icon = createTauntIcon(renderer, {300, 700});
	heal_icon = createHealIcon(renderer, {400, 700});
	melee_icon = createMeleeIcon(renderer, {500, 700});
    iceShard_icon = createIceShardIcon(renderer, {600, 700});
	rock_icon = createRockIcon(renderer, {700, 700});
	arrow_icon = createArrowIcon(renderer, {800, 700});
}

void WorldSystem::on_mouse_button(int button, int action, int mods)
{
	// For start menu and pause menu click detection
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && !story && (!isMakeupGame))
	{
		if (inButton(registry.motions.get(new_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT))
		{
			// START A NEW GAME
			// Direct to background story telling first
			int w, h;
			glfwGetWindowSize(window, &w, &h);
			backgroundImage = createStoryBackground(renderer, {window_width_px / 2, window_height_px / 2}, 1);
			dialogue = createBackgroundDiaogue(renderer, {window_width_px / 2, 650}, 1);
			story = 1;
			canStep = 0;
		}
		else if (inButton(registry.motions.get(load_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT))
		{
			loaded_game = true;
			loadedLevel = -1;
			tutorial_enabled = 0;
			canStep = 1;
			restart_game(false);
		}
		else if (inButton(registry.motions.get(makeup_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {
			// isMakeupGame = true;
			if (!isMakeupGame) {
				initializeMakeUpGame();
			}
			isMakeupGame = true;
			// initializeMakeUpGame();
		}
		else if (inButton(registry.motions.get(exit_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT))
		{
			// EXIT TO DESKTOP
			closeWindow = 1;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 1 && dragon == 0)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, {window_width_px / 2, window_height_px / 2}, 2);
		registry.remove_all_components_of(dialogue);
		dialogue = createBackgroundDiaogue(renderer, {window_width_px / 2, 650}, 2);
		story = 2;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 2 && dragon == 0)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		dialogue = createBackgroundDiaogue(renderer, {window_width_px / 2, 650}, 3);
		story = 3;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 3 && dragon == 0)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, {window_width_px / 2, window_height_px / 2}, 3);
		registry.remove_all_components_of(dialogue);
		dialogue = createBackgroundDiaogue(renderer, {window_width_px / 2, 650}, 4);
		story = 4;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 4 && dragon == 0)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, {w / 2, h / 2}, 4);
		registry.remove_all_components_of(dialogue);
		story = 5;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 5 && dragon == 0)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		dialogue = createBackgroundDiaogue(renderer, {window_width_px / 2, 650}, 5);
		story = 6;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 6 && dragon == 0)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, {w / 2, h / 2}, 5);
		story = 7;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 7 && dragon == 0)
	{
		// START A NEW GAME
		loadedLevel = 0;
		loaded_game = false;
		isFreeRoam = false;
		freeRoamLevel = 1;
		restart_game(false);
		canStep = 1;
		story = 8;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story >= 8 && story <= 17 && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		printf("STORY IS WHAT NUMBER: %g \n", float(story));
		dialogue = createLevelOneDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, (story - 7));
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 18)
	{
		story = 19;
		canStep = 1;
		restart_game();
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story >= 19 && story <= 24 && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		printf("STORY IS WHAT NUMBER: %g \n", float(story));
		dialogue = createLevelTwoDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, (story - 18));
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 25)
	{
		story = 26;
		canStep = 1;
		restart_game();
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story >= 26 && story <= 32 && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		printf("STORY IS WHAT NUMBER: %g \n", float(story));
		dialogue = createLevelThreeDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, (story - 24));
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 33)
	{
		registry.remove_all_components_of(dialogue);
		canStep = 1;
		story = 36;
		roundVec.clear(); // empty vector roundVec to create a new round
		createBackgroundObject(renderer, { 1160, 315 });
		auto ent = createBackgroundObject(renderer, { 550, 325 });
		registry.deformableEntities.get(ent).deformType2 = true;
		necromancer_phase_two = createNecromancerPhaseTwo(renderer, { 900, 350 });
		update_healthBars();
		createRound();
		checkRound();
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story >= 36 && story <= 39 && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		dialogue = createLevelFourDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, (story - 35));
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 40)
	{
		story = 42;

		// GO TO CONCLUSION
		while (registry.motions.entities.size() > 0) {
			registry.remove_all_components_of(registry.motions.entities.back());
		}
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2, h / 2 }, 11);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 42)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2, h / 2 }, 12);
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 43)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2, h / 2 }, 13);
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 44)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2, h / 2 }, 14);
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 45)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2, h / 2 }, 15);
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 46)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2, h / 2 }, 16);
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 47)
	{
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, registry.turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2, h / 2 }, 17);
		story++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 48)
	{
		/*registry.renderRequests.get(dialogue).used_texture = TEXTURE_ASSET_ID::LEVELFOURDIALOGUETWO;*/
		// Shut down game after last enemy defeated
		// closeWindow = 1;

		story = 50;
		canStep = 1;

		startMenuCleanUp();

		printf("won, return\n");
		return;
	}



	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && (beginning == 1 || beginning == 2) && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		beginning = 0;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && beginning >= 3 && beginning <= 5)
	{
		registry.remove_all_components_of(dialogue);
		dialogue = createLevelThreeDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, (beginning + 7));
		beginning++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && beginning == 6 && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		beginning = 0;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && beginning == 100 && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		dialogue = createFreeRoamLevelDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, (beginning - 98));
		beginning++;
		printf("BEGINNING IS ... %g \n", float(beginning));
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && beginning == 101 && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		free_roam_tutorial = createFreeRoamLevelTutorial(renderer, vec2(window_width_px / 2, window_height_px / 2));
		beginning = 0;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && beginning >= 200 && beginning <= 201 && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		dialogue = createFreeRoamLevelDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, (beginning - 193));
		beginning++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && beginning == 202 && dragon == 0)
	{
		registry.remove_all_components_of(dialogue);
		beginning = 0;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && dragon == 1)
	{
		registry.remove_all_components_of(dialogue);
		dragon = 0;
		canStep = 1;
		restart_game(false);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && dragon == 2)
	{
		registry.remove_all_components_of(dialogue);
		dialogue = createFreeRoamLevelDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, (dragon + 3));
		dragon++;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && dragon == 3)
	{
		registry.remove_all_components_of(dialogue);
		dragon = 0;
		canStep = 1;
		restart_game(false);
	}

	// gesture skill
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && canStep)
	{
		printf("gesture skill collecting active!\n");
		startMousePosCollect = 1;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE && canStep)
	{
		startMousePosCollect = 0;
		float maxX = -9999;
		float minX = 9999;
		float maxY = -9999;
		float minY = 9999;
		float aveX;
		float aveY;
		float totalX = 0;
		float totalY = 0;
		// Y increasing means going down! idk why
		float Yincreasing_switch = 1;
		float Xincreasing_switch = 1;
		float Ydecreasing_switch = 1;
		float Xdecreasing_switch = 1;

		for (int i = 0; i < mouseGestures.size(); i++)
		{
			// print out conditions
			//printf("collected mousePos: X=%f Y=%f\n", mouseGestures[i].x, mouseGestures[i].y);
			totalX += mouseGestures[i].x;
			totalY += mouseGestures[i].y;
			if (mouseGestures[i].x > maxX)
			{
				if (maxX != -9999)
				{
					Xdecreasing_switch = 0;
				}
				maxX = mouseGestures[i].x;
			}
			if (mouseGestures[i].x < minX)
			{
				if (minX != 9999)
				{
					Xincreasing_switch = 0;
				}
				minX = mouseGestures[i].x;
			}
			if (mouseGestures[i].y > maxY)
			{
				if (maxY != -9999)
				{
					Ydecreasing_switch = 0;
				}
				maxY = mouseGestures[i].y;
			}
			if (mouseGestures[i].y < minY)
			{
				if (minY != 9999)
				{
					Yincreasing_switch = 0;
				}
				minY = mouseGestures[i].y;
			}
		}
		//printf("gesture skill collecting de	!\n");
		aveX = totalX / mouseGestures.size();
		aveY = totalY / mouseGestures.size();
		//printf("Ave X is %f, Ave Y is %f, MaxminX is %f %f, MixminY is %f %f\n", aveX, aveY, maxX, minX, maxY, minY);
		//printf("X inc:%f X dec:%f Yinc:%f Y dec:%f\n", Xincreasing_switch, Xdecreasing_switch, Yincreasing_switch, Ydecreasing_switch);

		// horisontal line - skill 1: heal
		if (gestureSkillRemaining > 0)
		{
			if (Xincreasing_switch == 1 && Xdecreasing_switch == 0 &&
				maxX - aveX >= 200 && aveX - minX >= 200 &&
				maxY - aveY <= 50 && aveY - minY <= 50)
			{
				// launch heal skill
				Mix_Volume(5, 32);
				Mix_PlayChannel(5, registry.gesture_heal_sound, 0);
				sk->luanchCompanionTeamHeal(50, renderer);
				update_healthBars();
				gestureSkillRemaining--; // decrement gestureSkillRemaining
				printf("heal Gesture skill activated!");
			}
			// vertical line -skill 2: aoe damage
			if (Yincreasing_switch == 1 && Ydecreasing_switch == 0 &&
				maxX - aveX <= 50 && aveX - minX <= 50 &&
				maxY - aveY >= 150 && aveY - minY >= 150)
			{
				// launch aoe damage skill
				Mix_Volume(5, 32);
				Mix_PlayChannel(5, registry.gesture_aoe_sound, 0);
				sk->luanchEnemyTeamDamage(30, renderer);
				update_healthBars();
				gestureSkillRemaining--; // decrement gestureSkillRemaining
				printf("damage Gesture skill activated!");
			}
			// circle - skill 3: one more turn
			if (Xincreasing_switch == 0 && Xdecreasing_switch == 0 &&
				Yincreasing_switch == 0 && Ydecreasing_switch == 0 &&
				maxX - aveX <= 300 && aveX - minX <= 300 &&
				maxY - aveY <= 300 && aveY - minY <= 300)
			{
				// launch extra one turn
				Mix_Volume(5, 32);
				Mix_PlayChannel(5, registry.gesture_turn_sound, 0);
				extraCompanionTurn--;
				gestureSkillRemaining--; // decrement gestureSkillRemaining
				printf("one more turn skill activated!");
			}
		}
		else
		{
			Mix_Volume(5, 32);
			Mix_PlayChannel(5, registry.error_sound, 0);
		}

		mouseGestures.clear();
	}
	// other clicks
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && canStep)
	{
		printf("checking menu clicks\n");
		// Check menu clicks
		if (!pauseMenuOpened && inButton(registry.motions.get(open_menu_button).position, UI_BUTTON_HEIGHT, UI_BUTTON_HEIGHT))
		{
			printf("inside menu part\n");
			printf("opens menu\n");
			Motion menu_motion = registry.motions.get(open_menu_button);
			if(!isFreeRoam && !isMakeupGame){
				save_game_button = createUIButton(renderer, {menu_motion.position.x + menu_motion.scale.x / 2, menu_motion.position.y + menu_motion.scale.y / 3 + UI_BUTTON_HEIGHT}, SAVE_GAME);
				exit_game_button = createUIButton(renderer, {menu_motion.position.x + menu_motion.scale.x / 2, menu_motion.position.y + menu_motion.scale.y / 3 + UI_BUTTON_HEIGHT * 2}, EXIT_GAME);
			} else {
				exit_game_button = createUIButton(renderer, {menu_motion.position.x + menu_motion.scale.x / 2, menu_motion.position.y + menu_motion.scale.y / 3 + UI_BUTTON_HEIGHT}, EXIT_GAME);
			}
			registry.motions.get(exit_game_button).scale = {200, 80};
			pauseMenuOpened = 1;
			registry.renderRequests.get(open_menu_button).used_texture = TEXTURE_ASSET_ID::CLOSE_MENU;
		}
		else if (pauseMenuOpened)
		{
			if ((!isFreeRoam) && (!isMakeupGame) && (inButton(registry.motions.get(save_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)))
			{
				// SAVE THE CURRENT GAME STATE
				JSONLoader jl;
				jl.save_game();
				// Todo: implement
			}

			else if (inButton(registry.motions.get(exit_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT))
			{
				// GO BACK TO START MENU
				printf("clean up started\n");
				startMenuCleanUp();
				return;
			}

			else if (inButton(registry.motions.get(open_menu_button).position, UI_BUTTON_HEIGHT, UI_BUTTON_HEIGHT))
			{
				printf("closes menu\n");
				// HIDE MENU OPTIONS
				registry.renderRequests.get(open_menu_button).used_texture = TEXTURE_ASSET_ID::OPEN_MENU;
				// Clear on-screen buttons
				registry.remove_all_components_of(save_game_button);
				registry.remove_all_components_of(exit_game_button);
				pauseMenuOpened = 0;
			}
		}

		if(!isFreeRoam){
			showCorrectSkills();
		}

		if (player_turn == 1 && registry.checkRoundTimer.entities.size() == 0 && (!isFreeRoam))
		{
			if (registry.companions.has(currPlayer))
			{

				// iceshard
				if (inButton(registry.motions.get(iceShard_icon).position, ICON_WIDTH, ICON_HEIGHT) && canUseSkill(currPlayer, 0) && !(tutorial_enabled && curr_tutorial_box_num < 7))
				{
					if (selected_skill == -1)
					{
						registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICONSELECTED;
						selected_skill = 0;
					}
					else
					{
						registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICON;
						selected_skill = -1;
					}
				}
				// fireball
				else if (inButton(registry.motions.get(fireBall_icon).position, ICON_WIDTH, ICON_HEIGHT) && canUseSkill(currPlayer, 1) && !(tutorial_enabled && curr_tutorial_box_num < 7))
				{
					if (selected_skill == -1)
					{
						registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICONSELECTED;
						selected_skill = 1;
					}
					else
					{
						registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICON;
						selected_skill = -1;
					}
				}
				// rock
				else if (inButton(registry.motions.get(rock_icon).position, ICON_WIDTH, ICON_HEIGHT) && canUseSkill(currPlayer, 2) && !(tutorial_enabled && curr_tutorial_box_num < 7))
				{
					if (selected_skill == -1)
					{
						registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICONSELECTED;
						selected_skill = 2;
					}
					else
					{
						registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICON;
						selected_skill = -1;
					}
				}
				// heal
				else if (inButton(registry.motions.get(heal_icon).position, ICON_WIDTH, ICON_HEIGHT) && canUseSkill(currPlayer, 3) && !(tutorial_enabled && curr_tutorial_box_num < 7))
				{
					if (selected_skill == -1)
					{
						registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICONSELECTED;
						selected_skill = 3;
					}
					else
					{
						registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICON;
						selected_skill = -1;
					}
				}
				// taunt
				else if (inButton(registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT) && canUseSkill(currPlayer, 4) && !(tutorial_enabled && curr_tutorial_box_num < 7))
				{
					if (selected_skill == -1)
					{
						registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICONSELECTED;
						selected_skill = 4;
					}
					else
					{
						registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICON;
						selected_skill = -1;
					}
				}
				// melee
				else if (inButton(registry.motions.get(melee_icon).position, ICON_WIDTH, ICON_HEIGHT) && canUseSkill(currPlayer, 5) && !(tutorial_enabled && curr_tutorial_box_num < 7))
				{
					if (selected_skill == -1)
					{
						registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICONSELECTED;
						selected_skill = 5;
					}
					else
					{
						registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICON;
						selected_skill = -1;
					}
				}
				// arrow (Tutorial ability)
				else if (inButton(registry.motions.get(arrow_icon).position, ICON_WIDTH, ICON_HEIGHT) && canUseSkill(currPlayer, 6) && !(tutorial_enabled && curr_tutorial_box_num < 5)) {
					if (selected_skill == -1) {
						registry.renderRequests.get(arrow_icon).used_texture = TEXTURE_ASSET_ID::ARROWICONSELECTED;
						selected_skill = 6;
						tutorial_icon_selected = 1;
					}
					else {
						registry.renderRequests.get(arrow_icon).used_texture = TEXTURE_ASSET_ID::ARROWICON;
						selected_skill = -1;
					}
				}
				else {
					//iceshard
					if (selected_skill == 0) {
						sk->startIceShardAttack(currPlayer, currPlayer);
						selected_skill = -1;
						registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICON;
						playerUseMelee = 0; // added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
					}
					// fireball
					if (selected_skill == 1)
					{
						sk->startFireballAttack(currPlayer);
						selected_skill = -1;
						registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICON;
						playerUseMelee = 0; // added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
					}
					// rock
					if (selected_skill == 2)
					{
						for (int j = 0; j < registry.enemies.components.size(); j++)
						{
							PhysicsSystem physicsSystem;
							vec2 b_box = physicsSystem.get_custom_bounding_box(registry.enemies.entities[j]);
							if (inButton(registry.motions.get(registry.enemies.entities[j]).position, b_box.x, b_box.y))
							{
								sk->startRockAttack(currPlayer, registry.enemies.entities[j]);
								selected_skill = -1;
								registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICON;
								playerUseMelee = 0; // added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
							}
						}
					}
					// heal
					if (selected_skill == 3)
					{
						for (int j = 0; j < registry.companions.components.size(); j++)
						{
							PhysicsSystem physicsSystem;
							vec2 b_box = physicsSystem.get_custom_bounding_box(registry.companions.entities[j]);
							if (inButton(registry.motions.get(registry.companions.entities[j]).position, b_box.x, b_box.y))
							{
								sk->startHealAttack(currPlayer, registry.companions.entities[j]);
								prevPlayer = currPlayer;
								update_healthBars();
								selected_skill = -1;
								registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICON;
								playerUseMelee = 0; // added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
							}
						}
					}
					// taunt
					if (selected_skill == 4)
					{
						for (int j = 0; j < registry.enemies.components.size(); j++)
						{
							PhysicsSystem physicsSystem;
							vec2 b_box = physicsSystem.get_custom_bounding_box(registry.enemies.entities[j]);
							if (inButton(registry.motions.get(registry.enemies.entities[j]).position, b_box.x, b_box.y))
							{
								sk->startTauntAttack(currPlayer, registry.enemies.entities[j]);
								selected_skill = -1;
								registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICON;
								playerUseMelee = 0; // added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
							}
						}
					}
					// melee
					if (selected_skill == 5)
					{
						for (int j = 0; j < registry.enemies.components.size(); j++)
						{
							PhysicsSystem physicsSystem;
							vec2 b_box = physicsSystem.get_custom_bounding_box(registry.enemies.entities[j]);
							if (inButton(registry.motions.get(registry.enemies.entities[j]).position, b_box.x, b_box.y))
							{
								sk->startMeleeAttack(currPlayer, registry.enemies.entities[j], -1);
								playerUseMelee = 1;
								selected_skill = -1;
								registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICON;
							}
						}
					}
					//arrow
					if (selected_skill == 6) {
						sk->startArrowAttack(currPlayer);
						selected_skill = -1;
						tutorial_ability_fired = 1;
						registry.renderRequests.get(arrow_icon).used_texture = TEXTURE_ASSET_ID::ARROWICON;
						playerUseMelee = 0;
					}
				}
			}
			else
			{
				if (roundVec.empty())
				{
					printf("roundVec is empty at player turn, createRound now \n");
					createRound();
				}
				else
				{
					printf("no player at player turn \n");
					checkRound();
				}
			}
		}

		// Handle clicks on tutorial box
		if (tutorial_ability_fired && tutorial_icon_selected && tutorial_enabled && (curr_tutorial_box_num <= 4 || curr_tutorial_box_num == 7) && inButton(registry.motions.get(curr_tutorial_box).position, TUTORIAL_BOX_WIDTH, TUTORIAL_BOX_HEIGHT))
		{

			if (curr_tutorial_box_num == 7)
			{
				registry.remove_all_components_of(curr_tutorial_box);
				curr_tutorial_box_num += 1;
				tutorial_enabled = 0;
			}
			else
			{
				advanceTutorial(curr_tutorial_box, vec2(-1, -1));
			}
		}
		if (tutorial_icon_selected && curr_tutorial_box_num == 5)
		{
			// Do case 5 from advanceTutorial here
			tutorial_ability_fired = 0;
			advanceTutorial(curr_tutorial_box, vec2(700, 300));
		}
		else if (tutorial_ability_fired && curr_tutorial_box_num == 6)
		{
			// Do case 6 from advanceTutorial here
			advanceTutorial(curr_tutorial_box, vec2(600, 300));
			showCorrectSkills();
		}

		// Handle free-roam arrow launching
		if (isFreeRoam && beginning == 0 && dragon == 0 && !registry.renderRequests.has(dialogue)) {
			if (!registry.attackers.has(player_archer) && registry.companions.get(player_archer).curr_anim_type != JUMPING) {
				auto& arrow = registry.attackers.emplace(player_archer);
				arrow.attack_type = FREE_ROAM_ARROW;
				arrow.counter_ms = 525.f;
				arrow.old_pos = msPos;
				
				auto& motion = registry.motions.get(player_archer);

				if (motion.velocity.x != 0 && motion.velocity.y == 0) {
					registry.companions.get(player_archer).curr_anim_type = WALK_ATTACKING;
				}
				else {
					registry.companions.get(player_archer).curr_anim_type = ATTACKING;
				}

				if (motion.position.x <= msPos.x) {
					motion.scale.x = abs(motion.scale.x);
				}
				else {
					motion.scale.x = - abs(motion.scale.x);
				}
			}
		}
	}

	//makeup game clicks
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && isMakeupGame && !canStep && !story) {
		if (inButton(registry.motions.get(selectArcher).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if ((companion_size + 1) <= 4) {
				companion_size++;
				*placeSelections(companion_size, 1) = createPlayerArcher(renderer, checkPositions(companion_size, 1), 0);
				Entity entity = *placeSelections(companion_size, 1);
				Statistics& stat = registry.stats.get(entity);
				stat.speed = player_archer_speed;
				player_archer_speed++;
				updateSize();
				checkIfReady();
			}
			
		} 
		else if (inButton(registry.motions.get(selectMage).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if ((companion_size + 1) <= 4) {
				companion_size++;
				*placeSelections(companion_size, 1) = createPlayerMage(renderer, checkPositions(companion_size, 1));
				Entity entity = *placeSelections(companion_size, 1);
				Statistics& stat = registry.stats.get(entity);
				stat.speed = player_mage_speed;
				player_mage_speed++;
				updateSize();
				checkIfReady();
			}
		} 
		else if (inButton(registry.motions.get(selectSwordsman).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if ((companion_size + 1) <= 4) {
			companion_size++;
			*placeSelections(companion_size, 1) = createPlayerSwordsman(renderer, checkPositions(companion_size, 1));
			Entity entity = *placeSelections(companion_size, 1);
			Statistics& stat = registry.stats.get(entity);
			stat.speed = player_swordsman_speed;
			player_swordsman_speed++;
			updateSize();
			checkIfReady();
			}
		}
		else if (inButton(registry.motions.get(selectEnemyMage).position, 100, 100))
		{
			if ((enemy_size + 1) <= 4) {
			enemy_size++;
			*placeSelections(enemy_size, 2) = createEnemyMage(renderer, checkPositions(enemy_size, 2));
			Entity entity = *placeSelections(enemy_size, 2);
			Statistics& stat = registry.stats.get(entity);
			stat.speed = enemy_mage_speed;
			enemy_mage_speed++;
			updateSize();
			checkIfReady();
			}
		}
		else if (inButton(registry.motions.get(selectEnemySwordsman).position, 100, 100))
		{
			if ((enemy_size + 1) <= 4) {
				enemy_size++;
				*placeSelections(enemy_size, 2) = createEnemySwordsman(renderer, checkPositions(enemy_size, 2));
				Entity entity = *placeSelections(enemy_size, 2);
				Statistics& stat = registry.stats.get(entity);
				stat.speed = enemy_swordsman_speed;
				enemy_swordsman_speed++;
				updateSize();
				checkIfReady();
			}
		}
		else if (inButton(registry.motions.get(selectNecroOne).position, 100, 100))
		{
			if ((enemy_size + 3) <= 4) {
				float offset_x = 0.f;
				if(enemy_size == 1){
					offset_x = 50.f;
				}
			enemy_size += 3;
			*placeSelections(enemy_size, 2) = createNecromancerPhaseOne(renderer, checkPositions(enemy_size-2, 2));
			Entity entity = *placeSelections(enemy_size, 2);
			Motion& motion = registry.motions.get(entity);
			motion.position.x = motion.position.x - offset_x;
			Statistics& stat = registry.stats.get(entity);
			stat.speed = necro_one_speed;
			updateSize();
			checkIfReady();
			}
		}
		else if (inButton(registry.motions.get(selectNecroTwo).position, 100, 100))
		{
			if ((enemy_size + 3) <= 4) {
			enemy_size += 3;
			*placeSelections(enemy_size, 2) = createNecromancerPhaseTwo(renderer, checkPositions(enemy_size-1, 2));
			Entity entity = *placeSelections(enemy_size, 2);
			Statistics& stat = registry.stats.get(entity);
			stat.speed = necro_two_speed;
			updateSize();
			checkIfReady();
			}
		}

		if (isReset) {
			if (inButton(registry.motions.get(resetGameButton).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {
					companion_size = 0;
					enemy_size = 0;

					enemy_swordsman_speed = init_enemy_swordsman_speed;
					enemy_mage_speed = init_enemy_mage_speed;
					player_swordsman_speed = init_player_swordsman_speed;
					player_archer_speed = init_player_archer_speed;
					player_mage_speed = init_player_mage_speed;

					if(registry.companions.has(companionPosOne)){
						Companion& comp = registry.companions.get(companionPosOne);
						registry.remove_all_components_of(comp.healthbar);
						registry.remove_all_components_of(companionPosOne);
					}

					if(registry.companions.has(companionPosTwo)){
						Companion& comp = registry.companions.get(companionPosTwo);
						registry.remove_all_components_of(comp.healthbar);
						registry.remove_all_components_of(companionPosTwo);
					}

					if(registry.companions.has(companionPosThree)){
						Companion& comp = registry.companions.get(companionPosThree);
						registry.remove_all_components_of(comp.healthbar);
						registry.remove_all_components_of(companionPosThree);
					}

					if(registry.companions.has(companionPosFour)){
						Companion& comp = registry.companions.get(companionPosFour);
						registry.remove_all_components_of(comp.healthbar);
						registry.remove_all_components_of(companionPosFour);
					}

					if(registry.enemies.has(enemyPosOne)){
						Enemy& enemy = registry.enemies.get(enemyPosOne);
						registry.remove_all_components_of(enemy.healthbar);
						registry.remove_all_components_of(enemyPosOne);
					}

					if(registry.enemies.has(enemyPosTwo)){
						Enemy& enemy = registry.enemies.get(enemyPosTwo);
						registry.remove_all_components_of(enemy.healthbar);
						registry.remove_all_components_of(enemyPosTwo);
					}

					if(registry.enemies.has(enemyPosThree)){
						Enemy& enemy = registry.enemies.get(enemyPosThree);
						registry.remove_all_components_of(enemy.healthbar);
						registry.remove_all_components_of(enemyPosThree);
					}

					if(registry.enemies.has(enemyPosFour)){
						Enemy& enemy = registry.enemies.get(enemyPosFour);
						registry.remove_all_components_of(enemy.healthbar);
						registry.remove_all_components_of(enemyPosFour);
					}

					if (registry.renderRequests.has(startGameButton)) {
						registry.remove_all_components_of(startGameButton);
					}
					isReady = false;
					isReset = false;

					updateSize();
				}
			}

		if (isReady) {
			if (inButton(registry.motions.get(startGameButton).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {
				registry.remove_all_components_of(selectPanel);
				registry.remove_all_components_of(companionSize);
				registry.remove_all_components_of(enemySize);
				registry.remove_all_components_of(selectArcher);
				registry.remove_all_components_of(selectMage);
				registry.remove_all_components_of(selectSwordsman);
				registry.remove_all_components_of(selectEnemyMage);
				registry.remove_all_components_of(selectEnemySwordsman);
				registry.remove_all_components_of(selectNecroOne);
				registry.remove_all_components_of(selectNecroTwo);
				registry.remove_all_components_of(startGameButton);
				registry.remove_all_components_of(resetGameButton);

				
				if (registry.renderRequests.has(companionPosOne)) {
					Motion& motion = registry.motions.get(companionPosOne);
					motion.position.y = getYPosition(companionPosOne);
					Motion& health_bar_motion = registry.motions.get(registry.companions.get(companionPosOne).healthbar);
					health_bar_motion.position.y = motion.position.y - motion.scale.y/2;
				}
				if (registry.motions.has(companionPosTwo)) {
					Motion& motion = registry.motions.get(companionPosTwo);
					motion.position.y = getYPosition(companionPosTwo);
					Motion& health_bar_motion = registry.motions.get(registry.companions.get(companionPosTwo).healthbar);
					health_bar_motion.position.y = motion.position.y - motion.scale.y/2;
				}
				if (registry.motions.has(companionPosThree)) {
					Motion& motion = registry.motions.get(companionPosThree);
					motion.position.y = getYPosition(companionPosThree);
					Motion& health_bar_motion = registry.motions.get(registry.companions.get(companionPosThree).healthbar);
					health_bar_motion.position.y = motion.position.y - motion.scale.y/2;
				}
				if (registry.motions.has(companionPosFour)) {
					Motion& motion = registry.motions.get(companionPosFour);
					motion.position.y = getYPosition(companionPosFour);
					Motion& health_bar_motion = registry.motions.get(registry.companions.get(companionPosFour).healthbar);
					health_bar_motion.position.y = motion.position.y - motion.scale.y/2;
				}
				if (registry.motions.has(enemyPosOne)) {
					Motion& motion = registry.motions.get(enemyPosOne);
					motion.position.y = getYPosition(enemyPosOne);
					Motion& health_bar_motion = registry.motions.get(registry.enemies.get(enemyPosOne).healthbar);
					health_bar_motion.position.y = motion.position.y - motion.scale.y/2;
				}
				if (registry.motions.has(enemyPosTwo)) {
					Motion& motion = registry.motions.get(enemyPosTwo);
					motion.position.y = getYPosition(enemyPosTwo);
					Motion& health_bar_motion = registry.motions.get(registry.enemies.get(enemyPosTwo).healthbar);
					health_bar_motion.position.y = motion.position.y - motion.scale.y/2;
				}
				if (registry.motions.has(enemyPosThree)) {
					Motion& motion = registry.motions.get(enemyPosThree);
					motion.position.y = getYPosition(enemyPosThree);
					Motion& health_bar_motion = registry.motions.get(registry.enemies.get(enemyPosThree).healthbar);
					health_bar_motion.position.y = motion.position.y - motion.scale.y/2;
				 }

				if (registry.renderRequests.has(enemyPosFour)) {
					Motion& motion = registry.motions.get(enemyPosFour);
					motion.position.y = getYPosition(enemyPosFour);
					Motion& health_bar_motion = registry.motions.get(registry.enemies.get(enemyPosFour).healthbar);
					health_bar_motion.position.y = motion.position.y - motion.scale.y/2;
				}

				open_menu_button = createUIButton(renderer, { 100, 100 }, OPEN_MENU);
				createIcons();
				createRound();
				checkRound();
				gestureSkillRemaining = 1; // reset gesture skill remaining
				showCorrectSkills();
				displayPlayerTurn(); // display player turn when restart game
				update_healthBars();
				canStep = 1;
				Mix_FadeInMusic(registry.background_music, -1, 5000);
			}
		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && isMakeupGame && isFInished && canStep) {
		if (inButton(registry.motions.get(yesOption).position, 50, 30)) {
			fprintf(stderr, "click on yes");

			while (registry.motions.entities.size() > 0)
				registry.remove_all_components_of(registry.motions.entities.back());
			while (registry.renderRequests.entities.size() > 0)
				registry.remove_all_components_of(registry.renderRequests.entities.back());
			if (registry.charIndicator.components.size() != 0)
			{
				registry.remove_all_components_of(registry.charIndicator.entities[0]);
			}
			// Debugging for memory/component leaks
			registry.list_all_components();

			canStep = 0;
			isFInished = false;
			isReady = false;
			isReset = false;
			isMakeupGame = true;

			initializeMakeUpGame();
			companion_size = 0;
			enemy_size = 0;
			updateSize();



		}
		else if ((inButton(registry.motions.get(noOption).position, 100, 30))) {
			fprintf(stderr, "click on no");
			startMenuCleanUp();
		}

	}

}

void WorldSystem::advanceTutorial(Entity currTutorial, vec2 pos)
{
	vec2 next_box_pos = vec2(0, 0);
	switch (curr_tutorial_box_num)
	{
	case 0:
		next_box_pos = vec2(300, 300);
		break; // Companions
	case 1:
		next_box_pos = vec2(900, 300);
		break; // Enemies
	case 2:
		next_box_pos = vec2(950, 150);
		break; // Turn indicator
	case 3:
		next_box_pos = vec2(350, 100);
		break; // Menu
	case 4:
		next_box_pos = vec2(200, 550);
		tutorial_icon_selected = 0;
		break; // Ability intro
	// case 5: next_box_pos = vec2(700, 300); tutorial_ability_fired = 0; break;  // Ability targeting
	// case 6: next_box_pos = vec2(600, 300); break;  // End of tutorial
	default:
		break;
	}
	curr_tutorial_box_num += 1;

	// Change box location
	if (registry.motions.has(currTutorial)) {
		Motion& motion = registry.motions.get(currTutorial);
		motion.position = (pos.x != -1) ? pos : next_box_pos;
	}


	// Change box type
	TEXTURE_ASSET_ID tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_ONE;
	switch (curr_tutorial_box_num)
	{
	case 0:
		tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_ONE;
		break;
	case 1:
		tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_TWO;
		break;
	case 2:
		tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_THREE;
		break;
	case 3:
		tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_FOUR;
		break;
	case 4:
		tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_FIVE;
		showCorrectSkills();
		break;
	case 5:
		tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_SIX;
		showCorrectSkills();
		break;
	case 6:
		tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_SEVEN;
		break;
	case 7:
		tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_EIGHT;
		break;
	default:
		break;
	}
	RenderRequest &renderRequest = registry.renderRequests.get(currTutorial);
	renderRequest.used_texture = tutorial_box_num;
}

vec2 WorldSystem::placeDirection(vec2 mouse_position, vec2 icon_position, float width, float height)
{
	vec2 placePos;
	if (mouse_position.x <= icon_position.x && mouse_position.y >= icon_position.y)
	{
		placePos = vec2(mouse_position.x + 275.f, mouse_position.y - 75.f);
	}
	else if (mouse_position.x <= icon_position.x && mouse_position.y <= icon_position.y)
	{
		placePos = vec2(mouse_position.x + 275.f, mouse_position.y - 75.f);
	}
	else if (mouse_position.x >= icon_position.x && mouse_position.y >= icon_position.y)
	{
		placePos = vec2(mouse_position.x + 275.f, mouse_position.y - 75.f);
	}
	else if (mouse_position.x >= icon_position.x && mouse_position.y <= icon_position.y)
	{
		placePos = vec2(mouse_position.x + 275.f, mouse_position.y - 75.f);
	}
	return placePos;
}

void WorldSystem::on_mouse_move(vec2 mouse_position)
{
	// printf("x: %f, y: %f\n", mouse_position.x, mouse_position.y);
	msPos = mouse_position;
	sk->mousePos = mouse_position;
	if (!canStep && !story && (!isMakeupGame))
	{
		if (mouseInArea(registry.motions.get(new_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT))
		{
			registry.renderRequests.get(new_game_button).used_texture = TEXTURE_ASSET_ID::NEW_GAME_HOVER;
			Mix_Volume(5, 32);
			Mix_PlayChannel(5, registry.button_hover_sound, 0);
		}
		else
		{
			registry.renderRequests.get(new_game_button).used_texture = TEXTURE_ASSET_ID::NEW_GAME;
		}
		if (mouseInArea(registry.motions.get(load_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT))
		{
			registry.renderRequests.get(load_game_button).used_texture = TEXTURE_ASSET_ID::LOAD_GAME_HOVER;
			Mix_Volume(5, 32);
			Mix_PlayChannel(5, registry.button_hover_sound, 0);
		}
		else
		{
			registry.renderRequests.get(load_game_button).used_texture = TEXTURE_ASSET_ID::LOAD_GAME;
		}

		if (mouseInArea(registry.motions.get(makeup_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT))
		{
			registry.renderRequests.get(makeup_game_button).used_texture = TEXTURE_ASSET_ID::CREATE_GAME_HOVER;
			Mix_Volume(5, 32);
			Mix_PlayChannel(5, registry.button_hover_sound, 0);
		}
		else
		{
			registry.renderRequests.get(makeup_game_button).used_texture = TEXTURE_ASSET_ID::CREATE_GAME;
		}
		if (mouseInArea(registry.motions.get(exit_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT))
		{
			registry.renderRequests.get(exit_game_button).used_texture = TEXTURE_ASSET_ID::EXIT_HOVER;
			Mix_Volume(5, 32);
			Mix_PlayChannel(5, registry.button_hover_sound, 0);
		}
		else
		{
			registry.renderRequests.get(exit_game_button).used_texture = TEXTURE_ASSET_ID::EXIT_GAME;
		}
	}
	else if (canStep && (!isFreeRoam) && (!isMakeupGame))
	{

		if (mouseInArea(registry.motions.get(fireBall_icon).position, ICON_WIDTH, ICON_HEIGHT) && registry.renderRequests.get(fireBall_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE)
		{
			if (registry.toolTip.size() == 0)
			{
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(fireBall_icon).position, ICON_WIDTH, ICON_HEIGHT), "FB");
			}
		}
		else if (mouseInArea(registry.motions.get(iceShard_icon).position, ICON_WIDTH, ICON_HEIGHT) && registry.renderRequests.get(iceShard_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE)
		{
			if (registry.toolTip.size() == 0)
			{
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(iceShard_icon).position, ICON_WIDTH, ICON_HEIGHT), "IS");
			}
		}
		else if (mouseInArea(registry.motions.get(rock_icon).position, ICON_WIDTH, ICON_HEIGHT) && registry.renderRequests.get(rock_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE)
		{
			if (registry.toolTip.size() == 0)
			{
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(rock_icon).position, ICON_WIDTH, ICON_HEIGHT), "RK");
			}
		}
		else if (mouseInArea(registry.motions.get(heal_icon).position, ICON_WIDTH, ICON_HEIGHT) && registry.renderRequests.get(heal_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE)
		{
			if (registry.toolTip.size() == 0)
			{
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(heal_icon).position, ICON_WIDTH, ICON_HEIGHT), "HL");
			}
		}
		else if (mouseInArea(registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT) && registry.renderRequests.get(taunt_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE)
		{
			if (registry.toolTip.size() == 0)
			{
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT), "TT");
			}
		}
		else if (mouseInArea(registry.motions.get(melee_icon).position, ICON_WIDTH, ICON_HEIGHT) && registry.renderRequests.get(melee_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE)
		{
			if (registry.toolTip.size() == 0)
			{
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT), "ML");
			}
		}
		else if (mouseInArea(registry.motions.get(arrow_icon).position, ICON_WIDTH, ICON_HEIGHT) && registry.renderRequests.get(arrow_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE)
		{
			if (registry.toolTip.size() == 0)
			{
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(arrow_icon).position, ICON_WIDTH, ICON_HEIGHT), "AR");
			}
		}
		else
		{
			registry.remove_all_components_of(tooltip);
			registry.toolTip.clear();
		}
	}
	if (isMakeupGame && !canStep) {
		if (mouseInArea(registry.motions.get(selectArcher).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if (registry.hoverBox.size() == 0) {
				makeHoverBox(selectArcher);
			}

		} else if (mouseInArea(registry.motions.get(selectMage).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if (registry.hoverBox.size() == 0) {
			makeHoverBox(selectMage);
			}
		} else if (mouseInArea(registry.motions.get(selectSwordsman).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if (registry.hoverBox.size() == 0) {
			makeHoverBox(selectSwordsman);
			}
		} else if (mouseInArea(registry.motions.get(selectEnemyMage).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if (registry.hoverBox.size() == 0) {
			makeHoverBox(selectEnemyMage);
			}
		}else if (mouseInArea(registry.motions.get(selectEnemySwordsman).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if (registry.hoverBox.size() == 0) {
			makeHoverBox(selectEnemySwordsman);
			}
		} else if (mouseInArea(registry.motions.get(selectNecroOne).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if (registry.hoverBox.size() == 0) {
			makeHoverBox(selectNecroOne);
			}
		} else if (mouseInArea(registry.motions.get(selectNecroTwo).position, SELECTIONS_WIDTH, SELECTIONS_HEIGHT))
		{
			if (registry.hoverBox.size() == 0) {
				makeHoverBox(selectNecroTwo);
			}
		}
		else {
			registry.remove_all_components_of(hoverBoxTop);
			registry.remove_all_components_of(hoverBoxBottom);
			registry.remove_all_components_of(hoverBoxLeft);
			registry.remove_all_components_of(hoverBoxRight);
		}
	}

	//if (isMakeupGame && isReady) {
	//	registry.renderRequests.get(startGameButton).used_texture = TEXTURE_ASSET_ID::START_MAKEUP_HOVER;
	//}
	//else {
	//	registry.renderRequests.get(startGameButton).used_texture = TEXTURE_ASSET_ID::START_MAKEUP;
	//}
	//if (isMakeupGame && isReset) {
	//	registry.renderRequests.get(resetGameButton).used_texture = TEXTURE_ASSET_ID::RESET_MAKEUP_HOVER;
	//}
	//else {
	//	registry.renderRequests.get(resetGameButton).used_texture = TEXTURE_ASSET_ID::RESET_MAKEUP;
	//}
}

float WorldSystem::getYPosition(Entity entity){
	if(registry.enemies.has(entity) || registry.companions.has(entity)){
		auto type = registry.enemies.has(entity)? registry.enemies.get(entity).enemyType : registry.companions.get(entity).companionType;
		float y = 0.f;
		switch(type){
			case MAGE: y = 575; break;
			case SWORDSMAN: y = 530; break;
			case NECROMANCER_ONE: y = 530; break;
			case NECROMANCER_TWO: y = 350; break;
			case ARCHER: y = 575; break;
		}
		return y;
	}
	return -1;
}

bool WorldSystem::mouseInArea(vec2 buttonPos, float buttonX, float buttonY)
{
	float left_bound = buttonPos.x - (buttonX / 2);
	float right_bound = buttonPos.x + (buttonX / 2);
	float upper_bound = buttonPos.y - (buttonY / 2);
	float lower_bound = buttonPos.y + (buttonY / 2);
	if ((left_bound <= msPos.x) && (msPos.x <= right_bound))
	{
		if ((upper_bound <= msPos.y) && (msPos.y <= lower_bound))
		{
			return true;
		}
	}
	return false;
}

bool WorldSystem::inButton(vec2 buttonPos, float buttonX, float buttonY)
{
	if (msPos.x <= buttonPos.x + buttonX / 2 && msPos.x >= buttonPos.x - buttonX / 2)
	{
		if (msPos.y <= buttonPos.y + buttonY / 2 && msPos.y >= buttonPos.y - buttonY / 2)
		{
			return true;
		}
	}
	return false;
}

bool WorldSystem::inEntity(const Entity entity)
{
	PhysicsSystem physicsSystem;
	vec2 custom_pos = physicsSystem.get_custom_position(entity);
	vec2 custom_scale = physicsSystem.get_custom_bounding_box(entity);
	float left_bound = custom_pos.x - (custom_scale.x / 2);
	float right_bound = custom_pos.x + (custom_scale.x / 2);
	float upper_bound = custom_pos.y - (custom_scale.y / 2);
	float lower_bound = custom_pos.y + (custom_scale.y / 2);
	if ((left_bound <= msPos.x) && (msPos.x <= right_bound))
	{
		if ((upper_bound <= msPos.y) && (msPos.y <= lower_bound))
		{
			return true;
		}
	}
	return false;
}


void WorldSystem::deselectButton()
{
	registry.remove_all_components_of(selectedButton);
}

void WorldSystem::deselectButtons()
{
	if (selectedButton)
	{
		FIREBALLSELECTED = 0;
		SILENCESELECTED = 0;
		registry.remove_all_components_of(selectedButton);
	}
}

bool WorldSystem::canUseSkill(Entity user, int skill)
{
	Statistics pStat = registry.stats.get(user);
	return skill_character_aviability[pStat.classID][skill];
}

// helper that shows the correct skills based on current player
void WorldSystem::showCorrectSkills()
{
	if (currPlayer != NULL && registry.companions.has(currPlayer))
	{

		Statistics pStat = registry.stats.get(currPlayer);
		if (!skill_character_aviability[pStat.classID][0] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7))
		{	
			registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else
		{
			registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICON;
		}

		if (!skill_character_aviability[pStat.classID][1] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7))
		{	
			registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else
		{	
			registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICON;
		}

		if (!skill_character_aviability[pStat.classID][2] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7))
		{
			registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else
		{
			registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICON;
		}

		if (!skill_character_aviability[pStat.classID][3] || registry.taunts.has(currPlayer) || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7))
		{
			registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else
		{
			registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICON;
		}

		if (!skill_character_aviability[pStat.classID][4] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7))
		{
			registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else
		{
			registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICON;
		}

		if (!skill_character_aviability[pStat.classID][5] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7))
		{
			registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else
		{
			registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICON;
		}
		if (!skill_character_aviability[pStat.classID][6] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 5))
		{	
			registry.renderRequests.get(arrow_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else
		{	
			registry.renderRequests.get(arrow_icon).used_texture = TEXTURE_ASSET_ID::ARROWICON;
		}
	}
	else if (registry.enemies.has(currPlayer)) {
		registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		registry.renderRequests.get(arrow_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
	}
}

void WorldSystem::backgroundTelling() 
{

}


void WorldSystem::initializeFreeRoamOne() {

	createPlatform(renderer, { 100, 500 });
	createPlatform(renderer, { 400, 600 });
	createPlatform(renderer, { 450, 375 });
	createPlatform(renderer, { 700, 425 });
	createPlatform(renderer, { 900, 425 });
	createPlatform(renderer, { 1100, 425 });
	createTreasureChest(renderer, { 1050, 425 - PLATFORM_HEIGHT }, HEALTH_BOOST);

	std::vector<vec3> controlPoints;
	controlPoints.push_back(vec3(227, 160, 0));
	controlPoints.push_back(vec3(312, 260, 0));
	controlPoints.push_back(vec3(432, 150, 0));
	controlPoints.push_back(vec3(710, 240, 0));
	controlPoints.push_back(vec3(1070, 180, 0));
	controlPoints.push_back(vec3(1008, 59, 0));
	controlPoints.push_back(vec3(733, 148, 0));
	controlPoints.push_back(vec3(446, 75, 0));
	controlPoints.push_back(vec3(227, 160, 0));
	auto result = GenerateSpline(controlPoints, 40);
	renderer->splineControlPoints = controlPoints;
	// Keeping these for debugging purposes
	// printf("spline points: %i\n", result.size());
	// createSpline(renderer, controlPoints);
	spline = result;
	birdNextPostionTracker = 1;
	birdPositionDivisor = 1;
	free_roam_bird = createBird(renderer, vec2(227, 160));

	Mix_FadeInMusic(registry.wintervale_music, -1, 500);	// change to free roam level 1 music
}

void WorldSystem::initializeFreeRoamTwo() {

	createPlatform(renderer, { 550, 600 });
	createPlatform(renderer, { 650, 450 });
	createPlatform(renderer, { 100, 350 });
	createPlatform(renderer, { 300, 350 });
	createPlatform(renderer, { 1100, 225 });
	createPlatform(renderer, { 675, 225 });
	createTreasureChest(renderer, { 100, 350 - PLATFORM_HEIGHT }, HEALTH_BOOST);
	createTreasureChest(renderer, { 1100, 225 - PLATFORM_HEIGHT }, DAMAGE_BOOST);

	if (swarmSys->swarmInitialized) {
		swarmSys->resetSwarm();
	}
	else {
		swarmSys->initializeSwarmEntities(renderer);
	}
	Mix_FadeInMusic(registry.cestershire_music, -1, 500);
}

void WorldSystem::initializeMakeUpGame() {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	
	canStep = 0;
	
	createBackground(renderer, { w / 2, h / 2 }, TUTORIAL);

	registry.enemy_mage_hp = 30;
	registry.enemy_swordsman_hp = 150;

	registry.player_mage_hp = 70;
	registry.player_swordsman_hp = 250;
	registry.player_archer_hp = 175;
	registry.necro_minion_health = 10;
	registry.necro_1_health = 160;
	registry.necro_2_health = 200;

	selectPanel = createSelectPanel(renderer, { w / 2, 4*h/5 });
	companionSize = createSizeIndicator(renderer, { 1 * w / 6, 15 * h/ 16 },companion_size + 1);
	enemySize = createSizeIndicator(renderer, { 4 * w / 6, 15 * h / 16 }, enemy_size + 1);

	selectArcher = createSelections(renderer, { 1 * w / 15, 4 * h / 5 }, 1);
	selectMage = createSelections(renderer, {3 * w / 15, 4 * h / 5 }, 2);
	selectSwordsman = createSelections(renderer, { 5 * w / 15, 4 * h / 5 }, 3);

	selectEnemyMage = createSelections(renderer, { 9 * w / 15, 4 * h / 5 }, 2);
	selectEnemySwordsman = createSelections(renderer, { 10.5 * w / 15, 4 * h / 5 }, 3);
	selectNecroOne = createSelections(renderer, { 12 * w / 15, 4 * h / 5 }, 4);
	selectNecroTwo = createSelections(renderer, { 13.5 * w / 15, 4 * h / 5 }, 5);
	
	

}

void WorldSystem::makeHoverBox(Entity target) {
	
	float line_thickness = 3.f;

	float targetPosX = registry.motions.get(target).position.x;
	float targetPosY = registry.motions.get(target).position.y;

	float targetScaleX = registry.motions.get(target).scale.x;
	float targetScaleY = registry.motions.get(target).scale.y;

	vec2 line1_pos = { targetPosX, targetPosY + (targetScaleY / 4) };
	vec2 line2_pos = { targetPosX, targetPosY - (targetScaleY / 4) };
	vec2 line3_pos = { targetPosX + (targetScaleY / 4), targetPosY };
	vec2 line4_pos = { targetPosX - (targetScaleY / 4), targetPosY };

	vec2 line1_scale = { targetScaleX/2, line_thickness };
	vec2 line3_scale = { line_thickness, targetScaleY/2 };


	hoverBoxTop = createLine(line1_pos, line1_scale);
	hoverBoxBottom = createLine(line2_pos, line1_scale);
	hoverBoxLeft = createLine(line3_pos, line3_scale);
	hoverBoxRight = createLine(line4_pos, line3_scale);
	
	registry.hoverBox.emplace(hoverBoxTop);
	registry.hoverBox.emplace(hoverBoxBottom);
	registry.hoverBox.emplace(hoverBoxLeft);
	registry.hoverBox.emplace(hoverBoxRight);
}

vec2 WorldSystem::checkPositions(int number, int type) {
	vec2 pos = { 0,0 };
	if (type == 1) {
		switch (number)
		{
		case 1: pos = companionOnePos; break;
		case 2: pos = companionTwoPos; break;
		case 3: pos = companionThreePos; break;
		case 4: pos = companionFourPos; break;
		}
	}
	else if (type == 2) {
		switch (number)
		{
		case 1: pos = enemyOnePos; break;
		case 2: pos = enemyTwoPos; break;
		case 3: pos = enemyThreePos; break;
		case 4: pos = enemyFourPos; break;
		}
	}
	return pos;
}

Entity* WorldSystem::placeSelections(int number, int type) {
	Entity* entity = &emptyPos;
	if (type == 1) {
		switch (number)
		{
		case 1: entity = &companionPosOne; break;
		case 2: entity = &companionPosTwo; break;
		case 3: entity = &companionPosThree; break;
		case 4: entity = &companionPosFour; break;
		}
	}
	else if (type == 2) {
		switch (number)
		{
		case 1: entity = &enemyPosOne; break;
		case 2: entity = &enemyPosTwo; break;
		case 3: entity = &enemyPosThree; break;
		case 4: entity = &enemyPosFour; break;
		}
	}

	return entity;
}

void WorldSystem::updateSize() {
	if (companion_size == 0) {
		registry.renderRequests.get(companionSize).used_texture = TEXTURE_ASSET_ID::ZERO_OUT_OF_FOUR;
	}
	else if (companion_size == 1) {
		registry.renderRequests.get(companionSize).used_texture = TEXTURE_ASSET_ID::ONE_OUT_OF_FOUR;
	}
	else if (companion_size == 2) {
		registry.renderRequests.get(companionSize).used_texture = TEXTURE_ASSET_ID::TWO_OUT_OF_FOUR;
	}
	else if (companion_size == 3) {
		registry.renderRequests.get(companionSize).used_texture = TEXTURE_ASSET_ID::THREE_OUT_OF_FOUR;
	}
	else if (companion_size == 4) {
		registry.renderRequests.get(companionSize).used_texture = TEXTURE_ASSET_ID::FOUR_OUT_OF_FOUR;
	}

	if (enemy_size == 0) {
		registry.renderRequests.get(enemySize).used_texture = TEXTURE_ASSET_ID::ZERO_OUT_OF_FOUR;
	}
	else if (enemy_size == 1) {
		registry.renderRequests.get(enemySize).used_texture = TEXTURE_ASSET_ID::ONE_OUT_OF_FOUR;
	}
	else if (enemy_size == 2) {
		registry.renderRequests.get(enemySize).used_texture = TEXTURE_ASSET_ID::TWO_OUT_OF_FOUR;
	}
	else if (enemy_size == 3) {
		registry.renderRequests.get(enemySize).used_texture = TEXTURE_ASSET_ID::THREE_OUT_OF_FOUR;
	}
	else if (enemy_size == 4) {
		registry.renderRequests.get(enemySize).used_texture = TEXTURE_ASSET_ID::FOUR_OUT_OF_FOUR;
	}
}

void WorldSystem::checkIfReady() {
	if (companion_size > 0 && enemy_size > 0) {
		isReady = true;
		if (!registry.renderRequests.has(startGameButton)) {
			startGameButton = createUIButton(renderer, { 600,100 }, 9);
		}

	}

	if (companion_size > 0 || enemy_size > 0) {
		isReset = true;
		if (!registry.renderRequests.has(resetGameButton)) {
			resetGameButton = createUIButton(renderer, { 600,200}, 10);
		}
	}
}
void WorldSystem::renderBeginningStory() {
	if (!isFreeRoam && gameLevel == 1) {
		dialogue = createLevelOneDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, 11);
		beginning = 1;
	}
	if (!isFreeRoam && gameLevel == 2) {
		dialogue = createLevelTwoDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, 7);
		beginning = 2;
	}
	if (!isFreeRoam && gameLevel == 3) {
		dialogue = createLevelThreeDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, 9);
		beginning = 3;
	}
	if (isFreeRoam && freeRoamLevel == 1) {
		dialogue = createFreeRoamLevelDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, 1);
		beginning = 100;
	}
	if (isFreeRoam && freeRoamLevel == 2) {
		dialogue = createFreeRoamLevelDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, 6);
		beginning = 200;
	}
}

// call at level transition, make canStep = 0, 
void WorldSystem::renderDragonSpeech() {
	// check if no bird call this
	if (freeRoamLevel == 1 && registry.bird.components.size() <= 0) {
		dialogue = createFreeRoamLevelDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, 3);
		dragon = 1;
		canStep = 0;
		// Decrease health
		registry.HPDebuff++;
		Motion archerMotion = registry.motions.get(player_archer);
		createDebuffIndicator(renderer, vec2(archerMotion.position.x, archerMotion.position.y - archerMotion.scale.y - 10));
	}

	// check if has bird call this
	if (freeRoamLevel == 1 && registry.bird.components.size() != 0) {
		dialogue = createFreeRoamLevelDiaogue(renderer, { window_width_px / 2, window_height_px - window_height_px / 3 }, 4);
		dragon = 2;
		canStep = 0;
	}

	if (freeRoamLevel == 2) {
		restart_game(false);
	}
}

void WorldSystem::balanceHealthNumbers(int levelNum) {
	switch (levelNum) {
		case 0: {
			registry.player_archer_hp = 175;
			registry.enemy_mage_hp = 10;
			break;
		}
		case 1: {
			registry.enemy_mage_hp = 30;
			break;
		}
		case 2: {
			registry.player_mage_hp = 70;
			registry.enemy_swordsman_hp = 150;
			break;
		}
		case 3: {
			registry.player_swordsman_hp = 250;
			registry.necro_minion_health = 10;
			registry.necro_1_health = 160;
			registry.necro_2_health = 200;
			break;
		}
		default: break;
	}

	if (registry.HPBuff > 0) {
		for (int i = 0; i < registry.HPBuff; i++) {
			registry.player_archer_hp += 10;
			registry.player_mage_hp += 5;
			registry.swordsmanHPBuff++;
		}
		registry.HPBuff = 0;
	}

	if (registry.swordsmanHPBuff > 0 && levelNum == 3) {
		for (int i = 0; i < registry.swordsmanHPBuff; i++) {
			registry.player_swordsman_hp += 20;
		}
		registry.swordsmanHPBuff = 0;
	}

	if (registry.HPDebuff > 0) {
		if (levelNum == 2) {
			registry.player_archer_hp = registry.player_archer_hp - 15;
			registry.player_mage_hp = registry.player_mage_hp - 5;
		}
		else if (levelNum == 3) {
			registry.player_swordsman_hp = registry.player_swordsman_hp - 20;
		}
	}

}