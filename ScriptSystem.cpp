#include "ScriptSystem.hpp"

void ScriptSystem::load_bump_scripts()
{
	auto open_script = [](World& world, uint32_t entity) {
		auto* anim = world.GetComponent<Animation>(entity);
		auto* sprite = world.GetComponent<Sprite>(entity);
		auto* interactable = world.GetComponent<Interactable>(entity);
		interactable->triggered = true;

		const auto& state = anim->animations.at(anim->_state).front();
		anim->animations.at(anim->_state).pop_front();
		anim->animations.at(anim->_state).push_back(state);
		sprite->id = state.id;
		sprite->clip_x = state.clip_x;
		sprite->clip_y = state.clip_y;
		sprite->width = state.width;
		sprite->height = state.height;
	};

	bump_scripts.insert({ "OPEN", open_script });

}

void ScriptSystem::do_bump(uint32_t entity)
{
	auto* script = world.GetComponent<Scriptable>(entity);
	bump_scripts.at(script->OnBump)(world, entity);
}

void ScriptSystem::load_update_scripts()
{

}

ScriptSystem::ScriptSystem(World& _world, EventManager& _event_manager, WorldMap& _world_map):
	world(_world), event_manager(_event_manager), world_map(_world_map)
{
	event_manager.add_subscriber(EventTypes::BUMP_SCRIPT, *this);
}

void ScriptSystem::init()
{
	load_bump_scripts();
	load_update_scripts();
}

void ScriptSystem::update(float dt)
{
	auto entities = world.GetEntitiesWith<Scriptable>();
	for (auto e : entities) {
		auto* script = world.GetComponent<Scriptable>(e);
		if (script->OnUpdate != "") {
			update_scripts.at(script->OnUpdate)(world, e);
		}
	}
}

void ScriptSystem::on_tick()
{

}

void ScriptSystem::receive(EventTypes event)
{

}

void ScriptSystem::receive(EventTypes event, uint32_t actor)
{
	switch (event) {
	case EventTypes::BUMP_SCRIPT: do_bump(actor); break;
	}
}

void ScriptSystem::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void ScriptSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}
