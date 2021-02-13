#pragma once

#include "Level.hpp"
#include "World.hpp"
#include "Utils.hpp"
#include "RandomNumberGenerator.hpp"
#include "Pathfinding.hpp"

enum class Mask {
	NORTH = static_cast<uint8_t>(1) << 0,
	NORTHEAST = static_cast<uint8_t>(1) << 1,
	EAST = static_cast<uint8_t>(1) << 2,
	SOUTHEAST = static_cast<uint8_t>(1) << 3,
	CENTRE = static_cast<uint8_t>(0), // because we scan in a 3 x 3 grid so this effectively skips the entry
	SOUTH = static_cast<uint8_t>(1) << 4,
	SOUTHWEST = static_cast<uint8_t>(1) << 5,
	WEST = static_cast<uint8_t>(1) << 6,
	NORTHWEST = static_cast<uint8_t>(1) << 7,
};

class WorldMap
{
private:
	Level& town;
	World& world;
	RandomNumber randomiser;
	Path::ScentMap player_smells;
	std::unique_ptr<Level> dungeon;
	std::unique_ptr<EntityGrid> entity_grid;
	int dungeon_depth{ 0 };
	int map_width;
	int map_height;
	bool fov_update_needed{ false };

	std::vector<double> sin;
	std::vector<double> cos;

	void __create_dungeon();
	void populate_town();
	void populate_level();
	void ray_cast(int x, int y, int radius);
	void set_seed();
	void do_bitmask();
public:
	WorldMap(Level& _town, World& _world);
	~WorldMap() {};
	WorldMap& operator=(const WorldMap&) = delete;

	inline Level& get_level() { return dungeon_depth == 0 ? town : *(dungeon.get()); };
	inline int get_current_depth() const { return dungeon_depth; };
	inline void set_depth(int depth) { dungeon_depth = depth; };
	inline EntityGrid& get_entity_grid() { return *(entity_grid.get()); };
	inline unsigned int get_seed() const { return randomiser.seed; };
	inline Path::ScentMap& get_scent_map() { return player_smells; };

	void create_dungeon();
	void load_dungeon();
	void populate_entity_grid();
	void update_fov(int x, int y, int radius);
	void update_scent_trail(int stinkyness); // stinkyness * cost of a sideways step determines how big an area will leave a trial.
	void serialise(std::ofstream& file);
	void deserialise(const char* buffer, size_t& offset);
};

