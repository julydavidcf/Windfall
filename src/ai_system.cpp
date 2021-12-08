// internal
#include "ai_system.hpp"
#include "skill_system.hpp"

// UPDATED AI TREE 1/12/2021

RenderSystem* renderer; // to summon necrominion

AISystem::AISystem() {

}

AISystem::~AISystem() {

}

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
BTRunCheckCharacter::BTRunCheckCharacter(BTNode* c0, BTNode* c1, BTNode* c2, BTNode* c3, BTNode* c4)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
	m_children[2] = c2;
	m_children[3] = c3;
	m_children[4] = c4;
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
	if (m_index >= 5)
		return BTState::Success;

	// process current child
	BTState state = m_children[m_index]->process(e);

	// select a new active child and initialize its internal state
	if (state == BTState::Failure) {	// if child return success
		++m_index;	// increment index
		if (m_index >= 5) {	// check whether the second child is executed already
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
	printf("Pair run check swordsman taunt for me ... child = %g \n", float(m_index));	// print statement to visualize
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
	printf("Pair run check players dead for me ... child = %g \n", float(m_index));	// print statement to visualize
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
BTRunCheckNecroMinion::BTRunCheckNecroMinion(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckNecroMinion::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckNecroMinion::process(Entity e) {
	printf("Pair run check necro minion for me ... child = %g \n", float(m_index));	// print statement to visualize
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
BTRunCheckPlayerMageSilenced::BTRunCheckPlayerMageSilenced(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckPlayerMageSilenced::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckPlayerMageSilenced::process(Entity e) {
	printf("Pair run check player mage silenced for me ... child = %g \n", float(m_index));	// print statement to visualize
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
BTRunCheckUltiTurn::BTRunCheckUltiTurn(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckUltiTurn::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckUltiTurn::process(Entity e) {
	printf("Pair run check ultimate turn for me ... child = %g \n", float(m_index));	// print statement to visualize
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
BTRunCheckTurn::BTRunCheckTurn(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckTurn::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckTurn::process(Entity e) {
	printf("Pair run check turn for me ... child = %g \n", float(m_index));	// print statement to visualize
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
BTRunCheckShieldTurn::BTRunCheckShieldTurn(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckShieldTurn::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckShieldTurn::process(Entity e) {
	printf("Pair run check shield for me ... child = %g \n", float(m_index));	// print statement to visualize
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
BTRunCheckCrows::BTRunCheckCrows(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckCrows::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckCrows::process(Entity e) {
	printf("Pair run check crows for me ... child = %g \n", float(m_index));	// print statement to visualize
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
BTRunCheckNecroOneTaunt::BTRunCheckNecroOneTaunt(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckNecroOneTaunt::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckNecroOneTaunt::process(Entity e) {
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
BTRunCheckNecroTwoTaunt::BTRunCheckNecroTwoTaunt(BTNode* c0, BTNode* c1)	// build tree bottom up, we need to know children before building this node for instance
	: m_index(0) {
	m_children[0] = c0;
	m_children[1] = c1;
}

void BTRunCheckNecroTwoTaunt::init(Entity e)
{
	m_index = 0;	// set index to 0 to execute first child
	// initialize the first child
	const auto& child = m_children[m_index];
	child->init(e);
}

BTState BTRunCheckNecroTwoTaunt::process(Entity e) {
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
BTIfNecroOneTaunted::BTIfNecroOneTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfNecroOneTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfNecroOneTaunted::process(Entity e) {
	printf("Checking if necromancer one is taunted ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == NECROMANCER_ONE) {
			target = toGet;
		}
	}
	if (registry.taunts.has(target)) {
		printf("Necromancer One is indeed taunted \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfNecroOneNotTaunted::BTIfNecroOneNotTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfNecroOneNotTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfNecroOneNotTaunted::process(Entity e) {
	printf("Checking if necromancer one is taunted ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == NECROMANCER_ONE) {
			target = toGet;
		}
	}
	if (!registry.taunts.has(target)) {
		printf("Necromancer One is not taunted \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfNecroTwoTaunted::BTIfNecroTwoTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfNecroTwoTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfNecroTwoTaunted::process(Entity e) {
	printf("Checking if necromancer two is taunted ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == NECROMANCER_TWO) {
			target = toGet;
		}
	}
	if (registry.taunts.has(target)) {
		printf("Necromancer Two is indeed taunted \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfNecroTwoNotTaunted::BTIfNecroTwoNotTaunted(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfNecroTwoNotTaunted::init(Entity e) {
	m_child->init(e);
}

BTState BTIfNecroTwoNotTaunted::process(Entity e) {
	printf("Checking if necromancer two is taunted ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == NECROMANCER_TWO) {
			target = toGet;
		}
	}
	if (!registry.taunts.has(target)) {
		printf("Necromancer Two is not taunted \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfAtLeastOneLessThanHalf::BTIfAtLeastOneLessThanHalf(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfAtLeastOneLessThanHalf::init(Entity e) {
	m_child->init(e);
}

BTState BTIfAtLeastOneLessThanHalf::process(Entity e) {
	int toggle = 0;
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity currEntity = registry.enemies.entities[i];
		int currHealth = registry.stats.get(currEntity).health;
		int halfHealth = registry.stats.get(currEntity).max_health / 2;
		if (currHealth < halfHealth) {
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
	int toggle = 0;
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity currEntity = registry.enemies.entities[i];
		int currHealth = registry.stats.get(currEntity).health;
		int halfHealth = registry.stats.get(currEntity).max_health / 2;
		if (currHealth < halfHealth) {
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
	if (registry.stats.get(currPlayer).health < registry.stats.get(currPlayer).max_health / 2) {
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
	if (registry.stats.get(currPlayer).health < registry.stats.get(currPlayer).max_health / 2) {
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

// A general decorator with lambda condition
BTIfEnemyIsNecroMinion::BTIfEnemyIsNecroMinion(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfEnemyIsNecroMinion::init(Entity e) {
	m_child->init(e);
}

BTState BTIfEnemyIsNecroMinion::process(Entity e) {
	printf("Checking if enemy is necro minion ... \n");	// print statement to visualize
	if (registry.enemies.get(currPlayer).enemyType == NECROMANCER_MINION) {	// WORKS, if enemy character is necromancer minion, execute child which is melee attack
		printf("Enemy is indeed necro minion \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfEnemyIsNecroPhaseOne::BTIfEnemyIsNecroPhaseOne(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfEnemyIsNecroPhaseOne::init(Entity e) {
	m_child->init(e);
}

BTState BTIfEnemyIsNecroPhaseOne::process(Entity e) {
	printf("Checking if enemy is necro phase one ... \n");	// print statement to visualize
	if (registry.enemies.get(currPlayer).enemyType == NECROMANCER_ONE) {	// WORKS, if enemy character is necromancer phase one, execute child which is checking necromancer minions
		printf("Enemy is indeed necro phase one \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfHasNecroMinion::BTIfHasNecroMinion(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfHasNecroMinion::init(Entity e) {
	m_child->init(e);
}

BTState BTIfHasNecroMinion::process(Entity e) {
	printf("Checking if enemy side has necro minion ... \n");	// print statement to visualize
	int toggle = 0;
	for (int i = 0; i < registry.enemies.components.size(); i++) {	// checks enemy side for necromancer minion
		Entity E = registry.enemies.entities[i];
		if (registry.enemies.get(E).enemyType == NECROMANCER_MINION) {
			toggle = 1;
		}
	}
	if (toggle == 1) {	// if player side has necromancer minion, execute child which is check mage
		printf("Enemy side indeed has necro minion \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfNoNecroMinion::BTIfNoNecroMinion(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfNoNecroMinion::init(Entity e) {
	m_child->init(e);
}

BTState BTIfNoNecroMinion::process(Entity e) {
	printf("Checking if enemy side has necro minion ... \n");	// print statement to visualize
	int toggle = 0;
	for (int i = 0; i < registry.enemies.components.size(); i++) {	// checks enemy side for necromancer minion
		Entity E = registry.enemies.entities[i];
		if (registry.enemies.get(E).enemyType == NECROMANCER_MINION) {
			toggle = 1;
		}
	}
	if (toggle == 0) {	// if player side does not have necromancer minion, execute child which is summon necromancer minion
		printf("Enemy side does not have necro minion \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfPlayerMageSilenced::BTIfPlayerMageSilenced(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayerMageSilenced::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayerMageSilenced::process(Entity e) {
	printf("Checking if player mage is silenced ... \n");	// print statement to visualize
	// check if player mage is silenced
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == MAGE) {	// only cast taunt on companion mage
			target = toGet;
		}
	}
	if (registry.silenced.has(target)) {
		printf("Player mage is not silenced \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfPlayerMageNotSilenced::BTIfPlayerMageNotSilenced(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfPlayerMageNotSilenced::init(Entity e) {
	m_child->init(e);
}

BTState BTIfPlayerMageNotSilenced::process(Entity e) {
	printf("Checking if player mage is silenced ... \n");	// print statement to visualize
	// check if player mage is silenced
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == MAGE) {	// only cast taunt on companion mage
			target = toGet;
		}
	}
	if (!registry.silenced.has(target)) {
		printf("Player mage is not silenced \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfEnemyIsNecroPhaseTwo::BTIfEnemyIsNecroPhaseTwo(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfEnemyIsNecroPhaseTwo::init(Entity e) {
	m_child->init(e);
}

BTState BTIfEnemyIsNecroPhaseTwo::process(Entity e) {
	printf("Checking if enemy is necro phase two ... \n");	// print statement to visualize
	if (registry.enemies.get(currPlayer).enemyType == NECROMANCER_TWO) {	// WORKS, if enemy character is necromancer phase one, execute child which is checking necromancer minions
		printf("Enemy is indeed necro phase two \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfUltiTurn::BTIfUltiTurn(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfUltiTurn::init(Entity e) {
	m_child->init(e);
}

BTState BTIfUltiTurn::process(Entity e) {
	printf("Checking if it is ultimate's turn ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == NECROMANCER_TWO) {	// only necromancer two using ulti
			target = toGet;
		}
	}
	if (!registry.ultimate.has(target)) {
		printf("It is indeed ultimate's turn \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfNotUltiTurn::BTIfNotUltiTurn(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfNotUltiTurn::init(Entity e) {
	m_child->init(e);
}

BTState BTIfNotUltiTurn::process(Entity e) {
	printf("Checking if it is ultimate's turn ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == NECROMANCER_TWO) {	// only necromancer two using ulti
			target = toGet;
		}
	}
	if (registry.ultimate.has(target)) {
		printf("It is not ultimate's turn \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfTurnOne::BTIfTurnOne(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfTurnOne::init(Entity e) {
	m_child->init(e);
}

BTState BTIfTurnOne::process(Entity e) {
	printf("Checking if it is turn one ... \n");	// print statement to visualize
	printf("CURRPLAYER SPEED IS %g \n", float(registry.stats.get(currPlayer).speed));
	printf("PREVPLAYER SPEED IS %g \n", float(registry.stats.get(prevPlayer).speed));
	if (currPlayer != prevPlayer) {
		printf("It is indeed turn one \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfTurnTwo::BTIfTurnTwo(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfTurnTwo::init(Entity e) {
	m_child->init(e);
}

BTState BTIfTurnTwo::process(Entity e) {
	printf("Checking if it is turn two ... \n");	// print statement to visualize
	printf("CURRPLAYER SPEED IS %g \n", float(registry.stats.get(currPlayer).speed));
	printf("PREVPLAYER SPEED IS %g \n", float(registry.stats.get(prevPlayer).speed));
	if (currPlayer == prevPlayer) {
		printf("It is indeed turn two \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfShieldTurn::BTIfShieldTurn(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfShieldTurn::init(Entity e) {
	m_child->init(e);
}

BTState BTIfShieldTurn::process(Entity e) {
	printf("Checking if it is shield turn ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == NECROMANCER_TWO) {	// only necromancer two using shield
			target = toGet;
		}
	}
	if (!registry.shield.has(target)) {
		printf("It is indeed shield turn \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfNotShieldTurn::BTIfNotShieldTurn(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfNotShieldTurn::init(Entity e) {
	m_child->init(e);
}

BTState BTIfNotShieldTurn::process(Entity e) {
	printf("Checking if it is not shield turn ... \n");	// print statement to visualize
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == NECROMANCER_TWO) {	// only necromancer two using shield
			target = toGet;
		}
	}
	if (registry.shield.has(target)) {
		printf("It is not shield turn \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfCrowsZero::BTIfCrowsZero(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfCrowsZero::init(Entity e) {
	m_child->init(e);
}

BTState BTIfCrowsZero::process(Entity e) {
	printf("Checking if there are no more crows ... \n");	// print statement to visualize
	int x = 0; // FOR TESTING, TO REMOVE
	if (x == 0) {
		printf("There are no more crows \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

// A general decorator with lambda condition
BTIfCrowsMoreThanZero::BTIfCrowsMoreThanZero(BTNode* child)	// Has one child
	: m_child(child) {
}

void BTIfCrowsMoreThanZero::init(Entity e) {
	m_child->init(e);
}

BTState BTIfCrowsMoreThanZero::process(Entity e) {
	printf("Checking if there are crows ... \n");	// print statement to visualize
	int x = 0; // FOR TESTING, TO REMOVE
	if (x == 0) {
		printf("There are indeed crows \n");
		return m_child->process(e);
	}
	else {
		return BTState::Failure;
	}
}

void BTCastIceShard::init(Entity e) {
}
BTState BTCastIceShard::process(Entity e) {
	SkillSystem sk;
	printf("Shoot Ice Shard \n\n");	// print statement to visualize
	sk.startIceShardAttack(e, currPlayer);
	// return progress
	return BTState::Success;
}

void BTCastTauntOnMage::init(Entity e) {
}
BTState BTCastTauntOnMage::process(Entity e) {
	SkillSystem sk;
	printf("Cast Taunt \n\n");	// print statement to visualize
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == MAGE) {	// only cast taunt on companion mage
			target = toGet;
		}
	}
	sk.startTauntAttack(e, target);

	// return progress
	return BTState::Success;
}

void BTMeleeAttack::init(Entity e) {
}
BTState BTMeleeAttack::process(Entity e) {
	int j = 0;
	SkillSystem sk;
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.motions.get(toGet).position.x > j) {
			j = registry.motions.get(toGet).position.x;
			target = toGet;	// get nearest player entity
		}
	}
	sk.startMeleeAttack(e, target, -1);

	printf("Melee Attack \n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTMeleeAttackOnSwordsman::init(Entity e) {
}
BTState BTMeleeAttackOnSwordsman::process(Entity e) {
	int j = 0;
	SkillSystem sk;
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == SWORDSMAN) {
			target = toGet;
		}
	}
	sk.startMeleeAttack(e, target, -1);

	printf("Melee Attack \n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTCastRockOnSwordsman::init(Entity e) {
}
BTState BTCastRockOnSwordsman::process(Entity e) {
	SkillSystem sk;
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == SWORDSMAN) {
			target = toGet;
		}
	}
	sk.startRockAttack(e, target);
	printf("Cast Rock On Swordsman\n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTCastRockOnMage::init(Entity e) {
}
BTState BTCastRockOnMage::process(Entity e) {
	SkillSystem sk;
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == MAGE) {
			target = toGet;
		}
	}
	sk.startRockAttack(e, target);
	printf("Cast Rock On Mage\n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTCastHealOnLowestHP::init(Entity e) {
}
BTState BTCastHealOnLowestHP::process(Entity e) {
	int lowestHealth = 100;
	SkillSystem sk;
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		int currHealth = registry.stats.get(toGet).health;
		if (currHealth < lowestHealth) {
			lowestHealth = currHealth;
			target = toGet;
		}
	}
	sk.startHealAttack(e, target);

	printf("Cast Heal On Lowest HP \n\n");	// print statement to visualize

	// return progress
	return BTState::Success;
}

void BTCastHealOnSelf::init(Entity e) {
}
BTState BTCastHealOnSelf::process(Entity e) {
	SkillSystem sk;
	for (int i = 0; i < registry.enemies.components.size(); i++) {
		Entity toGet = registry.enemies.entities[i];
		if (registry.enemies.get(toGet).enemyType == MAGE) {
			target = toGet;
		}
	}
	sk.startHealAttack(e, target);
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

void BTSummonNecroMinion::init(Entity e) {
}
BTState BTSummonNecroMinion::process(Entity e) {
	// add skill for summoning
	SkillSystem sk;
	sk.startSummonAttack(e);
	printf("Summon NecroMinion \n\n");

	// return progress
	return BTState::Success;
}

void BTCastSilence::init(Entity e) {
}
BTState BTCastSilence::process(Entity e) {
	SkillSystem sk;
	printf("Cast Silence \n\n");
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == MAGE) {
			target = toGet;
		}
	}
	sk.startSilenceAttack(e, target);

	// return progress
	return BTState::Success;
}

void BTRandomTargetLightningAttack::init(Entity e) {
}
BTState BTRandomTargetLightningAttack::process(Entity e) {
	int randTarget = rand() % registry.companions.size();
	Entity toGet = registry.companions.entities[randTarget];
	target = toGet;
	SkillSystem sk;
	sk.startLightningAttack(e, target);
	printf("Cast Random Target Lightning Attack \n\n");

	// return progress
	return BTState::Success;
}

void BTLightningAttackOnSwordsman::init(Entity e) {
}
BTState BTLightningAttackOnSwordsman::process(Entity e) {
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == SWORDSMAN) {
			target = toGet;
		}
	}
	SkillSystem sk;
	sk.startLightningAttack(e, target);
	printf("Cast Random Target Lightning Attack \n\n");

	// return progress
	return BTState::Success;
}

void BTCastAOEAttack::init(Entity e) {
}
BTState BTCastAOEAttack::process(Entity e) {
	printf("Cast AOE Attack \n\n");
	int j = 0;
	SkillSystem sk;
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.motions.get(toGet).position.x > j) {
			j = registry.motions.get(toGet).position.x;
			target = toGet;	// get nearest player entity
		}
	}
	sk.startMeleeAttack(e, target, 1);
	// return progress
	return BTState::Success;
}

void BTCastParticleBeamCharge::init(Entity e) {
}
BTState BTCastParticleBeamCharge::process(Entity e) {
	printf("Cast Particle Beam Charge \n\n");
	SkillSystem sk;
	sk.startParticleBeamCharge(e, currPlayer);
	// return progress
	return BTState::Success;
}

void BTCastParticleBeamAttack::init(Entity e) {
}
BTState BTCastParticleBeamAttack::process(Entity e) {
	printf("Cast Particle Beam Attack \n\n");
	SkillSystem sk;
	sk.startParticleBeamAttack(e, currPlayer);
	// return progress
	return BTState::Success;
}

void BTCastShield::init(Entity e) {
}
BTState BTCastShield::process(Entity e) {
	printf("Cast Shield \n\n");
	SkillSystem sk;
	sk.startShieldAttack(e);
	// return progress
	return BTState::Success;
}

void BTCastCrowsAttack::init(Entity e) {
}
BTState BTCastCrowsAttack::process(Entity e) {
	printf("Cast Crows Attack \n\n");
	SkillSystem sk;	// FOR TESTING TO REMOVE
	sk.startHealAttack(e, currPlayer); // FOR TESTING TO REMOVE
	// return progress
	return BTState::Success;
}

void BTCastSingleTargetAttack::init(Entity e) {
}
BTState BTCastSingleTargetAttack::process(Entity e) {
	printf("Cast Single Target Attack \n\n");
	int j = 0;
	SkillSystem sk;	// FOR TESTING TO REMOVE
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.motions.get(toGet).position.x > j) {
			j = registry.motions.get(toGet).position.x;
			target = toGet;	// get nearest player entity
		}
	}
	sk.startMeleeAttack(e, target, 0);
	// return progress
	return BTState::Success;
}

void BTCastSingleTargetAttackOnSwordsman::init(Entity e) {
}
BTState BTCastSingleTargetAttackOnSwordsman::process(Entity e) {
	printf("Cast Single Target Attack \n\n");
	int j = 0;
	SkillSystem sk;	// FOR TESTING TO REMOVE
	for (int i = 0; i < registry.companions.components.size(); i++) {
		Entity toGet = registry.companions.entities[i];
		if (registry.companions.get(toGet).companionType == SWORDSMAN) {
			target = toGet;
		}
	}
	sk.startMeleeAttack(e, target, 0);
	// return progress
	return BTState::Success;
}

// ---------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// Set up enemy behavior tree flow
// Leaf Nodes
BTCastIceShard castIceShard;											// done
BTCastTauntOnMage castTaunt;											// done
BTMeleeAttack meleeAttack;												// done
BTMeleeAttackOnSwordsman meleeAttackOnSwordsman;						// done
BTCastRockOnSwordsman castRockOnSwordsman;								// done
BTCastRockOnMage castRockOnMage;										// done
BTCastHealOnLowestHP castHealOnLowestHP;								// done
BTCastHealOnSelf castHealOnSelf;										// done
BTDoNothing doNothing;													// done
BTSummonNecroMinion summonNecroMinion;									// done
BTCastSilence castSilence;												// done
BTRandomTargetLightningAttack randomTargetLightningAttack;				// done
BTLightningAttackOnSwordsman castLightningOnSwordsman;					// done
BTCastAOEAttack castAOEAttack;											// done
BTCastParticleBeamCharge castParticleBeamCharge;						// done
BTCastParticleBeamAttack castParticleBeamAttack;						// done
BTCastShield castShield;												// done
BTCastSingleTargetAttack castSingleTargetAttack;						// done
BTCastSingleTargetAttackOnSwordsman castSingleTargetAttackOnSwordsman;	// done
BTCastCrowsAttack castCrowsAttack;										// TODO

// Unused Nodes
BTIfCrowsZero crowsZero(&castCrowsAttack);							// TODO
BTIfCrowsMoreThanZero crowsMoreThanZero(&castSingleTargetAttack);	// TODO
BTRunCheckCrows checkCrows(&crowsZero, &crowsMoreThanZero);			// TODO

// Conditional Sub-Tree for Level 4 Nodes
BTIfPlayerMageNotSilenced notSilenced(&castSilence);								// done
BTIfPlayerMageSilenced isSilenced(&randomTargetLightningAttack);					// done
BTIfShieldTurn isShieldTurn(&castShield);											// done
BTIfNotShieldTurn notShieldTurn(&castSingleTargetAttack);							// done
BTIfPlayerSideHasSwordsman haveSwordsmanFour(&castSingleTargetAttackOnSwordsman);	// done
BTIfPlayerSideDoNotHaveSwordsman noSwordsmanFour(&castSingleTargetAttack);			// done

// Level 4 Nodes
BTRunCheckPlayerMageSilenced checkSilenced(&notSilenced, &isSilenced);			// run pair
BTRunCheckShieldTurn checkShieldTurn(&isShieldTurn, &notShieldTurn);			// run pair
BTRunCheckSwordsman checkSwordsmanFour(&haveSwordsmanFour, &noSwordsmanFour);	// run pair

// Conditional Sub-Tree for Level 3 Nodes
BTIfPlayerSideHasMageHardCoded haveMageThree(&castRockOnMage);							// done
BTIfPlayerSideDoNotHaveMageHardCoded doNotHaveMageThree(&castIceShard);					// done
BTIfMageHPBelowHalf mageBelowHalf(&castHealOnSelf);										// done
BTIfMageHPAboveHalf mageAboveHalf(&castHealOnLowestHP);									// done
BTIfPlayerMageTaunted isTaunted(&meleeAttack);											// done
BTIfPlayerMageNotTaunted notTaunted(&castTaunt);										// done
BTIfPlayerSideHasMageHardCoded haveMageTwo(&checkSilenced);								// done
BTIfPlayerSideDoNotHaveMageHardCoded doNotHaveMageTwo(&randomTargetLightningAttack);	// done
BTIfNecroTwoNotTaunted necroTwoNotTaunted(&checkShieldTurn);							// done
BTIfNecroTwoTaunted necroTwoTaunted(&checkSwordsmanFour);								// done

// Level 3 Nodes
BTRunCheckMage checkMageThree(&haveMageThree, &doNotHaveMageThree);
BTRunCheckMageHP checkMageHP(&mageBelowHalf, &mageAboveHalf);						// run pair
BTRunCheckPlayerMageTaunt checkTaunted(&isTaunted, &notTaunted);					// run pair
BTRunCheckMage checkMageTwo(&haveMageTwo, &doNotHaveMageTwo);						// run pair
BTRunCheckNecroTwoTaunt checkNecroTwoTaunt(&necroTwoNotTaunted, &necroTwoTaunted);	// run pair

// Conditional Sub-Tree for Level 2 Nodes
BTIfPlayerSideHasSwordsman haveSwordsman(&castRockOnSwordsman);						// done
BTIfPlayerSideDoNotHaveSwordsman noSwordsman(&castIceShard);						// done
BTIfNoneLessThanHalf none(&checkMageThree);											// done
BTIfAtLeastOneLessThanHalf atLeastOne(&checkMageHP);								
BTIfPlayerSideHasMageHardCoded haveMage(&checkTaunted);								// done
BTIfPlayerSideDoNotHaveMageHardCoded doNotHaveMage(&meleeAttack);					// done
BTIfPlayerSideHasSwordsman haveSwordsmanTwo(&meleeAttackOnSwordsman);				// done
BTIfPlayerSideDoNotHaveSwordsman noSwordsmanTwo(&meleeAttack);						// done
BTIfHasNecroMinion hasNecroMinion(&checkMageTwo);									// done
BTIfNoNecroMinion noNecroMinion(&summonNecroMinion);								// done
BTIfPlayerSideHasSwordsman haveSwordsmanThree(&castLightningOnSwordsman);			// done
BTIfPlayerSideDoNotHaveSwordsman noSwordsmanThree(&randomTargetLightningAttack);	// done
BTIfTurnOne turnOneA(&checkNecroTwoTaunt);											// done
BTIfTurnTwo turnTwoA(&castAOEAttack);												// done
BTIfTurnOne turnOneB(&castParticleBeamCharge);										// done
BTIfTurnTwo turnTwoB(&castParticleBeamAttack);										// done

// Level 2 Nodes
BTRunCheckSwordsman checkSwordsman(&haveSwordsman, &noSwordsman);					// run pair
BTRunCheckEnemyHP checkHP(&none, &atLeastOne);										// run pair
BTRunCheckMage checkMage(&haveMage, &doNotHaveMage);								// run pair
BTRunCheckSwordsman checkSwordsmanTwo(&haveSwordsmanTwo, &noSwordsmanTwo);			// run pair
BTRunCheckNecroMinion checkNecroMinion(&hasNecroMinion, &noNecroMinion);			// run pair
BTRunCheckSwordsman checkSwordsmanThree(&haveSwordsmanThree, &noSwordsmanThree);	// run pair
BTRunCheckTurn checkTurnA(&turnOneA, &turnTwoA);									// run pair
BTRunCheckTurn checkTurnB(&turnOneB, &turnTwoB);									// run pair

// Conditionl Sub-Tree for Level 1 Nodes
BTIfEnemyMageTaunted taunted(&checkSwordsman);					// done
BTIfEnemyMageNotTaunted nonTaunted(&checkHP);					// done
BTIfEnemySwordsmanNotTaunted swordsmanNotTaunted(&checkMage);	// done
BTIfEnemySwordsmanTaunted swordsmanTaunted(&checkSwordsmanTwo);	// done
BTIfNecroOneNotTaunted necroOneNotTaunted(&checkNecroMinion);	// done
BTIfNecroOneTaunted necroOneTaunted(&checkSwordsmanThree);		// done
BTIfNotUltiTurn notUltiTurn(&checkTurnA);						// done
BTIfUltiTurn isUltiTurn(&checkTurnB);							// done

// Level 1 Nodes
BTRunCheckMageTaunt checkEnemyMageTaunt(&taunted, &nonTaunted);							// run pair
BTRunCheckSwordsmanTaunt checkSwordsmanTaunt(&swordsmanNotTaunted, &swordsmanTaunted);	// run pair
BTRunCheckNecroOneTaunt checkNecroTaunt(&necroOneNotTaunted, &necroOneTaunted);			// run pair
BTRunCheckUltiTurn checkUltiTurn(&notUltiTurn, &isUltiTurn);							// run pair

// Conditional Sub-Trees for Level 0
BTIfEnemyIsMagician isMagician(&checkEnemyMageTaunt);			// done
BTIfEnemyIsSwordsman isSwordsman(&checkSwordsmanTaunt);			// done
BTIfEnemyIsNecroMinion isNecroMinion(&meleeAttack);				// done
BTIfEnemyIsNecroPhaseOne isNecroPhaseOne(&checkNecroTaunt);		// done
BTIfEnemyIsNecroPhaseTwo isNecroPhaseTwo(&checkUltiTurn);		// done

// Level 0 Root Node
BTRunCheckCharacter checkChar(&isMagician, &isSwordsman, &isNecroMinion, &isNecroPhaseOne, &isNecroPhaseTwo);	// run pair

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