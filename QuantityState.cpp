#include "QuantityState.hpp"

QuantityState::QuantityState(StateManager& _state_manager, World& _world, EventManager& _event_manager, Keyboard& _keyboard)
	: state_manager(_state_manager), world(_world), event_manager(_event_manager), keyboard(_keyboard), selected_item(MAX_ENTITIES + 1)
{
	event_manager.add_subscriber(EventTypes::SEND_ITEM_TO_QUANTITY_STATE, *this);
}

void QuantityState::handle_input(SDL_Event& event)
{
	SDL_PollEvent(&event);

	auto components = world.GetEntitiesWith<Player>();
	auto player_id = components[0];

	auto* stack = world.GetComponent<Stackable>(selected_item);

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_ESCAPE: state_manager.pop(1);  break;
	case SDLK_BACKSPACE: backspace();  break;
	case SDLK_0: quantity += "0"; break;
	case SDLK_KP_0: quantity += "0"; break;
	case SDLK_1: quantity += "1"; break;
	case SDLK_KP_1: quantity += "1"; break;
	case SDLK_2: quantity += "2"; break;
	case SDLK_KP_2: quantity += "2"; break;
	case SDLK_3: quantity += "3"; break;
	case SDLK_KP_3: quantity += "3"; break;
	case SDLK_4: quantity += "4"; break;
	case SDLK_KP_4: quantity += "4"; break;
	case SDLK_5: quantity += "5"; break;
	case SDLK_KP_5: quantity += "5"; break;
	case SDLK_6: quantity += "6"; break;
	case SDLK_KP_6: quantity += "6"; break;
	case SDLK_7: quantity += "7"; break;
	case SDLK_KP_7: quantity += "7"; break;
	case SDLK_8: quantity += "8"; break;
	case SDLK_KP_8: quantity += "8"; break;
	case SDLK_9: quantity += "9"; break;
	case SDLK_KP_9: quantity += "9"; break;
	case SDLK_RETURN: {
		if (quantity != "") {
			event_manager.push_event(EventTypes::DROP_ITEM_STACK, player_id, selected_item, static_cast<uint32_t>(std::stoi(quantity)));
			state_manager.pop(3);
			event_manager.push_event(EventTypes::TICK);
			break;
		}
	}
	}
}

void QuantityState::on_tick()
{

}

void QuantityState::update(float dt)
{

}

void QuantityState::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const
{
	renderer.DrawQuantity(quantity);
}

bool QuantityState::render_previous() const
{
	return render_prev;
}

GameState QuantityState::get_state() const
{
	return state;
}

void QuantityState::on_entrance(Renderer& renderer) const
{

}

void QuantityState::on_bury() const
{

}

void QuantityState::receive(EventTypes event)
{

}

void QuantityState::receive(EventTypes event, uint32_t actor)
{
	switch (event) {
	case EventTypes::SEND_ITEM_TO_QUANTITY_STATE: set_item(actor); break;
	}
}

void QuantityState::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void QuantityState::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}

void QuantityState::backspace()
{
	if (quantity != "") {
		quantity = quantity.substr(0, quantity.length() - 1);
	}
}
