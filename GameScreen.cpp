#include "GameScreen.hpp"

void GameScreen::descend_dungeon()
{
	world_map.set_depth(world_map.get_current_depth() + 1);
	world_map.create_dungeon();
	world_map.populate_entity_grid();
	auto components = world.GetComponents<Position, Player>();
	auto& [pos, player] = components[0]; // there will only be one player.
	world_map.update_fov(pos->x, pos->y, player->vision);

	auto& renderer = state_manager.get_renderer();
	renderer.DrawMap(world_map.get_level(world_map.get_current_depth()));

	pos->z++;
}

void GameScreen::return_to_safe_zone()
{
	world_map.set_depth(0);
	world_map.populate_entity_grid();

	auto components = world.GetComponents<Position, Player>();
	auto& [pos, player] = components[0]; // there will only be one player.
	world_map.update_fov(pos->x, pos->y, player->vision);

	auto& renderer = state_manager.get_renderer();
	renderer.DrawMap(world_map.get_level(world_map.get_current_depth()));
	
	pos->z = 0;
}

void GameScreen::open_doors()
{
	auto& grid = world_map.get_level(world_map.get_current_depth()).get_grid();
	auto player = world.GetEntitiesWith<Player>()[0];
	auto* pos = world.GetComponent<Position>(player);

	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (i == 0 && j == 0) {
				continue;
			}

			if (!grid.in_bounds(pos->x + i, pos->y + j)) {
				continue;
			}
			
			auto entities = world_map.get_entity_grid().get(pos->x + i, pos->y + j);
			for (auto e : entities) {
				auto* interactable = world.GetComponent<Interactable>(e);

				if (interactable == nullptr) {
					continue;
				}
				auto* script = world.GetComponent<Scriptable>(e);
				if (script->OnBump == "") {
					continue;
				}

				if (interactable->repeatable) {
					event_manager.push_event(EventTypes::BUMP_SCRIPT, e);
				}
				else {
					if (!interactable->triggered) {
						event_manager.push_event(EventTypes::BUMP_SCRIPT, e);
					}
				}
			}
		}
	}
}

GameScreen::GameScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, WorldMap& _world_map, bool _render_prev, unsigned int _id) :
	state_manager(_state_manager), world(_world), tex_manager(_tex_manager), event_manager(_event_manager), world_map(_world_map), render_prev(_render_prev), id(_id)
{
	event_manager.add_subscriber(EventTypes::ASCEND_DUNGEON, *this); // required to allow teleporting back to town.
	event_manager.add_subscriber(EventTypes::DESCEND_DUNGEON, *this);
	event_manager.add_subscriber(EventTypes::TICK, *this);
}

void GameScreen::handle_input(SDL_Event& event)
{
	auto entity = world.GetEntitiesWith<Player>()[0];
	auto* pos = world.GetComponent<Position>(entity);

	SDL_PollEvent(&event);
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
		case SDLK_KP_1: event_manager.push_event(EventTypes::MOVE_SOUTH_WEST, entity); break;
		case SDLK_KP_2: event_manager.push_event(EventTypes::MOVE_SOUTH, entity); break;
		case SDLK_KP_3: event_manager.push_event(EventTypes::MOVE_SOUTH_EAST, entity); break;
		case SDLK_KP_4: event_manager.push_event(EventTypes::MOVE_WEST, entity); break;
		case SDLK_KP_5: break;
		case SDLK_KP_6: event_manager.push_event(EventTypes::MOVE_EAST, entity); break;
		case SDLK_KP_7: event_manager.push_event(EventTypes::MOVE_NORTH_WEST, entity); break;
		case SDLK_KP_8: event_manager.push_event(EventTypes::MOVE_NORTH, entity); break;
		case SDLK_KP_9: event_manager.push_event(EventTypes::MOVE_NORTH_EAST, entity); break;
		case SDLK_SPACE: event_manager.push_event(EventTypes::DESCEND_DUNGEON); break;
		case SDLK_BACKSPACE: event_manager.push_event(EventTypes::ASCEND_DUNGEON); break;
		case SDLK_o: open_doors(); on_tick(); break;
		case SDLK_1: Prefab::create_explosion(world, pos->x, pos->y, tex_manager.LoadTexture("./Resources/exp2_0.png")); break;
		}
	}
	else if (event.type == SDL_QUIT) {
		state_manager.stop_playing();
	}
	SDL_PumpEvents();
}

void GameScreen::on_tick()
{
	auto components = world.GetComponents<Player, Position>();
	auto& [player, pos] = components[0];
	world_map.update_fov(pos->x, pos->y, player->vision);
	auto& renderer = state_manager.get_renderer();
	renderer.DrawMap(world_map.get_level(world_map.get_current_depth()));
}

void GameScreen::update(float dt)
{
	
}

void GameScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const
{
	renderer.DrawScene(fps, world_map);
}

void GameScreen::on_entrance(Renderer& renderer) const
{
	event_manager.push_event(EventTypes::PLAY_SOUND, id);
	renderer.DrawMap(world_map.get_level(world_map.get_current_depth()));
}

void GameScreen::on_bury() const
{

}

void GameScreen::receive(EventTypes event)
{
	switch (event) {
	case EventTypes::ASCEND_DUNGEON: return_to_safe_zone(); break;
	case EventTypes::DESCEND_DUNGEON: descend_dungeon(); break;
	case EventTypes::TICK: on_tick(); break;
	}
}

void GameScreen::receive(EventTypes event, uint32_t actor)
{

}

void GameScreen::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void GameScreen::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}