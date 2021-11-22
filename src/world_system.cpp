// Header
#include "world_system.hpp"
#include "world_init.hpp"
#include "physics_system.hpp"

// stlib
#include <cassert>
#include <sstream>

#include "ai_system.hpp"
#include "skill_system.hpp"

// Game configuration
const size_t MAX_TURTLES = 15;
const size_t MAX_FISH = 5;
const size_t TURTLE_DELAY_MS = 2000 * 3;
const size_t FISH_DELAY_MS = 5000 * 3;
const size_t BARRIER_DELAY = 4000;
const size_t ENEMY_TURN_TIME = 3000;
const vec2 TURN_INDICATOR_LOCATION = { 600, 150 };
const int NUM_DEATH_PARTICLES = 2000;
vec2 CURRPLAYER_LOCATION = {};

const float animation_timer = 250.f;
const float hit_position = 20.f;

Entity currPlayer;
//Entity target;
Entity prevPlayer;

//int playersDead = 0;
int playerUseMelee = 0;

vec2 msPos = vec2(0, 0);
bool is_ms_clicked = false;

float next_barrier_spawn = 1000;

float enemy_turn_timer = 1000;

int FIREBALLSELECTED = 0;
int SILENCESELECTED = 0;

int selected_skill = -1;

//mouse gesture skills related=============
int startMousePosCollect = 0;
std::vector<vec2> mouseGestures;
int gestureSkillRemaining = 1;
int extraCompanionTurn = 1;
//===========================================
int hover_skill = -1;
//selected button
Entity selectedButton;

//current projectile
Entity currentProjectile;

WorldSystem::WorldSystem()
	: points(0), gameLevel(1) {
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

WorldSystem::~WorldSystem() {
	// Destroy music components
	if (background_music != nullptr)
		Mix_FreeMusic(background_music);
	if (salmon_dead_sound != nullptr)
		Mix_FreeChunk(salmon_dead_sound);
	if (salmon_eat_sound != nullptr)
		Mix_FreeChunk(salmon_eat_sound);
	if (hit_enemy_sound != nullptr)
		Mix_FreeChunk(hit_enemy_sound);
	if (fireball_explosion_sound != nullptr)
		Mix_FreeChunk(fireball_explosion_sound);
	if (death_enemy_sound != nullptr)
		Mix_FreeChunk(death_enemy_sound);
	if (fire_spell_sound != nullptr)
		Mix_FreeChunk(fire_spell_sound);
	if (rock_spell_sound != nullptr)
		Mix_FreeChunk(rock_spell_sound);
	if (heal_spell_sound != nullptr)
		Mix_FreeChunk(heal_spell_sound);
	if (taunt_spell_sound != nullptr)
		Mix_FreeChunk(taunt_spell_sound);
	if (melee_spell_sound != nullptr)
		Mix_FreeChunk(melee_spell_sound);
	if (silence_spell_sound != nullptr)
		Mix_FreeChunk(silence_spell_sound);
	if (lightning_spell_sound != nullptr)
		Mix_FreeChunk(lightning_spell_sound);
	if (ice_spell_sound != nullptr)
		Mix_FreeChunk(ice_spell_sound);
	if (summon_spell_sound != nullptr)
		Mix_FreeChunk(summon_spell_sound);
	if (button_hover_sound != nullptr)
		Mix_FreeChunk(button_hover_sound);
	if (turning_sound != nullptr)
		Mix_FreeChunk(turning_sound);
	if (charge_spell_sound != nullptr)
		Mix_FreeChunk(charge_spell_sound);
	if (beam_spell_sound != nullptr)
		Mix_FreeChunk(beam_spell_sound);
	if (minion_spawn_sound != nullptr)
		Mix_FreeChunk(minion_spawn_sound);
	if (error_sound != nullptr)
		Mix_FreeChunk(error_sound);
	if (gesture_heal_sound != nullptr)
		Mix_FreeChunk(gesture_heal_sound);
	if (gesture_aoe_sound != nullptr)
		Mix_FreeChunk(gesture_aoe_sound);
	if (gesture_turn_sound != nullptr)
		Mix_FreeChunk(gesture_turn_sound);
	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

// Debugging
namespace {
	void glfw_err_cb(int error, const char* desc) {
		fprintf(stderr, "%d: %s", error, desc);
	}
}

// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow* WorldSystem::create_window(int width, int height) {
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit()) {
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
	window = glfwCreateWindow(width, height, "Windfall Milestone 2", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_key(_0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_move({ _0, _1 }); };

	// handle mosue click
	auto mouse_button_callback = [](GLFWwindow* wnd, int _0, int _1, int _2) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_button(_0, _1, _2); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	//////////////////////////////////////
	// Loading music and sounds with SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Failed to initialize SDL Audio");
		return nullptr;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		fprintf(stderr, "Failed to open audio device");
		return nullptr;
	}

	// TODO: Add different music and load sound effects later
	background_music = Mix_LoadMUS(audio_path("combatMusic.wav").c_str());
	salmon_dead_sound = Mix_LoadWAV(audio_path("salmon_dead.wav").c_str());
	salmon_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav").c_str());
	hit_enemy_sound = Mix_LoadWAV(audio_path("hit_enemy.wav").c_str());
	fireball_explosion_sound = Mix_LoadWAV(audio_path("fireball_explosion_short.wav").c_str());
	death_enemy_sound = Mix_LoadWAV(audio_path("death_enemy.wav").c_str());
	fire_spell_sound = Mix_LoadWAV(audio_path("fireball_spell.wav").c_str()); //https://mixkit.co/free-sound-effects/spell/
	rock_spell_sound = Mix_LoadWAV(audio_path("rock_spell.wav").c_str()); //https://mixkit.co/free-sound-effects/spell/
	heal_spell_sound = Mix_LoadWAV(audio_path("heal_spell.wav").c_str()); //https://mixkit.co/free-sound-effects/spell/
	taunt_spell_sound = Mix_LoadWAV(audio_path("taunt_spell.wav").c_str()); //https://mixkit.co/free-sound-effects/spell/
	melee_spell_sound = Mix_LoadWAV(audio_path("melee_spell.wav").c_str()); //https://mixkit.co/free-sound-effects/spell/
	silence_spell_sound = Mix_LoadWAV(audio_path("silence_spell.wav").c_str());	//https://freesound.org/people/Vicces1212/sounds/123757/
	lightning_spell_sound = Mix_LoadWAV(audio_path("lightning_spell.wav").c_str()); //https://freesound.org/people/Puerta118m/sounds/471691/
	ice_spell_sound = Mix_LoadWAV(audio_path("ice_spell.wav").c_str()); //https://freesound.org/people/EminYILDIRIM/sounds/550267/
	summon_spell_sound = Mix_LoadWAV(audio_path("summon_spell.wav").c_str()); //https://freesound.org/people/alonsotm/sounds/396500/
	button_hover_sound = Mix_LoadWAV(audio_path("button_hover.wav").c_str()); //https://freesound.org/people/wobesound/sounds/488382/
	turning_sound = Mix_LoadWAV(audio_path("turning.wav").c_str());//https://freesound.org/people/InspectorJ/sounds/416179/
	charge_spell_sound = Mix_LoadWAV(audio_path("charge_spell.wav").c_str()); //https://freesound.org/people/18hiltc/sounds/186048/
	beam_spell_sound = Mix_LoadWAV(audio_path("beam_spell.wav").c_str()); //https://freesound.org/people/MATRIXXX_/sounds/403297/
	minion_spawn_sound = Mix_LoadWAV(audio_path("minion_spawn.wav").c_str()); //https://freesound.org/people/Breviceps/sounds/453391/
	error_sound = Mix_LoadWAV(audio_path("error.wav").c_str()); //https://freesound.org/people/plasterbrain/sounds/423169/
	gesture_heal_sound = Mix_LoadWAV(audio_path("gesture_heal.wav").c_str()); //https://freesound.org/people/SilverIllusionist/sounds/580814/
	gesture_aoe_sound = Mix_LoadWAV(audio_path("gesture_aoe.wav").c_str()); //https://freesound.org/people/humanoide9000/sounds/329029/
	gesture_turn_sound = Mix_LoadWAV(audio_path("gesture_turn.wav").c_str()); //https://freesound.org/people/Aleks41/sounds/406063/

	if (background_music == nullptr
		|| salmon_dead_sound == nullptr
		|| salmon_eat_sound == nullptr
		|| hit_enemy_sound == nullptr
		|| fireball_explosion_sound == nullptr
		|| death_enemy_sound == nullptr
		|| fire_spell_sound == nullptr
		|| rock_spell_sound == nullptr
		|| heal_spell_sound == nullptr
		|| taunt_spell_sound == nullptr
		|| melee_spell_sound == nullptr
		|| silence_spell_sound == nullptr
		|| lightning_spell_sound == nullptr
		|| ice_spell_sound == nullptr
		|| summon_spell_sound == nullptr
		|| button_hover_sound == nullptr
		|| turning_sound == nullptr
		|| summon_spell_sound == nullptr
		|| charge_spell_sound == nullptr
		|| beam_spell_sound == nullptr
		|| minion_spawn_sound == nullptr
		|| error_sound == nullptr
		|| gesture_heal_sound == nullptr
		|| gesture_aoe_sound == nullptr
		|| gesture_turn_sound == nullptr) {
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
			audio_path("gesture_turn.wav").c_str()
			);
		return nullptr;
	}
	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg, AISystem* ai_arg, SkillSystem* skill_arg) {
	this->renderer = renderer_arg;
	this->ai = ai_arg;
	this->sk = skill_arg;

	Mix_VolumeMusic(0);
	Mix_PlayMusic(background_music, -1);
	Mix_VolumeChunk(hit_enemy_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(fireball_explosion_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(death_enemy_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(fire_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(rock_spell_sound,MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(heal_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(taunt_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(melee_spell_sound, MIX_MAX_VOLUME / 10);
	Mix_VolumeChunk(silence_spell_sound, MIX_MAX_VOLUME / 8);
	Mix_VolumeChunk(lightning_spell_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(ice_spell_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(summon_spell_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(button_hover_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(charge_spell_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(beam_spell_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(minion_spawn_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(error_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(gesture_heal_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(gesture_aoe_sound, MIX_MAX_VOLUME);
	Mix_VolumeChunk(gesture_turn_sound, MIX_MAX_VOLUME);

	fprintf(stderr, "Loaded music\n");

	// Start game with a start screen
	render_startscreen();
}

void WorldSystem::render_startscreen() {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	createStoryBackground(renderer, {w/2, h/2}, 6);
	new_game_button = createUIButton(renderer, { 600, 400 }, NEW_GAME);
	load_game_button = createUIButton(renderer, { 600, 500 }, LOAD_GAME);
	exit_game_button = createUIButton(renderer, { 600, 600 }, EXIT_GAME);
	registry.motions.get(exit_game_button).scale = { 150, 70 };
}

void WorldSystem::displayPlayerTurn() {
	displayTurnIndicator(1);
	if (registry.charIndicator.components.size() != 0) {
		registry.remove_all_components_of(registry.charIndicator.entities[0]);
	}
	createCharIndicator(renderer, CURRPLAYER_LOCATION, currPlayer);
}

void WorldSystem::displayEnemyTurn() {
	displayTurnIndicator(0);
	if (registry.charIndicator.components.size() != 0) {
		registry.remove_all_components_of(registry.charIndicator.entities[0]);
	}
	createCharIndicator(renderer, CURRPLAYER_LOCATION, currPlayer);
}

void WorldSystem::displayTurnIndicator(int isPlayerTurn) {
	if (registry.turnIndicators.components.size() == 0) {
		// Only initialize this entity once!
		turn_indicator = isPlayerTurn ? createPlayerTurn(renderer, TURN_INDICATOR_LOCATION) : createEnemyTurn(renderer, TURN_INDICATOR_LOCATION);
	}
	else {
		Motion& motion = registry.motions.get(turn_indicator);
		RenderRequest& renderRequest = registry.renderRequests.get(turn_indicator);
		motion.position = TURN_INDICATOR_LOCATION;
		renderRequest.used_texture = isPlayerTurn ? TEXTURE_ASSET_ID::PLAYER_TURN : TEXTURE_ASSET_ID::ENEMY_TURN;
	}
}

void WorldSystem::iceShardAttack(Entity currPlayer) {
	// Mix_PlayChannel(-1, fire_spell_sound, 0); // added fire spell sound but doesnt work
	Motion enemy = registry.motions.get(currPlayer);
	if (!registry.deathTimers.has(currPlayer)) {
		Motion enemy = registry.motions.get(currPlayer);
		Entity resultEntity = createIceShard(renderer, { enemy.position.x, enemy.position.y }, 3.14159, { -100, 0 }, 0);
		Motion* ballacc = &registry.motions.get(resultEntity);
		ballacc->acceleration = vec2(1000 * -100 / ICESHARDSPEED, 1000 * 0 / ICESHARDSPEED);
		Projectile* proj = &registry.projectiles.get(resultEntity);
		proj->flyingTimer = 2000.f;
	}
}

std::vector<Entity> roundVec;
void WorldSystem::createRound() {

	std::vector<int> speedVec;
	for (int i = 0; i < registry.enemies.components.size(); i++) {	// iterate through all enemies to get speed stats
		Entity& entity = registry.enemies.entities[i];

		// also decrement taunt duration if present
		if (registry.taunts.has(entity)) {
			Taunt* t = &registry.taunts.get(entity);
			t->duration--;
		}
		if (registry.bleeds.has(entity)) {
			Bleed* b = &registry.bleeds.get(entity);
			b->duration--;
			sk->launchBleedDMG(entity, renderer);
			update_healthBars();
		}
		// also decrement silence duration if present
		if (registry.silenced.has(entity)) {
			Silenced* s = &registry.silenced.get(entity);
			s->turns--;
			if (s->turns <= 0) {			// remove silence to add speed stat later if turns <= 0
				sk->removeSilence(entity);
			}
		}
		// also decrement ultimate duration if present
		if (registry.ultimate.has(entity)) {	// need to emplace ultimate onto necro2 for countdown when David implements the skill
			Ultimate* u = &registry.ultimate.get(entity);
			u->ultiDuration--;
			printf("ENTITY IS %g \n", float(registry.stats.get(entity).speed));
			printf("MY ULTIDURATION IS %g \n", float(u->ultiDuration));
			// need to remove the skill when duration <= 0
			if (u->ultiDuration <= 0) {
				sk->removeUltimate(entity);
			}
		}
		// also decrement shield duration if present
		if (registry.shield.has(entity)) {	// need to emplace shield onto necro2 for countdown when David implements the skill
			Shield* sh = &registry.shield.get(entity);
			if (sh->shieldDuration > 0) {
				sh->shieldDuration--;
				printf("ENTITY IS %g \n", float(registry.stats.get(entity).speed));
				printf("MY SHIELD IS %g \n", float(sh->shieldDuration));
			}
			else {
				//registry.remove_all_components_of(entity);
				sk->removeShield(entity);
			}
			
		}

		if (!registry.silenced.has(entity)) {
			Statistics& checkSpeed = registry.stats.get(entity);
			if (checkSpeed.speed == 2) {	// add extra turn for necromancer, change this if necromancer speed changes
				speedVec.push_back(checkSpeed.speed);
			}
			speedVec.push_back(checkSpeed.speed);
		}
	}

	for (int i = 0; i < registry.shieldIcons.components.size(); i++) {
		ShieldIcon& sh = registry.shieldIcons.components[i];
		sh.shieldDuration -= 1;
	}

	for (int i = 0; i < registry.companions.components.size(); i++) {	// iterate through all companions to get speed stats
		Entity& entity = registry.companions.entities[i];

		// also decrement taunt duration if present
		if (registry.taunts.has(entity)) {
			Taunt* t = &registry.taunts.get(entity);
			t->duration--;
		}
		// also decrement bleed duration if present
		if (registry.bleeds.has(entity)) {
			Bleed* b = &registry.bleeds.get(entity);
			b->duration--;
			sk->launchBleedDMG(entity, renderer);
			update_healthBars();
		}
		// also decrement silence duration if present
		if (registry.silenced.has(entity)) {
			Silenced* s = &registry.silenced.get(entity);
			s->turns--;
			if (s->turns <= 0) {			// remove silence to add speed stat later if turns <= 0
				sk->removeSilence(entity);
			}
		}

		if (!registry.silenced.has(entity)) {
			Statistics& checkSpeed = registry.stats.get(entity);
			if (extraCompanionTurn <= 0) {	// if extraCompanionTurn <= 0, add extra turn for all companions on screen
				speedVec.push_back(checkSpeed.speed);
			}
			speedVec.push_back(checkSpeed.speed);
		}
	}

	std::sort(speedVec.begin(), speedVec.end(), std::greater<int>());	// sorts in descending order

	for (int i = 0; i < speedVec.size(); i++) {
		for (int j = 0; j < registry.companions.components.size(); j++) {
			Entity& entity = registry.companions.entities[j]; // check companions stats
			Statistics& checkSpeed = registry.stats.get(entity);
			if (speedVec[i] == checkSpeed.speed) {
				roundVec.push_back(entity);	// push to roundVec for use in checkRound
			}
		}
		for (int j = 0; j < registry.enemies.components.size(); j++) {
			Entity& entity = registry.enemies.entities[j]; // check enemies stats
			Statistics& checkSpeed = registry.stats.get(entity);
			if (speedVec[i] == checkSpeed.speed) {
				roundVec.push_back(entity);	// push to roundVec for use in checkRound
			}
		}
	}

	if (extraCompanionTurn <= 0) {
		extraCompanionTurn = 1;
	}

	// print the sorted array
	for (int i = 0; i < roundVec.size(); i++) {
		printf("%g \n", float(registry.stats.get(roundVec[i]).speed));
	}
}

void WorldSystem::checkRound() {
	printf("am here at checkRound \n");

	if (roundVec.empty()) {	// if empty, create new round
		printf("roundVec is empty, creating a new round \n");
		createRound();
	}

	Entity toPlay = roundVec[0]; // get first element
	printf("erase %g \n", float(registry.stats.get(roundVec[0]).speed));
	roundVec.erase(roundVec.begin());	// erase the first element

	if (registry.companions.has(toPlay) && registry.stats.get(toPlay).health > 0) {	// toPlay is companion, put to currPlayer to pass for fireball
		printf("its %g player turn \n", float(registry.stats.get(toPlay).speed));
		player_turn = 1;
		currPlayer = toPlay;
		//update skills display
		showCorrectSkills();
	}
	else if (registry.enemies.has(toPlay) && registry.stats.get(toPlay).health > 0) {	// toPlay is enemy, put to currPlayer to pass for fireball
		printf("its %g enemy turn \n", float(registry.stats.get(toPlay).speed));
		player_turn = 0;
		currPlayer = toPlay;
		//update skills display
		showCorrectSkills();
	}
	else {
		printf("no player or enemy, checking round now \n");
		prevPlayer = currPlayer;
		checkRound();
		//update skills display
		showCorrectSkills();
	}

	CURRPLAYER_LOCATION = registry.motions.get(currPlayer).position;	// get currPlayer location

	printf("finished check round \n");
	// print the current round
	for (int i = 0; i < roundVec.size(); i++) {
		printf("CURRENT ROUND IS %g \n", float(registry.stats.get(roundVec[i]).speed));
	}
	printf("playerUseMelee is %g \n", float(playerUseMelee));
}

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	// Get the screen dimensions
	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);

	if (renderer->transitioningToNextLevel) {
		renderer->nextLevelTranistionPeriod_ms -= elapsed_ms_since_last_update;
		if (renderer->nextLevelTranistionPeriod_ms <= 0.) {
			renderer->transitioningToNextLevel = false;
			renderer->nextLevelTranistionPeriod_ms = renderer->DEFAULT_GAME_LEVEL_TRANSITION_PERIOD_MS;
		}
	}

	// restart game if enemies or companions are 0
	if ((registry.enemies.size() <= 0 || registry.companions.size() <= 0) && (registry.Particles.size() <= 0)) {
		restart_game();
	}

	// Updating window title with volume control
	std::stringstream title_ss;
	title_ss << "Music volume (z-key , x-key): " << Mix_VolumeMusic(-1) << " ,   Effects volume (c-key , v-key): " << Mix_VolumeChunk(death_enemy_sound, -1) << " ";
	glfwSetWindowTitle(window, title_ss.str().c_str());

	// Remove debug info from the last step
	while (registry.debugComponents.entities.size() > 0)
		registry.remove_all_components_of(registry.debugComponents.entities.back());

	// Removing out of screen entities
	auto& motions_registry = registry.motions;

	//Remove barrier
	auto& reflects_registry = registry.reflects;

	// Remove entities that leave the screen on the left side
	// Iterate backwards to be able to remove without unterfering with the next object to visit
	// (the containers exchange the last element with the current)
	for (int i = (int)motions_registry.components.size() - 1; i >= 0; --i) {
		Motion& motion = motions_registry.components[i];
		if (motion.position.x + abs(motion.scale.x) < 0.f) {
			registry.remove_all_components_of(motions_registry.entities[i]);
		}
		
	}

	//collect mouse gesture
	if (startMousePosCollect == 1) {
		mouseGestures.push_back(msPos);
		// creates dots
		createDot(renderer, msPos);
	}
	else {
		for (int j = 0; j < registry.dots.components.size(); j++) {
			registry.remove_all_components_of(registry.dots.entities[j]);
		}
	}

	//check taunt bleed and silence for enemy and companion
	for (int i = (int)registry.enemies.components.size() - 1; i >= 0; --i) {
		if (registry.taunts.has(registry.enemies.entities[i])) {
			if (registry.taunts.get(registry.enemies.entities[i]).duration <= 0) {
				sk->removeTaunt(registry.enemies.entities[i]);
			}
		}
		if (registry.bleeds.has(registry.enemies.entities[i])) {
			if (registry.bleeds.get(registry.enemies.entities[i]).duration <= 0) {
				sk->removeBleed(registry.enemies.entities[i]);
			}
		}
		if (registry.silenced.has(registry.enemies.entities[i])) {
			if (registry.silenced.get(registry.enemies.entities[i]).turns <= 0) {
				sk->removeSilence(registry.enemies.entities[i]);
			}
		}
	}
	for (int i = (int)registry.companions.components.size() - 1; i >= 0; --i) {
		if (registry.taunts.has(registry.companions.entities[i])) {
			if (registry.taunts.get(registry.companions.entities[i]).duration <= 0) {
				sk->removeTaunt(registry.companions.entities[i]);
			}
		}
		if (registry.bleeds.has(registry.companions.entities[i])) {
			if (registry.bleeds.get(registry.companions.entities[i]).duration <= 0) {
				sk->removeBleed(registry.companions.entities[i]);
			}
		}
		if (registry.silenced.has(registry.companions.entities[i])) {
			if (registry.silenced.get(registry.companions.entities[i]).turns <= 0) {
				sk->removeSilence(registry.companions.entities[i]);
			}
		}
	}

	for (int i = (int)registry.shieldIcons.components.size() - 1; i >= 0; --i) {
		if (registry.shield.has(registry.shieldIcons.entities[i])) {	// need to emplace shield onto necro2 for countdown when David implements the skill
			ShieldIcon* sh = &registry.shieldIcons.get(registry.shieldIcons.entities[i]);
			if (sh->shieldDuration < 0) {
				registry.remove_all_components_of(registry.shield.entities[i]);
			}
			

		}
	}

	// maintain correct health
	for (int i = (int)registry.stats.components.size() - 1; i >= 0; --i) {
		if (registry.stats.components[i].health > registry.stats.components[i].max_health) {
			Statistics* stat = &registry.stats.components[i];
			stat->health = stat->max_health;
			update_healthBars();
		}
	}

	// Walk
	for (Entity runner : registry.runners.entities) {
		RunTowards& run = registry.runners.get(runner);
		Motion& runner_motion = registry.motions.get(runner);
		run.counter_ms -= elapsed_ms_since_last_update;
		if (run.counter_ms <= 0.f) {
			printf("Reached destination\n");
			runner_motion.velocity = { 0.f, 0.f };

			auto& anim_type = registry.companions.has(runner) ? registry.companions.get(runner).curr_anim_type
				: registry.enemies.get(runner).curr_anim_type;

			auto& runner_type = registry.companions.has(runner) ? registry.companions.get(runner).companionType
				: registry.enemies.get(runner).enemyType;

			Entity healthbar = registry.companions.has(runner) ? registry.companions.get(runner).healthbar
				: registry.enemies.get(runner).healthbar;
			Motion& healthbar_motion = registry.motions.get(healthbar);
			healthbar_motion.velocity = runner_motion.velocity;

			anim_type = ATTACKING;

			// Attack
			Attack& attack = registry.attackers.emplace(runner);
			attack.attack_type = MELEE;
			attack.old_pos = run.old_pos;
			attack.target = run.target;

			if (runner_type == SWORDSMAN) {
				attack.counter_ms = 1250.f;
			}
			else if (runner_type == NECROMANCER_MINION) {
				attack.counter_ms = 800.f;
			}
			else if (runner_type == NECROMANCER_TWO) {
				runner_motion.position = vec2(run.target_position.x - 25, runner_motion.position.y);
				if (run.bleedOrAOE == 0) {
					attack.attack_type = BLEEDMELEE;
				}
				else {
					attack.attack_type = AOEMELEE;
				}
				attack.counter_ms = 1000.f;
			}
			
			registry.runners.remove(runner);

			// Replace with better melee sound effect
			//Mix_PlayChannel(-1, melee_spell_sound, 0);
		}
	}

	// Attack
	for (Entity attacker : registry.attackers.entities) {
		Attack& attack = registry.attackers.get(attacker);
		// Updating animation time
		attack.counter_ms -= elapsed_ms_since_last_update;
		printf("Updating time : %f\n",attack.counter_ms);
		if (!registry.deathTimers.has(attacker)) {
			if (attack.counter_ms <= 0.f || attack.attack_type == SUMMON) {
				// Attack
				if (registry.companions.has(attacker)) {
					printf("Companion is attacking\n");
					Companion& companion = registry.companions.get(attacker);
					Motion& companion_motion = registry.motions.get(attacker);
					switch (attack.attack_type) {
					case FIREBALL: {
						Mix_PlayChannel(-1, fire_spell_sound, 0);
						printf("Fireball attack companion\n");
						currentProjectile = sk->launchFireball(companion_motion.position, attack.old_pos,renderer);
						Projectile* proj = &registry.projectiles.get(currentProjectile);
						proj->flyingTimer = 2000.f;
						break;
					}
					case TAUNT: {
						printf("Taunt attack companion\n");
						sk->launchTaunt(attack.target,renderer);
						//basiclly to have something hitting the boundary
						currentProjectile = sk->launchFireball({ -20,-20 }, { 0,0 },renderer); // TODO: change to new launch dummy projectile without sound
						Motion* projm = &registry.motions.get(currentProjectile);
						projm->velocity = { -100,0 };
						projm->acceleration = { -100,0 };
						break;
					}
					case ROCK: {
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, rock_spell_sound, 0);
						currentProjectile = sk->launchRock(attack.target,renderer);
						break;
					}
					case MELEE: {
						companion_motion.position = attack.old_pos;
						Motion& healthbar_motion = registry.motions.get(companion.healthbar);
						healthbar_motion.position.x = attack.old_pos.x;
						sk->launchMelee(attack.target,renderer);
						break;
					}
					case ICESHARD: {
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, ice_spell_sound, 0);
						currentProjectile = sk->launchIceShard(companion_motion.position, attack.old_pos,renderer);
						break;
					}
					case HEAL: {
						Mix_PlayChannel(-1, heal_spell_sound, 0);
						sk->launchHeal(attack.target, 50,renderer);
						update_healthBars();
						//basiclly to have something hitting the boundary
						currentProjectile = sk->launchFireball({ -20,-20 }, { 0,0 },renderer);
						Motion* projm = &registry.motions.get(currentProjectile);
						projm->velocity = { -100,0 };
						projm->acceleration = { -100,0 };
						break;
					}
					default: break;
					}
					companion.curr_anim_type = IDLE;
					printf("Not attacking anymore in idle\n");
					registry.attackers.remove(attacker);
				}
				else if (registry.enemies.has(attacker)) {
					printf("Enemy is attacking\n");
					Enemy& enemy = registry.enemies.get(attacker);
					switch (attack.attack_type) {
					case TAUNT: {
						//Mix_PlayChannel(-1, taunt_spell_sound, 0);	// sound moved to skill_system
						printf("taunt attack enemy\n");
						sk->launchTaunt(attack.target,renderer);
						break;
					}
					case SILENCE: {
						//Mix_PlayChannel(-1, silence_spell_sound, 0);	// sound moved to skill_system
						printf("taunt attack enemy\n");
						sk->launchSilence(attack.target, renderer);
						break;
					}
					case ROCK: {
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, rock_spell_sound, 0);
						printf("Rock attack enemy\n");
						currentProjectile = sk->launchRock(attack.target,renderer);
						// sk->startParticleBeamAttack(attacker);
						break;
					}
					case LIGHTNING: {
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, lightning_spell_sound, 0);
						printf("Lightning attack enemy\n");
						currentProjectile = sk->launchLightning(attack.target, renderer);
						break;
					}
					case MELEE: {
						printf("melee attack enemy\n");
						Motion& motion = registry.motions.get(attacker);
						motion.position = attack.old_pos;
						Motion& healthbar_motion = registry.motions.get(enemy.healthbar);
						healthbar_motion.position.x = attack.old_pos.x;
						sk->launchMelee(attack.target,renderer);
						break;
					}
					case ICESHARD: {
						printf("ice shard attack enemy\n");
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, ice_spell_sound, 0);
						iceShardAttack(attack.target); // TODO
						break;
					}
					case HEAL: {
						Mix_PlayChannel(-1, heal_spell_sound, 0);
						printf("heal attack enemy\n");
						sk->launchHeal(attack.target, 30,renderer);
						update_healthBars();
						break;
					}
					case SUMMONING: {
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, summon_spell_sound, 0);
						printf("summon necrominion \n");
						sk->launchSummon(renderer);
						break;
					}
					case ULTI: {
						printf("ultimate attack enemy \n");
						sk->launchParticleBeam(attack.target);
						break;
					}
					case CHARGING: {
						printf("ultimate charge enemy \n");
						currentProjectile = sk->launchParticleBeamCharge(attack.target, renderer);
						break;
					}
					case BLEEDMELEE: {
						Motion& motion = registry.motions.get(attacker);
						motion.position = attack.old_pos;
						Motion& healthbar_motion = registry.motions.get(enemy.healthbar);
						healthbar_motion.position.x = attack.old_pos.x;
						sk->launchMelee(attack.target, renderer);
						sk->launchBleed(attack.target, renderer);
						break;
					}
					case AOEMELEE: {
						Motion& motion = registry.motions.get(attacker);
						motion.position = attack.old_pos;
						Motion& healthbar_motion = registry.motions.get(enemy.healthbar);
						healthbar_motion.position.x = attack.old_pos.x;
						if (registry.motions.has(player_mage)) {
							sk->launchMelee(player_mage, renderer);
						}
						if (registry.motions.has(player_swordsman)) {
							sk->launchMelee(player_swordsman, renderer);
						}
						break;
					}
					case SHIELD: {
						sk->launchNecroBarrier(attacker, renderer);
						break;
					}
					default: break;
					}
					enemy.curr_anim_type = IDLE;
					printf("Not attacking anymore in idle\n");
					registry.attackers.remove(attacker);
				}
			}
		}
	}

	if (player_turn == 1) {
		displayPlayerTurn();
		prevPlayer = currPlayer;
	}

	// this area is to check for edge cases for enemy to attack during their turn
	if (player_turn == 0) {
		//printf("prevPlayer is: %g \n", float(registry.stats.get(prevPlayer).speed));
		//printf("currPlayer is: %g \n", float(registry.stats.get(currPlayer).speed));

				if ((registry.checkRoundTimer.size()<=0)&&(registry.companions.size()>0)) {
					displayEnemyTurn();
					if (registry.enemies.has(currPlayer)) {	// check if enemies have currPlayer
						printf("Calling tree here\n");
						ai->callTree(currPlayer);
						prevPlayer = currPlayer;	// added this line to progress the necromancer phase 2 turn 2 after the first turn's tree is called, not sure if it will affect other things, need more testing
					}
					else {
						if (roundVec.empty()) {
							printf("roundVec is empty at enemy turn, createRound now \n");
							createRound();
						}
					}
				}
	}

	// Processing the salmon state
	assert(registry.screenStates.components.size() <= 1);
	ScreenState& screen = registry.screenStates.components[0];

	// update state of particles
	for (Entity entity : registry.Particles.entities) {
		Particle& particles = registry.Particles.get(entity);
		if (particles.areTypeDeath) {
			for (int i = 0; i < particles.deathParticles.size(); i++) {
				auto& particle = particles.deathParticles[i];
				// for (auto& particle : deathParticles.deathParticles) {
				particle.Life -= elapsed_ms_since_last_update;
				// if (particle.Life > 0.f) {
				if (particle.Life <= 0) {
					particles.fadedParticles++;
					delete[] particle.positions;
				}
				particle.motion.position.x -= particle.motion.velocity.y * (rand() % 17) * 0.3f;
				particle.motion.position.y -= particle.motion.velocity.x * (rand() % 17) * 0.3f;
				particle.Color.a -= 0.05f * 0.01f;
				particle.motion.angle += 0.5;
				if (particle.motion.angle >= (2 * M_PI)) {
					particle.motion.angle = 0;
				}
				particles.positions[i * 3 + 0] = particle.motion.position.x;
				particles.positions[i * 3 + 1] = particle.motion.position.y;
				particles.positions[i * 3 + 2] = particle.Life / particles.Life;
				// deathParticles.positions[i * 3 + 3] = particle.Life;
				// }
			}
			if (particles.fadedParticles == NUM_DEATH_PARTICLES) {
				delete[] particles.positions;
				particles.faded = true;
				registry.Particles.remove(entity);
				registry.remove_all_components_of(entity);	// added back in, kinda works
			}
		}
		else {
			int w, h;
			glfwGetFramebufferSize(window, &w, &h);
			auto maybeUpdateHealth = sk->updateParticleBeam(entity, elapsed_ms_since_last_update, (float)w, (float)h);
			if (maybeUpdateHealth.second == true) {
				// printf("life reached below 50%\n");
			}
			if (maybeUpdateHealth.first && !maybeUpdateHealth.second) {
				for (Entity entity : registry.companions.entities) {
					Statistics& stat = registry.stats.get(entity);
					stat.health -= 0.0001;
					// printf("inside\n");
					if (stat.health > 0) {
						// update_healthBars();
						Companion& companion = registry.companions.get(entity);
						Entity healthbar = companion.healthbar;
						Motion& motion = registry.motions.get(healthbar);
						motion.scale = vec2({ (HEALTHBAR_WIDTH * (stat.health / 100.f)), HEALTHBAR_HEIGHT });
					}
				}
			}
			//else {
			//	printf("outside\n");
			//}
		}
	}

	float min_counter_ms = 3000.f;
	for (Entity entity : registry.deathTimers.entities) {
		// progress timer
		DeathTimer& counter = registry.deathTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		if (counter.counter_ms < min_counter_ms) {
			min_counter_ms = counter.counter_ms;
		}

		// restart the game once the death timer expired
		if (counter.counter_ms < 0) {
			registry.deathTimers.remove(entity);
			if(registry.companions.has(entity)){
				registry.companions.remove(entity);
			} else if(registry.enemies.has(entity)){
				registry.enemies.remove(entity);
			}
			activate_deathParticles(entity);
			screen.darken_screen_factor = 0;
			// restart_game();
			return true;
		}
	}
	// Handle flying projectile timer
	for (Entity entity : registry.projectiles.entities) {
		Projectile& proj = registry.projectiles.get(entity);
		proj.flyingTimer -= elapsed_ms_since_last_update;
	}

	// update timer for enemy to return to its original position after being hit
	float min_counter_ms_2 = 500.f;
	for (Entity entity : registry.hit_timer.entities) {
		// progress timer
		HitTimer& hitCounter = registry.hit_timer.get(entity);
		hitCounter.counter_ms -= elapsed_ms_since_last_update;
		if (hitCounter.counter_ms < min_counter_ms_2) {
			min_counter_ms_2 = hitCounter.counter_ms;
		}

		if (hitCounter.counter_ms < 0) {
			registry.hit_timer.remove(entity);
			// check if entity is enemy or companion		
			if (!registry.deathTimers.has(entity)) {
				if (registry.companions.has(entity)) {
					registry.motions.get(entity).position.x += hit_position;
				}
				else {
					registry.motions.get(entity).position.x -= hit_position;
				}
			}
			return true;
		}
	}

	// update timer for checking round
	float min_counter_ms_3 = 4000.f;
	for (Entity entity : registry.checkRoundTimer.entities) {
		// progress timer
		CheckRoundTimer& timerCounter = registry.checkRoundTimer.get(entity);
		timerCounter.counter_ms -= elapsed_ms_since_last_update;
		if (timerCounter.counter_ms < min_counter_ms_3) {
			min_counter_ms_3 = timerCounter.counter_ms;
		}

		// check round once the timer expired
		if (timerCounter.counter_ms < 0) {
			registry.checkRoundTimer.remove(entity);
			printf("check round timer finished, checking round now \n");
			checkRound();
			return true;
		}
	}
	return true;
}

// Reset the world state to its initial state
void WorldSystem::restart_game(bool force_restart) {

	if (registry.companions.size() > 0 && registry.enemies.size() == 0) {
		gameLevel++;
		renderer->transitioningToNextLevel = true;
		renderer->gameLevel = gameLevel;
	}
	if (gameLevel > MAX_GAME_LEVELS) {
		gameLevel = 1;
		renderer->gameLevel = gameLevel;
	}
	if (registry.companions.size() == 0) {
		gameLevel = 1;
		renderer->gameLevel = gameLevel;
		// renderer->transitioningToNextLevel = true;
	}
	if (force_restart) {
		gameLevel = 1;
		renderer->gameLevel = gameLevel;
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

	// Debugging for memory/component leaks
	registry.list_all_components();

	int w, h;
	glfwGetWindowSize(window, &w, &h);
	// Render background before all else

	// Layer 1 (Last layer in background)
	createBackgroundLayerOne(renderer, { w / 2, h / 2 });
	// Layer 3
	createBackgroundLayerThree(renderer, { w / 2, h / 2 });
	// Layer 2
	createBackgroundLayerTwo(renderer, { w / 2, h / 2 });
	// Layer 4 (Foremost layer)
	createBackgroundLayerFour(renderer, { w / 2, h / 2 });

	// Pause menu button
	open_menu_button = createUIButton(renderer, { 100, 100 }, OPEN_MENU);

	// Create a player mage
	player_mage = createPlayerMage(renderer, { 150, 550 });
	// Create a player swordsman
	// player_swordsman = createPlayerSwordsman(renderer, { 275, 500 });
	// Create an enemy mage
	// enemy_mage = createEnemyMage(renderer, { 1050, 575 });
	// registry.colors.insert(enemy_mage, { 0.0, 0.0, 1.f });

	createBackgroundObject(renderer, { 1160, 315 });
	auto ent = createBackgroundObject(renderer, { 550, 325 });
	registry.backgroundObjects.get(ent).deformType2 = true;

	player_swordsman = createPlayerSwordsman(renderer, { 350, 450 });
	//// Create an enemy mage
	// enemy_mage = createEnemyMage(renderer, { 1050, 575 });
	// registry.colors.insert(enemy_mage, { 0.0, 0.0, 1.f });

	//necromancer_phase_one = createNecromancerPhaseOne(renderer, { 1000, 550 });
	necromancer_phase_two = createNecromancerPhaseTwo(renderer, { 900, 400 });
	//necromancer_minion = createNecromancerMinion(renderer, { 750, 550 });
	// registry.colors.insert(necromancer_phase_two, { 0.5, 0.5, 0.5 });

	if (gameLevel > 1) {
		// Create an enemy swordsman
		enemy_swordsman = createEnemySwordsman(renderer, { 875, 500 });
		registry.colors.insert(enemy_swordsman, { 0.f, 1.f, 1.f });
	}

	// Create the icons here
	taunt_icon = createTauntIcon(renderer, { 400, 700 });
	heal_icon = createHealIcon(renderer, { 550, 700 });
	melee_icon = createMeleeIcon(renderer, { 700, 700 });
	iceShard_icon = createIceShardIcon(renderer, { 850, 700 });
	fireBall_icon = createFireballIcon(renderer, { 1000, 700 });
	rock_icon = createRockIcon(renderer, { 1150, 700 });

	//Create a tooltip
	tooltip;
	player_turn = 1;	// player turn indicator
	gestureSkillRemaining = 1; // reset gesture skill remaining
	roundVec.clear();	// empty vector roundVec to create a new round
	createRound();
	checkRound();
	showCorrectSkills();
	displayPlayerTurn();	// display player turn when restart game

	// Create the first tutorial box
	tutorial_enabled = 1;
	curr_tutorial_box = createTutorialBox(renderer, { 600, 300 });
	curr_tutorial_box_num = 0;
}

void WorldSystem::update_health(Entity entity, Entity other_entity) {
	if (registry.projectiles.has(entity)) {
		Damage& damage = registry.damages.get(entity);
		Statistics* hp = nullptr;
		Entity healthbar;
		if (damage.isFriendly) {
			if (registry.enemies.has(other_entity)) {
				Enemy& enemy = registry.enemies.get(other_entity);
				healthbar = enemy.healthbar;
				hp = &registry.stats.get(other_entity);
			}
		}
		else {
			if (registry.companions.has(other_entity)) {
				Companion& companion = registry.companions.get(other_entity);
				healthbar = companion.healthbar;
				hp = &registry.stats.get(other_entity);
			}
		}
		if (hp) {
			hp->health = hp->health - (rand() % damage.range + damage.minDamage);
			Motion& motion = registry.motions.get(healthbar);
			if (registry.stats.get(currPlayer).health <= 0) {	// check if HP of currPlayer is 0, checkRound to skip this player
				if (!registry.deathTimers.has(other_entity)) {
					auto& deathTimer = registry.deathTimers.emplace(other_entity);

					prevPlayer = currPlayer;	// needed to allow checking of the edge case where the enemy dies and the next enemy goes

					if (!registry.checkRoundTimer.has(currPlayer)) {
						auto& timer = registry.checkRoundTimer.emplace(currPlayer);
						timer.counter_ms = deathTimer.counter_ms + animation_timer;
					}
					if (registry.companions.has(other_entity)) {
						printf("Companion is dead\n");
						Companion& companion = registry.companions.get(other_entity);
						companion.curr_anim_type = DEAD;
					}
					else if (registry.enemies.has(other_entity)) {
						printf("Enemy is dead\n");
						Enemy& enemy = registry.enemies.get(other_entity);
						enemy.curr_anim_type = DEAD;
					}
				}
				motion.scale = vec2({ (HEALTHBAR_WIDTH * (0.f / 100.f)), HEALTHBAR_HEIGHT });
			}
			else {
				if (hp->health <= 0) {
					if (!registry.deathTimers.has(other_entity)) {
						auto& deathTimer = registry.deathTimers.emplace(other_entity);

						prevPlayer = currPlayer;	// needed to allow checking of the edge case where the enemy dies and the next enemy goes

						if (!registry.checkRoundTimer.has(currPlayer)) {
							auto& timer = registry.checkRoundTimer.emplace(currPlayer);
							timer.counter_ms = deathTimer.counter_ms + animation_timer;
						}
						if (registry.companions.has(other_entity)) {
							printf("Companion is dead\n");
							Companion& companion = registry.companions.get(other_entity);
							companion.curr_anim_type = DEAD;
						}
						else if (registry.enemies.has(other_entity)) {
							printf("Enemy is dead\n");
							Enemy& enemy = registry.enemies.get(other_entity);
							enemy.curr_anim_type = DEAD;
						}
					}
					motion.scale = vec2({ (HEALTHBAR_WIDTH * (0.f / 100.f)), HEALTHBAR_HEIGHT });
				}
				else {
					motion.scale = vec2({ (HEALTHBAR_WIDTH * (hp->health / 100.f)), HEALTHBAR_HEIGHT });
				}
			}
		}
	}
}

void WorldSystem::update_healthBars() {
	for (Entity entity : registry.enemies.entities) {
		Enemy& enemy = registry.enemies.get(entity);
		Statistics& stat = registry.stats.get(entity);
		Entity healthbar = enemy.healthbar;
		Motion& motion = registry.motions.get(healthbar);
		motion.scale = vec2({ (HEALTHBAR_WIDTH * (stat.health / 100.f)), HEALTHBAR_HEIGHT });
	}
	for (Entity entity : registry.companions.entities) {
		Companion& enemy = registry.companions.get(entity);
		Statistics& stat = registry.stats.get(entity);
		Entity healthbar = enemy.healthbar;
		Motion& motion = registry.motions.get(healthbar);
		motion.scale = vec2({ (HEALTHBAR_WIDTH * (stat.health / 100.f)), HEALTHBAR_HEIGHT });
	}
}

void WorldSystem::activate_deathParticles(Entity entity)
{
	Particle particleEffects;
	particleEffects.motion.scale = vec2(10.f, 10.f);
	
	for (int p = 0; p < NUM_DEATH_PARTICLES; p++) {
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
		particleEffects.deathParticles.push_back(particle);
		particleEffects.positions[p * 3 + 0] = particle.motion.position.x;
		particleEffects.positions[p * 3 + 1] = particle.motion.position.y;
		particleEffects.positions[p * 3 + 2] = particle.Life/ particleEffects.Life;
		// particleEffects.positions[p * 4 + 3] = particle.Life;
	}
	if (!registry.Particles.has(entity)) {
		registry.Particles.insert(entity, particleEffects);
	}
}

// Compute collisions between entities
void WorldSystem::handle_collisions() {
	// reduce turn
	// Loop over all collisions detected by the physics system

	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		// Deal with fireball - Companion collisions
		if (registry.companions.has(entity)) {

			// Checking Projectile - Companion collisions
			if (registry.projectiles.has(entity_other)) {

				Damage& projDamage = registry.damages.get(entity_other);
				if (projDamage.isFriendly == 0) {	// check if isFriendly = 0 which hits companion
					// initiate death unless already dying
					if (!registry.deathTimers.has(entity)) {
						if (!registry.buttons.has(entity)) {

							update_health(entity_other, entity);
							registry.remove_all_components_of(entity_other);
							Mix_PlayChannel(-1, fireball_explosion_sound, 0); // added fireball hit sound
							showCorrectSkills();
							if (registry.stats.has(entity) && registry.stats.get(entity).health <= 0) {
								Mix_PlayChannel(-1, death_enemy_sound, 0); // added enemy death sound
							}
							else {
								Mix_PlayChannel(-1, hit_enemy_sound, 0); // new enemy hit sound							
							}
							// update only if hit_timer for entity does not already exist
							if (!registry.hit_timer.has(entity)) {
								registry.motions.get(entity).position.x -= 20; // character shifts backwards
								registry.hit_timer.emplace(entity); // to move character back to original position
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
				if (registry.companions.has(entity)) {
					printf("Companion is dead\n");
					Companion& companion = registry.companions.get(entity);
					companion.curr_anim_type = DEAD;
				}
				else if (registry.enemies.has(entity)) {
					printf("Enemy is dead\n");
					Enemy& enemy = registry.enemies.get(entity);
					enemy.curr_anim_type = DEAD;
				}
			}
		}
		// Deal with fireball - Enemy collisions
		else if (registry.enemies.has(entity)) {
			// Checking Projectile - Enemy collisions
			if (registry.projectiles.has(entity_other)) {
				Damage& projDamage = registry.damages.get(entity_other);
				if (projDamage.isFriendly == 1) {	// check if isFriendly = 1 which hits enemy
					// initiate death unless already dying
					if (!registry.deathTimers.has(entity)) {
						if (!registry.buttons.has(entity)) {

							update_health(entity_other, entity);
							registry.remove_all_components_of(entity_other);
							Mix_PlayChannel(-1, fireball_explosion_sound, 0); // added fireball hit sound
							if (registry.stats.has(entity) && registry.stats.get(entity).health <= 0) {
								// get rid of dead entity's stats indicators 
								sk->removeTaunt(entity);
								sk->removeSilence(entity);
								sk->removeBleed(entity);
								Mix_PlayChannel(-1, death_enemy_sound, 0); // added enemy death sound
							}

							else {
								Mix_PlayChannel(-1, hit_enemy_sound, 0); // new enemy hit sound							
							}
							// update only if hit_timer for entity does not already exist
							if (!registry.hit_timer.has(entity)) {
								registry.motions.get(entity).position.x += 20; // character shifts backwards
								registry.hit_timer.emplace(entity); // to move character back to original position
							}

							//enemy turn start
						
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
				if (registry.companions.has(entity)) {
					printf("Companion is dead\n");
					Companion& companion = registry.companions.get(entity);
					companion.curr_anim_type = DEAD;
				}
				else if (registry.enemies.has(entity)) {
					printf("Enemy is dead\n");
					Enemy& enemy = registry.enemies.get(entity);
					enemy.curr_anim_type = DEAD;
				}
			}
		}
		// handle collisions with background objects
		if (registry.backgroundObjects.has(entity) && registry.projectiles.has(entity_other)) {
			auto& backgroundObj = registry.backgroundObjects.get(entity);
			backgroundObj.shouldDeform = true;
			Mix_PlayChannel(-1, fireball_explosion_sound, 0);
			registry.remove_all_components_of(entity_other);
			//enemy turn start
			if (player_turn == 0) {
				if (!registry.checkRoundTimer.has(currPlayer)) {
					displayEnemyTurn();
					if (registry.enemies.has(currPlayer)) {	// check if enemies have currPlayer
						ai->callTree(currPlayer);
					}
					else {
						if (roundVec.empty()) {
							printf("roundVec is empty at enemy turn, createRound now \n");
							createRound();
						}
					}
				}
			}
		}
		// barrier collection
		else if (registry.projectiles.has(entity)) {
			if (registry.reflects.has(entity_other)) {
				//printf("colleds\n");
				//printf("%f\n", registry.motions.get(entity).velocity.x);
				if (registry.motions.get(entity).velocity.x > 0.f) {
					//printf("colleds1");
					Motion* reflectEM = &registry.motions.get(entity);

					reflectEM->velocity = vec2(-registry.motions.get(entity).velocity.x, reflectEM->velocity.y);
					reflectEM->acceleration = vec2(-registry.motions.get(entity).acceleration.x, reflectEM->acceleration.y);
					printf("before %f\n", reflectEM->angle);
					float reflectE = atan(registry.motions.get(entity).velocity.y / registry.motions.get(entity).velocity.x);
					if (registry.motions.get(entity).velocity.x < 0) {
						reflectE += M_PI;
					}
					reflectEM->angle = reflectE;
					printf("calculated %f\n", reflectE);
					printf("actual %f\n", reflectEM->angle);
				}
			}
		}
	}
	// Remove all collisions from this simulation step
	registry.collisions.clear();
}

void WorldSystem::handle_boundary_collision() {
	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	auto& projectilesRegistry = registry.projectiles;
	for (uint i = 0; i < projectilesRegistry.components.size(); i++) {
		Entity entity = projectilesRegistry.entities[i];
		if (registry.motions.get(entity).position.x <= 0 - 20 ||
			registry.motions.get(entity).position.x >= screen_width + 20 ||
			registry.motions.get(entity).position.y <= 0 - 20 ||
			registry.motions.get(entity).position.y >= screen_height + 20) {
			registry.remove_all_components_of(entity);
			registry.remove_all_components_of(entity);
			Mix_PlayChannel(-1, fireball_explosion_sound, 0);
			//enemy turn start
			
		}
	}
}

// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window)) || closeWindow;
}

// On key callback
void WorldSystem::on_key(int key, int, int action, int mod) {
	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

		restart_game(true);
	}

	// david test
	//if (action == GLFW_RELEASE && key == GLFW_KEY_Q) {
	//	sk->luanchNecroCompanionTeamBleed(renderer);
	//}

	//if (action == GLFW_RELEASE && key == GLFW_KEY_W) {
	//	sk->launchSpike(player_mage, renderer);
	//}

	if (action == GLFW_RELEASE && key == GLFW_KEY_E) {
		sk->launchNecroBarrier(necromancer_phase_two, renderer);
	}

	// Debugging
	if (key == GLFW_KEY_D) {
		if (action == GLFW_RELEASE)
			debugging.in_debug_mode = false;
		else
			debugging.in_debug_mode = true;
	}

	// testing particle beam
	//if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
	//	if (!registry.Particles.has(necromancer_phase_two)) {
	//		sk->startParticleBeamAttack(necromancer_phase_two);
	//	}
	//}

	// testing deformation of mesh. NOTE: render_system also needs to be updated
	// to use this
	/*if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
		renderer->shouldDeform = 1;
	}*/

	// Volume control (z = Decrease BGM vol., x = Increase BGM vol., c = Decrease effects vol., v = Increase effects vol.)
	if (action == GLFW_RELEASE && key == GLFW_KEY_Z) {
		Mix_VolumeMusic(Mix_VolumeMusic(-1) - MIX_MAX_VOLUME / 10);
	}
	if (action == GLFW_RELEASE && key == GLFW_KEY_X) {
		Mix_VolumeMusic(Mix_VolumeMusic(-1) + MIX_MAX_VOLUME / 10);
	}
	if (action == GLFW_RELEASE && key == GLFW_KEY_C) {
		Mix_VolumeChunk(hit_enemy_sound, Mix_VolumeChunk(hit_enemy_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(fireball_explosion_sound, Mix_VolumeChunk(fireball_explosion_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(death_enemy_sound, Mix_VolumeChunk(death_enemy_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(fire_spell_sound, Mix_VolumeChunk(fire_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(rock_spell_sound, Mix_VolumeChunk(rock_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(heal_spell_sound, Mix_VolumeChunk(heal_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(taunt_spell_sound, Mix_VolumeChunk(taunt_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(melee_spell_sound, Mix_VolumeChunk(melee_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(silence_spell_sound, Mix_VolumeChunk(silence_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(lightning_spell_sound, Mix_VolumeChunk(lightning_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(ice_spell_sound, Mix_VolumeChunk(ice_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(summon_spell_sound, Mix_VolumeChunk(summon_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(button_hover_sound, Mix_VolumeChunk(button_hover_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(charge_spell_sound, Mix_VolumeChunk(charge_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(beam_spell_sound, Mix_VolumeChunk(beam_spell_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(minion_spawn_sound, Mix_VolumeChunk(minion_spawn_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(error_sound, Mix_VolumeChunk(error_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(gesture_heal_sound, Mix_VolumeChunk(gesture_heal_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(gesture_aoe_sound, Mix_VolumeChunk(gesture_aoe_sound, -1) - MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(gesture_turn_sound, Mix_VolumeChunk(gesture_turn_sound, -1) - MIX_MAX_VOLUME / 10);
	}
	if (action == GLFW_RELEASE && key == GLFW_KEY_V) {
		Mix_VolumeChunk(hit_enemy_sound, Mix_VolumeChunk(hit_enemy_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(fireball_explosion_sound, Mix_VolumeChunk(fireball_explosion_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(death_enemy_sound, Mix_VolumeChunk(death_enemy_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(fire_spell_sound, Mix_VolumeChunk(fire_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(rock_spell_sound, Mix_VolumeChunk(rock_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(heal_spell_sound, Mix_VolumeChunk(heal_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(taunt_spell_sound, Mix_VolumeChunk(taunt_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(melee_spell_sound, Mix_VolumeChunk(melee_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(silence_spell_sound, Mix_VolumeChunk(silence_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(lightning_spell_sound, Mix_VolumeChunk(lightning_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(ice_spell_sound, Mix_VolumeChunk(ice_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(summon_spell_sound, Mix_VolumeChunk(summon_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(button_hover_sound, Mix_VolumeChunk(button_hover_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(charge_spell_sound, Mix_VolumeChunk(charge_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(beam_spell_sound, Mix_VolumeChunk(beam_spell_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(minion_spawn_sound, Mix_VolumeChunk(minion_spawn_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(error_sound, Mix_VolumeChunk(error_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(gesture_heal_sound, Mix_VolumeChunk(gesture_heal_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(gesture_aoe_sound, Mix_VolumeChunk(gesture_aoe_sound, -1) + MIX_MAX_VOLUME / 10);
		Mix_VolumeChunk(gesture_turn_sound, Mix_VolumeChunk(gesture_turn_sound, -1) + MIX_MAX_VOLUME / 10);
	}
}

void WorldSystem::on_mouse_button(int button, int action, int mods)
{
	// For start menu and pause menu click detection
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && !story) {
		if (inButton(registry.motions.get(new_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {

			// Direct to background story telling first
			int w, h;
			glfwGetWindowSize(window, &w, &h);

			backgroundImage = createStoryBackground(renderer, { w / 2,h / 2 }, 1);
			dialogue = createDiaogue(renderer, { w / 2, 650}, 1);

			story = 1;
		}
		else if (inButton(registry.motions.get(load_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {
			// LOAD THE SAVED JSON FILE (IF ANY)
			// Todo: implement
		}
		else if (inButton(registry.motions.get(exit_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {
			// EXIT TO DESKTOP
			closeWindow = 1;
		}
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 1) {
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2 ,h / 2 }, 2);
		registry.remove_all_components_of(dialogue);
		dialogue = createDiaogue(renderer, { w / 2, 650 }, 2);
		story = 2;

	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 2) {
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		dialogue = createDiaogue(renderer, { w / 2, 650 }, 3);
		story = 3;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 3) {
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2,h / 2 }, 3);
		registry.remove_all_components_of(dialogue);
		dialogue = createDiaogue(renderer, { w / 2, 650 }, 4);
		story = 4;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 4) {
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2,h / 2 }, 4);
		registry.remove_all_components_of(dialogue);
		story = 5;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 5) {
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		dialogue = createDiaogue(renderer, { w / 2, 650 }, 5);
		story = 6;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 6) {
		Mix_Volume(5, 32);
		Mix_PlayChannel(5, turning_sound, 0);
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		registry.remove_all_components_of(dialogue);
		registry.remove_all_components_of(backgroundImage);
		backgroundImage = createStoryBackground(renderer, { w / 2,h / 2 }, 5);
		story = 7;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !canStep && story == 7) {
		// START A NEW GAME			
		restart_game();
		canStep = 1;
	}


	//gesture skill
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && canStep) {
		printf("gesture skill collecting active!\n");
		startMousePosCollect = 1;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE && canStep) {
		startMousePosCollect = 0;
		float maxX = -9999;
		float minX = 9999;
		float maxY = -9999;
		float minY = 9999;
		float aveX;
		float aveY;
		float totalX=0;
		float totalY=0;
		// Y increasing means going down! idk why
		float Yincreasing_switch = 1;
		float Xincreasing_switch = 1;
		float Ydecreasing_switch = 1;
		float Xdecreasing_switch = 1;
		
		for (int i = 0; i < mouseGestures.size(); i++) {
			// print out conditions
			printf("collected mousePos: X=%f Y=%f\n",mouseGestures[i].x, mouseGestures[i].y);
			totalX += mouseGestures[i].x;
			totalY += mouseGestures[i].y;
			if (mouseGestures[i].x > maxX) {
				if (maxX != -9999) {
					Xdecreasing_switch = 0;
				}
				maxX = mouseGestures[i].x;
			}
			if (mouseGestures[i].x < minX) {
				if (minX != 9999) {
					Xincreasing_switch = 0;
				}
				minX = mouseGestures[i].x;
			}
			if (mouseGestures[i].y > maxY) {
				if (maxY != -9999) {
					Ydecreasing_switch = 0;
				}
				maxY = mouseGestures[i].y;
			}
			if (mouseGestures[i].y < minY) {
				if (minY != 9999) {
					Yincreasing_switch = 0;
				}
				minY = mouseGestures[i].y;
			}
		}
		printf("gesture skill collecting deactive!\n");
		aveX = totalX / mouseGestures.size();
		aveY = totalY / mouseGestures.size();
		printf("Ave X is %f, Ave Y is %f, MaxminX is %f %f, MixminY is %f %f\n",aveX, aveY, maxX, minX, maxY, minY );
		printf("X inc:%f X dec:%f Yinc:%f Y dec:%f\n",Xincreasing_switch,Xdecreasing_switch,Yincreasing_switch,Ydecreasing_switch);
		
		// horisontal line - skill 1: heal
		if (gestureSkillRemaining > 0) {
			if (Xincreasing_switch == 1 && Xdecreasing_switch == 0 &&
				maxX - aveX >= 200 && aveX - minX >= 200 &&
				maxY - aveY <= 50 && aveY - minY <= 50) {
				// launch heal skill
				Mix_Volume(5, 32);
				Mix_PlayChannel(5, gesture_heal_sound, 0);
				sk->luanchCompanionTeamHeal(50, renderer);
				update_healthBars();
				gestureSkillRemaining--;	// decrement gestureSkillRemaining
				printf("heal Gesture skill activated!");
			}
			// vertical line -skill 2: aoe damage
			if (Yincreasing_switch == 1 && Ydecreasing_switch == 0 &&
				maxX - aveX <= 50 && aveX - minX <= 50 &&
				maxY - aveY >= 150 && aveY - minY >= 150) {
				// launch aoe damage skill
				Mix_Volume(5, 32);
				Mix_PlayChannel(5, gesture_aoe_sound, 0);
				sk->luanchEnemyTeamDamage(30, renderer);
				update_healthBars();
				gestureSkillRemaining--;	// decrement gestureSkillRemaining
				printf("damage Gesture skill activated!");
			}
			// circle - skill 3: one more turn
			if (Xincreasing_switch == 0 && Xdecreasing_switch == 0 &&
				Yincreasing_switch == 0 && Ydecreasing_switch == 0 &&
				maxX - aveX <= 300 && aveX - minX <= 300 &&
				maxY - aveY <= 300 && aveY - minY <= 300) {
				// launch extra one turn
				Mix_Volume(5, 32);
				Mix_PlayChannel(5, gesture_turn_sound, 0);
				extraCompanionTurn--;
				gestureSkillRemaining--;	// decrement gestureSkillRemaining
				printf("one more turn skill activated!");
			}
		}
		else {
			Mix_Volume(5, 32);
			Mix_PlayChannel(5, error_sound, 0);
		}

	mouseGestures.clear();

	}
	//other clicks
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && canStep) {


		// Check menu clicks
		if (!pauseMenuOpened && inButton(registry.motions.get(open_menu_button).position, UI_BUTTON_HEIGHT, UI_BUTTON_HEIGHT)) {
			Motion menu_motion = registry.motions.get(open_menu_button);
			save_game_button = createUIButton(renderer, { menu_motion.position.x + menu_motion.scale.x / 2, menu_motion.position.y + menu_motion.scale.y / 3 + UI_BUTTON_HEIGHT }, SAVE_GAME);
			exit_game_button = createUIButton(renderer, { menu_motion.position.x + menu_motion.scale.x / 2, menu_motion.position.y + menu_motion.scale.y / 3 + UI_BUTTON_HEIGHT * 2 }, EXIT_GAME);
			registry.motions.get(exit_game_button).scale = { 200,80 };
			pauseMenuOpened = 1;
			registry.renderRequests.get(open_menu_button).used_texture = TEXTURE_ASSET_ID::CLOSE_MENU;
		}
		else if (pauseMenuOpened) {
			
			if (inButton(registry.motions.get(save_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {
				// SAVE THE CURRENT GAME STATE
				pauseMenuOpened = 0;
				// Todo: implement

			}
			
			if (inButton(registry.motions.get(exit_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {
				// GO BACK TO START MENU
				pauseMenuOpened = 0;
				canStep = 0;
				story = 0;
				while (registry.motions.entities.size() > 0)
					registry.remove_all_components_of(registry.motions.entities.back());
				render_startscreen();
				return;
			}
			
			if (inButton(registry.motions.get(open_menu_button).position, UI_BUTTON_HEIGHT, UI_BUTTON_HEIGHT)) {
				// HIDE MENU OPTIONS
				registry.renderRequests.get(open_menu_button).used_texture = TEXTURE_ASSET_ID::OPEN_MENU;
				// Clear on-screen buttons
				registry.remove_all_components_of(save_game_button);
				registry.remove_all_components_of(exit_game_button);
				pauseMenuOpened = 0;
			}	
		}


		//show all skill
		if (selected_skill != 0) {
			registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICON;
		}
		if (selected_skill != 1) {
			registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICON;
		}
		if (selected_skill != 2) {
			registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICON;
		}
		if (selected_skill != 3) {
			registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICON;
		}
		if (selected_skill != 4) {
			registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICON;
		}
		if (selected_skill != 5) {
			registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICON;
		}

		showCorrectSkills();

		if (player_turn == 1) {
			if (registry.companions.has(currPlayer)) {

				//iceshard
				if (inButton(registry.motions.get(iceShard_icon).position, ICON_WIDTH, ICON_HEIGHT)
					&& canUseSkill(currPlayer, 0)) {
					if (selected_skill == -1) {
						registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICONSELECTED;
						selected_skill = 0;
						tutorial_icon_selected = 1;
					}
					else {
						registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICON;
						selected_skill = -1;
					}
				}
				//fireball
				else if (inButton(registry.motions.get(fireBall_icon).position, ICON_WIDTH, ICON_HEIGHT)
					&& canUseSkill(currPlayer, 1)) {
					if (selected_skill == -1) {
						registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICONSELECTED;
						selected_skill = 1;
					}
					else {
						registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICON;
						selected_skill = -1;
					}
				}
				//rock
				else if (inButton(registry.motions.get(rock_icon).position, ICON_WIDTH, ICON_HEIGHT)
					&& canUseSkill(currPlayer, 2)) {
					if (selected_skill == -1) {
						registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICONSELECTED;
						selected_skill = 2;
					}
					else {
						registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICON;
						selected_skill = -1;
					}
				}
				//heal
				else if (inButton(registry.motions.get(heal_icon).position, ICON_WIDTH, ICON_HEIGHT)
					&& canUseSkill(currPlayer, 3)) {
					if (selected_skill == -1) {
						registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICONSELECTED;
						selected_skill = 3;
					}
					else {
						registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICON;
						selected_skill = -1;
					}
				}
				//taunt
				else if (inButton(registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT)
					&& canUseSkill(currPlayer, 4)) {
					if (selected_skill == -1) {
						registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICONSELECTED;
						selected_skill = 4;
					}
					else {
						registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICON;
						selected_skill = -1;
					}
				}
				//melee
				else if (inButton(registry.motions.get(melee_icon).position, ICON_WIDTH, ICON_HEIGHT)
					&& canUseSkill(currPlayer, 5)) {
					if (selected_skill == -1) {
						registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICONSELECTED;
						selected_skill = 5;
					}
					else {
						registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICON;
						selected_skill = -1;
					}
				}
				else {
					//iceshard
					if (selected_skill == 0) {
						sk->startIceShardAttack(currPlayer, currPlayer);
						selected_skill = -1;
						tutorial_ability_fired = 1;
						registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICON;
						playerUseMelee = 0;	// added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
					}
					//fireball
					if (selected_skill == 1) {
						sk->startFireballAttack(currPlayer);
						selected_skill = -1;
						registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICON;
						playerUseMelee = 0;	// added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
					}
					//rock
					if (selected_skill == 2) {
						for (int j = 0; j < registry.enemies.components.size(); j++) {
							PhysicsSystem physicsSystem;
							vec2 b_box = physicsSystem.get_custom_bounding_box(registry.enemies.entities[j]);
							if (inButton(registry.motions.get(registry.enemies.entities[j]).position, b_box.x, b_box.y)) {
								sk->startRockAttack(currPlayer, registry.enemies.entities[j]);
								selected_skill = -1;
								registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICON;
								playerUseMelee = 0;	// added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
							}
						}
					}
					// heal
					if (selected_skill == 3) {
						for (int j = 0; j < registry.companions.components.size(); j++) {
							PhysicsSystem physicsSystem;
							vec2 b_box = physicsSystem.get_custom_bounding_box(registry.companions.entities[j]);
							if (inButton(registry.motions.get(registry.companions.entities[j]).position, b_box.x, b_box.y)) {
								sk->startHealAttack(currPlayer, registry.companions.entities[j]);
								prevPlayer = currPlayer;
								update_healthBars();
								selected_skill = -1;
								registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICON;
								playerUseMelee = 0;	// added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
							}
						}
					}
					//taunt
					if (selected_skill == 4) {
						for (int j = 0; j < registry.enemies.components.size(); j++) {
							PhysicsSystem physicsSystem;
							vec2 b_box = physicsSystem.get_custom_bounding_box(registry.enemies.entities[j]);
							if (inButton(registry.motions.get(registry.enemies.entities[j]).position, b_box.x, b_box.y)) {
								sk->startTauntAttack(currPlayer, registry.enemies.entities[j]);
								selected_skill = -1;
								registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICON;
								playerUseMelee = 0;	// added this to switch back playerUseMelee to 0 so that we don't trigger unnecessary enemy attack
							}
						}
					}
					//melee
					if (selected_skill == 5) {
						for (int j = 0; j < registry.enemies.components.size(); j++) {
							PhysicsSystem physicsSystem;
							vec2 b_box = physicsSystem.get_custom_bounding_box(registry.enemies.entities[j]);
							if (inButton(registry.motions.get(registry.enemies.entities[j]).position, b_box.x, b_box.y)) {
								sk->startMeleeAttack(currPlayer, registry.enemies.entities[j], -1);
								playerUseMelee = 1;
								selected_skill = -1;
								registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICON;
							}
						}
					}
				}
			}
			else {
				if (roundVec.empty()) {
					printf("roundVec is empty at player turn, createRound now \n");
					createRound();
				}
				else {
					printf("no player at player turn \n");
					checkRound();
				}
			}
		}

		// Handle clicks on tutorial box
		if (tutorial_ability_fired && tutorial_icon_selected && tutorial_enabled
			&& (curr_tutorial_box_num <= 4 || curr_tutorial_box_num == 7) && inButton(registry.motions.get(curr_tutorial_box).position, TUTORIAL_BOX_WIDTH, TUTORIAL_BOX_HEIGHT)) {

			if (curr_tutorial_box_num == 7) {
				registry.remove_all_components_of(curr_tutorial_box);
				curr_tutorial_box_num += 1;
				tutorial_enabled = 0;
			}
			else {
				advanceTutorial(curr_tutorial_box, vec2(-1, -1));
			}
		}
		if (tutorial_icon_selected && curr_tutorial_box_num == 5) {
			// Do case 5 from advanceTutorial here
			tutorial_ability_fired = 0;
			advanceTutorial(curr_tutorial_box, vec2(700, 300));
		}
		else if (tutorial_ability_fired && curr_tutorial_box_num == 6) {
			// Do case 6 from advanceTutorial here
			advanceTutorial(curr_tutorial_box, vec2(600, 300));
			showCorrectSkills();
		}
	}
}

void WorldSystem::advanceTutorial(Entity currTutorial, vec2 pos) {
	vec2 next_box_pos = vec2(0, 0);
	switch (curr_tutorial_box_num) {
		case 0: next_box_pos = vec2(300, 300); break; // Companions 
		case 1: next_box_pos = vec2(900, 300); break; // Enemies
		case 2: next_box_pos = vec2(950, 150); break; // Turn indicator
		case 3: next_box_pos = vec2(350, 100); break; // Menu
		case 4: next_box_pos = vec2(800, 550); tutorial_icon_selected = 0; break; // Ability intro
		// case 5: next_box_pos = vec2(700, 300); tutorial_ability_fired = 0; break;  // Ability targeting
		// case 6: next_box_pos = vec2(600, 300); break;  // End of tutorial
		default: break;
	}
	curr_tutorial_box_num += 1;

	// Change box location
	Motion& motion = registry.motions.get(currTutorial);
	motion.position = (pos.x != -1) ? pos : next_box_pos;

	// Change box type
	TEXTURE_ASSET_ID tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_ONE;
	switch (curr_tutorial_box_num) {
		case 0: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_ONE; break;
		case 1: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_TWO; break;
		case 2: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_THREE; break;
		case 3: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_FOUR; break;
		case 4: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_FIVE; showCorrectSkills(); break;
		case 5: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_SIX; showCorrectSkills(); break;
		case 6: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_SEVEN; break;
		case 7: tutorial_box_num = TEXTURE_ASSET_ID::TUTORIAL_EIGHT; break;
		default: break;
	}
	RenderRequest& renderRequest = registry.renderRequests.get(currTutorial);
	renderRequest.used_texture = tutorial_box_num;
}

vec2 WorldSystem::placeDirection(vec2 mouse_position, vec2 icon_position, float width, float height) {
	vec2 placePos;
	if (mouse_position.x <= icon_position.x && mouse_position.y >= icon_position.y) {
		placePos = vec2(mouse_position.x + 275.f, mouse_position.y - 75.f);
	}
	else if (mouse_position.x <= icon_position.x && mouse_position.y <= icon_position.y) {
		placePos = vec2(mouse_position.x + 275.f, mouse_position.y - 75.f);
	}
	else if (mouse_position.x >= icon_position.x && mouse_position.y >= icon_position.y) {
		placePos = vec2(mouse_position.x + 275.f, mouse_position.y - 75.f);
	}
	else if (mouse_position.x >= icon_position.x && mouse_position.y <= icon_position.y) {
		placePos = vec2(mouse_position.x + 275.f, mouse_position.y - 75.f);
	}
	return placePos;
}

void WorldSystem::on_mouse_move(vec2 mouse_position) {
	msPos = mouse_position;
	sk->mousePos = mouse_position;
	if (!canStep && !story) {
		if (mouseInArea(registry.motions.get(new_game_button).position,UI_BUTTON_WIDTH , UI_BUTTON_HEIGHT)) {
			registry.renderRequests.get(new_game_button).used_texture = TEXTURE_ASSET_ID::NEW_GAME_HOVER;
			Mix_Volume(5, 32);
			Mix_PlayChannel(5, button_hover_sound, 0);
		}
		else {
			registry.renderRequests.get(new_game_button).used_texture = TEXTURE_ASSET_ID::NEW_GAME;
		}
		if (mouseInArea(registry.motions.get(load_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {
			registry.renderRequests.get(load_game_button).used_texture = TEXTURE_ASSET_ID::LOAD_GAME_HOVER;
			Mix_Volume(5, 32);
			Mix_PlayChannel(5, button_hover_sound, 0);
		}
		else {
			registry.renderRequests.get(load_game_button).used_texture = TEXTURE_ASSET_ID::LOAD_GAME;
		}
		if (mouseInArea(registry.motions.get(exit_game_button).position, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT)) {
			registry.renderRequests.get(exit_game_button).used_texture = TEXTURE_ASSET_ID::EXIT_HOVER;
			Mix_Volume(5, 32);
			Mix_PlayChannel(5, button_hover_sound, 0);
		}
		else {
			registry.renderRequests.get(exit_game_button).used_texture = TEXTURE_ASSET_ID::EXIT_GAME;
		}
	} else if (canStep) {

		if (mouseInArea(registry.motions.get(fireBall_icon).position, ICON_WIDTH, ICON_HEIGHT) 
			&& registry.renderRequests.get(fireBall_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE) {
			if (registry.toolTip.size() == 0) {
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(fireBall_icon).position, ICON_WIDTH, ICON_HEIGHT), "FB");
			}
		}
		else if (mouseInArea(registry.motions.get(iceShard_icon).position, ICON_WIDTH, ICON_HEIGHT)
			&& registry.renderRequests.get(iceShard_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE) {
			if (registry.toolTip.size() == 0) {
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(iceShard_icon).position, ICON_WIDTH, ICON_HEIGHT), "IS");
			}
		}
		else if (mouseInArea(registry.motions.get(rock_icon).position, ICON_WIDTH, ICON_HEIGHT) 
			&& registry.renderRequests.get(rock_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE) {
			if (registry.toolTip.size() == 0) {
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(rock_icon).position, ICON_WIDTH, ICON_HEIGHT), "RK");
			}
		}
		else if (mouseInArea(registry.motions.get(heal_icon).position, ICON_WIDTH, ICON_HEIGHT) 
			&& registry.renderRequests.get(heal_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE) {
			if (registry.toolTip.size() == 0) {
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(heal_icon).position, ICON_WIDTH, ICON_HEIGHT), "HL");
			}
		}
		else if (mouseInArea(registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT) 
			&& registry.renderRequests.get(taunt_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE) {
			if (registry.toolTip.size() == 0) {
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT), "TT");
			}
		}
		else if (mouseInArea(registry.motions.get(melee_icon).position, ICON_WIDTH, ICON_HEIGHT) 
			&& registry.renderRequests.get(melee_icon).used_texture != TEXTURE_ASSET_ID::EMPTY_IMAGE) {
			if (registry.toolTip.size() == 0) {
				tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT), "ML");
			}
		}
		else {
			registry.remove_all_components_of(tooltip);
			registry.toolTip.clear();
		}
	}
}

bool WorldSystem::mouseInArea(vec2 buttonPos, float buttonX, float buttonY) {
	float left_bound = buttonPos.x - (buttonX / 2);
	float right_bound = buttonPos.x + (buttonX / 2);
	float upper_bound = buttonPos.y - (buttonY / 2);
	float lower_bound = buttonPos.y + (buttonY / 2);
	if ((left_bound <= msPos.x) && (msPos.x <= right_bound)) {
		if ((upper_bound <= msPos.y) && (msPos.y <= lower_bound)) {
			return true;
		}
	}
	return false;
}

bool WorldSystem::inButton(vec2 buttonPos, float buttonX, float buttonY) {
	if (msPos.x <= buttonPos.x + buttonX / 2 && msPos.x >= buttonPos.x - buttonX / 2) {
		if (msPos.y <= buttonPos.y + buttonY / 2 && msPos.y >= buttonPos.y - buttonY / 2) {
			return true;
		}
	}
	return false;
}

bool WorldSystem::inEntity(const Entity entity) {
	PhysicsSystem physicsSystem;
	vec2 custom_pos = physicsSystem.get_custom_position(entity);
	vec2 custom_scale = physicsSystem.get_custom_bounding_box(entity);
	float left_bound = custom_pos.x - (custom_scale.x / 2);
	float right_bound = custom_pos.x + (custom_scale.x / 2);
	float upper_bound = custom_pos.y - (custom_scale.y / 2);
	float lower_bound = custom_pos.y + (custom_scale.y / 2);
	if ((left_bound <= msPos.x) && (msPos.x <= right_bound)) {
		if ((upper_bound <= msPos.y) && (msPos.y <= lower_bound)) {
			return true;
		}
	}
	return false;
}

void WorldSystem::deselectButton() {
	registry.remove_all_components_of(selectedButton);
}

void WorldSystem::deselectButtons() {
	if (selectedButton) {
		FIREBALLSELECTED = 0;
		SILENCESELECTED = 0;
		registry.remove_all_components_of(selectedButton);
	}
}

bool WorldSystem::canUseSkill(Entity user, int skill) {
	Statistics pStat = registry.stats.get(user);
	return skill_character_aviability[pStat.classID][skill];
}

// helper that shows the correct skills based on current player
void WorldSystem::showCorrectSkills() {
	if (currPlayer != NULL && registry.companions.has(currPlayer)) {
		Statistics pStat = registry.stats.get(currPlayer);
		if (!skill_character_aviability[pStat.classID][0] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 5)) {
			registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else {
			registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICON;
		}

		if (!skill_character_aviability[pStat.classID][1] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7)) {
			registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else {
			registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICON;
		}

		if (!skill_character_aviability[pStat.classID][2] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7)) {
			registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else {
			registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICON;
		}

		if (!skill_character_aviability[pStat.classID][3] || registry.taunts.has(currPlayer) || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7)) {
			registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else {
			registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICON;
		}

		if (!skill_character_aviability[pStat.classID][4] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7)) {
			registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else {
			registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICON;
		}

		if (!skill_character_aviability[pStat.classID][5] || pStat.health < 0 || (tutorial_enabled && curr_tutorial_box_num < 7)) {
			registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::EMPTY_IMAGE;
		}
		else {
			registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICON;
		}
	}
}



void WorldSystem::backgroundTelling() {



}