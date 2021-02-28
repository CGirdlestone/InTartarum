#pragma once

#include "Common.hpp"
#include "Keyboard.hpp"
#include "BaseState.hpp"
#include "BaseSystem.hpp"
#include "StateManager.hpp"
#include "World.hpp"
#include "WorldMap.hpp"
#include "Utils.hpp"

class TargetingScreen : public BaseState, BaseSystem
{
public:
	TargetingScreen(StateManager& _state_manager, World& _world, WorldMap& _world_map, EventManager& _event_manager, Keyboard& _keyboard);
	~TargetingScreen();
	
	virtual void handle_input(SDL_Event& event) override;
	virtual void on_tick() override;
	virtual void update(float dt) override;
	virtual void draw_scene(Renderer& renderer, const uint32_t fps, float dt) const override;
	virtual bool render_previous() const override;
	virtual GameState get_state() const override;
	virtual void on_entrance(Renderer& renderer) override;
	virtual void on_bury() const override;
	virtual void receive(EventTypes event) override;
	virtual void receive(EventTypes event, uint32_t actor)  override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target)  override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)  override;
	inline void set_item(uint32_t item) { selected_item = item; };
	inline void set_user(uint32_t _user) { user = _user; };
private:
	StateManager& state_manager;
	World& world;
	WorldMap& world_map;
	EventManager& event_manager;
	Keyboard& keyboard;
	bool render_prev{ true };
	GameState state{ GameState::TARGETING };
	uint32_t user{ MAX_ENTITIES + 1 };
	uint32_t selected_item{ MAX_ENTITIES + 1 };
	int x{ 0 };
	int y{ 0 };
	int tx{ 0 };
	int ty{ 0 };
	int aoe{ 0 };
	std::vector<std::tuple<int, int> > path{};
	void set_cursor_pos(uint32_t actor);
	void set_aoe(uint32_t item);
	void get_path();
	uint32_t get_target();
	void use_item();
};
