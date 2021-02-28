#include "HelpScreen.hpp"

HelpScreen::HelpScreen(StateManager& _state_manager,  World& _world,  EventManager& _event_manager,  Keyboard& _keyboard)
: state_manager(_state_manager), world(_world), event_manager(_event_manager), keyboard(_keyboard)
{

}

HelpScreen::~HelpScreen()
{

}

void HelpScreen::handle_input(SDL_Event& event) 
{
	SDL_PollEvent(&event);

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_ESCAPE: state_manager.pop(1); break;
	}
}

void HelpScreen::on_tick() 
{

}

void HelpScreen::update(float dt) 
{

}

void HelpScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const 
{
	renderer.DrawHelp();
}

bool HelpScreen::render_previous() const 
{
	return render_prev;
}

GameState HelpScreen::get_state() const 
{
	return state;
}

void HelpScreen::on_entrance(Renderer& renderer) 
{

}

void HelpScreen::on_bury() const 
{

}

