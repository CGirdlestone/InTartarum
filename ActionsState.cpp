#include "ActionsState.hpp"

ActionsState::ActionsState(StateManager& _state_manager, World& _world, EventManager& _event_manager, Keyboard& _keyboard) 
	: state_manager(_state_manager), world(_world), event_manager(_event_manager), keyboard(_keyboard), selected_item(MAX_ENTITIES + 1) 
{
	event_manager.add_subscriber(EventTypes::SEND_ITEM_TO_ACTION_STATE, *this);
	event_manager.add_subscriber(EventTypes::SEND_EQUIPPED_ITEM_TO_ACTION_STATE, *this);
}

void ActionsState::handle_input(SDL_Event& event)
{
	SDL_PollEvent(&event);

	if (in_equipment_list) {
		handle_equipment_input(event);
	}
	else {
		handle_inventory_input(event);
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
	renderer.DrawActions(in_equipment_list);
}

bool ActionsState::render_previous() const
{
	return render_prev;
}

GameState ActionsState::get_state() const
{
	return state;
}

void ActionsState::on_entrance(Renderer& renderer)
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
	case EventTypes::SEND_ITEM_TO_ACTION_STATE: set_item(actor); in_equipment_list = false; break;
	case EventTypes::SEND_EQUIPPED_ITEM_TO_ACTION_STATE: set_item(actor); in_equipment_list = true; break;
	}
}

void ActionsState::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void ActionsState::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}

void ActionsState::handle_inventory_input(SDL_Event& event)
{
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
	case SDLK_e: {
		event_manager.push_event(EventTypes::EQUIP_ITEM, player_id, selected_item);
		state_manager.pop(2);
		break;
	}
	case SDLK_u: {
		auto* useable = world.GetComponent<Useable>(selected_item);
		auto* script = world.GetComponent<Scriptable>(selected_item);
		if (useable == nullptr || script == nullptr) {
			event_manager.push_event(EventTypes::NO_USE);
			break;
		}
		if (script->OnUse == "") {
			break;
		}
		if (useable->type == UseableType::TARGETED) {
			state_manager.pop(2);
			state_manager.push(GameState::TARGETING);
			event_manager.push_event(EventTypes::SEND_ITEM_TO_TARGETING_STATE, player_id, selected_item);
			break;
		}
		else if (useable->type == UseableType::TARGETED_AOE) {
			state_manager.pop(2);
			state_manager.push(GameState::TARGETING);
			event_manager.push_event(EventTypes::SEND_AOE_ITEM_TO_TARGETING_STATE, player_id, selected_item);
			break;
		} else if (useable->type == UseableType::CONSUMABLE) {
			event_manager.push_event(EventTypes::CONSUME, player_id, selected_item);
			state_manager.pop(2);
			set_item(MAX_ENTITIES + 1);
			event_manager.push_event(EventTypes::TICK);
			break;
		}
		break;
	}
	case SDLK_q: {
		auto* useable = world.GetComponent<Useable>(selected_item);
		auto* script = world.GetComponent<Scriptable>(selected_item);
		if (useable == nullptr || script == nullptr) {
			event_manager.push_event(EventTypes::NO_USE);
			break;
		}
		if (useable->type == UseableType::CONSUMABLE) {
			event_manager.push_event(EventTypes::CONSUME, player_id, selected_item);
			state_manager.pop(2);
			set_item(MAX_ENTITIES + 1);
			event_manager.push_event(EventTypes::TICK);
			break;
		}
		break;
	}
	}
}

void ActionsState::handle_equipment_input(SDL_Event& event)
{
	auto components = world.GetEntitiesWith<Player>();
	auto player_id = components[0];

	auto* stack = world.GetComponent<Stackable>(selected_item);

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_ESCAPE: state_manager.pop(1);  break;
	case SDLK_d: {
		event_manager.push_event(EventTypes::DROP_EQUIPPED_ITEM, player_id, selected_item);
		state_manager.pop(2);
		set_item(MAX_ENTITIES + 1);
		event_manager.push_event(EventTypes::TICK);
		break;
	}
	case SDLK_e: {
		event_manager.push_event(EventTypes::UNEQUIP_ITEM, player_id, selected_item);
		state_manager.pop(2);
		set_item(MAX_ENTITIES + 1);
		event_manager.push_event(EventTypes::TICK);
		break;
	}
	}
}
