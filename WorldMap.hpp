#pragma once

#include "Level.hpp"
#include "World.hpp"
#include "Utils.hpp"

class WorldMap
{
private:
	Level& town;
	World& world;
	std::unique_ptr<Level> dungeon;
	std::unique_ptr<EntityGrid> entity_grid;
	int dungeon_depth{ 0 };
	int map_width;
	int map_height;
	bool fov_update_needed{ false };
	uint64_t level_seed{ 0 };

	std::vector<double> sin;
	std::vector<double> cos;

	void __create_dungeon();
	void populate_town();
	void populate_level();
	void ray_cast(int x, int y, int radius);
	void set_seed();
public:
	WorldMap(Level& _town, World& _world, int _width, int _height);
	~WorldMap() {};
	WorldMap(const WorldMap&) = delete;

	inline Level& get_level(int depth) { return depth == 0 ? town : *(dungeon.get()); };
	inline int get_current_depth() const { return dungeon_depth; };
	inline void set_depth(int depth) { dungeon_depth = depth; };
	void create_dungeon();
	void create_dungeon(uint64_t);
	inline EntityGrid& get_entity_grid() { return *(entity_grid.get()); };
	void populate_entity_grid();
	void update_fov(int x, int y, int radius);
	inline uint64_t get_seed() const { return level_seed; };
	void serialise(std::ofstream& file);
	void deserialise(const char* buffer, size_t& offset);
};

