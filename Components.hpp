#pragma once

#include "Common.hpp"
#include "Utils.hpp"

struct ISerializeable {
	virtual void serialise(std::ofstream& file) = 0;
	virtual void deserialise(const char* buffer, size_t& offset) = 0;
};

// forward declaration
class World;

enum class state { IDLE, WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN };

struct Position : public ISerializeable {
	Position() {};
	Position(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { };
	~Position() {};
	int x{ 0 };
	int y{ 0 };
	int z{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Player : public ISerializeable {
	Player() {};
	Player(int _vision) : vision(_vision) {};
	~Player() {};
	int level{ 1 };
	int vision{ 10 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Actor : public ISerializeable {

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Blocker : public ISerializeable {
	Blocker() {};
	Blocker(bool _blocks_view) : blocks_view(_blocks_view) {};
	~Blocker();
	bool blocks_view{ false };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct LightSource : public ISerializeable {
	LightSource() {};
	LightSource(int _radius) : radius(_radius) {};
	~LightSource() {};
	int radius{ 8 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Interactable : public ISerializeable {
	Interactable() {};
	Interactable(bool repeat) : repeatable(repeat) {};
	~Interactable() {};
	bool repeatable{ true };
	bool triggered{ false };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};


struct Sprite : public ISerializeable {
	Sprite() {};
	Sprite(unsigned int _id, int _clip_x, int _clip_y, int _width, int _height, int _depth, uint8_t _r = 255, uint8_t _g = 255, uint8_t _b = 255) :
		id(_id), clip_x(_clip_x), clip_y(_clip_y), width(_width), height(_height), depth(_depth), r(_r), g(_g), b(_b) {};
	~Sprite() {};
	unsigned int id{ 0 };
	int clip_x{ 0 };
	int clip_y{ 0 };
	int width{ 0 };
	int height{ 0 };
	int depth{ 0 };
	uint8_t r{ 0 };
	uint8_t g{ 0 };
	uint8_t b{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};


// A helper struct for the Animation Component
struct AnimFrame : public ISerializeable {
	AnimFrame() {};
	AnimFrame(unsigned int _id, unsigned int _clip_x, unsigned int _clip_y, unsigned int _width, unsigned int _height) :
		id(_id), clip_x(_clip_x), clip_y(_clip_y), width(_width), height(_height) {};
	unsigned int id{ 0 };
	unsigned int clip_x{ 0 };
	unsigned int clip_y{ 0 };
	unsigned int width{ 0 };
	unsigned int height{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};


struct Animation : public ISerializeable {
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

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};


struct Particle : public ISerializeable {
	Particle() {};
	Particle(float _lifetime) :lifetime(_lifetime) {};
	~Particle() {};
	float lifetime{ 0.0f };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};


struct Scriptable : public ISerializeable {
	Scriptable() {};
	Scriptable(uint32_t entity) : owner(entity) {};
	std::string OnInit{ "" };
	std::string OnUpdate{ "" };
	std::string OnBump{ "" };
	std::string OnDeath{ "" };
	uint32_t owner{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};