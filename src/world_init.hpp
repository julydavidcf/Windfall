#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are ahrd coded to the dimensions of the entity texture
const float MAGE_WIDTH = 190.f;
const float MAGE_HEIGHT = 150.f;
const float SWORDSMAN_WIDTH = 220.f;
const float SWORDSMAN_HEIGHT = 250.f;
const float NECROMANCER_WIDTH = 150.f;
const float NECROMANCER_HEIGHT = 180.f;
const float HEALTHBAR_WIDTH = 130.f;
const float HEALTHBAR_HEIGHT = 7.f;
const float FIREBALL_WIDTH = 80.f;
const float FIREBALL_HEIGHT = 30.f;
const float ARROW_WIDTH = 80.f;
const float ARROW_HEIGHT = 10.f;
const float FIREBALL_ICON_WIDTH = 80.f;
const float FIREBALL_ICON_HEIGHT = 80.f;
const float BARRIER_WIDTH = 50.f;
const float BARRIER_HEIGHT = 200.f;
const float PLAYERTURN_WIDTH = 300.f;
const float PLAYERTURN_HEIGHT = 100.f;
const float ENEMYTURN_WIDTH = 300.f;
const float ENEMYTURN_HEIGHT = 100.f;
const float BACKGROUND_WIDTH = 1800.f;
const float BACKGROUND_HEIGHT = 900.f;
const float ROCK_WIDTH = 75.f;
const float ROCK_HEIGHT = 75.f;

// the player mage
Entity createPlayerMage(RenderSystem* renderer, vec2 pos);
// the enemy mage
Entity createEnemyMage(RenderSystem* renderer, vec2 position);
// the player swordsman
Entity createPlayerSwordsman(RenderSystem* renderer, vec2 pos);
// the enemy swordsman
Entity createEnemySwordsman(RenderSystem* renderer, vec2 pos);
// the necromancer
Entity createNecromancer(RenderSystem* renderer, vec2 pos);

// Create background layers
Entity createBackgroundLayerOne(RenderSystem* renderer, vec2 pos);
Entity createBackgroundLayerTwo(RenderSystem* renderer, vec2 pos);
Entity createBackgroundLayerThree(RenderSystem* renderer, vec2 pos);
Entity createBackgroundLayerFour(RenderSystem* renderer, vec2 pos);

//healthbar
Entity createHealthBar(RenderSystem* renderer, vec2 position);
// the fireball
Entity createFireball(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, int isFriendly);

Entity createArrow(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, int isFriendly);
// the fireball icon
Entity createFireballIcon(RenderSystem* renderer, vec2 position);
Entity createFireballIconSelected(RenderSystem* renderer, vec2 position);

// Turn indicators
Entity createPlayerTurn(RenderSystem* renderer, vec2 position);
Entity createEnemyTurn(RenderSystem* renderer, vec2 position);
// barrier
Entity createBarrier(RenderSystem* renderer, vec2 position);

Entity createGreenCross(RenderSystem* renderer, vec2 position);
//rock for magw
Entity createRock(RenderSystem* renderer, vec2 position, int isFriendly);

Entity createMelee(RenderSystem* renderer, vec2 position, int isFriendly);
// a basic, textured enemy
Entity createEnemyMage(RenderSystem* renderer, vec2 position);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);



