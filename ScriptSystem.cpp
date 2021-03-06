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
		// particle effect
		auto* user_pos = world.GetComponent<Position>(entity);
		auto* sprite = world.GetComponent<Sprite>(entity);
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				uint32_t particle = world.CreateEntity();
				world.AddComponent<Position>(particle, user_pos->x + i, user_pos->y + j, user_pos->z, user_pos->world_x, user_pos->world_y);
				world.AddComponent<Sprite>(particle, 0, 10 * sprite->width, 2 * sprite->height, sprite->width, sprite->height, 0, 205, 92, 92);
				world.AddComponent<Particle>(particle, 0.3f);
			}
		}
	};

	death_scripts.insert({ "spawn_explosion", spawn_explosion_script });
}

void ScriptSystem::do_death(uint32_t entity)
{
	auto* script = world.GetComponent<Scriptable>(entity);
	death_scripts.at(script->OnDeath)(world, world_map, event_manager, sound_manager, texture_manager, tile_width, tile_height, entity, MAX_ENTITIES + 1, MAX_ENTITIES + 1);
}

void ScriptSystem::load_consume_scripts()
{
	auto heal = [](World& world, WorldMap& world_map, EventManager& event_manager, SoundManager& sound_manager, TextureManager& texture_manager, int tile_width, int tile_height, uint32_t entity, uint32_t target, uint32_t item) {
		std::string roll = "2d6";
		uint32_t health = static_cast<uint32_t>(utils::roll(roll));
		event_manager.push_event(EventTypes::HEAL, entity, health);
		event_manager.push_event(EventTypes::DECREASE_CHARGE, entity, item);

		auto* fighter = world.GetComponent<Fighter>(entity);
		if (fighter != nullptr) {
			fighter->hp += health;
			fighter->hp = fighter->hp > fighter->max_hp ? fighter->max_hp : fighter->hp;
		}


		// particle effect
		auto* user_pos = world.GetComponent<Position>(entity);
		auto* sprite = world.GetComponent<Sprite>(entity);
		for (int i = -2; i <= 2; i += 2) {
			for (int j = -2; j <= 2; j += 2) {
				if (i == 0 && j == 0) {
					continue;
				}
				uint32_t particle = world.CreateEntity();
				world.AddComponent<Position>(particle, user_pos->x + i, user_pos->y + j, user_pos->z, user_pos->world_x, user_pos->world_y);
				world.AddComponent<Sprite>(particle, 0, 10 * sprite->width, 2 * sprite->height, sprite->width, sprite->height, 0, 176, 196, 222);
				world.AddComponent<Particle>(particle, static_cast<float>(user_pos->x + i), static_cast<float>(user_pos->y + j), user_pos->x, user_pos->y);
			}
		}
		
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
			event_manager.push_event(EventTypes::CAST, entity, target, item);
		}
		else {
			event_manager.push_event(EventTypes::AOE_CAST, entity, target, item);
		}

		event_manager.push_event(EventTypes::DECREASE_CHARGE, entity, item);
		auto* fighter = world.GetComponent<Fighter>(target);
		if (fighter != nullptr) {
			std::string roll = "3d8";
			uint32_t dmg = static_cast<uint32_t>(utils::roll(roll));
			event_manager.push_event(EventTypes::APPLY_DAMAGE, entity, target, dmg);
		}

		// paticle effect
		auto* user_pos = world.GetComponent<Position>(entity);
		auto* sprite = world.GetComponent<Sprite>(entity);
		auto* target_pos = world.GetComponent<Position>(target);
		uint32_t particle = world.CreateEntity();
		world.AddComponent<Position>(particle, user_pos->x, user_pos->y, user_pos->z, user_pos->world_x, user_pos->world_y);
		world.AddComponent<Sprite>(particle, 0, 10 * sprite->width, 2 * sprite->height, sprite->width, sprite->height, 0, 205, 92, 92);
		world.AddComponent<Particle>(particle, static_cast<float>(user_pos->x), static_cast<float>(user_pos->y), target_pos->x, target_pos->y);
		std::string particle_death_script = "spawn_explosion";
		world.AddComponent<Scriptable>(particle);
		auto* script = world.GetComponent<Scriptable>(particle);
		script->OnDeath = particle_death_script;
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
		auto* fighter = world.GetComponent<Fighter>(entity);
		if (fighter != nullptr) {
			fighter->max_hp += 10;
			fighter->hp += 10;
		}
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
		auto* fighter = world.GetComponent<Fighter>(entity);
		if (fighter != nullptr) {
			fighter->max_hp -= 10;
			fighter->hp -= 10;
			if (fighter->hp <= 0) {
				fighter->hp = 1;
			}
		}
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
	event_manager.add_subscriber(EventTypes::ON_DEATH_SCRIPT, *this);
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
	case EventTypes::ON_DEATH_SCRIPT: do_death(actor); break;
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
