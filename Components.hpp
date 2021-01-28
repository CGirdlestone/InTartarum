#pragma once

#include "Common.hpp"

// forward declaration
class World;

enum class state { IDLE, WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN };

struct Position {
	Position() {};
	Position(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { };
	~Position() {};
	int x{ 0 };
	int y{ 0 };
	int z{ 0 };
};

struct Player {
	int level{ 1 };
};

struct Actor {

};

struct Blocker {
	
};

struct Interactable {
	Interactable() {};
	Interactable(bool repeat) : repeatable(repeat) {};
	~Interactable() {};
	bool repeatable{ true };
	bool triggered{ false };
};


struct Sprite {
	Sprite() {};
	Sprite(unsigned int _id, unsigned int _clip_x, unsigned int _clip_y, unsigned int _width, unsigned int _height, unsigned int _depth) :
		id(_id), clip_x(_clip_x), clip_y(_clip_y), width(_width), height(_height), depth(_depth) {};
	~Sprite() {};
	unsigned int id{ 0 };
	unsigned int clip_x{ 0 };
	unsigned int clip_y{ 0 };
	unsigned int width{ 0 };
	unsigned int height{ 0 };
	unsigned int depth{ 0 };
};


// A helper struct for the Animation Component
struct AnimFrame {
	AnimFrame(unsigned int _id, unsigned int _clip_x, unsigned int _clip_y, unsigned int _width, unsigned int _height) :
		id(_id), clip_x(_clip_x), clip_y(_clip_y), width(_width), height(_height) {};
	unsigned int id{ 0 };
	unsigned int clip_x{ 0 };
	unsigned int clip_y{ 0 };
	unsigned int width{ 0 };
	unsigned int height{ 0 };
};


struct Animation {
	Animation() {
		animations.insert({ state::IDLE, {} });
	};
	Animation(float _lifetime, unsigned int _id, unsigned int _clip_x, unsigned int _clip_y, unsigned int _width, unsigned int _height, bool is_dynamic = true) 
		: lifetime(_lifetime), dynamic(is_dynamic) {
		animations.insert({ state::IDLE, {AnimFrame(_id, _clip_x, _clip_y, _width, _height)} });
	};
	~Animation() {};
	float dt{ 0.0f };
	float lifetime{ 1.0f };
	state _state{ state::IDLE };
	std::map<state, std::deque<AnimFrame>> animations;
	bool dynamic{ true };
};


struct Particle {
	Particle() {};
	Particle(float _lifetime) :lifetime(_lifetime) {};
	~Particle() {};
	float lifetime{ 0.0f };
};


using Script = void(*)(World & world, uint32_t entity);
struct Scriptable {
	Scriptable(uint32_t entity) : owner(entity) {};
	std::string Init{ "" };
	std::string OnUpdate{ "" };
	std::string OnBump{ "" };
	uint32_t owner;
};