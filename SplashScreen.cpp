#include "SplashScreen.hpp"

SplashScreen::SplashScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, bool _render_prev, unsigned int id, unsigned int _music_id):
	state_manager(_state_manager), world(_world), tex_manager(_tex_manager), event_manager(_event_manager), render_prev(_render_prev), splash_id(id), music_id(_music_id)
{
	
}

void SplashScreen::handle_input(SDL_Event& event)
{
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT) {
		state_manager.stop_playing();
	}
	else if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_RETURN: {
			state_manager.push(GameState::CHARACTER_CREATION);  
			event_manager.push_event(EventTypes::BUTTON_CLICK);
			break;
		}
		case SDLK_KP_ENTER: {
			state_manager.push(GameState::GAME);
			event_manager.push_event(EventTypes::LOAD_GAME);
		}
		}
	}
}

void SplashScreen::on_tick()
{

}

void SplashScreen::update(float dt)
{

}

void SplashScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const
{
	renderer.DrawSplash(splash_id, fps, dt);
}

void SplashScreen::on_entrance(Renderer& renderer) const
{
	event_manager.push_event(EventTypes::FADE_IN_SOUND, music_id);
}

void SplashScreen::on_bury() const
{
	event_manager.push_event(EventTypes::FADE_OUT_SOUND);
}
