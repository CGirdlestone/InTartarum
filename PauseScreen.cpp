#include "PauseScreen.hpp"

PauseScreen::PauseScreen(StateManager& _state_manager,  World& _world,  EventManager& _event_manager,  Keyboard& _keyboard)
: state_manager(_state_manager), world(_world), event_manager(_event_manager), keyboard(_keyboard)
{

}

PauseScreen::~PauseScreen()
{

}

void PauseScreen::handle_input(SDL_Event& event) 
{

}

void PauseScreen::on_tick() 
{

}

void PauseScreen::update(float dt) 
{

}

void PauseScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const 
{

}

bool PauseScreen::render_previous() const 
{
	return render_prev;
}

GameState PauseScreen::get_state() const 
{
	return state;
}

void PauseScreen::on_entrance(Renderer& renderer) 
{

}

void PauseScreen::on_bury() const 
{

}

