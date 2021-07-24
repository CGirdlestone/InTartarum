
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Components.hpp"
#include "Keyboard.hpp"
#include "Window.hpp"
#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "World.hpp"
#include "EventManager.hpp"
#include "WorldMap.hpp"
#include "Renderer.hpp"
#include "Animator.hpp"
#include "ParticleSystem.hpp"
#include "SoundSystem.hpp"
#include "MoveSystem.hpp"
#include "ScriptSystem.hpp"
#include "InventorySystem.hpp"
#include "AISystem.hpp"
#include "CombatSystem.hpp"
#include "PlayerSystem.hpp"
#include "Prefabs.hpp"
#include "StateManager.hpp"
#include "SplashScreen.hpp"
#include "CharacterCreationScreen.hpp"
#include "GameScreen.hpp"
#include "InventoryScreen.hpp"
#include "ActionsState.hpp"
#include "QuantityState.hpp"
#include "TargetingScreen.hpp"
#include "HelpScreen.hpp"

void initialise_SDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		exit(1);
	}

	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	if (IMG_Init(img_flags) != img_flags) {
		exit(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		exit(1);
	}

	if (TTF_Init() < 0) {
		exit(1);
	}
}

void shutdown_SDL() {
	Mix_HaltMusic();
	SDL_Quit();
	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
}

void RegisterComponents(World& world) {
	world.RegisterComponent<Position>();
	world.RegisterComponent<Sprite>();
	world.RegisterComponent<Animation>();
	world.RegisterComponent<Scriptable>();
	world.RegisterComponent<Particle>();
	world.RegisterComponent<Player>();
	world.RegisterComponent<Actor>();
	world.RegisterComponent<Blocker>();
	world.RegisterComponent<Interactable>();
	world.RegisterComponent<LightSource>();
	world.RegisterComponent<Item>();
	world.RegisterComponent<Weapon>();
	world.RegisterComponent<Equipable>();
	world.RegisterComponent<Container>();
	world.RegisterComponent<Stackable>();
	world.RegisterComponent<AI>();
	world.RegisterComponent<Body>();
	world.RegisterComponent<ID>();
	world.RegisterComponent<Useable>();
	world.RegisterComponent<Fighter>();
}

void load_tiles(Renderer& renderer, TextureManager& tex_manager, unsigned int font_tileset, int tile_width, int tile_height) {
	renderer.AddTile(TileType::EMPTY, Sprite(font_tileset, 0 * tile_width, 0 * tile_height, tile_width, tile_height, 0));
	renderer.AddTile(TileType::FLOOR, Sprite(font_tileset, 14 * tile_width, 2 * tile_height, tile_width, tile_height, 0, 0x3D, 0x35, 0x2A));
	renderer.AddTile(TileType::GRASS, Sprite(font_tileset, 2 * tile_width, 2 * tile_height, tile_width, tile_height, 0, 0x86, 0xAF, 0x80));
	renderer.AddTile(TileType::SHALLOW_WATER, Sprite(font_tileset, 7 * tile_width, 15 * tile_height, tile_width, tile_height, 0, 0x87, 0xCE, 0xEB));
	renderer.AddTile(TileType::DEEP_WATER, Sprite(font_tileset, 7 * tile_width, 15 * tile_height, tile_width, tile_height, 0, 0x69, 0x95, 0xED));
	renderer.AddTile(TileType::ROAD, Sprite(font_tileset, 14 * tile_width, 7 * tile_height, tile_width, tile_height, 0, 0x55, 0x44, 0x44));
	renderer.AddTile(TileType::WALL_TL, Sprite(font_tileset, 9 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_TOP, Sprite(font_tileset, 13 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_TR,	Sprite(font_tileset, 11 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_LEFT, Sprite(font_tileset, 10 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_RIGHT, Sprite(font_tileset, 10 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_BL, Sprite(font_tileset, 8 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_BR,	Sprite(font_tileset, 12 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_BOTTOM,	Sprite(font_tileset, 13 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_LEFT_T,	Sprite(font_tileset, 12 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_RIGHT_T, Sprite(font_tileset, 9 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_TL, Sprite(font_tileset, 10 * tile_width, 13 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_TR, Sprite(font_tileset, 15 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_HORIZONTAL, Sprite(font_tileset, 4 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_VERTICAL, Sprite(font_tileset, 3 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_BL, Sprite(font_tileset, 0 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_BR, Sprite(font_tileset, 9 * tile_width, 13 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_LEFT_T, Sprite(font_tileset, 3 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_RIGHT_T, Sprite(font_tileset, 4 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::STAIRS, Sprite(font_tileset, 14 * tile_width, 3 * tile_height, tile_width, tile_height, 0, 0xCD, 0x5C, 0x5C));
}

int main(int argc, char* argv[])
{
	// create a new scope so all the SDL allocated objects are destroyed before the SDL quit functions are called. 
	{
		initialise_SDL();

		int tile_width{ 0 };
		int tile_height{ 0 };
		std::string tileset_path{ "./Resources/" };

		SmartLuaVM vm(nullptr, &lua_close);
		vm.reset(luaL_newstate());
		auto result = luaL_dofile(vm.get(), "./Config/window.lua");

		if (result == LUA_OK) {

			lua_getglobal(vm.get(), "tile_width");
			if (lua_isnumber(vm.get(), -1)) {
				tile_width = lua_tointeger(vm.get(), -1);
			}

			lua_getglobal(vm.get(), "tile_height");
			if (lua_isnumber(vm.get(), -1)) {
				tile_height = lua_tointeger(vm.get(), -1);
			}

			lua_getglobal(vm.get(), "tile_path");
			if (lua_isstring(vm.get(), -1)) {
				tileset_path += lua_tostring(vm.get(), -1);
			}
		}

		auto world = World();
		RegisterComponents(world);

		auto keyboard = Keyboard();
		auto window = Window();
		auto camera = Camera(window);

		auto tex_manager = TextureManager(window);

		auto font_tileset = tex_manager.LoadTexture(tileset_path);
		auto splash = tex_manager.LoadTexture("./Resources/fog01.png");
		auto fireball = tex_manager.LoadTexture("./Resources/Skills/fireball.png");

		auto entity_factory = EntityFactory(world, tex_manager);
		
		auto sound_manager = SoundManager();
		auto intro_music = sound_manager.LoadMusic("./Resources/Sounds/bleeding_out2.ogg");
		auto wind = sound_manager.LoadMusic("./Resources/Sounds/wind2.wav");

		auto button_click = sound_manager.LoadChunk("./Resources/Sounds/SFX/click3.wav");
		auto door_open = sound_manager.LoadChunk("./Resources/Sounds/SFX/doorOpen_2.ogg");
		auto door_close = sound_manager.LoadChunk("./Resources/Sounds/SFX/doorClose_2.ogg");
		auto hit_1 = sound_manager.LoadChunk("./Resources/Sounds/SFX/explosion_01.ogg");

		auto renderer = Renderer(world, window, tex_manager, camera);
		load_tiles(renderer, tex_manager, font_tileset, tile_width, tile_height);

		auto world_map = WorldMap(world, entity_factory, tex_manager);

		auto event_manager = EventManager(world);
		
		std::vector< std::reference_wrapper<BaseSystem> > systems;
		
		auto animator = Animator(world, event_manager);
		systems.push_back(std::reference_wrapper(animator));

		auto particles = ParticleSystem(world, event_manager);
		systems.push_back(std::reference_wrapper(particles));

		auto sound_system = SoundSystem(event_manager, sound_manager);
		systems.push_back(std::reference_wrapper(sound_system));

		auto move_system = MoveSystem(world, event_manager, camera, world_map, sound_manager);
		systems.push_back(std::reference_wrapper(move_system));

		auto message_log = MessageLog(world, event_manager);
		message_log.load_intro("./Resources/Data/intro.txt");
		systems.push_back(std::reference_wrapper(message_log));

		auto inventory_system = InventorySystem(world, event_manager, world_map, entity_factory);
		systems.push_back(std::reference_wrapper(inventory_system));

		auto script_system = ScriptSystem(world, event_manager, world_map, sound_manager, tex_manager);
		script_system.init();
		systems.push_back(std::reference_wrapper(script_system));

		auto ai_system = AISystem(world, event_manager, world_map);
		systems.push_back(std::reference_wrapper(ai_system));

		auto combat_system = CombatSystem(world, event_manager, world_map);
		systems.push_back(std::reference_wrapper(combat_system));

		auto player_system = PlayerSystem(world, event_manager, world_map);
		systems.push_back(std::reference_wrapper(player_system));
		

		auto state_manager = StateManager(world, renderer, event_manager);

		auto splash_screen = SplashScreen(state_manager, world, tex_manager, event_manager, keyboard, false, splash, intro_music);
		state_manager.add_state(splash_screen.get_state(), splash_screen);

		auto creation_screen = CharacterCreationScreen(state_manager, world, tex_manager, event_manager, keyboard, entity_factory, tile_width, tile_height, font_tileset, world_map.get_world_x(), world_map.get_world_y());
		state_manager.add_state(creation_screen.get_state(), creation_screen);

		auto game_screen = GameScreen(state_manager, world, tex_manager, event_manager, world_map, renderer, camera, message_log, keyboard, false, wind);
		state_manager.add_state(game_screen.get_state(), game_screen);

		auto inventory_screen = InventoryScreen(state_manager, world, event_manager, keyboard);
		state_manager.add_state(inventory_screen.get_state(), inventory_screen);

		auto action_state = ActionsState(state_manager, world, event_manager, keyboard);
		state_manager.add_state(action_state.get_state(), action_state);

		auto quantity_state = QuantityState(state_manager, world, event_manager, keyboard);
		state_manager.add_state(quantity_state.get_state(), quantity_state);

		auto targeting_state = TargetingScreen(state_manager, world, world_map, event_manager, keyboard);
		state_manager.add_state(targeting_state.get_state(), targeting_state);

		auto help_state = HelpScreen(state_manager, world, event_manager, keyboard);
		help_state.load_controls();
		state_manager.add_state(help_state.get_state(), help_state);

		SDL_Event event;

		uint32_t current_time{ 0 };
		uint32_t last_time{ 0 };
		float dt{ 0.0f };

		uint32_t fps_last{ 0 };
		uint32_t fps{ 0 };
		uint32_t frames{ 0 };

		state_manager.push(GameState::SPLASH_MENU);

		while (state_manager.is_playing()) {

			frames++;
			if (fps_last < SDL_GetTicks() - 1000) {
				fps_last = SDL_GetTicks();
				fps = frames;
				frames = 0;
			}

			current_time = SDL_GetTicks();
			dt = static_cast<float>(current_time - last_time) / 1000.0f;
			last_time = current_time;

			state_manager.draw_scene(fps, dt);
			state_manager.handle_input(event);
			state_manager.update(dt);

			std::for_each(systems.begin(), systems.end(), [dt](std::reference_wrapper<BaseSystem>& s) {s.get().update(dt); });
		}
	}

	shutdown_SDL();
	return 0;
}