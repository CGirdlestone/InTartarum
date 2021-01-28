#include "Animator.hpp"

Animator::Animator(World& _world, EventManager& _event_manager) : world(_world), event_manager(event_manager)
{
	// register to events

}

void Animator::update(float dt)
{
	auto animation_components = world.GetComponents<Sprite, Animation>();
	for (auto& [sprite, anim] : animation_components) {
		if (anim->dynamic) {
			anim->dt += dt;
		}

		if (anim->dt > anim->lifetime && anim->dynamic) {
			const auto& state = anim->animations.at(anim->_state).front();
			anim->animations.at(anim->_state).pop_front();
			anim->animations.at(anim->_state).push_back(state);
			sprite->id = state.id;
			sprite->clip_x = state.clip_x;
			sprite->clip_y = state.clip_y;
			sprite->width = state.width;
			sprite->height = state.height;
			anim->dt = 0.0f;
		}
	}
}

void Animator::on_tick()
{

}

void Animator::receive(EventTypes event)
{

}

void Animator::receive(EventTypes event, uint32_t actor)
{

}

void Animator::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void Animator::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}
