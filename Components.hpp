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
	Animation(float _lifetime, unsigned int _id, unsigned int _clip_x, unsigned int _clip_y, unsigned int _width, unsigned int _height) : lifetime(_lifetime) {
		animations.insert({ state::IDLE, {AnimFrame(_id, _clip_x, _clip_y, _width, _height)} });
	};
	Animation(float _lifetime) : lifetime(_lifetime) {
		animations.insert({ state::IDLE, {} });
	};
	~Animation() {};
	float dt{ 0.0f };
	float lifetime{ 1.0f };
	state _state{ state::IDLE };
	std::map<state, std::deque<AnimFrame>> animations;
};


struct RigidBody {
	RigidBody() {};
	RigidBody(float _vx, float _vy, float _mass) : vx(_vx), vy(_vy), mass(_mass) {};
	~RigidBody() {};
	float vx{ 0.0f }; 
	float vy{ 0.0f };
	float ax{ 0.0f };
	float ay{ 0.0f };
	float mass{ 1.0f };
	bool gravity{ true };
};


struct Particle {
	Particle() {};
	Particle(float _lifetime) :lifetime(_lifetime) {};
	~Particle() {};
	float lifetime{ 0.0f };
};


using Script = void(*)(World & world, uint32_t entity, SDL_Event& event);
struct Scriptable {
	Scriptable(uint32_t entity) : owner(entity) {};
	Script Init{ nullptr };
	Script OnUpdate{ nullptr };
	uint32_t owner;
};