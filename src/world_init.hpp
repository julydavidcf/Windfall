#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are ahrd coded to the dimensions of the entity texture
const float MAGE_WIDTH = 80.f;
const float MAGE_HEIGHT = 100.f;
const float ENEMY_MAGE_WIDTH = 80.f;
const float ENEMY_MAGE_HEIGHT = 100.f;
const float FIREBALL_WIDTH = 30.f;
const float FIREBALL_HEIGHT = 20.f;
const float FIREBALL_ICON_WIDTH = 40.f;
const float FIREBALL_ICON_HEIGHT = 40.f;

// the player mage
Entity createMage(RenderSystem* renderer, vec2 pos);
// the enemy mage
Entity createEnemyMage(RenderSystem* renderer, vec2 position);
// the fireball
Entity createFireball(RenderSystem* renderer, vec2 position);
// the fireball icon
Entity createFireballIcon(RenderSystem* renderer, vec2 position);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);


