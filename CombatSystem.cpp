#include "CombatSystem.hpp"

CombatSystem::CombatSystem(World& _world, EventManager& _event_manager, WorldMap& _world_map)
	:world(_world), event_manager(_event_manager), world_map(_world_map)
{
	event_manager.add_subscriber(EventTypes::BUMP_ATTACK, *this);
}

CombatSystem::~CombatSystem()
{

}

void CombatSystem::update(float dt) 
{

}

void CombatSystem::on_tick() 
{

}

void CombatSystem::receive(EventTypes event) 
{

}

void CombatSystem::receive(EventTypes event, uint32_t actor) 
{

}

void CombatSystem::receive(EventTypes event, uint32_t actor, uint32_t target) 
{
	switch (event) {
	case EventTypes::BUMP_ATTACK: try_hit(actor, target); 
	}
}

void CombatSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) 
{

}

void CombatSystem::try_hit(uint32_t actor, uint32_t target)
{
	auto* fighter = world.GetComponent<Fighter>(target);

	if (fighter != nullptr) {
		int dmg = 10;
		fighter->hp -= dmg;
		event_manager.push_event(EventTypes::DEAL_DAMAGE, actor, target, static_cast<uint32_t>(dmg));
	}
	else {
		event_manager.push_event(EventTypes::NO_USE);
	}
}

