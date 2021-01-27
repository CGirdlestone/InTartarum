#pragma once
#include "BaseSystem.hpp"
#include "EventManager.hpp"
#include "World.hpp"
#include "WorldMap.hpp"
#include "Camera.hpp"

class MoveSystem : public BaseSystem
{
private:
	World& world;
	EventManager& event_mananger;
	Camera& camera;
	WorldMap& world_map;
	bool can_move(uint32_t mover, int x, int y);
	void move(std::tuple<int, int> direction, uint32_t actor);
	int dungeon_depth{ 0 };
public:
	MoveSystem(World& _world, EventManager& _event_manager, Camera& _camera, WorldMap& _world_map);
	~MoveSystem() { };
	virtual void update(float dt);
	virtual void on_tick();
	virtual void receive(EventTypes event);
	virtual void receive(EventTypes event, uint32_t actor);
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target);
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item);
};

