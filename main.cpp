
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
#include "Prefabs.hpp"
#include "StateManager.hpp"
#include "SplashScreen.hpp"
#include "CharacterCreationScreen.hpp"
#include "GameScreen.hpp"

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
}

void load_tiles(Renderer& renderer, TextureManager& tex_manager, unsigned int font_tileset, int tile_width, int tile_height) {
	renderer.AddTile(TileType::EMPTY,		Sprite(font_tileset, 0 * tile_width, 0 * tile_height, tile_width, tile_height, 0));
	renderer.AddTile(TileType::FLOOR,		Sprite(font_tileset, 14 * tile_width, 2 * tile_height, tile_width, tile_height, 0, 0x3D, 0x35, 0x2A));
	renderer.AddTile(TileType::WALL_TL,		Sprite(font_tileset,  9 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_TOP,	Sprite(font_tileset,  13 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_TR,		Sprite(font_tileset,  11 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_LEFT,	Sprite(font_tileset,  10 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_RIGHT,	Sprite(font_tileset,  10 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_BL,		Sprite(font_tileset,  8 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_BR,		Sprite(font_tileset,  12 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_BOTTOM,	Sprite(font_tileset,  13 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_LEFT_T, Sprite(font_tileset, 12 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::WALL_RIGHT_T,Sprite(font_tileset, 9 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_TL,	Sprite(font_tileset, 10 * tile_width, 13 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_TR,	Sprite(font_tileset, 15 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_HORIZONTAL, Sprite(font_tileset, 4 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_VERTICAL, Sprite(font_tileset, 3 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_BL,	Sprite(font_tileset, 0 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_BR,	Sprite(font_tileset, 9 * tile_width, 13 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_LEFT_T, Sprite(font_tileset, 3 * tile_width, 12 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::BORDER_RIGHT_T, Sprite(font_tileset, 4 * tile_width, 11 * tile_height, tile_width, tile_height, 0, 0xBB, 0xAA, 0x99));
	renderer.AddTile(TileType::STAIRS,		Sprite(font_tileset, 14 * tile_width, 3 * tile_height, tile_width, tile_height, 0, 0xCD, 0x5C, 0x5C));
}

void build_town(Level& level, World& world, TextureManager& texture_manager, unsigned int font_tileset, int tile_width, int tile_height) {
	// placeholder
	auto& grid = level.get_grid();

	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			if (i == 0 && j ==0) {
				grid.set_tile(i, j, false, true, TileType::WALL_TL);
			}
			else if (i == grid.get_width() - 1 && j ==0) {
				grid.set_tile(i, j, false, true, TileType::WALL_TR);
			}
			else if (i == 0 && j == grid.get_height() - 1) {
				grid.set_tile(i, j, false, true, TileType::WALL_BL);
			}
			else if (i == grid.get_width() - 1 && j == grid.get_height() - 1) {
				grid.set_tile(i, j, false, true, TileType::WALL_BR);
			}
			else if (i == 0 || i == grid.get_width() - 1) {
				grid.set_tile(i, j, false, true, TileType::WALL_LEFT);
			}
			else if (j == 0 || j == grid.get_height() - 1) {
				grid.set_tile(i, j, false, true, TileType::WALL_TOP);
			}
			else {
				grid.set_tile(i, j, true, false, TileType::FLOOR);
			}
		}
	}
	
	for (int i = 10; i < 20; i++) {
		for (int j = 10; j < 16; j++) {
			if (i == 15 && j == 15) {
				grid.set_tile(i, j, true, false, TileType::FLOOR);
				continue;
			}
			if (i == 10 && j == 10) {
				grid.set_tile(i, j, false, true, TileType::WALL_TL);
			}
			else if (i == 20 - 1 && j == 10) {
				grid.set_tile(i, j, false, true, TileType::WALL_TR);
			}
			else if (i == 10 && j == 16 - 1) {
				grid.set_tile(i, j, false, true, TileType::WALL_BL);
			}
			else if (i == 20 - 1 && j == 16 - 1) {
				grid.set_tile(i, j, false, true, TileType::WALL_BR);
			}
			else if (i == 10 || i == 20 - 1) {
				grid.set_tile(i, j, false, true, TileType::WALL_LEFT);
			}
			else if (j == 10 || j == 16 - 1) {
				grid.set_tile(i, j, false, true, TileType::WALL_TOP);
			}
			else {
				grid.set_tile(i, j, true, false, TileType::FLOOR);
			}
		}
	}

	grid.set_tile(25, 25, true, false, TileType::STAIRS);

	auto door = world.CreateEntity();
	world.AddComponent<Position>(door, 15, 15, 0);
	world.AddComponent<Sprite>(door, font_tileset, 11 * tile_width, 2 * tile_height, tile_width, tile_height, 0, 0x55, 0x44, 0x44);
	world.AddComponent<Animation>(door, 0.1f, font_tileset, 13 * tile_width, 2 * tile_height, tile_width, tile_height, false);
	world.AddComponent<Interactable>(door);
	world.AddComponent<Blocker>(door, true);
	auto* door_animation = world.GetComponent<Animation>(door);
	door_animation->animations.at(state::IDLE).push_back(AnimFrame(font_tileset, 11 * tile_width, 2 * tile_height, tile_width, tile_height));

	world.AddComponent<Scriptable>(door, door);
	auto* script = world.GetComponent<Scriptable>(door);
	script->OnBump = "OPEN_DOOR";



	auto npc = world.CreateEntity();
	int npc_x{ 18 }, npc_y{ 21 };
	world.AddComponent<Position>(npc, npc_x, npc_y, 0);
	world.AddComponent<Sprite>(npc, font_tileset, 1 * tile_width, 0 * tile_height, tile_width, tile_height, 1);
	world.AddComponent<Blocker>(npc);
	world.AddComponent<Actor>(npc);

	auto chest = world.CreateEntity();
	world.AddComponent<Position>(chest, 15, 20, 0);
	world.AddComponent<Sprite>(chest, font_tileset, 11 * tile_width, 13 * tile_height, tile_width, tile_height, 0, 0x55, 0x44, 0x44);
	world.AddComponent<Animation>(chest, 0.1f, font_tileset, 12 * tile_width, 13 * tile_height, tile_width, tile_height, false);
	world.AddComponent<Blocker>(chest);
	world.AddComponent<Interactable>(chest);
	auto* chest_animation = world.GetComponent<Animation>(chest);
	chest_animation->animations.at(state::IDLE).push_back(AnimFrame(font_tileset, 11 * tile_width, 13 * tile_height, tile_width, tile_height));

	world.AddComponent<Scriptable>(chest, chest);
	script = world.GetComponent<Scriptable>(chest);
	script->OnBump = "OPEN_CHEST";

	auto fire = world.CreateEntity();
	world.AddComponent<Position>(fire, 23, 23, 0);
	world.AddComponent<Sprite>(fire, font_tileset, 5 * tile_width, 1 * tile_height, tile_width, tile_height, 0, 0xFF, 0x00, 0x00);
	world.AddComponent<Animation>(fire, 0.1f, font_tileset, 4 * tile_width, 2 * tile_height, tile_width, tile_height);
	auto fire_animation = world.GetComponent<Animation>(fire);
	fire_animation->animations.at(state::IDLE).push_back(AnimFrame(font_tileset, 5 * tile_width, 1 * tile_height, tile_width, tile_height));
	world.AddComponent<LightSource>(fire, 10);
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
				tile_width = lua_tonumber(vm.get(), -1);
			}

			lua_getglobal(vm.get(), "tile_height");
			if (lua_isnumber(vm.get(), -1)) {
				tile_height = lua_tonumber(vm.get(), -1);
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

		printf("%d\n%d\n", camera.get_width(), camera.get_height());
		printf("%d\n%d\n", window.GetWidth(), window.GetHeight());

		auto tex_manager = TextureManager(window);

		auto font_tileset = tex_manager.LoadTexture(tileset_path);
		auto empty_skill = tex_manager.LoadTexture("./Resources/Skills/EmptyButton.png");
		auto splash = tex_manager.LoadTexture("./Resources/fog01.png");
		auto parchment = tex_manager.LoadTexture("./Resources/Parchment.jpg");
		auto fireball = tex_manager.LoadTexture("./Resources/Skills/fireball.png");

		/*
		auto chest1_tex = tex_manager.LoadTexture("./Resources/Chest1.png");
		auto gui_0 = tex_manager.LoadTexture("./Resources/GUI0.png");
		auto gui_1 = tex_manager.LoadTexture("./Resources/GUI1.png");
		auto player_0 = tex_manager.LoadTexture("./Resources/Player0.png");
		auto player_1 = tex_manager.LoadTexture("./Resources/Player1.png");
		auto tree_0 = tex_manager.LoadTexture("./Resources/Tree0.png");
		auto tree_1 = tex_manager.LoadTexture("./Resources/Tree1.png");
		auto floor = tex_manager.LoadTexture("./Resources/Floor.png");
		auto water = tex_manager.LoadTexture("./Resources/Pit0.png");
		auto tile = tex_manager.LoadTexture("./Resources/Tile.png");
		auto door_0 = tex_manager.LoadTexture("./Resources/Door0.png");
		auto door_1 = tex_manager.LoadTexture("./Resources/Door1.png");
		auto effect_0 = tex_manager.LoadTexture("./Resources/Effect0.png");
		auto effect_1 = tex_manager.LoadTexture("./Resources/Effect1.png");
		auto walls = tex_manager.LoadTexture("./Resources/Wall.png");

		

		auto kenny_tileset = tex_manager.LoadTexture("./Resources/colored_packed.png");
		auto kenny_transparent = tex_manager.LoadTexture("./Resources/colored_transparent_packed.png");
		
		auto fire_tex = tex_manager.LoadTexture("./Resources/CampFireFinished.png");
		auto exp_tex = tex_manager.LoadTexture("./Resources/exp2_0.png");
		*/
		

		auto sound_manager = SoundManager();
		auto intro_music = sound_manager.LoadMusic("./Resources/Sounds/bleeding_out2.ogg");
		auto button_click = sound_manager.LoadChunk("./Resources/Sounds/SFX/click3.wav");
		auto wind = sound_manager.LoadMusic("./Resources/Sounds/wind2.wav");
		auto door_open = sound_manager.LoadChunk("./Resources/Sounds/SFX/doorOpen_2.ogg");
		auto door_close = sound_manager.LoadChunk("./Resources/Sounds/SFX/doorClose_2.ogg");

		auto renderer = Renderer(world, window, tex_manager, camera);
		load_tiles(renderer, tex_manager, font_tileset, tile_width, tile_height);
		
		auto town = Level();
		build_town(town, world, tex_manager, font_tileset, tile_width, tile_height);

		auto world_map = WorldMap(town, world);

		auto event_manager = EventManager(world);
		
		std::vector< std::reference_wrapper<BaseSystem> > systems;
		
		auto animator = Animator(world, event_manager);
		systems.push_back(std::reference_wrapper(animator));

		auto particles = ParticleSystem(world, event_manager);
		systems.push_back(std::reference_wrapper(particles));

		auto sound_system = SoundSystem(event_manager, sound_manager);
		systems.push_back(std::reference_wrapper(sound_system));

		auto move_system = MoveSystem(world, event_manager, camera, world_map);
		systems.push_back(std::reference_wrapper(move_system));

		auto message_log = MessageLog(world, event_manager);
		systems.push_back(std::reference_wrapper(message_log));

		auto script_system = ScriptSystem(world, event_manager, world_map, sound_manager, tex_manager);
		script_system.init();
		systems.push_back(std::reference_wrapper(script_system));
		

		auto state_manager = StateManager(world, renderer, event_manager);

		auto splash_screen = SplashScreen(state_manager, world, tex_manager, event_manager, keyboard, false, splash, intro_music);
		state_manager.add_state(splash_screen.get_state(), splash_screen);

		auto creation_screen = CharacterCreationScreen(state_manager, world, tex_manager, event_manager, keyboard, tile_width, tile_height, font_tileset);
		state_manager.add_state(creation_screen.get_state(), creation_screen);

		auto game_screen = GameScreen(state_manager, world, tex_manager, event_manager, world_map, renderer, camera, message_log, keyboard, false, wind);
		state_manager.add_state(GameState::GAME, game_screen);

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