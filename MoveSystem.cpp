#include "MoveSystem.hpp"

MoveSystem::MoveSystem(World& _world, EventManager& _event_manager, Camera& _camera, WorldMap& _world_map)
	: world(_world), camera(_camera), event_manager(_event_manager), world_map(_world_map)
{
	event_manager.add_subscriber(EventTypes::MOVE_NORTH, *this);
	event_manager.add_subscriber(EventTypes::MOVE_NORTH_WEST, *this);
	event_manager.add_subscriber(EventTypes::MOVE_NORTH_EAST, *this);
	event_manager.add_subscriber(EventTypes::MOVE_SOUTH, *this);
	event_manager.add_subscriber(EventTypes::MOVE_SOUTH_WEST, *this);
	event_manager.add_subscriber(EventTypes::MOVE_SOUTH_EAST, *this);
	event_manager.add_subscriber(EventTypes::MOVE_EAST, *this);
	event_manager.add_subscriber(EventTypes::MOVE_WEST, *this);
	event_manager.add_subscriber(EventTypes::ASCEND_DUNGEON, *this);
	event_manager.add_subscriber(EventTypes::DESCEND_DUNGEON, *this);
}

void MoveSystem::do_bump_script(uint32_t entity) 
{
	auto* interactable = world.GetComponent<Interactable>(entity);
	if (interactable != nullptr) {
		auto* script = world.GetComponent<Scriptable>(entity);
		if (script->OnBump != "") {
			if (interactable->repeatable) {
				event_manager.push_event(EventTypes::BUMP_SCRIPT, entity);
			}
			else {
				if (!interactable->triggered) {
					event_manager.push_event(EventTypes::BUMP_SCRIPT, entity);
				}
			}
		}
	}
}

bool MoveSystem::can_move(uint32_t mover, int x, int y)
{
	auto& level = world_map.get_level();
	auto& tile = level.get_grid().get_tile(x, y);
	
	if (!tile.walkable) {
		return false;
	}

	auto& entities = world_map.get_entity_grid().get(x, y);

	if (entities.empty()) {
		return true;
	}

	for (auto entity : entities) {
		auto* blocker = world.GetComponent<Blocker>(entity);
		if (blocker == nullptr) {
			//do_bump_script(entity);
			continue;
		}

		auto* actor = world.GetComponent<Actor>(entity);
		if (actor != nullptr) {
			//event_manager.push_event(EventTypes::BUMP_ATTACK, mover, entity); // don't have a combat system to handle this yet...
			return false;
		}

		//do_bump_script(entity);
		return false;
	}

	return true;
}

void MoveSystem::move(std::tuple<int, int> direction, uint32_t actor)
{
	auto& [x, y] = direction;
	auto* pos = world.GetComponent<Position>(actor);

	if (can_move(actor, pos->x + x, pos->y + y)) {
		world_map.get_entity_grid().remove_entity(actor, pos->x, pos->y);
		pos->x += x;
		pos->y += y;
		world_map.get_entity_grid().add_entity(actor, pos->x, pos->y);
		auto player = world.GetComponent<Player>(actor);
		if (player != nullptr) {
			camera.follow(pos->x, pos->y);
			event_manager.push_event(EventTypes::TICK);
		}
	}

}

void MoveSystem::update(float dt)
{

}

void MoveSystem::on_tick()
{

}

void MoveSystem::receive(EventTypes event)
{

}

void MoveSystem::receive(EventTypes event, uint32_t actor)
{
	switch (event) {
	case EventTypes::MOVE_NORTH: move(std::make_tuple(0, -1), actor); break;
	case EventTypes::MOVE_NORTH_WEST: move(std::make_tuple(-1, -1), actor); break;
	case EventTypes::MOVE_WEST: move(std::make_tuple(-1, 0), actor); break;
	case EventTypes::MOVE_SOUTH_WEST: move(std::make_tuple(-1, 1), actor); break;
	case EventTypes::MOVE_SOUTH: move(std::make_tuple(0, 1), actor); break;
	case EventTypes::MOVE_SOUTH_EAST: move(std::make_tuple(1, 1), actor); break;
	case EventTypes::MOVE_EAST: move(std::make_tuple(1, 0), actor); break;
	case EventTypes::MOVE_NORTH_EAST: move(std::make_tuple(1, -1), actor); break;
	}
}

void MoveSystem::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void MoveSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}
