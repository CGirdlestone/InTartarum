#include "ScriptSystem.hpp"

void ScriptSystem::load_bump_scripts()
{
	auto open_chest_script = [](World& world, EventManager& event_manager, SoundManager& sound_manager, uint32_t entity) {
		auto* anim = world.GetComponent<Animation>(entity);
		auto* sprite = world.GetComponent<Sprite>(entity);
		auto* interactable = world.GetComponent<Interactable>(entity);
		if (!interactable->triggered){
			event_manager.push_event(EventTypes::PLAY_CHUNK, sound_manager.LoadChunk("./Resources/Sounds/SFX/doorOpen_2.ogg"));
			interactable->triggered = true;
		}
		else {
			event_manager.push_event(EventTypes::PLAY_CHUNK, sound_manager.LoadChunk("./Resources/Sounds/SFX/doorClose_2.ogg"));
			interactable->triggered = false;
		}
		
		const auto& state = anim->animations.at(anim->_state).front();
		anim->animations.at(anim->_state).pop_front();
		anim->animations.at(anim->_state).push_back(state);
		sprite->id = state.id;
		sprite->clip_x = state.clip_x;
		sprite->clip_y = state.clip_y;
		sprite->width = state.width;
		sprite->height = state.height;

		
	};

	auto open_door_script = [](World& world, EventManager& event_manager, SoundManager& sound_manager, uint32_t entity) {
		auto* anim = world.GetComponent<Animation>(entity);
		auto* sprite = world.GetComponent<Sprite>(entity);
		auto* interactable = world.GetComponent<Interactable>(entity);
		if (!interactable->triggered) {
			event_manager.push_event(EventTypes::PLAY_CHUNK, sound_manager.LoadChunk("./Resources/Sounds/SFX/doorOpen_2.ogg"));
			interactable->triggered = true;
			world.RemoveComponent<Blocker>(entity);
		}
		else {
			event_manager.push_event(EventTypes::PLAY_CHUNK, sound_manager.LoadChunk("./Resources/Sounds/SFX/doorClose_2.ogg"));
			interactable->triggered = false;
			world.AddComponent<Blocker>(entity, true);
		}

		const auto& state = anim->animations.at(anim->_state).front();
		anim->animations.at(anim->_state).pop_front();
		anim->animations.at(anim->_state).push_back(state);
		sprite->id = state.id;
		sprite->clip_x = state.clip_x;
		sprite->clip_y = state.clip_y;
		sprite->width = state.width;
		sprite->height = state.height;


	};

	bump_scripts.insert({ "OPEN_CHEST", open_chest_script });
	bump_scripts.insert({ "OPEN_DOOR", open_door_script });
}

void ScriptSystem::do_bump(uint32_t entity)
{
	auto* script = world.GetComponent<Scriptable>(entity);
	bump_scripts.at(script->OnBump)(world, event_manager, sound_manager, entity);
}

void ScriptSystem::load_update_scripts()
{

}

ScriptSystem::ScriptSystem(World& _world, EventManager& _event_manager, WorldMap& _world_map, SoundManager& _sound_manager):
	world(_world), event_manager(_event_manager), world_map(_world_map), sound_manager(_sound_manager)
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
			update_scripts.at(script->OnUpdate)(world, event_manager, sound_manager, e);
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
