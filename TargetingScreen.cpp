#include "TargetingScreen.hpp"

TargetingScreen::TargetingScreen(StateManager& _state_manager, World& _world, EventManager& _event_manager, Keyboard& _keyboard)
	: state_manager(_state_manager), world(_world), event_manager(_event_manager), keyboard(_keyboard)
{

}

TargetingScreen::~TargetingScreen()
{

}

void TargetingScreen::handle_input(SDL_Event& event) 
{

}

void TargetingScreen::on_tick() 
{

}

void TargetingScreen::update(float dt) 
{

}

void TargetingScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const 
{

}

bool TargetingScreen::render_previous() const 
{

}

GameState TargetingScreen::get_state() const 
{

}

void TargetingScreen::on_entrance(Renderer& renderer) 
{

}

void TargetingScreen::on_bury() const 
{

}

void TargetingScreen::receive(EventTypes event) 
{

}

void TargetingScreen::receive(EventTypes event, uint32_t actor)  
{

}

void TargetingScreen::receive(EventTypes event, uint32_t actor, uint32_t target)  
{

}

void TargetingScreen::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)  
{

}

