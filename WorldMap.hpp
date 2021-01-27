#pragma once

#include "Level.hpp"
#include "World.hpp"

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

	void populate_town();
	void populate_level();
public:
	WorldMap(Level& _town, World& _world, int _width, int _height);
	~WorldMap() {};
	WorldMap(const WorldMap&) = delete;
	inline Level& get_level(int depth) { return depth == 0 ? town : *(dungeon.get()); };
	inline int get_current_depth() const { return dungeon_depth; };
	inline void set_depth(int depth) { dungeon_depth = depth; };
	void create_dungeon();
	inline EntityGrid& get_entity_grid() { return *(entity_grid.get()); };
	void populate_entity_grid();
};

