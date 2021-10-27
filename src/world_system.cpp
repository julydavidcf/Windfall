// Header
#include "world_system.hpp"
#include "world_init.hpp"
#include "physics_system.hpp"

// stlib
#include <cassert>
#include <sstream>

#include "physics_system.hpp"

// Game configuration
const size_t MAX_TURTLES = 15;
const size_t MAX_FISH = 5;
const size_t TURTLE_DELAY_MS = 2000 * 3;
const size_t FISH_DELAY_MS = 5000 * 3;
const size_t BARRIER_DELAY = 4000;
const size_t ENEMY_TURN_TIME = 3000;
const vec2 TURN_INDICATOR_LOCATION = { 600, 150 };
const int NUM_DEATH_PARTICLES = 500;

vec2 msPos = vec2(0, 0);

float next_barrier_spawn = 1000;

float enemy_turn_timer = 3000;

//Button status
int FIREBALLSELECTED = 0;
int SILENCESELECTED = 0;

//selected button
Entity selectedButton;



//current projectile
Entity currentProjectile;

WorldSystem::WorldSystem()
	: points(0) {
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
	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

// Debugging
namespace {
	void glfw_err_cb(int error, const char *desc) {
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
	window = glfwCreateWindow(width, height, "Windfall Milestone 1", nullptr, nullptr);
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

	// handel mosue click
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

	if (background_music == nullptr || salmon_dead_sound == nullptr || salmon_eat_sound == nullptr
		|| hit_enemy_sound == nullptr || fireball_explosion_sound == nullptr || death_enemy_sound == nullptr) {
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("combatMusic.wav").c_str(),
			audio_path("salmon_dead.wav").c_str(),
			audio_path("salmon_eat.wav").c_str(),
			audio_path("hit_enemy.wav").c_str(),
			audio_path("fireball_explosion_short.wav").c_str(),
			audio_path("death_enemy.wav").c_str());
		return nullptr;
	}

	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
	// Playing background music indefinitely (Later)
	// Mix_PlayMusic(background_music, -1); // silence music for now
	fprintf(stderr, "Loaded music\n");

	// Set all states to default
    restart_game();
}

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	// Get the screen dimensions
	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);

	//player turn



	//enemy turn counter starting
	if (player_turn == 0) {
		enemy_turn_timer -= elapsed_ms_since_last_update;
		if (registry.turnIndicators.components.size() != 0) {
			registry.remove_all_components_of(registry.turnIndicators.entities[0]);
		}
		createEnemyTurn(renderer, TURN_INDICATOR_LOCATION);
	}
	else {
		if (registry.turnIndicators.components.size() != 0) {
			registry.remove_all_components_of(registry.turnIndicators.entities[0]);
		}
		createPlayerTurn(renderer, TURN_INDICATOR_LOCATION);
	}

	//give player a turn when enemy turn is over
	if (enemy_turn_timer < 0) {
		player_turn = 1;
		enemy_turn_timer = ENEMY_TURN_TIME;
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
	for (int i = (int)motions_registry.components.size()-1; i>=0; --i) {
	    Motion& motion = motions_registry.components[i];
		if (motion.position.x + abs(motion.scale.x) < 0.f) {
		    registry.remove_all_components_of(motions_registry.entities[i]);
		}
		// remove barrier
		if (registry.reflects.has(motions_registry.entities[i])) {
			if (motion.velocity.x>50.f) {
				printf("in2");
				registry.remove_all_components_of(motions_registry.entities[i]);
			}
		}
	}

	// create wall periodiclly
	//next_barrier_spawn -= elapsed_ms_since_last_update;
	//if (next_barrier_spawn < 0) {
	//	next_barrier_spawn = BARRIER_DELAY;
	//	createBarrier(renderer, registry.motions.get(basicEnemy).position);
	//}



	// Processing the salmon state
	assert(registry.screenStates.components.size() <= 1);
    ScreenState &screen = registry.screenStates.components[0];

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
			// registry.remove_all_components_of(entity);
		}
	}

    float min_counter_ms = 3000.f;
	for (Entity entity : registry.deathTimers.entities) {
		// progress timer
		DeathTimer& counter = registry.deathTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		if(counter.counter_ms < min_counter_ms){
		    min_counter_ms = counter.counter_ms;
		}

		// restart the game once the death timer expired
		if (counter.counter_ms < 0) {
			registry.deathTimers.remove(entity);
			screen.darken_screen_factor = 0;
            restart_game();
			return true;
		}
	}
	// reduce window brightness if any of the present salmons is dying
	screen.darken_screen_factor = 1 - min_counter_ms / 3000;

	// update timer for enemyMage to return to its original position after being hit
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
			registry.motions.get(entity).position.x -= 20;
			return true;
		}
	}

	return true;
}

// Reset the world state to its initial state
void WorldSystem::restart_game() {
	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Reset the game speed
	current_speed = 1.f;

	// player turn indicator

	player_turn = 1;



	// Remove all entities that we created
	// All that have a motion, we could also iterate over all fish, turtles, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0)
	    registry.remove_all_components_of(registry.motions.entities.back());

	// Debugging for memory/component leaks
	registry.list_all_components();

	// Create a player mage
	player_mage = createPlayerMage(renderer, { 200, 450 });
	// Create a player swordsman
	player_swordsman = createPlayerSwordsman(renderer, { 350, 400 });


	// Create an enemy mage
	enemy_mage = createEnemyMage(renderer, { 900, 450 });
	registry.colors.insert(enemy_mage, { 0.0, 0.0, 1.f });
	// Create an enemy swordsman
	enemy_swordsman = createEnemySwordsman(renderer, { 700, 400 });
	registry.colors.insert(enemy_swordsman, { 0.f, 1.f, 1.f });
	// Create the necromancer
	necromancer = createNecromancer(renderer, { 1100, 400 });

	fireball_icon = createFireballIcon(renderer, { 600, 700 });
	silence_icon = createSilenceIcon(renderer, { 800, 700 });;

}

void WorldSystem::update_health(Entity entity, Entity other_entity) {
	if(registry.projectiles.has(entity)){
		Damage& damage = registry.damages.get(entity);
		HP* hp = nullptr;
		Entity healthbar;
		if(damage.isFriendly){
			if(registry.enemies.has(other_entity)){
				Enemy& enemy = registry.enemies.get(other_entity);
				healthbar = enemy.healthbar;
				hp = &registry.healthPoints.get(other_entity);
			}
		} else {
			if(registry.companions.has(other_entity)){
				Companion& enemy = registry.companions.get(other_entity);
				healthbar = enemy.healthbar;
				hp = &registry.healthPoints.get(other_entity);
			}
		}
		if(hp){
			hp->health = hp->health - (rand() % damage.range + damage.minDamage);
			Motion& motion = registry.motions.get(healthbar);
			if(hp->health<=0){
				if(!registry.deathTimers.has(other_entity)){
					registry.deathTimers.emplace(other_entity);
				}
				motion.scale = vec2({ (HEALTHBAR_WIDTH*(99.f/100.f)), HEALTHBAR_HEIGHT });
				
			} else {
				motion.scale = vec2({ (HEALTHBAR_WIDTH*(hp->health/100.f)), HEALTHBAR_HEIGHT });
			}
		}
	}
}

// Compute collisions between entities
void WorldSystem::handle_collisions() {
	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;
    
		// TODO: Deal with fireball - enemyMage collisions
		if (registry.enemies.has(entity)) {
			//Player& player = registry.players.get(entity);

			// Checking Projectile - Enemy collisions
			if (registry.projectiles.has(entity_other)) {
				// initiate death unless already dying
				if (!registry.deathTimers.has(entity)) {
					if (!registry.buttons.has(entity)) {

						update_health(entity_other, entity);
						registry.remove_all_components_of(entity_other); // causing abort error because of key input
						Mix_PlayChannel(-1, fireball_explosion_sound, 0); // added fireball hit sound
						if (registry.healthPoints.has(entity) && registry.healthPoints.get(entity).health <= 0) {
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
						// reduce HP of enemyMage by __ amount
						// if HP = 0, call death animation and possibly included death sound, 
						//		add death timer, restart game
					}
	
				}
				
			}


			// create death particles. Register for rendering.
			if (registry.healthPoints.has(entity) && registry.healthPoints.get(entity).health <= 0)
			{				
				// get rid of dead entity's healthbar.
				Entity entityHealthbar = registry.enemies.get(entity).healthbar;
				registry.motions.remove(entityHealthbar);

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
		if (registry.motions.get(entity).position.x <= 20 ||
			registry.motions.get(entity).position.x >= screen_width - 20 ||
			registry.motions.get(entity).position.y <= 20 ||
			registry.motions.get(entity).position.y >= screen_height - 20) {
			registry.remove_all_components_of(entity);
			Mix_PlayChannel(-1, fireball_explosion_sound, 0);
		}
	}
}

// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}

// On key callback

void WorldSystem::on_key(int key, int, int action, int mod) {	
	// TODO: Handle mouse click on fireball icon
	if (action == GLFW_RELEASE && key == GLFW_MOUSE_BUTTON_LEFT) {

	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

        restart_game();
	}

	// Debugging
	if (key == GLFW_KEY_D) {
		if (action == GLFW_RELEASE)
			debugging.in_debug_mode = false;
		else
			debugging.in_debug_mode = true;
	}

	// Control the current speed with `<` `>`
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_COMMA) {
		current_speed -= 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD) {
		current_speed += 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	current_speed = fmax(0.f, current_speed);

	// Manual create barrier
	//if (action == GLFW_RELEASE && key == GLFW_KEY_B) {
	//	createBarrier(renderer, registry.motions.get(basicEnemy).position);
	//}
}
//fireball
void WorldSystem::on_mouse_button( int button , int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		// fireball
		if (player_turn == 1) {
			Motion fireballIcon = registry.motions.get(fireball_icon);
			Motion silenceIcon = registry.motions.get(silence_icon);
			if (inButton(fireballIcon.position, FIREBALL_ICON_WIDTH, FIREBALL_ICON_HEIGHT)) {
				if (FIREBALLSELECTED == 0) {
					deselectButtons();
					selectedButton = createFireballIconSelected(renderer, { fireballIcon.position.x,fireballIcon.position.y });
					FIREBALLSELECTED = 1;

				}
				else {
					deselectButton();
					FIREBALLSELECTED = 0;
				}
			} else if(inButton(silenceIcon.position, SILENCE_ICON_WIDTH, SILENCE_ICON_HEIGHT)){
				if (SILENCESELECTED == 0) {
					deselectButtons();
					selectedButton = createSilenceIconSelected(renderer, { silenceIcon.position.x,silenceIcon.position.y });
					SILENCESELECTED = 1;

				}
				else {
					deselectButton();
					SILENCESELECTED = 0;
				}
			}
			else {
				if (FIREBALLSELECTED == 1) {
					Motion player = registry.motions.get(player_mage);
					currentProjectile = launchFireball(player.position);
					FIREBALLSELECTED = 0;
					//active this when ai is done
					player_turn = 0;
					deselectButton();
				} else if(SILENCESELECTED == 1){
					// CURRENTLY: only puts the enemy in silenced component
					// with the turn to be initialized to 1
					// TODO: when the turn system is implemented
					// each time doing an attack check if the entity is silenced
					// if so decrement the turn by 1 and if it's 0 remove
					// the entity. If it is silenced the entity should make *no*
					// moves. 
					for(Entity enemy: registry.enemies.entities){
						if(inEntity(enemy)&&(!registry.silenced.has(enemy))){
							printf("ENEMY FOUND!\n");
							Silenced& silenced = registry.silenced.emplace(enemy);
							Entity silence_bubble = createSilenceBubble(renderer, registry.motions.get(enemy).position);
							printf("Enemy silenced\n");
							break;
						} else {
							printf("MISS!\n");
						}
					}
					SILENCESELECTED = 0;
					//active this when ai is done
					player_turn = 0;
					deselectButton();
				}
			}
		}


		
	}

}

void WorldSystem::on_mouse_move(vec2 mouse_position) {
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// TODO A1: HANDLE SALMON ROTATION HERE
	// xpos and ypos are relative to the top-left of the window, the salmon's
	// default facing direction is (1, 0)
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	msPos = mouse_position;
	//printf("%f", msPos.x);
}


bool WorldSystem::inButton(vec2 buttonPos, float buttonX, float buttonY) {
	if (msPos.x <= buttonPos.x + buttonX/2 && msPos.x >= buttonPos.x - buttonX/2) {
		if (msPos.y <= buttonPos.y + buttonY/2 && msPos.y >= buttonPos.y - buttonY/2) {
			return true;
		}
	}
	return false;
}

bool WorldSystem::inEntity(const Entity entity) {
	PhysicsSystem physicsSystem;
	vec2 custom_pos = physicsSystem.get_custom_position(entity);
	vec2 custom_scale = physicsSystem.get_custom_bounding_box(entity);
	float left_bound = custom_pos.x - (custom_scale.x/2);
	float right_bound = custom_pos.x + (custom_scale.x/2);
	float upper_bound = custom_pos.y - (custom_scale.y/2);
	float lower_bound = custom_pos.y + (custom_scale.y/2);
	if((left_bound <= msPos.x)&&(msPos.x <= right_bound)){
		if((upper_bound <= msPos.y)&&(msPos.y <= lower_bound)){
			return true;
		}
	} 
	return false;
}

void WorldSystem::deselectButton() {
	registry.remove_all_components_of(selectedButton);
}

void WorldSystem::deselectButtons() {
	if(selectedButton){
		FIREBALLSELECTED = 0;
		SILENCESELECTED = 0;
		registry.remove_all_components_of(selectedButton);
	}
}



//skills
Entity WorldSystem::launchFireball(vec2 startPos) {

	float proj_x = startPos.x + 50;
	float proj_y = startPos.y;
	float mouse_x = msPos.x;
	float mouse_y = msPos.y;

	float dx = mouse_x - proj_x;
	float dy = mouse_y - proj_y;
	float dxdy = sqrt((dx*dx) + (dy*dy));
	float vx = FIREBALLSPEED * dx / dxdy;
	float vy = FIREBALLSPEED * dy / dxdy;

	//printf("%f%f\n", vx, vy);

	float angle = atan(dy / dx);
	if (dx < 0) {
		angle += M_PI;
	}
	printf("Angle: %f\n", angle);
	Entity resultEntity = createFireball(renderer, { startPos.x + 50, startPos.y }, angle, {vx,vy}, 1);
	Motion* ballacc = &registry.motions.get(resultEntity);
	ballacc->acceleration = vec2(1000 * vx/ FIREBALLSPEED, 1000 * vy/ FIREBALLSPEED);
	
	// ****temp**** enemy randomly spawn barrier

	int rng = rand() % 10;
	if (rng >= 4) {
		createBarrier(renderer, registry.motions.get(enemy_mage).position);
	}


	return  resultEntity;
}