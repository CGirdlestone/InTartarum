#include "CharacterCreationScreen.hpp"

void CharacterCreationScreen::load_character_classes()
{
	SmartLuaVM vm(nullptr, &lua_close);
	vm.reset(luaL_newstate());
	auto result = luaL_dofile(vm.get(), "./Resources/Data/PlayerClasses/PlayerClasses.lua");

	if (result == LUA_OK) {
		// push to character class table to lua stack
		lua_getglobal(vm.get(), "character_classes");

		if (lua_istable(vm.get(), -1)) {
			// get the number of defined classes and loop over them extracting the data.
			auto num_classes = lua_rawlen(vm.get(), -1);
			for (int i = 1; i < num_classes + 1; i++) {
				lua_pushnumber(vm.get(), i); // push table index
				lua_gettable(vm.get(), -2); // retrieve sub-table 
				if (lua_istable(vm.get(), -1)) {
					lua_pushstring(vm.get(), "name");
					lua_gettable(vm.get(), -2);
					auto name = std::string(lua_tostring(vm.get(), -1));
					lua_pop(vm.get(), 1); // pop name

					lua_pushstring(vm.get(), "description");
					lua_gettable(vm.get(), -2);
					auto description = std::string(lua_tostring(vm.get(), -1));
					lua_pop(vm.get(), 1); // pop description

					lua_pushstring(vm.get(), "stats");
					lua_gettable(vm.get(), -2);
					std::vector<int> stats;
					auto num_stats = lua_rawlen(vm.get(), -1);
					if (!lua_istable(vm.get(), -1)) {
						printf("Missing stat array");
						return;
					}
					for (int j = 1; j < num_stats + 1; j++) {
						lua_pushnumber(vm.get(), j);
						lua_gettable(vm.get(), -2);
						auto stat = lua_tointeger(vm.get(), -1);
						stats.push_back(stat);
						lua_pop(vm.get(), 1); // pop stat
					}
					lua_pop(vm.get(), 1); // pop stat table

					lua_pushstring(vm.get(), "sprite");
					lua_gettable(vm.get(), -2);
					if (!lua_istable(vm.get(), -1)) {
						printf("Missing sprite array");
						return;
					}

					lua_pushstring(vm.get(), "tilesheet");
					lua_gettable(vm.get(), -2);
					if (!lua_isstring(vm.get(), -1)) {
						printf("Should be sprite file path!");
						return;
					}
					std::string tilesheet = std::string(lua_tostring(vm.get(), -1));
					auto path = "./Resources/" + tilesheet;
					auto sprite_id = tex_manager.LoadTexture(path);
					lua_pop(vm.get(), 1); // pop sprite path

					lua_pushstring(vm.get(), "clip_x");
					lua_gettable(vm.get(), -2);
					if (!lua_isnumber(vm.get(), -1)) {
						printf("Should be clip x!");
						return;
					}
					auto clip_x = lua_tointeger(vm.get(), -1);
					lua_pop(vm.get(), 1); // pop clip x

					lua_pushstring(vm.get(), "clip_y");
					lua_gettable(vm.get(), -2);
					if (!lua_isnumber(vm.get(), -1)) {
						printf("Should be clip y!");
						return;
					}
					auto clip_y = lua_tointeger(vm.get(), -1);
					lua_pop(vm.get(), 1); // pop clip y

					lua_pop(vm.get(), 1); // pop the sprite array

					lua_pop(vm.get(), 1); // pop current sub-table

					char_options.insert({ i - 1, CharacterClass(name, description, stats, sprite_id, clip_x, clip_y) });
				}
				else {
					printf("missing sub table!");
				}

			}

		}
	}
	else {
		std::string error_msg = lua_tostring(vm.get(), -1);
		printf(error_msg.c_str());
	}
}

CharacterCreationScreen::CharacterCreationScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, Keyboard& _keyboard):
	state_manager(_state_manager), world(_world), tex_manager(_tex_manager), event_manager(_event_manager), keyboard(_keyboard)
{
	stats = { "STR", "DEX", "CON", "WIS", "INT", "CHA" };
	auto id = tex_manager.LoadTexture("./Resources/Player0.png");

	load_character_classes();
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
		world.AddComponent<Sprite>(entity, char_options.at(selection).id, char_options.at(selection).clip_x * TILE_SIZE, char_options.at(selection).clip_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, 1);
		world.AddComponent<Player>(entity, 8);
		auto* p = world.GetComponent<Player>(entity);
		world.AddComponent<Blocker>(entity);
		world.AddComponent<Animation>(entity, 0.2f, char_options.at(selection).id, char_options.at(selection).clip_x * TILE_SIZE, char_options.at(selection).clip_y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		auto* entity_animation = world.GetComponent<Animation>(entity);
		entity_animation->animations.at(state::IDLE).push_back(AnimFrame(char_options.at(selection).id + 1, char_options.at(selection).clip_x * TILE_SIZE, char_options.at(selection).clip_y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

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


