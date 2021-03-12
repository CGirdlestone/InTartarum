#pragma once

#include "Level.hpp"
#include "World.hpp"
#include "Utils.hpp"
#include "RandomNumberGenerator.hpp"
#include "Pathfinding.hpp"
#include "EntityFactory.hpp"
#include "TextureManager.hpp"

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
	World& world;
	EntityFactory& entity_factory;
	TextureManager& texture_manager;
	RandomNumber randomiser;
	Path::ScentMap player_smells;
	std::unique_ptr<Level> town;
	std::unique_ptr<Level> overworld;
	std::vector<std::vector<bool> > overworld_explored;
	std::unique_ptr<Level> dungeon;
	std::vector<unsigned int> overworld_seeds;
	std::unique_ptr<EntityGrid> entity_grid;
	int dungeon_depth{ 0 };
	int map_width{ 0 };
	int map_height{ 0 };
	int overworld_width{ 0 };
	int overworld_height{ 0 };
	int town_x{ 0 };
	int town_y{ 0 };
	int world_x{ 0 };
	int world_y{ 0 };
	bool fov_update_needed{ false };

	std::vector<double> sin;
	std::vector<double> cos;

	void build_town();
	void __create_dungeon(std::unique_ptr<Level>& _level);
	void __create_overworld(std::unique_ptr<Level>& level);
	void populate_town();
	void populate_level();
	void ray_cast(int x, int y, int radius);
	void set_seed();
	void do_bitmask();
public:
	WorldMap(World& _world, EntityFactory& _entity_factory, TextureManager& texture_manager);
	~WorldMap() {};
	WorldMap& operator=(const WorldMap&) = delete;

	Level& get_level();
	inline int get_current_depth() const { return dungeon_depth; };
	inline void set_depth(int depth) { dungeon_depth = depth; };
	inline EntityGrid& get_entity_grid() { return *(entity_grid.get()); };
	inline unsigned int get_seed() const { return randomiser.seed; };
	inline Path::ScentMap& get_scent_map() { return player_smells; };
	inline int get_world_x() const { return world_x; };
	inline int get_world_y() const { return world_y; };
	inline void set_world_x(int _world_x) { world_x = _world_x; };
	inline void set_world_y(int _world_y) { world_y = _world_y; };
	inline int get_world_width() const { return overworld_width; };
	inline int get_world_height() const { return overworld_height; };

	void create_dungeon();
	void create_world();
	void load_dungeon();
	void store_overworld_exploration_data();
	void read_overworld_exploration_data();
	void load_overworld_level(int x, int y);
	void populate_entity_grid();
	void update_fov(int x, int y, int radius);
	void update_scent_trail(int stinkyness); // stinkyness * cost of a sideways step determines how big an area will leave a trial.
	void serialise(std::ofstream& file);
	void deserialise(const char* buffer, size_t& offset);
};

