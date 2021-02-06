#include "Keyboard.hpp"

Keyboard::Keyboard()
{
	keys.insert({ SDLK_ESCAPE, false });
	keys.insert({ SDLK_UP, false });
	keys.insert({ SDLK_DOWN, false });
	keys.insert({ SDLK_LEFT, false });
	keys.insert({ SDLK_RIGHT, false });
	keys.insert({ SDLK_1, false });
	keys.insert({ SDLK_2, false });
	keys.insert({ SDLK_3, false });
	keys.insert({ SDLK_4, false });
	keys.insert({ SDLK_5, false });
	keys.insert({ SDLK_6, false });
	keys.insert({ SDLK_7, false });
	keys.insert({ SDLK_8, false });
	keys.insert({ SDLK_9, false });
	keys.insert({ SDLK_0, false });
	keys.insert({ SDLK_a, false });
	keys.insert({ SDLK_b, false });
	keys.insert({ SDLK_c, false });
	keys.insert({ SDLK_d, false });
	keys.insert({ SDLK_e, false });
	keys.insert({ SDLK_f, false });
	keys.insert({ SDLK_g, false });
	keys.insert({ SDLK_h, false });
	keys.insert({ SDLK_i, false });
	keys.insert({ SDLK_j, false });
	keys.insert({ SDLK_k, false });
	keys.insert({ SDLK_l, false });
	keys.insert({ SDLK_m, false });
	keys.insert({ SDLK_n, false });
	keys.insert({ SDLK_o, false });
	keys.insert({ SDLK_p, false });
	keys.insert({ SDLK_q, false });
	keys.insert({ SDLK_r, false });
	keys.insert({ SDLK_s, false });
	keys.insert({ SDLK_t, false });
	keys.insert({ SDLK_u, false });
	keys.insert({ SDLK_v, false });
	keys.insert({ SDLK_w, false });
	keys.insert({ SDLK_x, false });
	keys.insert({ SDLK_y, false });
	keys.insert({ SDLK_z, false });
	keys.insert({ SDLK_BACKSPACE, false });
	keys.insert({ SDLK_COMMA, false });
	keys.insert({ SDLK_F1, false });
	keys.insert({ SDLK_F2, false });
	keys.insert({ SDLK_F3, false });
	keys.insert({ SDLK_F4, false });
	keys.insert({ SDLK_F5, false });
	keys.insert({ SDLK_F6, false });
	keys.insert({ SDLK_F7, false });
	keys.insert({ SDLK_F8, false });
	keys.insert({ SDLK_F9, false });
	keys.insert({ SDLK_F10, false });
	keys.insert({ SDLK_KP_0, false });
	keys.insert({ SDLK_KP_1, false });
	keys.insert({ SDLK_KP_2, false });
	keys.insert({ SDLK_KP_3, false });
	keys.insert({ SDLK_KP_4, false });
	keys.insert({ SDLK_KP_5, false });
	keys.insert({ SDLK_KP_6, false });
	keys.insert({ SDLK_KP_7, false });
	keys.insert({ SDLK_KP_8, false });
	keys.insert({ SDLK_KP_9, false });
	keys.insert({ SDLK_KP_PERIOD, false });
	keys.insert({ SDLK_KP_PLUS, false });
	keys.insert({ SDLK_KP_MINUS, false });
	keys.insert({ SDLK_LSHIFT, false });
	keys.insert({ SDLK_LCTRL, false });
	keys.insert({ SDLK_LALT, false });
	keys.insert({ SDLK_TAB, false });
	keys.insert({ SDLK_SPACE, false });
	keys.insert({ SDLK_RETURN, false });
	keys.insert({ SDLK_KP_ENTER, false });
	keys.insert({ SDLK_LEFTBRACKET, false });
	keys.insert({ SDLK_RIGHTBRACKET, false });
	keys.insert({ SDLK_PERIOD, false });
	keys.insert({ SDLK_MINUS, false });
	keys.insert({ SDLK_EQUALS, false });
	keys.insert({ SDLK_SEMICOLON, false });
	keys.insert({ SDLK_SLASH, false });
	keys.insert({ SDLK_QUOTE, false });
}

SDL_Keycode Keyboard::handle_input(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN){
		SDL_Keycode key = event.key.keysym.sym;
		if (!keys.at(key)) {
			press(key);
			return key;
		}
	}
	else if (event.type == SDL_KEYUP) {
		SDL_Keycode key = event.key.keysym.sym;
		if (keys.at(key)) {
			release(key);
		}
	}

	return SDLK_UNKNOWN;
}

void Keyboard::press(SDL_Keycode key)
{
	keys.at(key) = true;
}

void Keyboard::release(SDL_Keycode key)
{
	keys.at(key) = false;
}
