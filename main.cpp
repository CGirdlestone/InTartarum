
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Components.hpp"
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

void load_tiles(Renderer& renderer, TextureManager& tex_manager, unsigned int tile_size) {
	auto floor = tex_manager.LoadTexture("./Resources/Floor.png");
	auto tree = tex_manager.LoadTexture("./Resources/Tree0.png");
	auto water = tex_manager.LoadTexture("./Resources/Pit0.png");
	auto tile = tex_manager.LoadTexture("./Resources/Tile.png");
	renderer.AddTile(TileType::EMPTY,		Sprite(floor, 10 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::DIRT_LOOSE,	Sprite(floor, 1 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::DIRT_MED,	Sprite(floor, 2 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::DIRT_HEAVY,	Sprite(floor, 3 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::DIRT_PACKED, Sprite(floor, 4 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_MIDDLE,Sprite(floor, 8 * tile_size, 7 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_LEFT,	Sprite(floor, 7 * tile_size, 7 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_RIGHT, Sprite(floor, 9 * tile_size, 7 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_TOP,	Sprite(floor, 8 * tile_size, 6 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_BOTTOM,Sprite(floor, 8 * tile_size, 8 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_TR,	Sprite(floor, 9 * tile_size, 6 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_TL,	Sprite(floor, 7 * tile_size, 6 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_BR,	Sprite(floor, 9 * tile_size, 8 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_BL,	Sprite(floor, 7 * tile_size, 8 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::TREE_01,		Sprite(tree,  3 * tile_size, 3 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::TREE_02,		Sprite(tree,  3 * tile_size, 6 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::TREE_03,		Sprite(tree,  3 * tile_size, 12 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::TREE_04,		Sprite(tree,  3 * tile_size, 15 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::WATER,		Sprite(water, 1 * tile_size, 9 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::STAIRS,		Sprite(tile,  7 * tile_size, 3 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::STONE_01,	Sprite(floor, 1 * tile_size, 7 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::STONE_02,	Sprite(floor, 8 * tile_size, 19 * tile_size, tile_size, tile_size, 0));
}

void build_town(Level& level, World& world, TextureManager& texture_manager, const int TILE_SIZE) {
	// placeholder
	auto& grid = level.get_grid();

	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			if ((i == 0 || i == grid.get_width() - 1) || (j == 0 || j == grid.get_height() - 1)) {
				grid.set_tile(i, j, false, true, TileType::STONE_01);
			}
			else {
				grid.set_tile(i, j, true, false, TileType::GRASS_MIDDLE);
			}
		}
	}
	
	for (int i = 10; i < 20; i++) {
		for (int j = 10; j < 16; j++) {
			if (i == 15 && j == 15) {
				grid.set_tile(i, j, true, false, TileType::STONE_02);
				continue;
			}
			if ((i == 10 || i == 19) || (j == 10 || j == 15)) {
				grid.set_tile(i, j, false, true, TileType::STONE_01);
				continue;
			}
			grid.set_tile(i, j, true, false, TileType::STONE_02);
		}
	}

	grid.set_tile(25, 25, true, false, TileType::STAIRS);

	auto door0 = texture_manager.LoadTexture("./Resources/Door0.png");
	auto door1 = texture_manager.LoadTexture("./Resources/Door1.png");
	auto door = world.CreateEntity();
	world.AddComponent<Position>(door, 15, 15, 0);
	world.AddComponent<Sprite>(door, door0, 0 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, 0);
	world.AddComponent<Animation>(door, 0.1f, door1, 0 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, false);
	world.AddComponent<Interactable>(door);
	world.AddComponent<Blocker>(door, true);
	auto* door_animation = world.GetComponent<Animation>(door);
	door_animation->animations.at(state::IDLE).push_back(AnimFrame(door0, 0 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));

	world.AddComponent<Scriptable>(door, door);
	auto* script = world.GetComponent<Scriptable>(door);
	script->OnBump = "OPEN_DOOR";


	auto player_0 = texture_manager.LoadTexture("./Resources/Player0.png");
	auto player_1 = texture_manager.LoadTexture("./Resources/Player1.png");
	auto npc = world.CreateEntity();
	int npc_x{ 18 }, npc_y{ 21 };
	world.AddComponent<Position>(npc, npc_x, npc_y, 0);
	world.AddComponent<Sprite>(npc, player_0, 0 * TILE_SIZE, 8 * TILE_SIZE, TILE_SIZE, TILE_SIZE, 1);
	world.AddComponent<Blocker>(npc);
	world.AddComponent<Actor>(npc);
	world.AddComponent<Animation>(npc, 0.2f, player_0, 0 * TILE_SIZE, 8 * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	auto* npc_animation = world.GetComponent<Animation>(npc);
	npc_animation->animations.at(state::IDLE).push_back(AnimFrame(player_1, 0 * TILE_SIZE, 8 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
}

int main(int argc, char* argv[])
{
	srand(time(0));
	// create a new scope so all the SDL allocated objects are destroyed before the SDL quit functions are called. 
	{
		const int TILE_SIZE{ 16 };
		const int WIDTH{ 90 };
		const int HEIGHT{ 46 };
		const int MAP_WIDTH{ 110 };
		const int MAP_HEIGHT{ 60 };

		initialise_SDL();

		auto world = World();
		RegisterComponents(world);


		auto window = Window("Sticky", WIDTH, HEIGHT, TILE_SIZE, TILE_SIZE);
		auto camera = Camera(0, 0, WIDTH - 20, HEIGHT, MAP_WIDTH, MAP_HEIGHT, 2);


		auto tex_manager = TextureManager(window);
		auto splash = tex_manager.LoadTexture("./Resources/fog01.png");
		auto chest0_tex = tex_manager.LoadTexture("./Resources/Chest0.png");
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

		auto empty_skill = tex_manager.LoadTexture("./Resources/Skills/EmptyButton.png");
		auto fireball = tex_manager.LoadTexture("./Resources/Skills/fireball.png");

		auto kenny_tileset = tex_manager.LoadTexture("./Resources/colored_packed.png");
		auto kenny_transparent = tex_manager.LoadTexture("./Resources/colored_transparent_packed.png");
		
		auto fire_tex = tex_manager.LoadTexture("./Resources/CampFireFinished.png");
		auto exp_tex = tex_manager.LoadTexture("./Resources/exp2_0.png");

		auto parchment = tex_manager.LoadTexture("./Resources/Parchment.jpg");


		auto sound_manager = SoundManager();
		auto intro_music = sound_manager.LoadMusic("./Resources/Sounds/bleeding_out2.ogg");
		auto button_click = sound_manager.LoadChunk("./Resources/Sounds/SFX/click3.wav");
		auto wind = sound_manager.LoadMusic("./Resources/Sounds/wind2.wav");
		auto door_open = sound_manager.LoadChunk("./Resources/Sounds/SFX/doorOpen_2.ogg");
		auto door_close = sound_manager.LoadChunk("./Resources/Sounds/SFX/doorClose_2.ogg");

		auto renderer = Renderer(world, window, tex_manager, camera);
		load_tiles(renderer, tex_manager, TILE_SIZE);


		auto town = Level(MAP_WIDTH, MAP_HEIGHT);
		build_town(town, world, tex_manager, TILE_SIZE);

		auto world_map = WorldMap(town, world, MAP_WIDTH, MAP_HEIGHT);


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

		auto script_system = ScriptSystem(world, event_manager, world_map, sound_manager);
		script_system.init();
		systems.push_back(std::reference_wrapper(script_system));
		

		auto state_manager = StateManager(world, renderer, event_manager);

		auto splash_screen = SplashScreen(state_manager, world, tex_manager, event_manager, false, splash, intro_music);
		state_manager.add_state(splash_screen.get_state(), splash_screen);

		auto game_screen = GameScreen(state_manager, world, tex_manager, event_manager, world_map, false, wind);
		state_manager.add_state(GameState::GAME, game_screen);


		auto entity = world.CreateEntity();
		int start_x{ 19 }, start_y{ 19 };
		world.AddComponent<Position>(entity, start_x, start_y, 0);
		world.AddComponent<Sprite>(entity, player_0, 1 * TILE_SIZE, 11 * TILE_SIZE, TILE_SIZE, TILE_SIZE, 1);
		world.AddComponent<Player>(entity, 8);
		auto* p = world.GetComponent<Player>(entity);
		world.AddComponent<Blocker>(entity);
		world.AddComponent<Animation>(entity, 0.2f, player_0, 1 * TILE_SIZE, 11 * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		auto* entity_animation = world.GetComponent<Animation>(entity);
		entity_animation->animations.at(state::IDLE).push_back(AnimFrame(player_1, 1 * TILE_SIZE, 11 * TILE_SIZE, TILE_SIZE, TILE_SIZE));

		auto chest = world.CreateEntity();
		world.AddComponent<Position>(chest, 15, 20, 0);
		world.AddComponent<Sprite>(chest, chest0_tex, 1 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, 0);
		world.AddComponent<Animation>(chest, 0.1f, chest1_tex, 1 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, false);
		world.AddComponent<Blocker>(chest);
		world.AddComponent<Interactable>(chest);
		auto* chest_animation = world.GetComponent<Animation>(chest);
		chest_animation->animations.at(state::IDLE).push_back(AnimFrame(chest0_tex, 1 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
		
		world.AddComponent<Scriptable>(chest, chest);
		auto* script = world.GetComponent<Scriptable>(chest);
		script->OnBump = "OPEN_CHEST";


		auto fire = world.CreateEntity();
		world.AddComponent<Position>(fire, 25, 25, 0);
		world.AddComponent<Sprite>(fire, fire_tex, 0, 0, 64, 64, 0);
		world.AddComponent<Animation>(fire, 0.1f, fire_tex, 0, 0, 64, 64);
		auto fire_animation = world.GetComponent<Animation>(fire);
		for (int i = 1; i < 5; i++) { fire_animation->animations.at(state::IDLE).push_back(AnimFrame(fire_tex, 64 * i, 0, 64, 64)); }
		world.AddComponent<LightSource>(fire, 10);

		world_map.populate_entity_grid();

		SDL_Event event;

		uint32_t current_time{ 0 };
		uint32_t last_time{ 0 };
		float dt{ 0.0f };

		uint32_t fps_last{ 0 };
		uint32_t fps{ 0 };
		uint32_t frames{ 0 };

		bool playing{ true };

		state_manager.push(GameState::SPLASH_MENU);

		camera.follow(start_x, start_y);
		world_map.update_fov(start_x, start_y, p->vision);

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