#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are ahrd coded to the dimensions of the entity texture
const float MAGE_WIDTH = 0.4f * 100.f;
const float MAGE_HEIGHT = 0.4f * 300.f;
const float ENEMY_MAGE_WIDTH = 0.4f * 100.f;
const float ENEMY_MAGE_HEIGHT = 0.4f * 300.f;

// the player mage
Entity createMage(RenderSystem* renderer, vec2 pos);
// the enemy mage
Entity createEnemyMage(RenderSystem* renderer, vec2 position);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);


