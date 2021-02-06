#pragma once

#include "StateManager.hpp"
#include "Keyboard.hpp"
#include "EventManager.hpp"

class SplashScreen : public BaseState
{
private:
	StateManager& state_manager;
	World& world;
	TextureManager& tex_manager;
	EventManager& event_manager;
	Keyboard& keyboard;
	bool render_prev{ false };
	unsigned int splash_id;
	unsigned int music_id;
	GameState state{ GameState::SPLASH_MENU };
public:
	SplashScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, Keyboard& _keyboard, bool _render_prev, unsigned int id, unsigned int _music_id);
	virtual void handle_input(SDL_Event& event) override;
	virtual void on_tick() override;
	virtual void update(float dt) override;
	virtual void draw_scene(Renderer& renderer, const uint32_t fps, float dt) const override;
	inline virtual bool render_previous() const override { return render_prev; };
	inline virtual GameState get_state() const override { return state; };
	virtual void on_entrance(Renderer& renderer) const;
	virtual void on_bury() const;
};

