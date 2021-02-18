#include "SplashScreen.hpp"

SplashScreen::SplashScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, Keyboard& _keyboard, bool _render_prev, unsigned int id, unsigned int _music_id):
	state_manager(_state_manager), world(_world), tex_manager(_tex_manager), event_manager(_event_manager), keyboard(_keyboard), render_prev(_render_prev), splash_id(id), music_id(_music_id)
{
	std::ifstream file("save.txt");
	if (file.good()) {
		save_game = true;
	}
}

void SplashScreen::handle_input(SDL_Event& event)
{
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT) {
		state_manager.stop_playing();
	}

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_UP: {
		option = option - 1 < 0 ? 0 : option - 1;
		break;
	}
	case SDLK_DOWN: {
		option = option + 1 > static_cast<int>(save_game) + 1 ? static_cast<int>(save_game) + 1 : option + 1;
		break;
	}
	case SDLK_RETURN: {
		switch (option) {
		case 0: {
			if (save_game) {
				event_manager.push_event(EventTypes::LOAD_GAME);
				state_manager.push(GameState::GAME);
			}
			else {
				state_manager.push(GameState::CHARACTER_CREATION);
			}
			break;
		}
		case 1: {
			if (save_game) {
				state_manager.push(GameState::CHARACTER_CREATION);
				break;
			}
			else {
				state_manager.stop_playing();
				break;
			}
		}
		case 2: {
			state_manager.stop_playing();
			return;
		}
			  event_manager.push_event(EventTypes::BUTTON_CLICK);
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
	renderer.DrawSplash(splash_id, fps, dt, option, save_game);
}

void SplashScreen::on_entrance(Renderer& renderer)
{
	event_manager.push_event(EventTypes::FADE_IN_SOUND, music_id);
}

void SplashScreen::on_bury() const
{
	event_manager.push_event(EventTypes::FADE_OUT_SOUND);
}
