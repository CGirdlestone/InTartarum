#include "InventoryScreen.hpp"

void InventoryScreen::handle_input(SDL_Event& event)
{
	auto components = world.GetComponents<Player, Container>();
	auto& [player, container] = components[0];

	int num_items = static_cast<int>(container->inventory.size());
	
	SDL_PollEvent(&event);

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_ESCAPE: state_manager.pop(1); break;
	case SDLK_UP: option = option - 1 >= 0 ? option - 1 : num_items - 1; break;
	case SDLK_DOWN: option = option + 1 < num_items ? option + 1 : 0; break;
	case SDLK_SPACE: state_manager.push(GameState::ACTIONS); event_manager.push_event(EventTypes::SEND_ITEM_TO_ACTION_STATE, container->inventory[option]); break;
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

	renderer.DrawInventory(container->inventory, equipment_slots, option);
}

bool InventoryScreen::render_previous() const
{
	return render_prev;
}

GameState InventoryScreen::get_state() const
{
	return state;
}

void InventoryScreen::on_entrance(Renderer& renderer) const
{

}

void InventoryScreen::on_bury() const
{

}
