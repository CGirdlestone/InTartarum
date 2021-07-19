#pragma once

#include "BaseSystem.hpp"
#include "EventManager.hpp"
#include "World.hpp"
#include "WorldMap.hpp"
#include "Pathfinding.hpp"

class AISystem : public BaseSystem
{
public:
	AISystem(World& _world, EventManager& _event_manager, WorldMap& _world_map);
	~AISystem();
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
	void random_walk(uint32_t mob);
	void hunt_player(uint32_t mob);
	void track_player(uint32_t mob);
	void do_move(uint32_t mob, int dx, int dy);
};
