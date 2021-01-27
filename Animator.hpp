#pragma once

#include "Components.hpp"
#include "EventManager.hpp"

class Animator : public BaseSystem
{
private:
	World& world;
	EventManager& event_manager;
public:
	Animator(World& _world, EventManager& _event_manager);
	~Animator() { };

	void update(float dt) override;
	void on_tick() override;
	void receive(EventTypes event) override;
	void receive(EventTypes event, uint32_t actor) override;
	void receive(EventTypes event, uint32_t actor, uint32_t target) override;
	void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) override;
};

