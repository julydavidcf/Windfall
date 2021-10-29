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

Entity currPlayer;
Entity target;
Entity prevPlayer;
int isTaunt = 0;
int enemyTaunt = 0;

vec2 msPos = vec2(0, 0);
bool is_ms_clicked = false;

float next_barrier_spawn = 1000;

float enemy_turn_timer = 1000;

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

void WorldSystem::fireballAttack(Entity currPlayer) {
	Motion enemy = registry.motions.get(currPlayer);
	if (!registry.deathTimers.has(currPlayer)) {
		Entity resultEntity = createFireball(renderer, { enemy.position.x, enemy.position.y }, 3.14159, { -100, 0 }, 0);
		Motion* ballacc = &registry.motions.get(resultEntity);
		ballacc->acceleration = vec2(1000 * -100 / FIREBALLSPEED, 1000 * 0 / FIREBALLSPEED);
		Projectile* proj = &registry.projectiles.get(resultEntity);
		proj->flyingTimer = 2000.f;
	}
}

void WorldSystem::rockAttack(Entity target) {
	Motion enemy = registry.motions.get(target);
	if (!registry.deathTimers.has(target)) {
		// fireball action temporary until able to call behavior tree
		Entity resultEntity = createRock(renderer, { enemy.position.x, enemy.position.y - 300 }, 0);
		Projectile* proj = &registry.projectiles.get(resultEntity);
		proj->flyingTimer = 2000.f;
	}
}

void WorldSystem::healSkill(Entity target, float amount) {
	if (registry.stats.has(target)) {
		vec2 targetp = registry.motions.get(target).position;
		createGreenCross(renderer, targetp);
		Statistics* tStats = &registry.stats.get(target);
		if (tStats->health + amount > tStats->max_health) {
			tStats->health = tStats->max_health;
		}
		else
		{
			tStats->health += amount;
		}
	}
	update_healthBars();
}

void WorldSystem::meleeSkill(Entity target) {
	Motion enemy = registry.motions.get(target);
	Entity resultEntity = createMelee(renderer, { enemy.position.x, enemy.position.y }, 0);
}

void WorldSystem::tauntSkill(Entity target) {
	registry.taunts.emplace(target);
	Taunt* t = &registry.taunts.get(target);
	t->duration = 3;
}

std::vector<Entity> roundVec;
void WorldSystem::createRound() {
	std::vector<int> speedVec;
	for (int i = 0; i < registry.enemies.components.size(); i++) {	// iterate through all enemies to get speed stats
		Entity& entity = registry.enemies.entities[i];
		Statistics& checkSpeed = registry.stats.get(entity);
		speedVec.push_back(checkSpeed.speed);		
	}
	
	for (int i = 0; i < registry.companions.components.size(); i++) {	// iterate through all companions to get speed stats
		Entity& entity = registry.companions.entities[i];
		Statistics& checkSpeed = registry.stats.get(entity);
		speedVec.push_back(checkSpeed.speed);
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
	}
	else if (registry.enemies.has(toPlay) && registry.stats.get(toPlay).health > 0) {	// toPlay is enemy, put to currPlayer to pass for fireball
		printf("its %g enemy turn \n", float(registry.stats.get(toPlay).speed));
		player_turn = 0;
		currPlayer = toPlay;
	}
	else {
		printf("no player or enemy, checking round now \n");
		prevPlayer = currPlayer;
		checkRound();
	}
}

// -----------------------------------------------------------------------------------------------------------------------
// Define enemy behavior tree nodes
// The return type of behaviour tree processing
enum class BTState {
	Running,
	Success,
	Failure
};

std::string toString(BTState s)
{
	switch (s)
	{
	case BTState::Running:   return "Running";
	case BTState::Success:   return "Success";
	case BTState::Failure: return "Failure";
	default:      return "[Unknown BTState]";
	}
}

// The base class representing any node in our behaviour tree
// Does not have any pointers
class BTNode {
public:
	virtual void init(Entity e) {};

	virtual BTState process(Entity e) = 0;
	WorldSystem worldSystem;
};

// A composite node that loops through all children and exits when one fails
class BTRunCheckMage : public BTNode {
private:
	int m_index;
	BTNode* m_children[2];	// Run pair has two children, using an array

public:
	BTRunCheckMage(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
		: m_index(0) {
		m_children[0] = c0;
		m_children[1] = c1;
	}

	void init(Entity e) override
	{
		m_index = 0;	// set index to 0 to execute first child
		// initialize the first child
		const auto& child = m_children[m_index];
		child->init(e);
	}

	BTState process(Entity e) override {
		printf("Pair run check mage ... child = %g \n", float(m_index));	// print statement to visualize
		if (m_index >= 2)
			return BTState::Success;

		// process current child
		BTState state = m_children[m_index]->process(e);

		// select a new active child and initialize its internal state
		if (state == BTState::Success) {	// if child return success
			++m_index;	// increment index
			if (m_index >= 2) {	// check whether the second child is executed already
				return BTState::Success;
			}
			else {
				m_children[m_index]->init(e);	// initialize next child to run 
				return BTState::Running;
			}
		}
		else {
			return state;
		}
	}
};

// A composite node that loops through all children and exits when one fails
class BTRunCheckTaunt : public BTNode {
private:
	int m_index;
	BTNode* m_children[2];	// Run pair has two children, using an array

public:
	BTRunCheckTaunt(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
		: m_index(0) {
		m_children[0] = c0;
		m_children[1] = c1;
	}

	void init(Entity e) override
	{
		m_index = 0;	// set index to 0 to execute first child
		// initialize the first child
		const auto& child = m_children[m_index];
		child->init(e);
	}

	BTState process(Entity e) override {
		printf("Pair run check taunt ... child = %g \n", float(m_index));	// print statement to visualize
		if (m_index >= 2)
			return BTState::Success;

		// process current child
		BTState state = m_children[m_index]->process(e);

		// select a new active child and initialize its internal state
		if (state == BTState::Success) {	// if child return success
			++m_index;	// increment index
			if (m_index >= 2) {	// check whether the second child is executed already
				return BTState::Success;
			}
			else {
				m_children[m_index]->init(e);	// initialize next child to run 
				return BTState::Running;
			}
		}
		else {
			return state;
		}
	}
};

// A composite node that loops through all children and exits when one fails
class BTRunCheckCharacter : public BTNode {
private:
	int m_index;
	BTNode* m_children[2];	// Run pair has two children, using an array

public:
	BTRunCheckCharacter(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
		: m_index(0) {
		m_children[0] = c0;
		m_children[1] = c1;
	}

	void init(Entity e) override
	{
		m_index = 0;	// set index to 0 to execute first child
		// initialize the first child
		const auto& child = m_children[m_index];
		child->init(e);
	}

	BTState process(Entity e) override {
		printf("Pair run check character ... child = %g \n", float(m_index));	// print statement to visualize
		if (m_index >= 2)
			return BTState::Success;

		// process current child
		BTState state = m_children[m_index]->process(e);

		// select a new active child and initialize its internal state
		if (state == BTState::Success) {	// if child return success
			++m_index;	// increment index
			if (m_index >= 2) {	// check whether the second child is executed already
				return BTState::Success;
			}
			else {
				m_children[m_index]->init(e);	// initialize next child to run 
				return BTState::Running;
			}
		}
		else {
			return state;
		}
	}
};

// A composite node that loops through all children and exits when one fails
class BTRunCheckEnemyTaunt : public BTNode {
private:
	int m_index;
	BTNode* m_children[2];	// Run pair has two children, using an array

public:
	BTRunCheckEnemyTaunt(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
		: m_index(0) {
		m_children[0] = c0;
		m_children[1] = c1;
	}

	void init(Entity e) override
	{
		m_index = 0;	// set index to 0 to execute first child
		// initialize the first child
		const auto& child = m_children[m_index];
		child->init(e);
	}

	BTState process(Entity e) override {
		printf("Pair run check taunt for me ... child = %g \n", float(m_index));	// print statement to visualize
		if (m_index >= 2)
			return BTState::Success;

		// process current child
		BTState state = m_children[m_index]->process(e);

		// select a new active child and initialize its internal state
		if (state == BTState::Success) {	// if child return success
			++m_index;	// increment index
			if (m_index >= 2) {	// check whether the second child is executed already
				return BTState::Success;
			}
			else {
				m_children[m_index]->init(e);	// initialize next child to run 
				return BTState::Running;
			}
		}
		else {
			return state;
		}
	}
};

// A composite node that loops through all children and exits when one fails
class BTRunCheckEnemyHP : public BTNode {
private:
	int m_index;
	BTNode* m_children[2];	// Run pair has two children, using an array

public:
	BTRunCheckEnemyHP(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
		: m_index(0) {
		m_children[0] = c0;
		m_children[1] = c1;
	}

	void init(Entity e) override
	{
		m_index = 0;	// set index to 0 to execute first child
		// initialize the first child
		const auto& child = m_children[m_index];
		child->init(e);
	}

	BTState process(Entity e) override {
		printf("Pair run check enemy HP for me ... child = %g \n", float(m_index));	// print statement to visualize
		if (m_index >= 2)
			return BTState::Success;

		// process current child
		BTState state = m_children[m_index]->process(e);

		// select a new active child and initialize its internal state
		if (state == BTState::Success) {	// if child return success
			++m_index;	// increment index
			if (m_index >= 2) {	// check whether the second child is executed already
				return BTState::Success;
			}
			else {
				m_children[m_index]->init(e);	// initialize next child to run 
				return BTState::Running;
			}
		}
		else {
			return state;
		}
	}
};

// A composite node that loops through all children and exits when one fails
class BTRunCheckMageHP : public BTNode {
private:
	int m_index;
	BTNode* m_children[2];	// Run pair has two children, using an array

public:
	BTRunCheckMageHP(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
		: m_index(0) {
		m_children[0] = c0;
		m_children[1] = c1;
	}

	void init(Entity e) override
	{
		m_index = 0;	// set index to 0 to execute first child
		// initialize the first child
		const auto& child = m_children[m_index];
		child->init(e);
	}

	BTState process(Entity e) override {
		printf("Pair run check mage HP for me ... child = %g \n", float(m_index));	// print statement to visualize
		if (m_index >= 2)
			return BTState::Success;

		// process current child
		BTState state = m_children[m_index]->process(e);

		// select a new active child and initialize its internal state
		if (state == BTState::Success) {	// if child return success
			++m_index;	// increment index
			if (m_index >= 2) {	// check whether the second child is executed already
				return BTState::Success;
			}
			else {
				m_children[m_index]->init(e);	// initialize next child to run 
				return BTState::Running;
			}
		}
		else {
			return state;
		}
	}
};

// A composite node that loops through all children and exits when one fails
class BTRunCheckSwordsman : public BTNode {
private:
	int m_index;
	BTNode* m_children[2];	// Run pair has two children, using an array

public:
	BTRunCheckSwordsman(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
		: m_index(0) {
		m_children[0] = c0;
		m_children[1] = c1;
	}

	void init(Entity e) override
	{
		m_index = 0;	// set index to 0 to execute first child
		// initialize the first child
		const auto& child = m_children[m_index];
		child->init(e);
	}

	BTState process(Entity e) override {
		printf("Pair run check swordsman for me ... child = %g \n", float(m_index));	// print statement to visualize
		if (m_index >= 2)
			return BTState::Success;

		// process current child
		BTState state = m_children[m_index]->process(e);

		// select a new active child and initialize its internal state
		if (state == BTState::Success) {	// if child return success
			++m_index;	// increment index
			if (m_index >= 2) {	// check whether the second child is executed already
				return BTState::Success;
			}
			else {
				m_children[m_index]->init(e);	// initialize next child to run 
				return BTState::Running;
			}
		}
		else {
			return state;
		}
	}
};

// A general decorator with lambda condition
class BTIfPlayerSideDoNotHaveMageHardCoded : public BTNode
{
public:
	BTIfPlayerSideDoNotHaveMageHardCoded(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("Checking if player side has no mage ... \n");	// print statement to visualize
		int toggle = 0;
		for (int i = 0; i < registry.companions.components.size(); i++) {	// checks player side for mage NOT WORKING
			Entity E = registry.companions.entities[i];
			if (registry.companions.get(E).companionType == MAGE) {
				toggle = 1;
			}
		}
		if (toggle == 0) {	// if player side has no mage, execute child which is fireball
			printf("Player side do not have mage \n");
			return m_child->process(e);
		}
		else
			return BTState::Success;
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfPlayerSideHasMageHardCoded : public BTNode
{
public:
	BTIfPlayerSideHasMageHardCoded(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("Checking if player side has mage ... \n");	// print statement to visualize
		int toggle = 0;
		for (int i = 0; i < registry.companions.components.size(); i++) {	// checks player side for mage NOT WORKING
			Entity toCheck = registry.companions.entities[i];
			if (registry.companions.get(toCheck).companionType == MAGE) {
				toggle = 1;
			}
		}
		if (toggle == 1) {	// if player side has mage, execute child which is check taunt (fireball for now)
			printf("Player side indeed has mage \n");
			return m_child->process(e);
		}
		else
			return BTState::Success;
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfMagicianTauntedHardCoded : public BTNode
{
public:
	BTIfMagicianTauntedHardCoded(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("If magician is taunted ... \n");	// print statement to visualize
		// check if player mage is taunted
		if (isTaunt == 1) {	// if player mage is taunted, execute child which is fireball
			printf("Magician is indeed taunted \n");
			return m_child->process(e);
		}
		else
			return BTState::Success;
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfMagicianNotTauntedHardCoded : public BTNode
{
public:
	BTIfMagicianNotTauntedHardCoded(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("If magician is not taunted ... \n");	// print statement to visualize
		// check if player mage is taunted
		if (isTaunt == 0) {	// if player mage is not taunted, execute child which is taunt
			printf("Magician is not taunted \n");
			return m_child->process(e);
		}
		else
			return BTState::Success;
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfEnemyIsSwordsman : public BTNode
{
public:
	BTIfEnemyIsSwordsman(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("Checking if enemy is swordsman ... \n");	// print statement to visualize
		if (registry.enemies.get(currPlayer).enemyType == SWORDSMAN) {	// WORKS, if enemy character is swordsman, execute child which is checking player mage
			printf("Enemy is indeed swordsman \n");
			return m_child->process(e);
		}
		else {
			return BTState::Success;
		}
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfEnemyIsMagician : public BTNode
{
public:
	BTIfEnemyIsMagician(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("Checking if enemy is magician ... \n");	// print statement to visualize
		if (registry.enemies.get(currPlayer).enemyType == MAGE) {	// WORKS, if enemy character is magician, execute child which is temporarily fireball
			printf("Enemy is indeed magician \n");
			return m_child->process(e);
		}
		else {
			return BTState::Success;
		}
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfIAmTaunted : public BTNode
{
public:
	BTIfIAmTaunted(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("Checking if I am taunted ... \n");	// print statement to visualize
		// to implement checking of taunt
		if (enemyTaunt == 1) {
			printf("I am indeed taunted \n");
			return m_child->process(e);
		}
		else {
			return BTState::Success;
		}
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfIAmNotTaunted : public BTNode
{
public:
	BTIfIAmNotTaunted(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("Checking if I am not taunted ... \n");	// print statement to visualize
		// to implement checking of taunt
		if (enemyTaunt == 0) {
			printf("I am not taunted \n");
			return m_child->process(e);
		}
		else {
			return BTState::Success;
		}
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfOneLessThanHalf : public BTNode
{
public:
	BTIfOneLessThanHalf(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		int lowestHealth = 50;	// set HP to half
		int toggle = 0;
		for (int i = 0; i < registry.enemies.components.size(); i++) {
			Entity currEntity = registry.enemies.entities[i];
			int currHealth = registry.stats.get(currEntity).health;
			if (currHealth < lowestHealth) {
				lowestHealth = currHealth;
				toggle = 1;
			}
		}
		printf("Checking if at least one is less than half HP ... \n");	// print statement to visualize
		printf("Check health toggle = %g \n", float(toggle));
		if (toggle == 1) {
			printf("There is at least one with less than half HP \n");
			return m_child->process(e);
		}
		else {
			return BTState::Success;
		}
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfNoneLessThanHalf : public BTNode
{
public:
	BTIfNoneLessThanHalf(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		int lowestHealth = 50;	// set HP to half
		int toggle = 0;
		for (int i = 0; i < registry.enemies.components.size(); i++) {
			Entity currEntity = registry.enemies.entities[i];
			int currHealth = registry.stats.get(currEntity).health;
			if (currHealth < lowestHealth) {
				lowestHealth = currHealth;
				toggle = 1;
			}
		}
		printf("Checking if no characters have less than half HP ... \n");	// print statement to visualize
		if (toggle == 0) {
			printf("There is no characters with less than half HP \n");
			return m_child->process(e);
		}
		else {
			return BTState::Success;
		}
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfMageHPBelowHalf : public BTNode
{
public:
	BTIfMageHPBelowHalf(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		int toggle = 0;
		if (registry.stats.get(currPlayer).health < 50) {
			toggle = 1;
		}
		printf("Checking if mage HP is below half ... \n");	// print statement to visualize
		if (toggle == 1) {
			printf("Mage HP is indeed below half \n");
			return m_child->process(e);
		}
		else {
			return BTState::Success;
		}
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfMageHPAboveHalf : public BTNode
{
public:
	BTIfMageHPAboveHalf(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		int toggle = 0;
		printf("Accessing currPlayer \n");
		printf("currPlayer HP is %g \n", float(registry.stats.get(currPlayer).health));
		if (registry.stats.get(currPlayer).health < 50) {
			toggle = 1;
		}
		printf("Checking if mage HP is above half ... \n");	// print statement to visualize
		if (toggle == 0) {
			printf("Mage HP is indeed above half \n");
			return m_child->process(e);
		}
		else {
			return BTState::Success;
		}
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfPlayerSideHasSwordsman : public BTNode
{
public:
	BTIfPlayerSideHasSwordsman(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("Checking if player side has swordsman ... \n");	// print statement to visualize
		int toggle = 0;
		for (int i = 0; i < registry.companions.components.size(); i++) {	// checks player side for mage NOT WORKING
			Entity toCheck = registry.companions.entities[i];
			if (registry.companions.get(toCheck).companionType == SWORDSMAN) {
				toggle = 1;
			}
		}
		if (toggle == 1) {	// if player side has mage, execute child which is check taunt (fireball for now)
			printf("Player side indeed has swordsman \n");
			return m_child->process(e);
		}
		else
			return BTState::Success;
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

// A general decorator with lambda condition
class BTIfPlayerSideDoNotHaveSwordsman : public BTNode
{
public:
	BTIfPlayerSideDoNotHaveSwordsman(BTNode* child)	// Has one child
		: m_child(child) {
	}

	virtual void init(Entity e) override {
		m_child->init(e);
	}

	virtual BTState process(Entity e) override {
		printf("Checking if player side has swordsman ... \n");	// print statement to visualize
		int toggle = 0;
		for (int i = 0; i < registry.companions.components.size(); i++) {
			Entity toCheck = registry.companions.entities[i];
			if (registry.companions.get(toCheck).companionType == SWORDSMAN) {
				toggle = 1;
			}
		}
		if (toggle == 0) {	// if player side has mage, execute child which is check taunt (fireball for now)
			printf("Player side do not have swordsman \n");
			return m_child->process(e);
		}
		else
			return BTState::Success;
	}
private:
	BTNode* m_child;	// one child stored in BTNode as a pointer
};

class BTCastFireball : public BTNode {
private:
	void init(Entity e) override {
	}
	BTState process(Entity e) override {
		printf("Shoot fireball \n\n");	// print statement to visualize
		worldSystem.fireballAttack(currPlayer);
		// return progress
		return BTState::Success;
	}
};

class BTCastTaunt : public BTNode {
private:
	void init(Entity e) override {
	}
	BTState process(Entity e) override {
		worldSystem.tauntSkill(target);
		Taunt* t = &registry.taunts.get(target);
		t->duration = 3;
		isTaunt = 1;
		printf("Cast Taunt \n\n");	// print statement to visualize

		// return progress
		return BTState::Success;
	}
};

class BTMeleeAttack : public BTNode {
private:
	void init(Entity e) override {
	}
	BTState process(Entity e) override {
		int i = 0;
		for (int i = 0; i < registry.companions.components.size(); i++) {	// checks player side for mage NOT WORKING
			Entity toGet = registry.companions.entities[i];
			if (registry.motions.get(toGet).position.x > i) {
				i = registry.motions.get(toGet).position.x;
				target = toGet;	// get nearest player entity
			}
		}
		//worldSystem.meleeSkill(target); // TODO: melee target
		Enemy& enemy = registry.enemies.get(e);
		enemy.curr_anim_type = MELEE;
		Attack& attack = registry.attackers.emplace(e);
		attack.attack_type = MELEE;
		attack.target = target;
		printf("Melee Attack \n\n");	// print statement to visualize

		// return progress
		return BTState::Success;
	}
};

class BTCastRock : public BTNode {
private:
	void init(Entity e) override {
	}
	BTState process(Entity e) override {
		for (int i = 0; i < registry.companions.components.size(); i++) {
			Entity toGet = registry.companions.entities[i];
			if (registry.companions.get(toGet).companionType == SWORDSMAN) {
				target = toGet;
			}
		}
		//worldSystem.rockAttack(e, target); // TODO: to get rock attack target ONLY SWORDSMAN
		Enemy& enemy = registry.enemies.get(e);
		enemy.curr_anim_type = CASTING;
		Attack& attack = registry.attackers.emplace(e);
		attack.attack_type = ROCK;
		attack.target = target;
		printf("Cast Rock \n\n");	// print statement to visualize

		// return progress
		return BTState::Success;
	}
};

class BTCastHeal : public BTNode {
private:
	void init(Entity e) override {
	}
	BTState process(Entity e) override {
		for (int i = 0; i < registry.enemies.components.size(); i++) {
			Entity toGet = registry.enemies.entities[i];
			if (registry.enemies.get(toGet).enemyType == SWORDSMAN) {
				target = toGet;
			}
		}
		//worldSystem.healSkill(target, 100); // TODO: to heal target
		Enemy& enemy = registry.enemies.get(e);
		enemy.curr_anim_type = CASTING;
		Attack& attack = registry.attackers.emplace(e);
		attack.attack_type = HEAL;
		attack.target = target;
		printf("Cast Heal \n\n");	// print statement to visualize

		// return progress
		return BTState::Success;
	}
};

class BTCastHealOnSelf : public BTNode {
private:
	void init(Entity e) override {
	}
	BTState process(Entity e) override {
		for (int i = 0; i < registry.enemies.components.size(); i++) {	// checks player side for mage NOT WORKING
			Entity toGet = registry.enemies.entities[i];
			if (registry.enemies.get(toGet).enemyType == MAGE) {
				target = toGet;
			}
		}
		//worldSystem.healSkill(target, 100); // TODO: to heal target
		Enemy& enemy = registry.enemies.get(e);
		enemy.curr_anim_type = CASTING;
		Attack& attack = registry.attackers.emplace(e);
		attack.attack_type = HEAL;
		attack.target = target;
		printf("Cast Heal On Self \n\n");	// print statement to visualize

		// return progress
		return BTState::Success;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Set up enemy behavior tree flow
// Leaf Nodes
BTCastFireball castFireball;
BTCastTaunt castTaunt;				// to implement
BTMeleeAttack meleeAttack;			// to implement
BTCastRock castRock;				// to implement
BTCastHeal castHeal;				// to implement
BTCastHealOnSelf castHealOnSelf;	// to implement

// Conditional Sub-Tree for Level 3 Nodes
BTIfMageHPBelowHalf mageBelowHalf(&castHealOnSelf);				// done
BTIfMageHPAboveHalf mageAboveHalf(&castHeal);					// done
BTIfPlayerSideHasSwordsman haveSwordsman(&castRock);			// partial <- cast thunderbolt at swordsman specifically
BTIfPlayerSideDoNotHaveSwordsman noSwordsman(&castFireball);	// done

// Level 3 Nodes
BTRunCheckMageHP checkMageHP(&mageBelowHalf, &mageAboveHalf);		// run pair do not need any further implementation? can merge all run pairs later and test
BTRunCheckSwordsman checkSwordsman(&haveSwordsman, &noSwordsman);	// run pair

// Conditional Sub-Tree for Level 2 Nodes
BTIfOneLessThanHalf atLeastOne(&checkMageHP);						// done
BTIfNoneLessThanHalf none(&checkSwordsman);							// done
BTIfMagicianTauntedHardCoded isTaunted(&meleeAttack);				// to implement <- melee attack at magician specifically
BTIfMagicianNotTauntedHardCoded notTaunted(&castTaunt);				// to implement <- cast taunt at magician specifically

// Level 2 Nodes
BTRunCheckEnemyHP checkHP(&none, &atLeastOne);			// run pair
BTRunCheckTaunt checkTaunted(&isTaunted, &notTaunted);	// run pair

// Conditionl Sub-Tree for Level 1 Nodes
BTIfIAmNotTaunted nonTaunted(&checkHP);								// to implement
BTIfIAmTaunted taunted(&checkSwordsman);							// to implement
BTIfPlayerSideHasMageHardCoded haveMage(&checkTaunted);				// done
BTIfPlayerSideDoNotHaveMageHardCoded doNotHaveMage(&meleeAttack);	// done

// Level 1 Nodes
BTRunCheckEnemyTaunt checkEnemyTaunt(&taunted, &nonTaunted);	// run pair
BTRunCheckMage checkMage(&haveMage, &doNotHaveMage);			// run pair

// Conditional Sub-Trees for Level 0
BTIfEnemyIsMagician isMagician(&checkEnemyTaunt);	// done
BTIfEnemyIsSwordsman isSwordsman(&checkMage);		// done

// Level 0 Root Node
BTRunCheckCharacter checkChar(&isMagician, &isSwordsman);	// run pair

// --------------------------------------------------------------------------------

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	// Get the screen dimensions
	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);

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

	for (int i = (int)registry.taunts.components.size() - 1; i >= 0; --i) {
		if (registry.taunts.components[i].duration <= 0) {
			registry.remove_all_components_of(registry.taunts.entities[i]);
			printf("taunt removed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		}
	}

	// Attack
	for(Entity attacker: registry.attackers.entities){
		Attack& attack = registry.attackers.get(attacker);
		// Updating animation time
		attack.counter_ms -= elapsed_ms_since_last_update;
		printf("Animation seconds left: %f\n", attack.counter_ms);
		if(attack.counter_ms<=0.f){
			// Attack
			if(registry.companions.has(attacker)){
				printf("Companion is attacking\n");
				Companion& companion = registry.companions.get(attacker);
				Motion& companion_motion = registry.motions.get(attacker);
				switch(attack.attack_type){
					case FIREBALL: currentProjectile = launchFireball(companion_motion.position, attack.mouse_pos); break;
				}
				companion.curr_anim_type = IDLE;
				printf("Not attacking anymore in idle\n");
				registry.attackers.remove(attacker);
			} else if(registry.enemies.has(attacker)){
				printf("Enemy is attacking\n");
				Enemy& enemy = registry.enemies.get(attacker);
				switch(attack.attack_type){
					case ROCK: rockAttack(attack.target); break;
					case HEAL: healSkill(attack.target, 100); break;
					case MELEE: meleeSkill(attack.target); break;
				}
				enemy.curr_anim_type = IDLE;
				printf("Not attacking anymore in idle\n");
				registry.attackers.remove(attacker);
			}
		}
	}

	if (player_turn == 1) {
		prevPlayer = currPlayer;

	}
	// this area is to check for edge cases to allow for enemies to hit twice if the round allows it
	if (player_turn == 0) {
		displayEnemyTurn();
		if (registry.companions.has(prevPlayer) && registry.enemies.has(currPlayer)) {	// checks if selected character has died so as to progress to an enemy's
			if (registry.stats.get(prevPlayer).health <= 0) {
				checkChar.init(currPlayer);
				for (int i = 0; i < 100; i++) {
					BTState state = checkChar.process(currPlayer);
					if (state != BTState::Running) {	// break out of for loop when all branches checked
						break;
					}
				}
				// temporaryFireball(currPlayer);
				printf("enemy has attacked, checkRound now \n");
				checkRound();
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
					checkChar.init(currPlayer);
					for (int i = 0; i < 100; i++) {
						BTState state = checkChar.process(currPlayer);
						if (state != BTState::Running) {	// break out of for loop when all branches checked
							break;
						}
					}
					// temporaryFireball(currPlayer);
					printf("enemy has attacked, checkRound now \n");
					checkRound();
				}
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
			registry.remove_all_components_of(entity);	// added back in, kinda works
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
					registry.motions.get(entity).position.x += 20;
				}
				else {
					registry.motions.get(entity).position.x -= 20;
				}
			}				
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

	player_turn = 1;	// player turn indicator
	roundVec.clear();	// empty vector roundVec to create a new round
	createRound();

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
	// Create an enemy swordsman
	enemy_swordsman = createEnemySwordsman(renderer, { 875, 500 });
	registry.colors.insert(enemy_swordsman, { 0.f, 1.f, 1.f });
	// Create the necromancer
	// necromancer = createNecromancer(renderer, { 1100, 400 }); // remove for now
	// Create the fireball icon
	fireball_icon = createFireballIcon(renderer, { 600, 700 });
	silence_icon = createSilenceIcon(renderer, { 800, 700 });;

}




void WorldSystem::update_health(Entity entity, Entity other_entity) {
	if(registry.projectiles.has(entity)){
		Damage& damage = registry.damages.get(entity);
		Statistics* hp = nullptr;
		Entity healthbar;
		if(damage.isFriendly){
			if(registry.enemies.has(other_entity)){
				Enemy& enemy = registry.enemies.get(other_entity);
				healthbar = enemy.healthbar;
				hp = &registry.stats.get(other_entity);
			}
		} else {
			if(registry.companions.has(other_entity)){
				Companion& companion = registry.companions.get(other_entity);
				healthbar = companion.healthbar;
				hp = &registry.stats.get(other_entity);
			}
		}
		if(hp){
			hp->health = hp->health - (rand() % damage.range + damage.minDamage);
			Motion& motion = registry.motions.get(healthbar);
			if (registry.stats.get(currPlayer).health <= 0) {	// check if HP of currPlayer is 0, checkRound to skip this player
				if (!registry.deathTimers.has(other_entity)) {
					registry.deathTimers.emplace(other_entity);
				}
				checkRound();
				motion.scale = vec2({ (HEALTHBAR_WIDTH * (99.f / 100.f)), HEALTHBAR_HEIGHT });
			}
			else {
				if (hp->health <= 0) {
					if (!registry.deathTimers.has(other_entity)) {
						registry.deathTimers.emplace(other_entity);
					}
					motion.scale = vec2({ (HEALTHBAR_WIDTH * (99.f / 100.f)), HEALTHBAR_HEIGHT });
				}
				else {
					motion.scale = vec2({ (HEALTHBAR_WIDTH * (hp->health / 100.f)), HEALTHBAR_HEIGHT });
				}
			}
		}
	}
}
/*
void WorldSystem::update_healthBars() {
	printf("updating healths\n");
	for(Entity entity: registry.stats.entities){
		printf("in loop\n");
		Statistics& hp = registry.stats.get(entity);
		Entity healthbar;
		if(registry.enemies.has(entity)){
			printf("if?\n");
			Enemy& enemy = registry.enemies.get(entity);
			printf("end if?\n");
			healthbar = enemy.healthbar;
		} else if (registry.companions.has(entity)){
			printf("else?\n");
			Companion& companion = registry.companions.get(entity);
			printf("end else?\n");
			healthbar = companion.healthbar;
		}
		if(healthbar){
			Motion& motion = registry.motions.get(healthbar);
			if(hp.health<=0){
				if(!registry.deathTimers.has(entity)){
					registry.deathTimers.emplace(entity);
				}
				motion.scale = vec2({ (HEALTHBAR_WIDTH*(99.f/100.f)), HEALTHBAR_HEIGHT });
				
			} else {
				motion.scale = vec2({ (HEALTHBAR_WIDTH*(hp.health/100.f)), HEALTHBAR_HEIGHT });
			}
		}
		Motion& motion = registry.motions.get(healthbar);
			
		
	}
}
*/

void WorldSystem::update_healthBars() {
	for(Entity entity: registry.enemies.entities){
		Enemy& enemy = registry.enemies.get(entity);
		Statistics& stat = registry.stats.get(entity);
		Entity healthbar = enemy.healthbar;
		Motion& motion = registry.motions.get(healthbar);
		motion.scale = vec2({ (HEALTHBAR_WIDTH*(stat.health/100.f)), HEALTHBAR_HEIGHT });	
	}
	for(Entity entity: registry.companions.entities){
		Companion& enemy = registry.companions.get(entity);
		Statistics& stat = registry.stats.get(entity);
		Entity healthbar = enemy.healthbar;
		Motion& motion = registry.motions.get(healthbar);
		motion.scale = vec2({ (HEALTHBAR_WIDTH*(stat.health/100.f)), HEALTHBAR_HEIGHT });	
	}
}



//void WorldSystem::simple_hp_update(Entity target) {
//	Entity healthbar = registry.enemies.get(target);
//	Motion& motion = registry.motions.get(healthbar);
//	if (hp->health <= 0) {
//		if (!registry.deathTimers.has(target)) {
//			registry.deathTimers.emplace(target);
//		}
//		motion.scale = vec2({ (HEALTHBAR_WIDTH * (99.f / 100.f)), HEALTHBAR_HEIGHT });
//
//	}
//	else {
//		motion.scale = vec2({ (HEALTHBAR_WIDTH * (hp->health / 100.f)), HEALTHBAR_HEIGHT });
//	}
//}

// Compute collisions between entities
void WorldSystem::handle_collisions() {
	// reduce turne

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
							displayPlayerTurn();	// displays player turn when enemy hits collide
						}
					}
				}
			}
			// create death particles. Register for rendering.
			if (registry.stats.has(entity) && registry.stats.get(entity).health <= 0)
			{
				// get rid of dead entity's healthbar.
				Entity entityHealthbar = registry.companions.get(entity).healthbar;
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
								displayEnemyTurn();
								if (registry.enemies.has(currPlayer)) {	// check if enemies have currPlayer
									prevPlayer = currPlayer;
									checkChar.init(currPlayer);
									for (int i = 0; i < 100; i++) {
										BTState state = checkChar.process(currPlayer);
										if (state != BTState::Running) {	// break out of for loop when all branches checked
											break;
										}
									}
									// temporaryFireball(currPlayer);
									printf("enemy has attacked, checkRound now \n");
									checkRound();
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

			// create death particles. Register for rendering.
			if (registry.stats.has(entity) && registry.stats.get(entity).health <= 0)
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
			//enemy turn start
			if (player_turn == 0) {
				displayEnemyTurn();
				if (registry.enemies.has(currPlayer)) {	// check if enemies have currPlayer
					checkChar.init(currPlayer);
					for (int i = 0; i < 100; i++) {
						BTState state = checkChar.process(currPlayer);
						if (state != BTState::Running) {	// break out of for loop when all branches checked
							break;
						}
					}
					// temporaryFireball(currPlayer);
					printf("enemy has attacked, checkRound now \n");
					checkRound();
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

// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}

// On key callback

void WorldSystem::on_key(int key, int, int action, int mod) {	
	// TODO: Handle mouse click on fireball icon
	if (action == GLFW_RELEASE && key == GLFW_MOUSE_BUTTON_LEFT) {

	}
	// Test for skills
	if (action == GLFW_RELEASE && key == GLFW_KEY_K) {
		launchMelee(enemy_swordsman);
		
	}

	if (action == GLFW_RELEASE && key == GLFW_KEY_H) {
		healTarget(player_swordsman, 30);
	}

	if (action == GLFW_RELEASE && key == GLFW_KEY_L) {
		launchRock(player_mage);
	}

	if (action == GLFW_RELEASE && key == GLFW_KEY_T) {
		launchTaunt(player_swordsman);
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

        restart_game();
	}


	// temp arrow skill "A"
	if (action == GLFW_RELEASE && key == GLFW_KEY_A) {
		launchArrow(registry.motions.get(player_mage).position);
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
}

void WorldSystem::on_mouse_button( int button , int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		// fireball
		if (player_turn == 1) {
			displayPlayerTurn();
			if (registry.companions.has(currPlayer)) {
				Motion icon = registry.motions.get(fireball_icon);
				if (inButton(icon.position, FIREBALL_ICON_WIDTH, FIREBALL_ICON_HEIGHT)) {
					if (FIREBALLSELECTED == 0) {
						selectedButton = createFireballIconSelected(renderer, { icon.position.x,icon.position.y });
						FIREBALLSELECTED = 1;
					}
					else {
						deselectButton();
						FIREBALLSELECTED = 0;
					}
				}
				else {
					if (FIREBALLSELECTED == 1) {
						Motion player = registry.motions.get(currPlayer);	// need to change to based on turn system
						Companion& player_companion = registry.companions.get(currPlayer);
						Attack& attacker = registry.attackers.emplace(currPlayer);
						printf("Companion is casting\n");
						player_companion.curr_anim_type = CASTING;
						attacker.attack_type = FIREBALL;
						attacker.mouse_pos = msPos;
						//attacker.attack_type = CASTING;
						//currentProjectile = launchFireball(player.position);
						FIREBALLSELECTED = 0;
						//active this when ai is done
						deselectButton();
						printf("player has attacked, checkRound now \n");
						checkRound();
					}
				}
			} else {
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

void WorldSystem::on_mouse_move(vec2 mouse_position) {
	msPos = mouse_position;
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
void WorldSystem::healTarget(Entity target, float amount) {

	vec2 targetp = registry.motions.get(target).position;
	createGreenCross(renderer, targetp);
	if (registry.stats.has(target)) {
		Statistics* tStats = &registry.stats.get(target);
		if (tStats->health + amount > tStats->max_health) {
			tStats->health = tStats->max_health;
		}
		else
		{
			tStats->health += amount;
		}		
	}
	update_healthBars();
}

void WorldSystem::damageTarget(Entity target, float amount) {
	if (registry.stats.has(target)) {
		Statistics* tStats = &registry.stats.get(target);
			tStats->health -= amount;
	}
	update_healthBars();
}

Entity WorldSystem::launchArrow(vec2 startPos) {

	float proj_x = startPos.x + 50;
	float proj_y = startPos.y;
	float mouse_x = msPos.x;
	float mouse_y = msPos.y;

	float dx = mouse_x - proj_x;
	float dy = mouse_y - proj_y;
	float dxdy = sqrt((dx * dx) + (dy * dy));
	float vx = ARROWSPEED * dx / dxdy;
	float vy = ARROWSPEED * dy / dxdy;

	float angle = atan(dy / dx);
	if (dx < 0) {
		angle += M_PI;
	}
	Entity resultEntity = createArrow(renderer, { startPos.x + 50, startPos.y }, angle, { vx,vy }, 1);
	Motion* arrowacc = &registry.motions.get(resultEntity);
	arrowacc->acceleration = vec2(200 * vx / ARROWSPEED, 200 * vy / ARROWSPEED);
	return  resultEntity;
}


Entity WorldSystem::launchFireball(vec2 startPos, vec2 mouse_pos) {

	float proj_x = startPos.x + 50;
	float proj_y = startPos.y;
	float mouse_x = mouse_pos.x;
	float mouse_y = mouse_pos.y;

	float dx = mouse_x - proj_x;
	float dy = mouse_y - proj_y;
	float dxdy = sqrt((dx*dx) + (dy*dy));
	float vx = FIREBALLSPEED * dx / dxdy;
	float vy = FIREBALLSPEED * dy / dxdy;

	float angle = atan(dy / dx);
	if (dx < 0) {
		angle += M_PI;
	}
	Entity resultEntity = createFireball(renderer, { startPos.x + 50, startPos.y }, angle, {vx,vy}, 1);
	Motion* ballacc = &registry.motions.get(resultEntity);
	ballacc->acceleration = vec2(1000 * vx/ FIREBALLSPEED, 1000 * vy/ FIREBALLSPEED);
	Projectile* proj = &registry.projectiles.get(resultEntity);
	proj->flyingTimer = 2000.f;
	
	// ****temp**** enemy randomly spawn barrier REMOVED FOR NOW
	//int rng = rand() % 10;
	//if (rng >= 4) {
	//	createBarrier(renderer, registry.motions.get(enemy_mage).position);
	//}
	return  resultEntity;
}

Entity WorldSystem::launchRock(Entity target) {
	int isFriendly = 0;
	vec2 targetp = registry.motions.get(target).position;
	if (registry.companions.has(target)) {
		int isFriendly = 0;
	}
	Entity resultEntity = createRock(renderer, {targetp.x, targetp.y - 300}, isFriendly);
	Projectile* proj = &registry.projectiles.get(resultEntity);
	proj->flyingTimer = 2000.f;
	return  resultEntity;
}

Entity WorldSystem::launchMelee(Entity target) {
	int isFriendly = 1;
	vec2 targetp = registry.motions.get(target).position;
	if (registry.companions.has(target)) {
		int isFriendly = 0;
	}
	Entity resultEntity = createMelee(renderer, { targetp.x,targetp.y }, isFriendly);
	return  resultEntity;
}

void WorldSystem::launchTaunt(Entity target) {
	registry.taunts.emplace(target);
	Taunt* t =& registry.taunts.get(target);
	t->duration = 3;
	printf("taunted!!!!!!!!!!!!!!!!!!!!!!!\n");
}
