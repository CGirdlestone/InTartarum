#include "CharacterCreationScreen.hpp"

bool CharacterCreationScreen::load_character_classes()
{
	SmartLuaVM vm(nullptr, &lua_close);
	vm.reset(luaL_newstate());
	auto result = luaL_dofile(vm.get(), "./Resources/Data/PlayerClasses/PlayerClasses.lua");

	if (result != LUA_OK) {
		return false;
	}
	// push the character class table to lua stack
	lua_getglobal(vm.get(), "character_classes");

	if (!lua_istable(vm.get(), -1)) {
		std::string error_msg = lua_tostring(vm.get(), -1);
		printf(error_msg.c_str());
		return false;
	}

	// get the number of defined classes and loop over them extracting the data.
	auto num_classes = lua_rawlen(vm.get(), -1);
	for (int i = 1; i < num_classes + 1; i++) {
		lua_pushnumber(vm.get(), i); // push table index
		lua_gettable(vm.get(), -2); // retrieve sub-table 

		if (!lua_istable(vm.get(), -1)) {
			return false;
		}

		auto name = utils::read_lua_string(vm, "name", -2);
		if (name == "") {
			return false;
		}

		auto description = utils::read_lua_string(vm, "description", -2);
		if (description == "") {
			return false;
		}

		std::vector<int> stats;
		lua_pushstring(vm.get(), "stats");
		lua_gettable(vm.get(), -2);
		if (!lua_istable(vm.get(), -1)) {
			return false;
		}

		auto num_stats = lua_rawlen(vm.get(), -1);
		for (int j = 1; j < num_stats + 1; j++) {
			auto stat = utils::read_lua_int(vm, j, -2);
			if (stat == -1) {
				return false;
			}
			stats.push_back(stat);
		}
		lua_pop(vm.get(), 1); // pop stat table

		lua_pushstring(vm.get(), "sprite");
		lua_gettable(vm.get(), -2);
		if (!lua_istable(vm.get(), -1)) {
			return false;
		}

		auto tilesheet = utils::read_lua_string(vm, "tilesheet", -2);
		if (tilesheet == "") {
			return false;
		}
		auto path = "./Resources/" + tilesheet;
		auto sprite_id = tex_manager.LoadTexture(path);

		auto clip_x = utils::read_lua_int(vm, "clip_x", -2);
		if (clip_x == -1) {
			return false;
		}

		auto clip_y = utils::read_lua_int(vm, "clip_y", -2);
		if (clip_y == -1) {
			return false;
		}

		lua_pop(vm.get(), 1); // pop the sprite array

		auto hit_die = utils::read_lua_int(vm, "hit_die", -2);
		if (hit_die == -1) {
			return false;
		}

		lua_pop(vm.get(), 1); // pop current sub-table

		char_options.insert({ i - 1, CharacterClass(name, description, stats, sprite_id, clip_x, clip_y, hit_die) });
	}
	return true;
}

CharacterCreationScreen::CharacterCreationScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, Keyboard& _keyboard, int _tile_width, int _tile_height, unsigned int _tileset):
	state_manager(_state_manager), world(_world), tex_manager(_tex_manager), event_manager(_event_manager), keyboard(_keyboard), tile_width(_tile_width), tile_height(_tile_height), tileset(_tileset)
{
	stats = { "STR", "DEX", "CON", "WIS", "INT", "CHA" };

	if (!load_character_classes()) {
		printf("Error loading lua character classes.\n");
		exit(1);
	}
}

void CharacterCreationScreen::handle_input(SDL_Event& event)
{
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT) {
		state_manager.stop_playing();
	}

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_UP: {
		selection = selection - 1 < 0 ? static_cast<int>(char_options.size()) - 1 : selection - 1;
		break;
	}
	case SDLK_DOWN: {
		selection = selection + 1 > static_cast<int>(char_options.size()) - 1 ? 0 : selection + 1;
		break;
	}
	case SDLK_RETURN: {
		auto entity = world.CreateEntity();
		int start_x{ 19 }, start_y{ 19 };
		world.AddComponent<Position>(entity, start_x, start_y, 0);
		world.AddComponent<Sprite>(entity, tileset, 0 * tile_width, 4 * tile_height, tile_width, tile_height, 1, 0xFF, 0xA7, 0x5D);
		world.AddComponent<Player>(entity, 8);
		auto* p = world.GetComponent<Player>(entity);
		world.AddComponent<Blocker>(entity);
	
		state_manager.push(GameState::GAME);
		return;
	}
	}
}

void CharacterCreationScreen::on_tick()
{

}

void CharacterCreationScreen::update(float dt)
{

}

void CharacterCreationScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const
{
	renderer.DrawCharacterSelectionScene(fps, char_options, selection, stats);
}

void CharacterCreationScreen::on_entrance(Renderer& renderer) const
{

}

void CharacterCreationScreen::on_bury() const
{

}


