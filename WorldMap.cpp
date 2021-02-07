#include "WorldMap.hpp"
#include <iostream>

void WorldMap::__create_dungeon()
{
	auto level = std::make_unique<Level>();
	auto& grid = level.get()->get_grid();

	// place holder - full dungeon generation to go here.
	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			auto& tile = grid.get_tile(i, j);
			auto roll = randomiser.sample(1, 100);
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
}

void WorldMap::populate_town()
{

}

void WorldMap::populate_level()
{

}

void WorldMap::ray_cast(int x, int y, int radius)
{
	auto& grid = get_level().get_grid();
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

void WorldMap::set_seed()
{
	randomiser.new_seed();
}

WorldMap::WorldMap(Level& _town, World& _world): town(_town), world(_world), randomiser(), player_smells(), dungeon(nullptr), entity_grid(nullptr)
{
	SmartLuaVM vm(nullptr, &lua_close);
	vm.reset(luaL_newstate());
	auto result = luaL_dofile(vm.get(), "./Config/window.lua");

	if (result == LUA_OK) {
		lua_getglobal(vm.get(), "map_width");
		if (lua_isnumber(vm.get(), -1)) {
			map_width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "map_height");
		if (lua_isnumber(vm.get(), -1)) {
			map_height = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
	}
	else {
		std::string error_msg = lua_tostring(vm.get(), -1);
		printf(error_msg.c_str());
	}

	auto e_grid = std::make_unique<EntityGrid>(map_width, map_height);
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
	set_seed();
	__create_dungeon();
}

void WorldMap::load_dungeon()
{
	__create_dungeon();
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
	auto& grid = get_level().get_grid();
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

void WorldMap::update_scent_trail(int stinkyness)
{
	auto components = world.GetComponents<Player, Position>();
	auto& [player, pos] = components[0];
	player_smells = Path::scent_map(get_level(), pos->x, pos->y, stinkyness);
}

void WorldMap::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(dungeon_depth));
	utils::serialiseUint64(file, static_cast<uint64_t>(randomiser.seed));

	auto& grid = get_level().get_grid();

	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			auto& tile = grid.get_tile(i, j);
			utils::serialiseUint32(file, static_cast<uint32_t>(tile.visible));
			utils::serialiseUint32(file, static_cast<uint32_t>(tile.explored));
		}
	}
}

void WorldMap::deserialise(const char* buffer, size_t& offset)
{
	dungeon_depth = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	auto level_seed = utils::deserialiseUint64(buffer, offset);
	
	randomiser.set_seed(static_cast<unsigned int>(level_seed));
	load_dungeon();
	
	auto& grid = get_level().get_grid();

	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			auto& tile = grid.get_tile(i, j);
			tile.visible = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
			tile.explored = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
		}
	}
}
