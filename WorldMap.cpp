#include "WorldMap.hpp"

void WorldMap::populate_town()
{

}

void WorldMap::populate_level()
{

}

WorldMap::WorldMap(Level& _town, World& _world, int _width, int _height) :town(_town), world(_world),
map_width(_width), map_height(_height), dungeon(nullptr), entity_grid(nullptr)
{
	auto e_grid = std::make_unique<EntityGrid>(_width, _height);
	entity_grid.swap(e_grid);
}

void WorldMap::create_dungeon()
{
	auto level = std::make_unique<Level>(map_width, map_height);
	auto& grid = level.get()->get_grid();

	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++){
			auto& tile = grid.get_tile(i, j);
			auto roll = rand() % 100;
			if (roll < 10) {
				tile.set(false, false, TileType::WATER);
			}
			else if (roll < 20) {
				tile.set(false, true, TileType::TREE_01);
			}
		}
	}
	dungeon_depth++;
	dungeon.swap(level);
	// populate dungeon level
}

void WorldMap::populate_entity_grid()
{
	auto entities = world.GetEntitiesWith<Position>();
	// filter for only entities at this dungeon depth
	entities.erase(std::remove_if(entities.begin(), entities.end(), [this](uint32_t e) { 
		auto* p = this->world.GetComponent<Position>(e); 
		return p->z != this->dungeon_depth; 
		}
	), entities.end());

	for (auto e : entities) {
		auto* p = world.GetComponent<Position>(e);
		entity_grid.get()->add_entity(e, p->x, p->y);

	}
}
