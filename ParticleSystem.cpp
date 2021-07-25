
#include "ParticleSystem.hpp"
#include "Components.hpp"


ParticleSystem::ParticleSystem(World& _world, EventManager& _event_manager) : world(_world), event_manager(_event_manager)
{
	// Register to events
}

void ParticleSystem::update(float dt)
{
	auto particles = world.GetEntitiesWith<Particle, Position>();
	for (auto entity : particles) {
		auto* p = world.GetComponent<Particle>(entity);
		auto* pos = world.GetComponent<Position>(entity);
		if (p == nullptr || pos == nullptr) {
			continue;
		}

		if (p->lifetime < 0.0f) {
			if ((int)p->x == p->target_x && (int)p->y == p->target_y) {
				auto* script = world.GetComponent<Scriptable>(entity);
				if (script != nullptr) {
					if (script->OnDeath != "") {
						event_manager.push_event(EventTypes::ON_DEATH_SCRIPT, entity);
					}
				}
				world.KillEntity(entity);
			}
			else {
				p->x += p->velocity * (static_cast<float>(p->target_x) + 0.5f - p->x) * dt;
				p->y += p->velocity * (static_cast<float>(p->target_y) + 0.5f - p->y) * dt;
				pos->x = static_cast<int>(p->x);
				pos->y = static_cast<int>(p->y);
			}
		}
		else {
			p->lifetime -= dt;
			if (p->lifetime < 0.0f) {
				world.KillEntity(entity);
			}
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

