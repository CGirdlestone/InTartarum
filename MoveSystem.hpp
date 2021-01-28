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
	EventManager& event_manager;
	Camera& camera;
	WorldMap& world_map;
	bool can_move(uint32_t mover, int x, int y);
	void move(std::tuple<int, int> direction, uint32_t actor);
	void do_bump_script(uint32_t entity);
public:
	MoveSystem(World& _world, EventManager& _event_manager, Camera& _camera, WorldMap& _world_map);
	~MoveSystem() { };
	virtual void update(float dt) override;
	virtual void on_tick() override;
	virtual void receive(EventTypes event) override;
	virtual void receive(EventTypes event, uint32_t actor) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) override;
};

