#include "CombatSystem.hpp"

CombatSystem::CombatSystem(World& _world, EventManager& _event_manager, WorldMap& _world_map)
	:world(_world), event_manager(_event_manager), world_map(_world_map)
{

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

}

void CombatSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) 
{

}

