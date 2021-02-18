#include "ActionsState.hpp"

ActionsState::ActionsState(StateManager& _state_manager, World& _world, EventManager& _event_manager, Keyboard& _keyboard) 
	: state_manager(_state_manager), world(_world), event_manager(_event_manager), keyboard(_keyboard), selected_item(MAX_ENTITIES + 1) 
{
	event_manager.add_subscriber(EventTypes::SEND_ITEM_TO_ACTION_STATE, *this);
}

void ActionsState::handle_input(SDL_Event& event)
{
	SDL_PollEvent(&event);

	auto components = world.GetEntitiesWith<Player>();
	auto player_id = components[0];

	auto* stack = world.GetComponent<Stackable>(selected_item);

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_ESCAPE: state_manager.pop(1);  break;
	case SDLK_d: {
		if (stack == nullptr) {
			event_manager.push_event(EventTypes::DROP_ITEM, player_id, selected_item); 
			state_manager.pop(2); 
			set_item(MAX_ENTITIES + 1); 
			event_manager.push_event(EventTypes::TICK);
			break;
		}
		else {
			state_manager.push(GameState::QUANTITY);
			event_manager.push_event(EventTypes::SEND_ITEM_TO_QUANTITY_STATE, selected_item); break;
		}

	}
	}
}

void ActionsState::on_tick()
{

}

void ActionsState::update(float dt)
{

}

void ActionsState::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const
{
	renderer.DrawActions();
}

bool ActionsState::render_previous() const
{
	return render_prev;
}

GameState ActionsState::get_state() const
{
	return state;
}

void ActionsState::on_entrance(Renderer& renderer) const
{

}

void ActionsState::on_bury() const
{

}

void ActionsState::receive(EventTypes event)
{

}

void ActionsState::receive(EventTypes event, uint32_t actor)
{
	switch (event) {
	case EventTypes::SEND_ITEM_TO_ACTION_STATE: set_item(actor); break;
	}
}

void ActionsState::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void ActionsState::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}
