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
enum class Slot { HEAD, CHEST, LEFT_HAND, RIGHT_HAND, NECK, LEGS, HANDS, BOOTS, RING, QUIVER, BACK};
enum class Attitude { NEUTRAL, HOSTILE, FRIENDLY };

struct Position : public ISerializeable {
	Position() {};
	Position(int _x, int _y, int _z) : x(_x), y(_y), z(_z){ };
	Position(int _x, int _y, int _z, int _world_x, int _world_y) : x(_x), y(_y), z(_z), world_x(_world_x), world_y(_world_y) { };
	~Position() {};
	int x{ 0 };
	int y{ 0 };
	int z{ 0 };
	int world_x{ 0 };
	int world_y{ 0 };
	int x_offset{ 0 };
	int y_offset{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Player : public ISerializeable {
	Player() {};
	Player(int _vision) : vision(_vision) {};
	~Player() {};
	int level{ 1 };
	int vision{ 10 };
	uint32_t xp{ 0 };
	uint32_t next_xp{ 100 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Container : public ISerializeable {
	Container() {};
	~Container() {};
	Container(int _weight_capacity) : weight_capacity(_weight_capacity) {};

	std::vector<uint32_t> inventory;
	int weight_capacity{ 100 };
	int weight{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Actor : public ISerializeable {
	Actor() {};
	~Actor() {};
	Actor(const std::string& _name, const std::string& _desc) : name(_name), description(_desc) {};

	std::string name{ "" };
	std::string description{ "" };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct AI : public ISerializeable {
	AI() {};
	AI(Attitude _attitude, bool _blind, bool _scent, bool _smart, uint32_t _xp) : attitude(_attitude), blind(_blind), scent(_scent), smart(_smart), xp(_xp) {};
	~AI() {};

	Attitude attitude{ Attitude::NEUTRAL };
	bool smart{ false };
	bool blind{ false };
	bool scent{ false };
	uint32_t xp{ 0 };
	std::vector<std::tuple<int, int> > path{};
	
	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Blocker : public ISerializeable {
	Blocker() {};
	Blocker(bool _blocks_view) : blocks_view(_blocks_view) {};
	~Blocker() {};
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
	Animation(float _lifetime, bool is_dynamic) : lifetime(_lifetime), dynamic(is_dynamic) {
		animations.insert({ state::IDLE, {} });
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
	std::string OnEquip{ "" };
	std::string OnUnequip{ "" };
	std::string OnUse{ "" };
	std::string OnHit{ "" };
	uint32_t owner{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Fighter : public ISerializeable {
	Fighter() {};
	~Fighter() {};
	Fighter(int _hp, int _defence, int _str, int _dex, int _con, int _wis, int _int, int _cha);
	int max_hp{ 0 };
	int hp{ 0 };
	int defence{ 0 };
	int base_strength{ 0 };
	int str_buff{ 0 };
	int str_mod{ 0 };
	int base_dexterity{ 0 };
	int dex_buff{ 0 };
	int dex_mod{ 0 };
	int base_constitution{ 0 };
	int con_buff{ 0 };
	int con_mod{ 0 };
	int base_wisdom{ 0 };
	int wis_buff{ 0 };
	int wis_mod{ 0 };
	int base_intelligence{ 0 };
	int int_buff{ 0 };
	int int_mod{ 0 };
	int base_charisma{ 0 };
	int cha_buff{ 0 };
	int cha_mod{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Item : public ISerializeable {
	Item() {};
	~Item() {};
	Item(const std::string& _name, const std::string& _description, int _weight) : name(_name), description(_description), weight(_weight) {};
	std::string name{ "" };
	std::string description{ "" };
	int weight{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Stackable : public ISerializeable {
	Stackable() {};
	~Stackable() {};
	
	int quantity{ 1 };
	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Equipable : public ISerializeable {
	Equipable() {};
	~Equipable() {};
	Equipable(Slot _slot) : slot(_slot) {};
	Slot slot{ Slot::HEAD };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Weapon : public ISerializeable {
	Weapon() {};
	~Weapon() {};
	Weapon(int _num_dice, int _sides) : num_dice(_num_dice), sides(_sides) {};
	int num_dice{ 0 };
	int sides{ 0 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct Body : public ISerializeable {
	Body(): equipment(11, MAX_ENTITIES + 1) {};
	~Body() {};

	std::vector<uint32_t> equipment;
	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

struct ID : public ISerializeable {
	ID() {};
	~ID() {};
	ID(std::string _id) : id(_id) {};

	std::string id{ "" };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

enum class UseableType { CONSUMABLE, TARGETED, TARGETED_AOE };
struct Useable : public ISerializeable {
	Useable() {};
	~Useable() {};
	Useable(UseableType _type, int _charges) : type(_type), charges(_charges) {};

	UseableType type{ UseableType::CONSUMABLE };
	int charges{ -1 };
	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

