#include "InventoryScreen.hpp"

void InventoryScreen::handle_input(SDL_Event& event)
{
	SDL_PollEvent(&event);

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_ESCAPE: state_manager.pop(1); break;
	case SDLK_UP: option--; break;
	case SDLK_DOWN: option++; break;
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

	renderer.DrawInventory(container->inventory, option);
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
