#pragma once

#include "Common.hpp"

class Keyboard
{
public:
	Keyboard();
	~Keyboard() {};
	Keyboard& operator=(const Keyboard&) = delete;

	SDL_Keycode handle_input(SDL_Event& event);
	
	const bool is_pressed(SDL_Keycode key) const { return keys.at(key); };
private:
	void press(SDL_Keycode key);
	void release(SDL_Keycode key);

	std::map<SDL_Keycode, bool> keys;
};

