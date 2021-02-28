#include "InventoryScreen.hpp"

void InventoryScreen::handle_input(SDL_Event& event)
{
	SDL_PollEvent(&event);

	if (in_equipment_list) {
		handle_equipment_input(event);
	}
	else {
		handle_inventory_input(event);
	}
}

void InventoryScreen::on_tick()
{

}

void InventoryScreen::update(float dt)
{

}

void InventoryScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const
{
	auto components = world.GetComponents<Player, Container>();
	auto& [player, container] = components[0];

	renderer.DrawInventory(container->inventory, equipment_slots, option, in_equipment_list);
}

bool InventoryScreen::render_previous() const
{
	return render_prev;
}

GameState InventoryScreen::get_state() const
{
	return state;
}

void InventoryScreen::on_entrance(Renderer& renderer)
{
	auto components = world.GetComponents<Player, Container>();
	auto& [player, container] = components[0];

	if (container->inventory.empty()) {
		option = -1;
	}
	else {
		option = 0;
	}

	in_equipment_list = false;
}

void InventoryScreen::on_bury() const
{

}

void InventoryScreen::handle_inventory_input(SDL_Event& event)
{
	auto components = world.GetComponents<Player, Container>();
	auto& [player, container] = components[0];

	int num_items = static_cast<int>(container->inventory.size());

	auto key = keyboard.handle_input(event);

	if (static_cast<int>(key) >= 97 && static_cast<int>(key) <= 117){
		if (option != -1) {
			auto _option = static_cast<int>(key) - 97;
			if (_option < num_items) {
				state_manager.push(GameState::ACTIONS);
				event_manager.push_event(EventTypes::SEND_ITEM_TO_ACTION_STATE, container->inventory[_option]);
				return;
			}
		}
	}

	switch (key) {
	case SDLK_ESCAPE: state_manager.pop(1); break;
	case SDLK_UP: option = option - 1 >= 0 ? option - 1 : num_items - 1; break;
	case SDLK_DOWN: option = option + 1 < num_items ? option + 1 : 0; break;
	case SDLK_TAB: in_equipment_list = !in_equipment_list; break;
	case SDLK_SPACE: {
		if (option != -1) {
			state_manager.push(GameState::ACTIONS);
			event_manager.push_event(EventTypes::SEND_ITEM_TO_ACTION_STATE, container->inventory[option]);
			break;
		}
	}
	}
}

void InventoryScreen::handle_equipment_input(SDL_Event& event)
{
	auto components = world.GetComponents<Player, Container, Body>();
	auto& [player, container, body] = components[0];

	auto key = keyboard.handle_input(event);

	if (static_cast<int>(key) >= 97 && static_cast<int>(key) <= 117) {
		auto _option = static_cast<int>(key) - 97;
		if (_option < body->equipment.size()) {
			if (body->equipment[_option] != MAX_ENTITIES + 1) {
				state_manager.push(GameState::ACTIONS);
				event_manager.push_event(EventTypes::SEND_EQUIPPED_ITEM_TO_ACTION_STATE, body->equipment[_option]);
				return;
			}
		}
	}

	switch (key) {
	case SDLK_ESCAPE: state_manager.pop(1); break;
	case SDLK_TAB: in_equipment_list = !in_equipment_list; break;
	}
}
