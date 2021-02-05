
#include "ParticleSystem.hpp"
#include "Components.hpp"


ParticleSystem::ParticleSystem(World& _world, EventManager& _event_manager) : world(_world), event_manager(_event_manager)
{
	// Register to events
}

void ParticleSystem::update(float dt)
{
	auto particles = world.GetEntitiesWith<Particle>();
	for (auto entity : particles) {
		auto* p = world.GetComponent<Particle>(entity);
		if (p == nullptr) {
			continue;
		}
		p->lifetime -= dt;
		if (p->lifetime < 0.0f) {
			world.KillEntity(entity);
		}
	}
}

void ParticleSystem::on_tick()
{

}

void ParticleSystem::receive(EventTypes event)
{

}

void ParticleSystem::receive(EventTypes event, uint32_t actor)
{

}

void ParticleSystem::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void ParticleSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}

