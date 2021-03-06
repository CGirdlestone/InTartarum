#pragma once

#include "Common.hpp"
#include "Keyboard.hpp"
#include "BaseState.hpp"
#include "BaseSystem.hpp"
#include "StateManager.hpp"
#include "World.hpp"
#include "Utils.hpp"

class QuantityState : public BaseState, BaseSystem
{
public:
	QuantityState(StateManager& _state_manager, World& _world, EventManager& _event_manager, Keyboard& _keyboard);
	~QuantityState() {};

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
	void backspace();

private:
	StateManager& state_manager;
	World& world;
	EventManager& event_manager;
	Keyboard& keyboard;
	bool render_prev{ true };
	GameState state{ GameState::QUANTITY };
	uint32_t selected_item;
	std::string quantity{ "" };
};

