#include "TradingScreen.hpp"

TradingScreen::TradingScreen(StateManager& _state_manager,  World& _world,  EventManager& _event_manager,  Keyboard& _keyboard)
: state_manager(_state_manager), world(_world), event_manager(_event_manager), keyboard(_keyboard)
{

}

TradingScreen::~TradingScreen()
{

}

void TradingScreen::handle_input(SDL_Event& event) 
{

}

void TradingScreen::on_tick() 
{

}

void TradingScreen::update(float dt) 
{

}

void TradingScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const 
{

}

bool TradingScreen::render_previous() const 
{
	return render_prev;
}

GameState TradingScreen::get_state() const 
{
	return state;
}

void TradingScreen::on_entrance(Renderer& renderer) 
{

}

void TradingScreen::on_bury() const 
{

}

