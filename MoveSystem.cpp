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

bool MoveSystem::is_move_within_level(int x, int y)
{
	auto& level = world_map.get_level();
	auto& grid = level.get_grid();

	return grid.in_bounds(x, y);
}

void MoveSystem::move_over_world(uint32_t mover, int x, int y)
{
	auto* pos = world.GetComponent<Position>(mover);

	// as we're moving in units (either tiles or levels), we can use the delta move to transition levels and ensure we get the right direction.
	if (pos->x + x >= 0 && pos->x + x < world_map.get_level().get_width() &&
		(pos->y + y < 0 || pos->y + y >= world_map.get_level().get_height())) {
		pos->world_y += y;
	}
	else if (pos->y + y >= 0 && pos->y + y < world_map.get_level().get_height() &&
		(pos->x + x < 0 || pos->x + x >= world_map.get_level().get_width())) {
		pos->world_x += x;
	}
	else {
		pos->world_x += x;
		pos->world_y += y;
	}

	// update the player's local (level) coordinates.
	if (pos->x + x < 0) {
		pos->x = world_map.get_level().get_width() - 1;
	}
	else if (pos->x + x > world_map.get_level().get_width() - 1) {
		pos->x = 0;
	}

	if (pos->y + y < 0) {
		pos->y = world_map.get_level().get_height() - 1;
	}
	else if (pos->y + y > world_map.get_level().get_height() - 1) {
		pos->y = 0;
	}
}

bool MoveSystem::in_world_bounds(int x, int y)
{
	return (x >= 0 && x < world_map.get_world_width() && y >= 0 && y < world_map.get_world_height());
}

bool MoveSystem::can_move(uint32_t mover, int x, int y)
{
	auto& level = world_map.get_level();
	auto& grid = level.get_grid();
	auto& tile = level.get_grid().get_tile(x, y);

	if (!grid.in_bounds(x, y)) {
		return false;
	}
	
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
			event_manager.push_event(EventTypes::MELEE_HIT);
			camera.follow(pos->x, pos->y);
			event_manager.push_event(EventTypes::TICK);
		}
	}
	else {
		auto player = world.GetComponent<Player>(actor);
		
		// only the player can transition between overworld levels
		if (player != nullptr) {
			if (!is_move_within_level(pos->x + x, pos->y + y)) {
				if (in_world_bounds(pos->world_x + x, pos->world_y + y)) {
					
					// update the player's global (overworld) coordinates.
					move_over_world(actor, x, y);
					event_manager.push_event(EventTypes::OVERWORLD_MOVEMENT);
					camera.follow(pos->x, pos->y);
					return;
				}
			}
			event_manager.push_event(EventTypes::BLOCKED_MOVEMENT);
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
