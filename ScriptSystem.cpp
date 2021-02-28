#include "ScriptSystem.hpp"

void ScriptSystem::load_bump_scripts()
{
	auto open_chest_script = [](World& world, WorldMap& world_map, EventManager& event_manager, SoundManager& sound_manager, TextureManager& texture_manager, int tile_width, int tile_height, uint32_t entity, uint32_t target, uint32_t item) {
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

	auto open_door_script = [](World& world, WorldMap& world_map, EventManager& event_manager, SoundManager& sound_manager, TextureManager& texture_manager, int tile_width, int tile_height, uint32_t entity, uint32_t target, uint32_t item) {
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
	bump_scripts.at(script->OnBump)(world, world_map, event_manager, sound_manager, texture_manager, tile_width, tile_height, entity, MAX_ENTITIES + 1, MAX_ENTITIES + 1);
}

void ScriptSystem::load_update_scripts()
{
	
}

void ScriptSystem::load_death_scripts()
{
	auto spawn_explosion_script = [](World& world, WorldMap& world_map, EventManager& event_manager, SoundManager& sound_manager, TextureManager& texture_manager, int tile_width, int tile_height, uint32_t entity, uint32_t target, uint32_t item) {
		auto* pos = world.GetComponent<Position>(entity);
		auto* script = world.GetComponent<Scriptable>(entity);
		Prefab::create_explosion(world, pos->x * tile_width, pos->y * tile_height, pos->z, texture_manager.LoadTexture("./Resources/exp2_0.png"));
		world.KillEntity(entity);
	};

	bump_scripts.insert({ "SPAWN_EXPLOSION", spawn_explosion_script });
}

void ScriptSystem::do_death(uint32_t entity)
{
	auto* script = world.GetComponent<Scriptable>(entity);
	death_scripts.at(script->OnDeath)(world, world_map, event_manager, sound_manager, texture_manager, tile_width, tile_height, entity, MAX_ENTITIES + 1, MAX_ENTITIES + 1);
}

void ScriptSystem::load_consume_scripts()
{
	auto heal = [](World& world, WorldMap& world_map, EventManager& event_manager, SoundManager& sound_manager, TextureManager& texture_manager, int tile_width, int tile_height, uint32_t entity, uint32_t target, uint32_t item) {
		event_manager.push_event(EventTypes::HEAL);
		event_manager.push_event(EventTypes::DECREASE_CHARGE, entity, item);
	};

	consume_scripts.insert({ "heal", heal });
}

void ScriptSystem::do_consume(uint32_t entity, uint32_t item)
{
	auto* script = world.GetComponent<Scriptable>(item);
	consume_scripts.at(script->OnUse)(world, world_map, event_manager, sound_manager, texture_manager, tile_width, tile_height, entity, MAX_ENTITIES + 1, item);
}

void ScriptSystem::load_use_scripts()
{
	auto fireball = [](World& world, WorldMap& world_map, EventManager& event_manager, SoundManager& sound_manager, TextureManager& texture_manager, int tile_width, int tile_height, uint32_t entity, uint32_t target, uint32_t item) {
		
		if (target != MAX_ENTITIES + 1) {
			auto* pos = world.GetComponent<Position>(target);
			event_manager.push_event(EventTypes::CAST, entity, target, item);
		}
		else {
			event_manager.push_event(EventTypes::AOE_CAST, entity, item);
		}

		event_manager.push_event(EventTypes::DECREASE_CHARGE, entity, item);

	};

	use_scripts.insert({ "fireball", fireball });
}

void ScriptSystem::do_use(uint32_t entity, uint32_t target, uint32_t item)
{
	auto* script = world.GetComponent<Scriptable>(item);
	use_scripts.at(script->OnUse)(world, world_map, event_manager, sound_manager, texture_manager, tile_width, tile_height, entity, target, item);
}

void ScriptSystem::load_equip_scripts()
{
	auto buff_health = [](World& world, WorldMap& world_map, EventManager& event_manager, SoundManager& sound_manager, TextureManager& texture_manager, int tile_width, int tile_height, uint32_t entity, uint32_t target, uint32_t item) {
		event_manager.push_event(EventTypes::BUFF_HEALTH, entity, item);
	};

	equip_scripts.insert({ "buff_health", buff_health });
}

void ScriptSystem::do_equip(uint32_t entity, uint32_t item)
{
	auto* script = world.GetComponent<Scriptable>(item);
	equip_scripts.at(script->OnEquip)(world, world_map, event_manager, sound_manager, texture_manager, tile_width, tile_height, entity, MAX_ENTITIES + 1, item);
}

void ScriptSystem::load_unequip_scripts()
{
	auto debuff_health = [](World& world, WorldMap& world_map, EventManager& event_manager, SoundManager& sound_manager, TextureManager& texture_manager, int tile_width, int tile_height, uint32_t entity, uint32_t target, uint32_t item) {
		event_manager.push_event(EventTypes::DEBUFF_HEALTH, entity, item);
	};

	unequip_scripts.insert({ "debuff_health", debuff_health });

}

void ScriptSystem::do_unequip(uint32_t entity, uint32_t item)
{
	auto* script = world.GetComponent<Scriptable>(item);
	unequip_scripts.at(script->OnUnequip)(world, world_map, event_manager, sound_manager, texture_manager, tile_width, tile_height, entity, MAX_ENTITIES + 1, item);
}

ScriptSystem::ScriptSystem(World& _world, EventManager& _event_manager, WorldMap& _world_map, SoundManager& _sound_manager, TextureManager& _texture_manager)
	: world(_world), event_manager(_event_manager), world_map(_world_map), sound_manager(_sound_manager), texture_manager(_texture_manager),
	tile_width(0), tile_height(0), Lua_VM(nullptr, lua_close)
{
	SmartLuaVM vm(nullptr, &lua_close);
	vm.reset(luaL_newstate());
	auto result = luaL_dofile(vm.get(), "./Config/window.lua");
	std::string _font{ "" };

	if (result == LUA_OK) {
		lua_getglobal(vm.get(), "tile_width");
		if (lua_isnumber(vm.get(), -1)) {
			tile_width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "tile_height");
		if (lua_isnumber(vm.get(), -1)) {
			tile_height = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
	}

	event_manager.add_subscriber(EventTypes::BUMP_SCRIPT, *this);
	event_manager.add_subscriber(EventTypes::ON_USE_SCRIPT, *this);
	event_manager.add_subscriber(EventTypes::ON_EQUIP_SCRIPT, *this);
	event_manager.add_subscriber(EventTypes::ON_UNEQUIP_SCRIPT, *this);
	Lua_VM.reset(luaL_newstate());
}

ScriptSystem::~ScriptSystem()
{

}

void ScriptSystem::init()
{
	load_bump_scripts();
	load_update_scripts();
	load_death_scripts();
	load_consume_scripts();
	load_use_scripts();
	load_equip_scripts();
	load_unequip_scripts();
}

void ScriptSystem::update(float dt)
{
	auto entities = world.GetEntitiesWith<Scriptable>();
	for (auto e : entities) {
		auto* script = world.GetComponent<Scriptable>(e);
		if (script->OnUpdate != "") {
			update_scripts.at(script->OnUpdate)(world, world_map, event_manager, sound_manager, texture_manager, tile_width, tile_height, e, MAX_ENTITIES + 1, MAX_ENTITIES + 1);
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

void ScriptSystem::receive(EventTypes event, uint32_t actor, uint32_t item)
{
	switch (event) {
	case EventTypes::ON_USE_SCRIPT: do_consume(actor, item); break;
	case EventTypes::ON_EQUIP_SCRIPT: do_equip(actor, item); break;
	case EventTypes::ON_UNEQUIP_SCRIPT: do_unequip(actor, item); break;
	}
}

void ScriptSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{
	switch (event) {
	case EventTypes::ON_USE_SCRIPT: do_use(actor, target, item); break;
	}
}
