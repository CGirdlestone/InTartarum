#pragma once

#include "EventManager.hpp"

class ParticleSystem : public BaseSystem
{
private:
	World& world;
	EventManager& event_manager;
public:
	ParticleSystem(World& _world, EventManager& _event_manager);
	~ParticleSystem() { };

	void update(float dt) override;
	void on_tick() override;
	void receive(EventTypes event) override;
	void receive(EventTypes event, uint32_t actor) override;
	void receive(EventTypes event, uint32_t actor, uint32_t target) override;
	void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) override;
};