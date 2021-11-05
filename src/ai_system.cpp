// internal
#include "ai_system.hpp"
#include "world_system.hpp"

void AISystem::step(float elapsed_ms)
{
	(void)elapsed_ms; // placeholder to silence unused warning until implemented
}

// -----------------------------------------------------------------------------------------------------------------------
Entity target;
int playersDead = 0;
void BTNode::init(Entity e) {};

// Define enemy behavior tree nodes
// The return type of behaviour tree processing

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
BTRunCheckMage::BTRunCheckMage(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckMage::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckMage::process(Entity e) {
	printf("Pair run check mage ... child = %g \n", float(m_index));	// print statement to visualize
	if (m_index >= 2)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
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

// A composite node that loops through all children and exits when one fails
BTRunCheckPlayerMageTaunt::BTRunCheckPlayerMageTaunt(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckPlayerMageTaunt::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckPlayerMageTaunt::process(Entity e) {
	printf("Pair run check taunt ... child = %g \n", float(m_index));	// print statement to visualize
	if (m_index >= 2)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
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

// A composite node that loops through all children and exits when one fails
BTRunCheckCharacter::BTRunCheckCharacter(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckCharacter::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckCharacter::process(Entity e) {
	printf("Pair run check character ... child = %g \n", float(m_index));	// print statement to visualize
	if (m_index >= 2)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
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

// A composite node that loops through all children and exits when one fails
BTRunCheckMageTaunt::BTRunCheckMageTaunt(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckMageTaunt::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckMageTaunt::process(Entity e) {
	printf("Pair run check taunt for me ... child = %g \n", float(m_index));	// print statement to visualize
	if (m_index >= 2)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
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

// A composite node that loops through all children and exits when one fails
BTRunCheckEnemyHP::BTRunCheckEnemyHP(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckEnemyHP::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckEnemyHP::process(Entity e) {
	printf("Pair run check enemy HP for me ... child = %g \n", float(m_index));	// print statement to visualize
	if (m_index >= 2)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
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

// A composite node that loops through all children and exits when one fails
BTRunCheckMageHP::BTRunCheckMageHP(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckMageHP::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckMageHP::process(Entity e) {
	printf("Pair run check mage HP for me ... child = %g \n", float(m_index));	// print statement to visualize
	if (m_index >= 2)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
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

// A composite node that loops through all children and exits when one fails
BTRunCheckSwordsman::BTRunCheckSwordsman(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckSwordsman::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckSwordsman::process(Entity e) {
	printf("Pair run check swordsman for me ... child = %g \n", float(m_index));	// print statement to visualize
	if (m_index >= 2)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
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

// A composite node that loops through all children and exits when one fails
BTRunCheckSwordsmanTaunt::BTRunCheckSwordsmanTaunt(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckSwordsmanTaunt::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckSwordsmanTaunt::process(Entity e) {
	printf("Pair run check swordsman for me ... child = %g \n", float(m_index));	// print statement to visualize
	if (m_index >= 2)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
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

// A composite node that loops through all children and exits when one fails
BTRunCheckPlayersDead::BTRunCheckPlayersDead(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckPlayersDead::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckPlayersDead::process(Entity e) {
	printf("Pair run check swordsman for me ... child = %g \n", float(m_index));	// print statement to visualize
	if (m_index >= 2)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
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

// A general decorator with lambda condition
BTIfPlayerSideDoNotHaveMageHardCoded::BTIfPlayerSideDoNotHaveMageHardCoded(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayerSideDoNotHaveMageHardCoded::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayerSideDoNotHaveMageHardCoded::process(Entity e) {
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
		return BTState::Failure;
}

// A general decorator with lambda condition
BTIfPlayerSideHasMageHardCoded::BTIfPlayerSideHasMageHardCoded(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayerSideHasMageHardCoded::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayerSideHasMageHardCoded::process(Entity e) {
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
		return BTState::Failure;
}

// A general decorator with lambda condition
BTIfPlayerMageTaunted::BTIfPlayerMageTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayerMageTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayerMageTaunted::process(Entity e) {
	printf("If magician is taunted ... \n");	// print statement to visualize
	// check if player mage is taunted
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == MAGE) {	// only cast taunt on companion mage
			target = toGet;
		}
	}
	if (registry.taunts.has(target)) {
		printf("Player mage is indeed taunted \n");
		return m_child->process(e);
	}
	else
		return BTState::Failure;
}

// A general decorator with lambda condition
BTIfPlayerMageNotTaunted::BTIfPlayerMageNotTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayerMageNotTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayerMageNotTaunted::process(Entity e) {
	printf("If player mage is not taunted ... \n");	// print statement to visualize
	// check if player mage is taunted
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == MAGE) {	// only cast taunt on companion mage
			target = toGet;
		}
	}
	if (!registry.taunts.has(target)) {
		printf("Player mage is not taunted \n");
		return m_child->process(e);
	}
	else
		return BTState::Failure;
}

// A general decorator with lambda condition
BTIfEnemyIsSwordsman::BTIfEnemyIsSwordsman(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfEnemyIsSwordsman::init(Entity e) {
	m_child->init(e);
}

BTState BTIfEnemyIsSwordsman::process(Entity e) {
	printf("Checking if enemy is swordsman ... \n");	// print statement to visualize
	if (registry.enemies.get(currPlayer).enemyType == SWORDSMAN) {	// WORKS, if enemy character is swordsman, execute child which is checking player mage
		printf("Enemy is indeed swordsman \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfEnemyIsMagician::BTIfEnemyIsMagician(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfEnemyIsMagician::init(Entity e) {
	m_child->init(e);
}

BTState BTIfEnemyIsMagician::process(Entity e) {
	printf("Checking if enemy is magician ... \n");	// print statement to visualize
	if (registry.enemies.get(currPlayer).enemyType == MAGE) {	// WORKS, if enemy character is magician, execute child which is temporarily fireball
		printf("Enemy is indeed magician \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfEnemyMageTaunted::BTIfEnemyMageTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfEnemyMageTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfEnemyMageTaunted::process(Entity e) {
	printf("Checking if enemy mage is taunted ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == MAGE) {
			target = toGet;
		}
	}
	if (registry.taunts.has(target)) {
		printf("Enemy mage is indeed taunted \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfEnemyMageNotTaunted::BTIfEnemyMageNotTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfEnemyMageNotTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfEnemyMageNotTaunted::process(Entity e) {
	printf("Checking if enemy mage is taunted ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == MAGE) {
			target = toGet;
		}
	}
	if (!registry.taunts.has(target)) {
		printf("Enemy mage is not taunted \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfOneLessThanHalf::BTIfOneLessThanHalf(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfOneLessThanHalf::init(Entity e) {
	m_child->init(e);
}

BTState BTIfOneLessThanHalf::process(Entity e) {
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
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfNoneLessThanHalf::BTIfNoneLessThanHalf(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfNoneLessThanHalf::init(Entity e) {
	m_child->init(e);
}

BTState BTIfNoneLessThanHalf::process(Entity e) {
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
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfMageHPBelowHalf::BTIfMageHPBelowHalf(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfMageHPBelowHalf::init(Entity e) {
	m_child->init(e);
}

BTState BTIfMageHPBelowHalf::process(Entity e) {
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
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfMageHPAboveHalf::BTIfMageHPAboveHalf(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfMageHPAboveHalf::init(Entity e) {
	m_child->init(e);
}

BTState BTIfMageHPAboveHalf::process(Entity e) {
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
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfPlayerSideHasSwordsman::BTIfPlayerSideHasSwordsman(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayerSideHasSwordsman::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayerSideHasSwordsman::process(Entity e) {
	printf("Checking if player side has swordsman ... \n");	// print statement to visualize
	int toggle = 0;
	for (int i = 0; i < registry.companions.components.size(); i++) {	// checks player side for mage
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
		return BTState::Failure;
}

// A general decorator with lambda condition
BTIfPlayerSideDoNotHaveSwordsman::BTIfPlayerSideDoNotHaveSwordsman(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayerSideDoNotHaveSwordsman::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayerSideDoNotHaveSwordsman::process(Entity e) {
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
		return BTState::Failure;
}

// A general decorator with lambda condition
BTIfEnemySwordsmanNotTaunted::BTIfEnemySwordsmanNotTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfEnemySwordsmanNotTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfEnemySwordsmanNotTaunted::process(Entity e) {
	printf("Checking if enemy swordsman is taunted ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == SWORDSMAN) {
			target = toGet;
		}
	}
	if (!registry.taunts.has(target)) {
		printf("Enemy swordsman is not taunted \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfEnemySwordsmanTaunted::BTIfEnemySwordsmanTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfEnemySwordsmanTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfEnemySwordsmanTaunted::process(Entity e) {
	printf("Checking if enemy swordsman is taunted ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == SWORDSMAN) {
			target = toGet;
		}
	}
	if (registry.taunts.has(target)) {
		printf("Enemy swordsman is indeed taunted \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfPlayersDead::BTIfPlayersDead(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayersDead::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayersDead::process(Entity e) {
	printf("Checking if player characters are dead ... \n");	// print statement to visualize
	//if (registry.enemies.components.size() == 0) {
	//	printf("Player characters are indeed dead \n");
	//	return m_child->process(e);
	//}
	if (playersDead == 1) {
		printf("Player characters are indeed dead \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfPlayersAlive::BTIfPlayersAlive(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayersAlive::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayersAlive::process(Entity e) {
	printf("Checking if player characters are alive ... \n");	// print statement to visualize
	//if (registry.enemies.components.size() > 0) {
	//	printf("Player characters are indeed alive \n");
	//	return m_child->process(e);
	//}
	if (playersDead == 0) {
		printf("Player characters are indeed alive \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}


void BTCastIceShard::init(Entity e) {
}
BTState BTCastIceShard::process(Entity e) {
	printf("Shoot Ice Shard \n\n");	// print statement to visualize
	worldSystem.startIceShardAttack(e, currPlayer);
	// return progress
	return BTState::Success;
}

void BTCastTaunt::init(Entity e) {
}
BTState BTCastTaunt::process(Entity e) {
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == MAGE) {	// only cast taunt on companion mage
			target = toGet;
		}
	}
	worldSystem.startTauntAttack(e, target);
	printf("Cast Taunt \n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTMeleeAttack::init(Entity e) {
}
BTState BTMeleeAttack::process(Entity e) {
	int i = 0;
	for (int i = 0; i < registry.companions.components.size(); i++) {	// checks player side for mage NOT WORKING
		Entity toGet = registry.companions.entities[i];
		if (registry.motions.get(toGet).position.x > i) {
			i = registry.motions.get(toGet).position.x;
			target = toGet;	// get nearest player entity
		}
	}
	worldSystem.startMeleeAttack(e, target);

	printf("Melee Attack \n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTCastRock::init(Entity e) {
}
BTState BTCastRock::process(Entity e) {
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == SWORDSMAN) {
			target = toGet;
		}
	}
	worldSystem.startRockAttack(e, target);
	printf("Cast Rock \n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTCastHeal::init(Entity e) {
}
BTState BTCastHeal::process(Entity e) {
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == SWORDSMAN) {
			target = toGet;
		}
	}
	worldSystem.startHealAttack(e, target);

	printf("Cast Heal \n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTCastHealOnSelf::init(Entity e) {
}
BTState BTCastHealOnSelf::process(Entity e) {
	for (int i = 0; i < registry.enemies.components.size(); i++) {	// checks player side for mage NOT WORKING
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == MAGE) {
			target = toGet;
		}
	}
	worldSystem.startHealAttack(e, target);
	printf("Cast Heal On Self \n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTDoNothing::init(Entity e) {
}
BTState BTDoNothing::process(Entity e) {
	printf("Do Nothing \n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

// ---------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// Set up enemy behavior tree flow
// Leaf Nodes
BTCastIceShard castIceShard;
BTCastTaunt castTaunt;				// done
BTMeleeAttack meleeAttack;			// done
BTCastRock castRock;				// done
BTCastHeal castHeal;				// done
BTCastHealOnSelf castHealOnSelf;	// done
BTDoNothing doNothing;

// Conditional Sub-Tree for Level 3 Nodes
BTIfMageHPBelowHalf mageBelowHalf(&castHealOnSelf);				// done
BTIfMageHPAboveHalf mageAboveHalf(&castHeal);					// done
BTIfPlayerSideHasSwordsman haveSwordsman(&castRock);			// done
BTIfPlayerSideDoNotHaveSwordsman noSwordsman(&castIceShard);	// done
BTIfPlayerMageTaunted isTaunted(&meleeAttack);					// done
BTIfPlayerMageNotTaunted notTaunted(&castTaunt);				// done

// Level 3 Nodes
BTRunCheckMageHP checkMageHP(&mageBelowHalf, &mageAboveHalf);		// run pair do not need any further implementation? can merge all run pairs later and test
BTRunCheckSwordsman checkSwordsman(&haveSwordsman, &noSwordsman);	// run pair
BTRunCheckPlayerMageTaunt checkTaunted(&isTaunted, &notTaunted);	// run pair

// Conditional Sub-Tree for Level 2 Nodes
BTIfOneLessThanHalf atLeastOne(&checkMageHP);						// done
BTIfNoneLessThanHalf none(&checkSwordsman);							// done
BTIfPlayerSideHasMageHardCoded haveMage(&checkTaunted);				// done
BTIfPlayerSideDoNotHaveMageHardCoded doNotHaveMage(&meleeAttack);	// done

// Level 2 Nodes
BTRunCheckEnemyHP checkHP(&none, &atLeastOne);			// run pair
BTRunCheckMage checkMage(&haveMage, &doNotHaveMage);	// run pair

// Conditionl Sub-Tree for Level 1 Nodes
BTIfEnemyMageNotTaunted nonTaunted(&checkHP);					// done
BTIfEnemyMageTaunted taunted(&checkSwordsman);					// done
BTIfEnemySwordsmanNotTaunted swordsmanNotTaunted(&checkMage);	// done
BTIfEnemySwordsmanTaunted swordsmanTaunted(&meleeAttack);		// done

// Level 1 Nodes
BTRunCheckMageTaunt checkEnemyMageTaunt(&taunted, &nonTaunted);							// run pair
BTRunCheckSwordsmanTaunt checkSwordsmanTaunt(&swordsmanNotTaunted, &swordsmanTaunted);	// run pair

// Conditional Sub-Trees for Level 0
BTIfEnemyIsMagician isMagician(&checkEnemyMageTaunt);	// done
BTIfEnemyIsSwordsman isSwordsman(&checkSwordsmanTaunt);	// done

// Level 0 Root Node
BTRunCheckCharacter checkChar(&isMagician, &isSwordsman);	// run pair

// check if players are dead, if so do nothing
BTIfPlayersAlive isAlive(&checkChar);
BTIfPlayersDead isDead(&doNothing);
BTRunCheckPlayersDead checkPlayersDead(&isAlive, &isDead);	// DOESN'T WORK

// --------------------------------------------------------------------------------

void AISystem::callTree(Entity currPlayer)
{
	checkPlayersDead.init(currPlayer);
	for (int i = 0; i < 100; i++) {
		BTState state = checkPlayersDead.process(currPlayer);
		if (state == BTState::Success) {	// break out of for loop when all branches checked
			break;
		}
	}
}