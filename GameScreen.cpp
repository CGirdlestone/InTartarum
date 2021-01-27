#include "GameScreen.hpp"

void GameScreen::descend_dungeon()
{
	dungeon_depth++;
	world_map.create_dungeon();
	world_map.populate_entity_grid();

	auto& renderer = state_manager.get_renderer();
	renderer.DrawMap(world_map.get_level(dungeon_depth));

	event_manager.push_event(EventTypes::DESCEND_DUNGEON);

	auto components = world.GetComponents<Position, Player>();
	auto& [pos, player] = components[0];
	pos->z++;
}

void GameScreen::return_to_safe_zone()
{
	dungeon_depth = 0;
	world_map.set_depth(dungeon_depth);
	auto& renderer = state_manager.get_renderer();
	renderer.DrawMap(world_map.get_level(dungeon_depth));
	event_manager.push_event(EventTypes::ASCEND_DUNGEON);
	
	auto components = world.GetComponents<Position, Player>();
	auto& [pos, player] = components[0];
	pos->z = 0;
}

GameScreen::GameScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, WorldMap& _world_map, bool _render_prev, unsigned int _id) :
	state_manager(_state_manager), world(_world), tex_manager(_tex_manager), event_manager(_event_manager), world_map(_world_map), render_prev(_render_prev), id(_id)
{

}

void GameScreen::handle_input(SDL_Event& event)
{
	auto entity = world.GetEntitiesWith<Player>()[0];

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
		case SDLK_SPACE: descend_dungeon(); break;
		case SDLK_BACKSPACE: return_to_safe_zone(); break;
		}
	}
	else if (event.type == SDL_QUIT) {
		state_manager.stop_playing();
	}
	SDL_PumpEvents();
}

void GameScreen::on_tick()
{

}

void GameScreen::update(float dt)
{

}

void GameScreen::draw_scene(Renderer& renderer, const uint32_t fps) const
{
	renderer.DrawScene(fps, world_map);
}

void GameScreen::on_entrance(Renderer& renderer) const
{
	event_manager.push_event(EventTypes::PLAY_SOUND, id);
	renderer.DrawMap(world_map.get_level(dungeon_depth));
}

void GameScreen::on_bury() const
{

}
