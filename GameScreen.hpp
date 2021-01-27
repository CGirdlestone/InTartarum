#pragma once

#include "StateManager.hpp"
#include "EventManager.hpp"
#include "WorldMap.hpp"

class GameScreen : public BaseState
{
private:
	StateManager& state_manager;
	World& world;
	TextureManager& tex_manager;
	EventManager& event_manager;
	WorldMap& world_map;
	bool render_prev{ false };
	unsigned int id;
	GameState state{ GameState::GAME };
	int dungeon_depth{ 0 };

	void descend_dungeon();
	void return_to_safe_zone();

public:
	GameScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, WorldMap& _world_map, bool _render_prev, unsigned int _id);
	virtual void handle_input(SDL_Event& event) override;
	virtual void on_tick() override;
	virtual void update(float dt) override;
	virtual void draw_scene(Renderer& renderer, const uint32_t fps) const override;
	inline virtual bool render_previous() const override { return render_prev; };
	inline virtual GameState get_state() const override { return state; };
	virtual void on_entrance(Renderer& renderer) const;
	virtual void on_bury() const;
	inline const int get_depth() const { return dungeon_depth; };
};



