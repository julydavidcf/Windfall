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
const int NUM_DEATH_PARTICLES = 120;

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
		|| melee_spell_sound == nullptr) {
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
			audio_path("death_enemy.wav").c_str());
		return nullptr;
	}
	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg, AISystem* ai_arg, SkillSystem* skill_arg) {
	this->renderer = renderer_arg;
	this->ai = ai_arg;
	this->sk = skill_arg;

	// Playing background music indefinitely (Later)
	//Mix_VolumeMusic(8);	// adjust volume from 0 to 128
	//Mix_PlayMusic(background_music, -1);

	fprintf(stderr, "Loaded music\n");

	// Set all states to default
	restart_game();
}

void WorldSystem::displayPlayerTurn() {
	if (registry.turnIndicators.components.size() != 0) {
		registry.remove_all_components_of(registry.turnIndicators.entities[0]);
	}
	createPlayerTurn(renderer, TURN_INDICATOR_LOCATION);
}

void WorldSystem::displayEnemyTurn() {
	if (registry.turnIndicators.components.size() != 0) {
		registry.remove_all_components_of(registry.turnIndicators.entities[0]);
	}
	createEnemyTurn(renderer, TURN_INDICATOR_LOCATION);
}

void WorldSystem::iceShardAttack(Entity currPlayer) {
	Mix_PlayChannel(-1, fire_spell_sound, 0); // added fire spell sound but doesnt work
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
		Statistics& checkSpeed = registry.stats.get(entity);
		speedVec.push_back(checkSpeed.speed);

		// also decrement taunt duration if present
		if (registry.taunts.has(entity)) {
			Taunt* t = &registry.taunts.get(entity);
			t->duration--;
		}
	}

	for (int i = 0; i < registry.companions.components.size(); i++) {	// iterate through all companions to get speed stats
		Entity& entity = registry.companions.entities[i];
		Statistics& checkSpeed = registry.stats.get(entity);
		speedVec.push_back(checkSpeed.speed);

		// also decrement taunt duration if present
		if (registry.taunts.has(entity)) {
			Taunt* t = &registry.taunts.get(entity);
			t->duration--;
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

	// here I have the sorted array
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

	printf("finished check round \n");
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
	if (registry.enemies.size() <= 0 || registry.companions.size() <= 0) {
		restart_game();
	}

	// Updating window title with points (MAYBE USE FOR LATER)
	//std::stringstream title_ss;
	//title_ss << "Points: " << points;
	//glfwSetWindowTitle(window, title_ss.str().c_str());

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
		// remove barrier
		if (registry.reflects.has(motions_registry.entities[i])) {
			if (motion.velocity.x > 50.f) {
				printf("in2");
				registry.remove_all_components_of(motions_registry.entities[i]);
			}
		}
	}

	//check taunt for enemy and companion
	for (int i = (int)registry.enemies.components.size() - 1; i >= 0; --i) {
		if (registry.taunts.has(registry.enemies.entities[i])) {
			if (registry.taunts.get(registry.enemies.entities[i]).duration <= 0) {
				sk->removeTaunt(registry.enemies.entities[i]);
			}
		}
	}
	for (int i = (int)registry.companions.components.size() - 1; i >= 0; --i) {
		if (registry.taunts.has(registry.companions.entities[i])) {
			if (registry.taunts.get(registry.companions.entities[i]).duration <= 0) {
				sk->removeTaunt(registry.companions.entities[i]);
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
			attack.counter_ms = 1250.f;
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
		if (!registry.deathTimers.has(attacker)) {
			if (attack.counter_ms <= 0.f) {
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
						Mix_PlayChannel(5, rock_spell_sound, 0);
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
						Mix_PlayChannel(-1, taunt_spell_sound, 0);	// TODO
						printf("taunt attack enemy\n");
						sk->launchTaunt(attack.target,renderer);
						break;
					}
					case ROCK: {
						Mix_Volume(5, 32);
						Mix_PlayChannel(5, rock_spell_sound, 0);
						printf("Rock attack enemy\n");
						currentProjectile = sk->launchRock(attack.target,renderer);
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
		printf("prevPlayer is: %g \n", float(registry.stats.get(prevPlayer).speed));
		printf("currPlayer is: %g \n", float(registry.stats.get(currPlayer).speed));

		if (!registry.checkRoundTimer.has(currPlayer)) {
			displayEnemyTurn();
			if (registry.companions.has(prevPlayer) && registry.enemies.has(currPlayer)) {	// First case: Checks if selected companion character has died so as to progress to an enemy's
				if (registry.stats.get(prevPlayer).health <= 0 || playerUseMelee == 1) {	// Second case: (Brute force) playerUseMelee checks if the previous player used melee attack
					prevPlayer = currPlayer;
					ai->callTree(currPlayer);
				}
			}
			if (registry.enemies.has(prevPlayer) && registry.enemies.has(currPlayer)) {	// checks if enemy is going right after another enemy's turn
				enemy_turn_timer -= elapsed_ms_since_last_update;
				if (enemy_turn_timer < 0) {
					if (registry.companions.size() == 0) {
						restart_game();
					}
					else {
						prevPlayer = currPlayer;
						ai->callTree(currPlayer);
					}
				}
				if (registry.stats.get(prevPlayer).health <= 0) {	// Checks if selected enemy character has died so as to progress to an enemy's TO TEST
					ai->callTree(currPlayer);
				}
			}
		}
	}

	// Processing the salmon state
	assert(registry.screenStates.components.size() <= 1);
	ScreenState& screen = registry.screenStates.components[0];

	// update state of death particles
	for (Entity entity : registry.deathParticles.entities) {
		DeathParticle& deathParticles = registry.deathParticles.get(entity);
		for (auto& particle : deathParticles.deathParticles) {
			particle.Life -= elapsed_ms_since_last_update;
			if (particle.Life > 0.f) {
				particle.motion.position.x -= particle.motion.velocity.y * (rand() % 17) * 0.3f;
				particle.motion.position.y -= particle.motion.velocity.x * (rand() % 17) * 0.3f;
				particle.Color.a -= 0.05f * 0.01f;
				particle.motion.angle += 0.5;
				if (particle.motion.angle >= (2 * M_PI)) {
					particle.motion.angle = 0;
				}
			}
			else {
				deathParticles.fadedParticles++;
			}
		}
		if (deathParticles.fadedParticles >= NUM_DEATH_PARTICLES) {
			registry.deathParticles.remove(entity);
			registry.remove_all_components_of(entity);	// added back in, kinda works
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
	// Layer 2
	createBackgroundLayerTwo(renderer, { w / 2, h / 2 });
	// Layer 3
	createBackgroundLayerThree(renderer, { w / 2, h / 2 });
	// Layer 4 (Foremost layer)
	createBackgroundLayerFour(renderer, { w / 2, h / 2 });

	// Create a player mage
	player_mage = createPlayerMage(renderer, { 100, 575 });
	// Create a player swordsman
	player_swordsman = createPlayerSwordsman(renderer, { 275, 500 });
	// Create an enemy mage
	enemy_mage = createEnemyMage(renderer, { 1050, 575 });
	registry.colors.insert(enemy_mage, { 0.0, 0.0, 1.f });

	if (gameLevel > 1) {
		// Create an enemy swordsman
		enemy_swordsman = createEnemySwordsman(renderer, { 875, 500 });
		registry.colors.insert(enemy_swordsman, { 0.f, 1.f, 1.f });
	}
	// Create the fireball icon
	fireball_icon = createFireballIcon(renderer, { 600, 700 });

	// Create the icons here
	taunt_icon = createTauntIcon(renderer, { 300, 700 });
	heal_icon = createHealIcon(renderer, { 400, 700 });
	melee_icon = createMeleeIcon(renderer, { 500, 700 });
	iceShard_icon = createIceShardIcon(renderer, { 600, 700 });
	fireBall_icon = createFireballIcon(renderer, { 700, 700 });
	rock_icon = createRockIcon(renderer, { 800, 700 });

	//Create a tooltip
	tooltip;

	player_turn = 1;	// player turn indicator
	roundVec.clear();	// empty vector roundVec to create a new round
	createRound();
	checkRound();
	showCorrectSkills();

	displayPlayerTurn();	// display player turn when restart game
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
	DeathParticle particleEffects;
	for (int p = 0; p <= NUM_DEATH_PARTICLES; p++) {
		auto& motion = registry.motions.get(entity);
		DeathParticle particle;
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
	}
	if (!registry.deathParticles.has(entity)) {
		registry.deathParticles.insert(entity, particleEffects);
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
			// create death particles. Register for rendering.

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
		if (registry.enemies.has(entity)) {
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
							if (player_turn == 0) {
								if (!registry.checkRoundTimer.has(currPlayer)) {
									displayEnemyTurn();
									if (registry.enemies.has(currPlayer)) {	// check if enemies have currPlayer
										prevPlayer = currPlayer;
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
					}
				}
			}

			// create death particles. Register for rendering.
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
		// barrier collection
		if (registry.projectiles.has(entity)) {
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
	}
}

// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}

// On key callback
void WorldSystem::on_key(int key, int, int action, int mod) {
	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

		restart_game(true);
	}

	// Debugging
	if (key == GLFW_KEY_D) {
		if (action == GLFW_RELEASE)
			debugging.in_debug_mode = false;
		else
			debugging.in_debug_mode = true;
	}
}

void WorldSystem::on_mouse_button(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

		//show all skill
		if (selected_skill != 0) {
			registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICON;
		}
		if (selected_skill != 1) {
			registry.renderRequests.get(fireball_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICON;
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
		//disable some skill
		showCorrectSkills();

		if (player_turn == 1) {
			if (registry.companions.has(currPlayer)) {

				//iceshard
				if (inButton(registry.motions.get(iceShard_icon).position, ICON_WIDTH, ICON_HEIGHT)
					&& canUseSkill(currPlayer, 0)) {
					if (selected_skill == -1) {
						registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICONSELECTED;
						selected_skill = 0;
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
								sk->startMeleeAttack(currPlayer, registry.enemies.entities[j]);
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
	}
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
	if (mouseInArea(registry.motions.get(fireBall_icon).position, ICON_WIDTH, ICON_HEIGHT)) {
		if (registry.toolTip.size() == 0) {
			tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(fireBall_icon).position, ICON_WIDTH, ICON_HEIGHT), "FB");
		}
	}
	else if (mouseInArea(registry.motions.get(iceShard_icon).position, ICON_WIDTH, ICON_HEIGHT)) {
		if (registry.toolTip.size() == 0) {
			tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(iceShard_icon).position, ICON_WIDTH, ICON_HEIGHT), "IS");
		}
	}
	else if (mouseInArea(registry.motions.get(rock_icon).position, ICON_WIDTH, ICON_HEIGHT)) {
		if (registry.toolTip.size() == 0) {
			tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(rock_icon).position, ICON_WIDTH, ICON_HEIGHT), "RK");
		}
	}
	else if (mouseInArea(registry.motions.get(heal_icon).position, ICON_WIDTH, ICON_HEIGHT)) {
		if (registry.toolTip.size() == 0) {
			tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(heal_icon).position, ICON_WIDTH, ICON_HEIGHT), "HL");
		}
	}
	else if (mouseInArea(registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT)) {
		if (registry.toolTip.size() == 0) {
			tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT), "TT");
		}
	}
	else if (mouseInArea(registry.motions.get(melee_icon).position, ICON_WIDTH, ICON_HEIGHT)) {
		if (registry.toolTip.size() == 0) {
			tooltip = createTooltip(renderer, placeDirection(msPos, registry.motions.get(taunt_icon).position, ICON_WIDTH, ICON_HEIGHT), "ML");
		}
	}
	else {
		registry.renderRequests.remove(tooltip);
		registry.toolTip.clear();
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
		if (!skill_character_aviability[pStat.classID][0] || pStat.health < 0) {
			registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICONDISABLED;
		}
		else {
			registry.renderRequests.get(iceShard_icon).used_texture = TEXTURE_ASSET_ID::ICESHARDICON;
		}

		if (!skill_character_aviability[pStat.classID][1] || pStat.health < 0) {
			registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICONDISABLED;
		}
		else {
			registry.renderRequests.get(fireBall_icon).used_texture = TEXTURE_ASSET_ID::FIREBALLICON;
		}

		if (!skill_character_aviability[pStat.classID][2] || pStat.health < 0) {
			registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICONDISABLED;
		}
		else {
			registry.renderRequests.get(rock_icon).used_texture = TEXTURE_ASSET_ID::ROCKICON;
		}

		if (!skill_character_aviability[pStat.classID][3] || registry.taunts.has(currPlayer) || pStat.health < 0) {
			registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICONDISABLED;
		}
		else {
			registry.renderRequests.get(heal_icon).used_texture = TEXTURE_ASSET_ID::HEALICON;
		}

		if (!skill_character_aviability[pStat.classID][4] || pStat.health < 0) {
			registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICONDISABLED;
		}
		else {
			registry.renderRequests.get(taunt_icon).used_texture = TEXTURE_ASSET_ID::TAUNTICON;
		}

		if (!skill_character_aviability[pStat.classID][5] || pStat.health < 0) {
			registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICONDISABLED;
		}
		else {
			registry.renderRequests.get(melee_icon).used_texture = TEXTURE_ASSET_ID::MELEEICON;
		}
	}
}

