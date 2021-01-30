#include "WorldMap.hpp"
#include <iostream>

void WorldMap::populate_town()
{

}

void WorldMap::populate_level()
{

}

void WorldMap::ray_cast(int x, int y, int radius)
{
	auto& grid = get_level(get_current_depth()).get_grid();
	grid.get_tile(x, y).visible = true;
	grid.get_tile(x, y).explored = true;

	for (unsigned int i = 0; i < sin.size(); ++i) {
		
		bool hit{ false };

		double	ray_dir_x{ cos[i] };
		double	ray_dir_y{ sin[i] };

		int		ray_pos_x{ x };
		int		ray_pos_y{ y };

		double	delta_x{ 1 / std::abs(ray_dir_x) };
		double	delta_y{ 1 / std::abs(ray_dir_y) };

		int		step_x{ ray_dir_x < 0 ? -1 : 1 };
		int		step_y{ ray_dir_y < 0 ? -1 : 1 };

		double	dist_x{ delta_x };
		double	dist_y{ delta_y };

		bool	in_range{ true };

		int dx{ 0 };
		int dy{ 0 };

		while (!hit && (dx * dx + dy * dy <= radius * radius)) {

			if (dist_x <= dist_y) {
				dist_x += delta_x;
				ray_pos_x += step_x;
				dx++;
			}
			else {
				dist_y += delta_y;
				ray_pos_y += step_y;
				dy++;
			}

			if (grid.in_bounds(ray_pos_x, ray_pos_y)) {
				grid.get_tile(ray_pos_x, ray_pos_y).visible = true;
				grid.get_tile(ray_pos_x, ray_pos_y).explored = true;

				bool object_blocker{ false };
				
				auto entities = get_entity_grid().get(ray_pos_x, ray_pos_y);
				if (!entities.empty()) {
					for (auto e : entities) {
						auto* blocker = world.GetComponent<Blocker>(e);
						if (blocker == nullptr) {
							continue;
						}

						if (blocker->blocks_view) {
							object_blocker = true;
							break;
						}
					}
				}
				if (object_blocker) {
					hit = true;
					continue;
				}

				if (grid.get_tile(ray_pos_x, ray_pos_y).blocks_view) {
					hit = true;
				}
			}
		}
	}
}

WorldMap::WorldMap(Level& _town, World& _world, int _width, int _height) :town(_town), world(_world),
map_width(_width), map_height(_height), dungeon(nullptr), entity_grid(nullptr)
{
	auto e_grid = std::make_unique<EntityGrid>(_width, _height);
	entity_grid.swap(e_grid);

	double pi = 3.141592;

	int interval{ 360 };

	for (int i = 0; i < interval; ++i) {
		sin.push_back(std::sin(static_cast<double>(i) * pi / static_cast<double>(interval / 2)));
		cos.push_back(std::cos(static_cast<double>(i) * pi / static_cast<double>(interval / 2)));
	}
}

void WorldMap::create_dungeon()
{
	auto level = std::make_unique<Level>(map_width, map_height);
	auto& grid = level.get()->get_grid();

	// place holder - full dungeon generation to go here.
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
			else {
				tile.set(true, false, TileType::GRASS_MIDDLE);
			}
		}
	}
	dungeon.swap(level);

	// create mobs & items here.
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

	entity_grid.get()->clear();

	for (auto e : entities) {
		auto* p = world.GetComponent<Position>(e);
		entity_grid.get()->add_entity(e, p->x, p->y);
	}
}

void WorldMap::update_fov(int x, int y, int radius)
{
	auto& grid = get_level(get_current_depth()).get_grid();
	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			grid.get_tile(i, j).visible = false;
		}
	}
	ray_cast(x, y, radius);

	auto lights = world.GetComponents<LightSource, Position>();
	lights.erase(std::remove_if(lights.begin(), lights.end(), [this](const std::tuple<LightSource*, Position*>& data) { 
		auto& [ls, pos] = data;
		return pos->z != this->dungeon_depth;
	}
	), lights.end());

	for (auto& [ls, pos] : lights) {
		ray_cast(pos->x, pos->y, ls->radius);
	}
}
