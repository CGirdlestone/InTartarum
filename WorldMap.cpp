#include "WorldMap.hpp"

void WorldMap::build_town()
{
	auto level = std::make_unique<Level>(map_width, map_height);
	auto& grid = level.get()->get_grid();

	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			/*if (i == 0 || j == 0 || i == grid.get_width() - 1 || j == grid.get_height() - 1) {
				grid.set_tile(i, j, false, true, TileType::GRASS);
			}
			else {
				grid.set_tile(i, j, true, false, TileType::GRASS);
			}*/
			grid.set_tile(i, j, true, false, TileType::GRASS);
		}
	}

	for (int i = 10; i < 21; i++) {
		for (int j = 10; j < 16; j++) {
			if (i == 15 && j == 15) {
				grid.set_tile(i, j, true, false, TileType::FLOOR);
				continue;
			}
			if (i == 10 || j == 10 || i == 21 - 1 || j == 16 - 1) {
				grid.set_tile(i, j, false, true, TileType::FLOOR);
			}
			else {
				grid.set_tile(i, j, true, false, TileType::FLOOR);
			}
		}
	}

	grid.set_tile(25, 25, true, false, TileType::STAIRS);

	std::string door = "door";
	entity_factory.create_item(door, 15, 15, 0, world_x, world_y);

	std::string npc = "npc";
	entity_factory.create_npc(npc, 15, 11, 0, world_x, world_y);

	std::string chest = "chest";
	entity_factory.create_item(chest, 11, 11, 0, world_x, world_y);

	std::string sword = "fire_sword";
	entity_factory.create_item(sword, 21, 21, 0, world_x, world_y);

	std::string arrow = "arrow";
	entity_factory.create_item(arrow, 20, 20, 0, world_x, world_y);
	entity_factory.create_item(arrow, 19, 20, 0, world_x, world_y);

	std::string fire = "camp_fire";
	entity_factory.create_item(fire, 15, 13, 0, world_x, world_y);

	std::string bat = "bat";
	entity_factory.create_mob(bat, 15, 25, 0, world_x, world_y);

	town.swap(level);
}

void WorldMap::__create_dungeon(std::unique_ptr<Level>& _level)
{
	auto level = std::make_unique<Level>();
	auto& grid = level.get()->get_grid();

	// place holder - full dungeon generation to go here.
	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			auto& tile = grid.get_tile(i, j);
			auto roll = randomiser.sample(1, 100);
			if (roll < 10) {
				tile.set(false, false, TileType::SHALLOW_WATER);
			}
			else {
				tile.set(true, false, TileType::GRASS);
			}
		}
	}
	_level.swap(level);
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

void WorldMap::do_bitmask()
{
	auto& grid = get_level().get_grid();

	Mask directions[9] = { Mask::NORTHWEST, Mask::NORTH, Mask::NORTHEAST, Mask::WEST, Mask::CENTRE, Mask::EAST, Mask::SOUTHWEST, Mask::SOUTH, Mask::SOUTHEAST };

	auto _grid = std::make_unique<uint8_t[]>(grid.get_width() * grid.get_height());

	for (int j = 0; j < grid.get_height(); j++) {
		for (int i = 0; i < grid.get_width(); i++){
			if (grid.get_tile(i, j).walkable) {
				continue;
			}
			_grid.get()[i + j * grid.get_width()] = 0;
			uint8_t mask{ 0 };
			for (int k = 0; k < 9; k++) {
				auto n = k % 3 - 1;
				auto m = k / 3 - 1;
				if (!grid.in_bounds(i + n , j + m) || (n == 0 && m == 0)) {
					continue;
				}
				if (n * m != 0) {
					continue;
				}
				auto& tile = grid.get_tile(i + n, j + m);
				if (!tile.walkable) {
					mask |= static_cast<uint8_t>(directions[k]);
				}
			}
			_grid.get()[i + j * grid.get_width()] = mask;
		}
	}

	for (int j = 0; j < grid.get_height(); j++) {
		for (int i = 0; i < grid.get_width(); i++) {
			auto& tile = grid.get_tile(i, j);
			auto mask = _grid.get()[i + j * grid.get_width()];
			switch (mask){
				case static_cast<uint8_t>(Mask::NORTH) : tile.type = TileType::WALL_LEFT; break;
				case static_cast<uint8_t>(Mask::SOUTH) : tile.type = TileType::WALL_LEFT; break;
				case static_cast<uint8_t>(Mask::EAST) : tile.type = TileType::WALL_TOP; break;
				case static_cast<uint8_t>(Mask::WEST) : tile.type = TileType::WALL_TOP; break;
				case static_cast<uint8_t>(Mask::NORTH) | static_cast<uint8_t>(Mask::EAST) : tile.type = TileType::WALL_BL; break;
				case static_cast<uint8_t>(Mask::NORTH) | static_cast<uint8_t>(Mask::WEST) : tile.type = TileType::WALL_BR; break;
				case static_cast<uint8_t>(Mask::SOUTH) | static_cast<uint8_t>(Mask::EAST) : tile.type = TileType::WALL_TL; break;
				case static_cast<uint8_t>(Mask::SOUTH) | static_cast<uint8_t>(Mask::WEST) : tile.type = TileType::WALL_TR; break;
				case static_cast<uint8_t>(Mask::SOUTH) | static_cast<uint8_t>(Mask::NORTH) : tile.type = TileType::WALL_LEFT; break;
				case static_cast<uint8_t>(Mask::EAST) | static_cast<uint8_t>(Mask::WEST) : tile.type = TileType::WALL_TOP; break;
		}
		}
	}

}

WorldMap::WorldMap(World& _world, EntityFactory& _entity_factory, TextureManager& _texture_manager)
	: world(_world), entity_factory(_entity_factory), texture_manager(_texture_manager), randomiser(), player_smells(), 
	town(nullptr), overworld(nullptr), dungeon(nullptr), overworld_seeds({}), entity_grid(nullptr)
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
		lua_getglobal(vm.get(), "overworld_width");
		if (lua_isnumber(vm.get(), -1)) {
			overworld_width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "overworld_height");
		if (lua_isnumber(vm.get(), -1)) {
			overworld_height = static_cast<int>(lua_tonumber(vm.get(), -1));
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

	town_x = randomiser.sample(0, overworld_width);
	town_y = randomiser.sample(0, overworld_height);
	world_x = town_x;
	world_y = town_y;
	build_town();
	do_bitmask();

	for (int j = 0; j < overworld_height; j++) {
		for (int i = 0; i < overworld_width; i++) {
			randomiser.new_seed();
			overworld_seeds.push_back(randomiser.seed);
		}
	}
	
	std::vector<bool> _explored(map_width * map_height, false);
	std::vector<std::vector<bool> > _overworld_explored(overworld_width * overworld_height, _explored);
	
	overworld_explored = _overworld_explored;
}

Level& WorldMap::get_level()
{
	//check the dungeon depth, if it's 0, return the overworld map based on the players overworld x & y coords
	if (dungeon_depth == 0) {
		if (world_x == town_x && world_y == town_y) {
			return *(town.get());
		}

		return *(overworld.get());
	}
	else {
		return *(dungeon.get());
	}
}

void WorldMap::create_dungeon()
{
	set_seed();
	__create_dungeon(dungeon);
}

void WorldMap::create_world()
{
	// Maybe needed?
}

void WorldMap::load_dungeon()
{
	__create_dungeon(dungeon);
}

void WorldMap::store_overworld_exploration_data()
{
	auto& grid = get_level().get_grid();

	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			overworld_explored[world_x + world_y * overworld_width][i + j * grid.get_width()] = grid.get_tile(i, j).explored;
		}
	}
}

void WorldMap::read_overworld_exploration_data()
{
	auto& grid = get_level().get_grid();

	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			grid.get_tile(i, j).explored = overworld_explored[world_x + world_y * overworld_width][i + j * grid.get_width()];
		}
	}
}

void WorldMap::load_overworld_level(int x, int y)
{
	auto overworld_seed = overworld_seeds[x + y * overworld_width];
	randomiser.set_seed(overworld_seed);
	store_overworld_exploration_data();
	set_world_x(x);
	set_world_y(y);
	__create_dungeon(overworld);
	read_overworld_exploration_data();
}

void WorldMap::populate_entity_grid()
{
	auto entities = world.GetEntitiesWith<Position>();
	// filter for only entities at this dungeon depth
	entities.erase(std::remove_if(entities.begin(), entities.end(), [this](uint32_t e) { 
		auto* p = this->world.GetComponent<Position>(e); 
		bool on_level = p->z == this->dungeon_depth;
		bool in_overland_level = p->world_x == world_x && p->world_y == world_y;
		return !on_level || !in_overland_level; 
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
	utils::serialiseUint32(file, static_cast<uint32_t>(town_x));
	utils::serialiseUint32(file, static_cast<uint32_t>(town_y));
	utils::serialiseUint32(file, static_cast<uint32_t>(world_x));
	utils::serialiseUint32(file, static_cast<uint32_t>(world_y));
	utils::serialiseVector(file, overworld_seeds);

	utils::serialiseUint32(file, static_cast<uint32_t>(overworld_explored.size()));
	for (const auto& vec : overworld_explored) {
		utils::serialiseVectorBool(file, vec);
	}

	// serialise the current level's visible and explored flags
	{
		auto& grid = get_level().get_grid();
		for (int i = 0; i < grid.get_width(); i++) {
			for (int j = 0; j < grid.get_height(); j++) {
				auto& tile = grid.get_tile(i, j);
				utils::serialiseUint32(file, static_cast<uint32_t>(tile.visible));
				utils::serialiseUint32(file, static_cast<uint32_t>(tile.explored));
			}
		}
	}
}

void WorldMap::deserialise(const char* buffer, size_t& offset)
{
	dungeon_depth = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	auto level_seed = utils::deserialiseUint64(buffer, offset);
	town_x = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	town_y = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	world_x = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	world_y = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	overworld_seeds = utils::deserialiseVector(buffer, offset);

	auto num_vecs = utils::deserialiseUint32(buffer, offset);

	for (uint32_t i = 0; i < num_vecs; i++) {
		auto contents = utils::deserialiseVectorBool(buffer, offset);
		overworld_explored[i] = contents;
	}
	
	// rebuild current level from the seed if it's not the starting town.
	randomiser.set_seed(static_cast<unsigned int>(level_seed));
	if (!(world_x == town_x && world_y == town_y)) {
		__create_dungeon(overworld);
	}

	if (dungeon_depth > 0) {
		load_dungeon();
	}
	
	// set the current levels visible and explored flags
	auto& grid = get_level().get_grid();
	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			auto& tile = grid.get_tile(i, j);
			tile.visible = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
			tile.explored = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
		}
	}
}
