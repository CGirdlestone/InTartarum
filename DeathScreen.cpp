#include "DeathScreen.hpp"

DeathScreen::DeathScreen(StateManager& _state_manager, World& _world, EventManager& _event_manager, Keyboard& _keyboard)
	: state_manager(_state_manager), world(_world), event_manager(_event_manager), keyboard(_keyboard)
{

}

DeathScreen::~DeathScreen()
{

}

void DeathScreen::handle_input(SDL_Event& event) 
{

}

void DeathScreen::on_tick() 
{

}

void DeathScreen::update(float dt) 
{

}

void DeathScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const 
{

}

bool DeathScreen::render_previous() const 
{

}

GameState DeathScreen::get_state() const 
{

}

void DeathScreen::on_entrance(Renderer& renderer) 
{

}

void DeathScreen::on_bury() const 
{

}

