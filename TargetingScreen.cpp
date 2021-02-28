#include "TargetingScreen.hpp"

TargetingScreen::TargetingScreen(StateManager& _state_manager, World& _world, WorldMap& _world_map, EventManager& _event_manager, Keyboard& _keyboard)
	: state_manager(_state_manager), world(_world), world_map(_world_map), event_manager(_event_manager), keyboard(_keyboard)
{
	event_manager.add_subscriber(EventTypes::SEND_ITEM_TO_TARGETING_STATE, *this);
	event_manager.add_subscriber(EventTypes::SEND_USER_TO_TARGETING_STATE, *this); 
	event_manager.add_subscriber(EventTypes::SEND_AOE_ITEM_TO_TARGETING_STATE, *this);
	event_manager.add_subscriber(EventTypes::LOOK, *this);
}

TargetingScreen::~TargetingScreen()
{

}

void TargetingScreen::handle_input(SDL_Event& event) 
{
	SDL_PollEvent(&event);

	auto components = world.GetEntitiesWith<Player>();
	auto player_id = components[0];

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_KP_1: tx -= 1; ty += 1; get_path(); break;
	case SDLK_KP_2: ty += 1; get_path(); break;
	case SDLK_KP_3: tx += 1; ty += 1; get_path(); break;
	case SDLK_KP_4: tx -= 1; get_path(); break;
	case SDLK_KP_5: break;
	case SDLK_KP_6: tx += 1; get_path(); break;
	case SDLK_KP_7: tx -= 1; ty = -1; get_path(); break;
	case SDLK_KP_8: ty -= 1; get_path(); break;
	case SDLK_KP_9: tx += 1; ty -= 1; get_path(); break;
	case SDLK_ESCAPE: state_manager.pop(1); break;
	case SDLK_RETURN: use_item(); break;
	}
}

void TargetingScreen::on_tick() 
{

}

void TargetingScreen::update(float dt) 
{

}

void TargetingScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const 
{
	if (selected_item == MAX_ENTITIES + 1) {
		renderer.DrawLook(tx, ty);
	}
	else {
		renderer.DrawTargeting(x, y, path, 10);
	}
}

bool TargetingScreen::render_previous() const 
{
	return render_prev;
}

GameState TargetingScreen::get_state() const 
{
	return state;
}

void TargetingScreen::on_entrance(Renderer& renderer) 
{
	x = 0;
	y = 0;
	tx = 0;
	ty = 0;
	path = { std::make_tuple(x, y) };
	selected_item = MAX_ENTITIES + 1;
}

void TargetingScreen::on_bury() const 
{

}

void TargetingScreen::receive(EventTypes event) 
{

}

void TargetingScreen::receive(EventTypes event, uint32_t actor)  
{
	switch (event) {
	case EventTypes::LOOK: set_cursor_pos(actor);  break;
	}
}

void TargetingScreen::receive(EventTypes event, uint32_t actor, uint32_t target)  
{
	switch (event) {
	case EventTypes::SEND_ITEM_TO_TARGETING_STATE: set_item(target); set_cursor_pos(actor); set_user(actor); break;
	case EventTypes::SEND_USER_TO_TARGETING_STATE: set_user(actor); break;
	case EventTypes::SEND_AOE_ITEM_TO_TARGETING_STATE: set_item(target); set_cursor_pos(actor); set_user(actor); set_aoe(target); break;
	}
}

void TargetingScreen::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)  
{

}

void TargetingScreen::set_cursor_pos(uint32_t actor)
{
	auto pos = world.GetComponent<Position>(actor);
	x = pos->x;
	y = pos->y;
	tx = x;
	ty = y;
}

void TargetingScreen::set_aoe(uint32_t item)
{
	// TO DO
}

void TargetingScreen::get_path()
{
	path = Path::bresenham(world_map.get_level(), x, y, tx, ty);
}

uint32_t TargetingScreen::get_target()
{
	auto entities = world_map.get_entity_grid().get(tx, ty);

	for (auto e : entities) {
		if (world.GetComponent<AI>(e) != nullptr) {
			return e;
		}
	}

	return MAX_ENTITIES + 1;
}

void TargetingScreen::use_item()
{
	if (selected_item != MAX_ENTITIES + 1) {
		auto target = get_target();
		if (target == MAX_ENTITIES + 1) {
			event_manager.push_event(EventTypes::INVALID_TARGET);
			return;
		}
		event_manager.push_event(EventTypes::USE_ITEM, user, target, selected_item);
		state_manager.pop(1);
	}
}

