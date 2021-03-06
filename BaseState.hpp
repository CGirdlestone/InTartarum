#pragma once

#include "Common.hpp"
#include "TextureManager.hpp"
#include "Renderer.hpp"

enum class GameState { 
	SPLASH_MENU, 
	CHARACTER_CREATION, 
	GAME, 
	PAUSED, 
	ACTIONS, 
	INVENTORY, 
	QUANTITY, 
	CHARACTER_SCREEN, 
	GAME_OVER, 
	GAME_WIN, 
	TARGETING, 
	HELP,
};

class BaseState
{
public:
	virtual ~BaseState() = 0 { };
	virtual void handle_input(SDL_Event& event) = 0;
	virtual void on_tick() = 0;
	virtual void update(float dt) = 0;
	virtual void draw_scene(Renderer& renderer, const uint32_t fps, float dt) const = 0;
	virtual bool render_previous() const = 0;
	virtual GameState get_state() const = 0;
	virtual void on_entrance(Renderer& renderer) = 0;
	virtual void on_bury() const = 0;
};


