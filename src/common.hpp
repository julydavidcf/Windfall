#pragma once

// stlib
#include <fstream> // stdout, stderr..
#include <string>
#include <tuple>
#include <vector>

// glfw (OpenGL)
#define NOMINMAX
#include <gl3w.h>
#include <GLFW/glfw3.h>

// The glm library provides vector and matrix operations as in GLSL
#include <glm/vec2.hpp>				// vec2
#include <glm/ext/vector_int2.hpp>  // ivec2
#include <glm/vec3.hpp>             // vec3
#include <glm/mat3x3.hpp>           // mat3
using namespace glm;

#include "tiny_ecs.hpp"

// Simple utility functions to avoid mistyping directory name
// audio_path("audio.ogg") -> data/audio/audio.ogg
// Get defintion of PROJECT_SOURCE_DIR from:
#include "../ext/project_path.hpp"
inline std::string data_path() { return std::string(PROJECT_SOURCE_DIR) + "data"; };
inline std::string shader_path(const std::string& name) {return std::string(PROJECT_SOURCE_DIR) + "/shaders/" + name;};
inline std::string textures_path(const std::string& name) {return data_path() + "/textures/" + std::string(name);};
inline std::string audio_path(const std::string& name) {return data_path() + "/audio/" + std::string(name);};
inline std::string mesh_path(const std::string& name) {return data_path() + "/meshes/" + std::string(name);};

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#ifndef NUM_SWARM_PARTICLES
#define NUM_SWARM_PARTICLES 40
#endif

// The 'Transform' component handles transformations passed to the Vertex shader
// (similar to the gl Immediate mode equivalent, e.g., glTranslate()...)
// We recomment making all components non-copyable by derving from ComponentNonCopyable
struct Transform {
	mat3 mat = { { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f}, { 0.f, 0.f, 1.f} }; // start with the identity
	void scale(vec2 scale);
	void rotate(float radians);
	void translate(vec2 offset);
};

bool gl_has_errors();

extern Entity currPlayer;
extern Entity prevPlayer;
// Players + Enemies
extern Entity prevPlayer;
extern Entity player_archer;
extern Entity player_mage;
extern Entity enemy_mage;
extern Entity player_swordsman;
extern Entity enemy_swordsman;
extern Entity necromancer;
extern Entity necromancer_phase_one;
extern Entity necromancer_phase_two;
extern Entity necromancer_minion;

extern std::vector<Entity> roundVec;


extern Entity silence_icon;

//icons
extern Entity melee_icon;
extern Entity iceShard_icon;
extern Entity fireBall_icon;
extern Entity taunt_icon;
extern Entity heal_icon;
extern Entity rock_icon;
extern Entity arrow_icon;

extern int window_width_px;
extern int window_height_px;

extern int story;
extern int16 gameLevel;
extern int16 loadedLevel;

// Free roam variables
extern bool isFreeRoam;
extern int freeRoamLevel;