
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
}

void load_tiles(Renderer& renderer, TextureManager& tex_manager, unsigned int tile_size) {
	auto id = tex_manager.LoadTexture("./Resources/colored_packed.png");
	renderer.AddTile(TileType::EMPTY,		Sprite(id, 0 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::DIRT_LOOSE,	Sprite(id, 1 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::DIRT_MED,	Sprite(id, 2 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::DIRT_HEAVY,	Sprite(id, 3 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::DIRT_PACKED, Sprite(id, 4 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_THIN,	Sprite(id, 5 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_MED,	Sprite(id, 6 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::GRASS_THICK, Sprite(id, 7 * tile_size, 0 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::TREE_01,		Sprite(id, 0 * tile_size, 1 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::TREE_02,		Sprite(id, 1 * tile_size, 1 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::TREE_03,		Sprite(id, 2 * tile_size, 1 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::TREE_04,		Sprite(id, 3 * tile_size, 1 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::WATER,		Sprite(id, 8 * tile_size, 5 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::STAIRS,		Sprite(id, 3 * tile_size, 6 * tile_size, tile_size, tile_size, 0));
	renderer.AddTile(TileType::STONE,		Sprite(id, 10 * tile_size, 17 * tile_size, tile_size, tile_size, 0));
}

void build_town(Level& level) {
	// placeholder
	auto& grid = level.get_grid();

	for (int i = 0; i < grid.get_width(); i++) {
		for (int j = 0; j < grid.get_height(); j++) {
			if ((i == 0 || i == grid.get_width() - 1) || (j == 0 || j == grid.get_height() - 1)) {
				grid.set_tile(i, j, false, true, TileType::STONE);
			}
		}
	}
	
	for (int i = 10; i < 20; i++) {
		for (int j = 10; j < 16; j++) {
			if (i == 15 && j == 15) {
				grid.set_tile(i, j, true, false, TileType::EMPTY);
				continue;
			}
			if ((i == 10 || i == 19) || (j == 10 || j == 15)) {
				grid.set_tile(i, j, false, true, TileType::STONE);
			}
		}
	}

	grid.set_tile(25, 25, true, false, TileType::STAIRS);
}

int main(int argc, char* argv[])
{
	srand(time(0));
	// create a new scope so all the SDL allocated objects are destroyed before the SDL quit functions are called. 
	{
		const int TILE_SIZE{ 16 };
		const int WIDTH{ 80 };
		const int HEIGHT{ 45 };

		initialise_SDL();

		auto world = World();
		RegisterComponents(world);


		auto window = Window("Sticky", WIDTH, HEIGHT, TILE_SIZE);
		auto camera = Camera(0, 0, WIDTH - 10, HEIGHT, 2 * WIDTH, 2 * HEIGHT);


		auto tex_manager = TextureManager(window);
		auto bat_tex = tex_manager.LoadTexture("./Resources/32x32-bat-sprite.png");
		auto fire_tex = tex_manager.LoadTexture("./Resources/CampFireFinished.png");
		auto exp_tex = tex_manager.LoadTexture("./Resources/exp2_0.png");
		auto kenny_tileset = tex_manager.LoadTexture("./Resources/colored_packed.png");
		auto kenny_transparent = tex_manager.LoadTexture("./Resources/colored_transparent_packed.png");
		auto splash = tex_manager.LoadTexture("./Resources/blizzardskull.png");

		auto sound_manager = SoundManager();
		auto intro_music = sound_manager.LoadMusic("./Resources/Sounds/bleeding_out2.ogg");
		auto button_click = sound_manager.LoadChunk("./Resources/Sounds/SFX/click3.wav");
		auto wind = sound_manager.LoadMusic("./Resources/Sounds/wind2.wav");

		auto renderer = Renderer(world, window, tex_manager, camera);
		load_tiles(renderer, tex_manager, TILE_SIZE);


		auto town = Level(2 * WIDTH, 2 * HEIGHT);
		build_town(town);

		auto world_map = WorldMap(town, world, 2 * WIDTH, 2 * HEIGHT);


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
		

		auto state_manager = StateManager(world, renderer, event_manager);

		auto splash_screen = SplashScreen(state_manager, world, tex_manager, event_manager, false, splash, intro_music);
		state_manager.add_state(splash_screen.get_state(), splash_screen);

		auto game_screen = GameScreen(state_manager, world, tex_manager, event_manager, world_map, false, wind);
		state_manager.add_state(GameState::GAME, game_screen);


		auto entity = world.CreateEntity();
		int start_x{ 20 }, start_y{ 20 };
		world.AddComponent<Position>(entity, start_x, start_y, 0);
		world.AddComponent<Sprite>(entity, kenny_transparent, 24 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, 1);
		world.AddComponent<Player>(entity);
		world.AddComponent<Blocker>(entity);
		camera.follow(start_x, start_y);

		auto npc = world.CreateEntity();
		int npc_x{ 18 }, npc_y{ 21 };
		world.AddComponent<Position>(npc, npc_x, npc_y, 0);
		world.AddComponent<Sprite>(npc, kenny_transparent, 25 * TILE_SIZE, 1 * TILE_SIZE, TILE_SIZE, TILE_SIZE, 1);
		world.AddComponent<Blocker>(npc);
		world.AddComponent<Actor>(npc);

		auto fire = world.CreateEntity();
		world.AddComponent<Position>(fire, 25, 25, 0);
		world.AddComponent<Sprite>(fire, fire_tex, 0, 0, 64, 64, 0);
		world.AddComponent<Animation>(fire, 0.1f, fire_tex, 0, 0, 64, 64);
		auto fire_animation = world.GetComponent<Animation>(fire);
		for (int i = 1; i < 5; i++) { fire_animation->animations.at(state::IDLE).push_back(AnimFrame(fire_tex, 64 * i, 0, 64, 64)); }

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

			state_manager.draw_scene(fps);
			state_manager.handle_input(event);
			state_manager.update(dt);

			std::for_each(systems.begin(), systems.end(), [dt](std::reference_wrapper<BaseSystem>& s) {s.get().update(dt); });

			auto scripts = world.GetComponents<Scriptable>();
			for (auto& s : scripts) {
				s->OnUpdate(world, entity, event);
			}
		}
	}

	shutdown_SDL();
	return 0;
}