#pragma once

#include "BaseSystem.hpp"
#include "EventManager.hpp"
#include "World.hpp"
#include "WorldMap.hpp"

class PlayerSystem : public BaseSystem
{
public:
	PlayerSystem(World& _world, EventManager& _event_manager, WorldMap& _world_map);
	~PlayerSystem();
	virtual void update(float dt) override;
	virtual void on_tick() override;
	virtual void receive(EventTypes event) override;
	virtual void receive(EventTypes event, uint32_t actor) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) override;
private:
	World& world;
	EventManager& event_manager;
	WorldMap& world_map;

	void add_xp(uint32_t actor);
};
