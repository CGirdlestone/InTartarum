#include "AISystem.hpp"

AISystem::AISystem(World& _world, EventManager& _event_manager, WorldMap& _world_map)
	: world(_world), event_manager(_event_manager), world_map(_world_map)
{
	event_manager.add_subscriber(EventTypes::TICK, *this);
}

AISystem::~AISystem()
{

}

void AISystem::update(float dt)
{

}

void AISystem::on_tick()
{
	auto mobs = world.GetEntitiesWith<AI, Position>();

	mobs.erase(std::remove_if(mobs.begin(), mobs.end(), [this](const uint32_t e) {return this->world.GetComponent<Position>(e)->z != this->world_map.get_current_depth(); }), mobs.end());

	for (auto mob : mobs) {
		auto* pos = world.GetComponent<Position>(mob);
		auto* ai = world.GetComponent<AI>(mob);

		if (ai->attitude == Attitude::HOSTILE) {
			if (ai->blind) {
				if (world_map.get_scent_map().is_marked(pos->x, pos->y)) {
					track_player(mob);
				}
				else {
					// whatever blind mobs do when not tracking player
				}
			}
			else {
				if (world_map.get_level().get_grid().get_tile(pos->x, pos->y).visible) {
					if (ai->smart) {
						auto* fighter = world.GetComponent<Fighter>(mob);
						if (fighter != nullptr) {
							if (fighter->max_hp >= 5 * fighter->hp) {
								// if the mob has a health potion, use it
								auto* container = world.GetComponent<Container>(mob);
								if (container != nullptr) {
									for (auto _item : container->inventory) {
										auto* item = world.GetComponent<Useable>(_item);
										auto* script = world.GetComponent<Scriptable>(_item);
										if (item != nullptr && script != nullptr) {
											if (item->type == UseableType::CONSUMABLE) {
												event_manager.push_event(EventTypes::ON_USE_SCRIPT, mob, _item);
												break;
											}
										}
									}
								}
							}
							else {
								auto* container = world.GetComponent<Container>(mob);
								if (container != nullptr) {
									bool scroll_used{ false };

									for (auto _item : container->inventory) {
										auto* item = world.GetComponent<Useable>(_item);
										auto* script = world.GetComponent<Scriptable>(_item);
										if (item != nullptr && script != nullptr) {
											if (item->type == UseableType::TARGETED || item->type == UseableType::TARGETED_AOE) {
												uint32_t target = world.GetEntitiesWith<Player>()[0];
												event_manager.push_event(EventTypes::ON_USE_SCRIPT, mob, target, _item);
												scroll_used = true;
												break;
											}
										}
									}
									if (!scroll_used) {
										hunt_player(mob);
									}
								}
								else {
									hunt_player(mob);
								}
							}
						}
					}
					else {
						hunt_player(mob);
					}
				}
				else {
					if (ai->smart) {
						// whatever smart mobs do when they can't see the player
						auto* container = world.GetComponent<Container>(mob);
						auto* body = world.GetComponent<Body>(mob);
						if (body != nullptr && container != nullptr) {
							for (auto item : container->inventory) {
								if (world.GetComponent<Equipable>(item)) {
									event_manager.push_event(EventTypes::EQUIP_ITEM, mob, item);
									break;
								}
							}
						}
					}
					else {
						// whatever stupid mobs do when they can't see the player
					}
				}
			}
		}
		else if (ai->attitude == Attitude::NEUTRAL) {
			// whatever neutral mobs do
		}
		else if (ai->attitude == Attitude::FRIENDLY) {
			// whatever friendly mobs do
		}
	}
}

void AISystem::receive(EventTypes event)
{
	switch (event) {
	case EventTypes::TICK: on_tick(); break;
	}
}

void AISystem::receive(EventTypes event, uint32_t actor)
{

}

void AISystem::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void AISystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}

void AISystem::random_walk(uint32_t mob)
{

}

void AISystem::hunt_player(uint32_t mob)
{
	auto* pos = world.GetComponent<Position>(mob);
	auto* ai = world.GetComponent<AI>(mob);

	auto components = world.GetComponents<Player, Position>();
	auto& [player, player_pos] = components[0];

	if (ai->path.empty() || ai->path.size() < 5) {
		ai->path = Path::a_star(world_map.get_level(), pos->x, pos->y, player_pos->x, player_pos->y);
	}

	auto& [x, y] = ai->path.back();
	ai->path.pop_back();

	auto dx = x - pos->x;
	auto dy = y - pos->y;

	do_move(mob, dx, dy);
}

void AISystem::track_player(uint32_t mob)
{
	auto* pos = world.GetComponent<Position>(mob);

	auto [x, y] = world_map.get_scent_map().track(pos->x, pos->y);

	auto dx = x - pos->x;
	auto dy = y - pos->y;

	do_move(mob, dx, dy);
}

void AISystem::do_move(uint32_t mob, int dx, int dy)
{
	if (dx == 0 && dy == -1) {
		event_manager.push_event(EventTypes::MOVE_NORTH, mob);
	}
	else if (dx == 1 && dy == -1) {
		event_manager.push_event(EventTypes::MOVE_NORTH_EAST, mob);
	}
	else if (dx == 1 && dy == 0) {
		event_manager.push_event(EventTypes::MOVE_EAST, mob);
	}
	else if (dx == 1 && dy == 1) {
		event_manager.push_event(EventTypes::MOVE_SOUTH_EAST, mob);
	}
	else if (dx == 0 && dy == 1) {
		event_manager.push_event(EventTypes::MOVE_SOUTH, mob);
	}
	else if (dx == -1 && dy == 1) {
		event_manager.push_event(EventTypes::MOVE_SOUTH_WEST, mob);
	}
	else if (dx == -1 && dy == 0) {
		event_manager.push_event(EventTypes::MOVE_WEST, mob);
	}
	else if (dx == -1 && dy == -1) {
		event_manager.push_event(EventTypes::MOVE_NORTH_WEST, mob);
	}
}

