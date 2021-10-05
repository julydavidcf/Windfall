#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are ahrd coded to the dimensions of the entity texture
const float MAGE_WIDTH = 110.f;
const float MAGE_HEIGHT = 100.f;
const float ENEMY_MAGE_WIDTH = 110.f;
const float ENEMY_MAGE_HEIGHT = 100.f;
const float HEALTHBAR_WIDTH = 80.f;
const float HEALTHBAR_HEIGHT = 10.f;
const float FIREBALL_WIDTH = 80.f;
const float FIREBALL_HEIGHT = 30.f;
const float FIREBALL_ICON_WIDTH = 80.f;
const float FIREBALL_ICON_HEIGHT = 80.f;

// the player mage
Entity createMage(RenderSystem* renderer, vec2 pos);
// the enemy mage
Entity createEnemyMage(RenderSystem* renderer, vec2 position);
//healthbar
Entity createHealthBar(RenderSystem* renderer, vec2 position);
// the fireball
Entity createFireball(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, int isFriendly);
// the fireball icon
Entity createFireballIcon(RenderSystem* renderer, vec2 position);
Entity createFireballIconSelected(RenderSystem* renderer, vec2 position);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);


