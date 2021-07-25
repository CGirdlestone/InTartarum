#include "EntityFactory.hpp"

EntityFactory::EntityFactory(World& _world, TextureManager& _texture_manager)
    : world(_world), texture_manager(_texture_manager)
{

}

uint32_t EntityFactory::create_mob(std::string& entity_name, int x, int y, int z, int world_x, int world_y)
{
    /* Create the position component and push the corret table onto the lua stack. */
    SmartLuaVM vm(nullptr, lua_close);
    vm.reset(luaL_newstate());
    std::string mob_file = "./Resources/Data/Monsters/mobs.lua";
    load_file(vm, mob_file);

    auto entity = world.CreateEntity();
    world.AddComponent<Position>(entity, x, y, z, world_x, world_y);

    lua_getglobal(vm.get(), entity_name.c_str()); // push object name
    if (!lua_istable(vm.get(), -1)) {
        printf("Expected table!");
    }

    create_entity(vm, entity);
    return entity;
}

uint32_t EntityFactory::create_player( int world_x, int world_y)
{
    SmartLuaVM vm(nullptr, lua_close);
    vm.reset(luaL_newstate());
    std::string player_file = "./Resources/Data/PlayerData/Player.lua";
    load_file(vm, player_file);

    auto entity = world.CreateEntity();
    
    lua_getglobal(vm.get(), "player_table"); // get player_table
    if (!lua_istable(vm.get(), -1)) {
        printf("Expected table!");
    }
    
    create_entity(vm, entity);
    auto* pos = world.GetComponent<Position>(entity);
    pos->world_x = world_x;
    pos->world_y = world_y;
    return entity;
}

uint32_t EntityFactory::create_prop(std::string& entity_name, int x, int y, int z, int world_x, int world_y)
{
    /* Create the position component and push the correct table onto the lua stack. */
    SmartLuaVM vm(nullptr, lua_close);
    vm.reset(luaL_newstate());
    std::string prop_file = "./Resources/Data/Objects/props.lua";
    load_file(vm, prop_file);

    auto entity = world.CreateEntity();
    world.AddComponent<Position>(entity, x, y, z, world_x, world_y);

    lua_getglobal(vm.get(), entity_name.c_str()); // push object name
    if (!lua_istable(vm.get(), -1)) {
        printf("Expected table!");
    }

    create_entity(vm, entity);
    return entity;
}

uint32_t EntityFactory::create_item(std::string& entity_name, int x, int y, int z, int world_x, int world_y)
{
    /* Create the position component and push the correct table onto the lua stack. */
    SmartLuaVM vm(nullptr, lua_close);
    vm.reset(luaL_newstate());
    std::string item_file = "./Resources/Data/Objects/items.lua";
    load_file(vm, item_file);

    auto entity = world.CreateEntity();
    world.AddComponent<Position>(entity, x, y, z, world_x, world_y);

    lua_getglobal(vm.get(), entity_name.c_str()); // push object name
    if (!lua_istable(vm.get(), -1)) {
        printf("Expected table!");
    }

    create_entity(vm, entity);
    return entity;
}

uint32_t EntityFactory::create_item(std::string& entity_name)
{
    /* Push the corret table onto the lua stack. */
    SmartLuaVM vm(nullptr, lua_close);
    vm.reset(luaL_newstate());
    std::string item_file = "./Resources/Data/Objects/items.lua";
    load_file(vm, item_file);

    auto entity = world.CreateEntity(); 
    
    lua_getglobal(vm.get(), entity_name.c_str()); // push object name
    if (!lua_istable(vm.get(), -1)) {
        printf("Expected table!");
    }

    create_entity(vm, entity);
    return entity;
}

uint32_t EntityFactory::create_npc(std::string& entity_name, int x, int y, int z, int world_x, int world_y)
{
    SmartLuaVM vm(nullptr, lua_close);
    vm.reset(luaL_newstate());
    std::string item_file = "./Resources/Data/NPCs/npcs.lua";
    load_file(vm, item_file);

    auto entity = world.CreateEntity();
    world.AddComponent<Position>(entity, x, y, z, world_x, world_y);

    lua_getglobal(vm.get(), entity_name.c_str()); // push object name
    if (!lua_istable(vm.get(), -1)) {
        printf("Expected table!");
    }

    create_entity(vm, entity);
    return entity;
}



bool EntityFactory::load_file(SmartLuaVM& vm, const std::string& lua_file)
{
    auto result = luaL_dofile(vm.get(), lua_file.c_str());
    if (result != LUA_OK) {
        std::string error_msg = lua_tostring(vm.get(), -1);
        printf(error_msg.c_str());
        return false;
    }
    return true;
}

void EntityFactory::create_entity(SmartLuaVM& vm, uint32_t& entity)
{
    lua_pushnil(vm.get());
    while (lua_next(vm.get(), -2) != 0) {
        if (!lua_isstring(vm.get(), -2)) {
            std::string error_msg = lua_tostring(vm.get(), -1);
            printf(error_msg.c_str());
            return;
        }
        auto component = std::string(lua_tostring(vm.get(), -2));
        lua_pushstring(vm.get(), component.c_str());

        if (component == "position") {
            create_position(vm, entity);
        }
        else if (component == "sprite") {
            create_sprite(vm, entity);
        }
        else if (component == "player") {
            create_player(vm, entity);
        }
        else if (component == "blocker") {
            create_blocker(vm, entity);
        }
        else if (component == "item") {
            create_item(vm, entity);
        }
        else if (component == "equipable") {
            create_equipable(vm, entity);
        }
        else if (component == "script") {
            create_script(vm, entity);
        }
        else if (component == "weapon") {
            create_weapon(vm, entity);
        }
        else if (component == "animation") {
            create_animation(vm, entity);
        }
        else if (component == "interactable") {
            create_interactable(vm, entity);
        }
        else if (component == "container") {
            create_container(vm, entity);
        }
        else if (component == "stackable") {
            create_stackable(vm, entity);
        }
        else if (component == "body") {
            create_body(vm, entity);
        }
        else if (component == "id") {
            create_id(vm, entity);
        }
        else if (component == "ai") {
            create_ai(vm, entity);
        }
        else if (component == "actor") {
            create_actor(vm, entity);
        }
        else if (component == "useable") {
            create_useable(vm, entity);
        }
        else if (component == "fighter") {
            create_fighter(vm, entity);
        }
        else if (component == "armour") {
            create_armour(vm, entity);
        }
        lua_pop(vm.get(), 2);
    }
    lua_pop(vm.get(), 1);
}

void EntityFactory::create_position(SmartLuaVM& vm, uint32_t& entity)
{
    auto x = utils::read_lua_int(vm, "x", -3);
    auto y = utils::read_lua_int(vm, "y", -3);
    auto z = utils::read_lua_int(vm, "z", -3);

    world.AddComponent<Position>(entity, x, y, z);
}

void EntityFactory::create_sprite(SmartLuaVM& vm, uint32_t& entity)
{
    auto tileset = utils::read_lua_string(vm, "tilesheet", -3);
    auto path = "./Resources/" + tileset;
    auto id = texture_manager.LoadTexture(path);
    auto clip_x = utils::read_lua_int(vm, "clip_x", -3);
    auto clip_y = utils::read_lua_int(vm, "clip_y", -3);
    auto width = utils::read_lua_int(vm, "width", -3);
    auto height = utils::read_lua_int(vm, "height", -3);
    auto depth = utils::read_lua_int(vm, "depth", -3);
    auto red = utils::read_lua_int(vm, "red", -3);
    auto green = utils::read_lua_int(vm, "green", -3);
    auto blue = utils::read_lua_int(vm, "blue", -3);

    world.AddComponent<Sprite>(entity, id, clip_x * width, clip_y * height, width, height, depth, red, green, blue);
}

void EntityFactory::create_player(SmartLuaVM& vm, uint32_t& entity)
{
    auto vision = utils::read_lua_int(vm, "vision", -3);

    world.AddComponent<Player>(entity, vision);
}

void EntityFactory::create_actor(SmartLuaVM& vm, uint32_t& entity)
{
    auto name = utils::read_lua_string(vm, "name", -3);
    auto description = utils::read_lua_string(vm, "description", -3);
    world.AddComponent<Actor>(entity, name, description);
}

void EntityFactory::create_blocker(SmartLuaVM& vm, uint32_t& entity)
{
    auto blocks_view = utils::read_lua_bool(vm, "blocks_view", -3);
    world.AddComponent<Blocker>(entity, blocks_view);
}

void EntityFactory::create_interactable(SmartLuaVM& vm, uint32_t& entity)
{
    auto repeatable = utils::read_lua_bool(vm, "repeatable", -3);
    world.AddComponent<Interactable>(entity, repeatable);
}

void EntityFactory::create_animation(SmartLuaVM& vm, uint32_t& entity)
{
    // the current lua stack -> KEY(copy) | VALUE | KEY | TABLE
    auto lifetime = utils::read_lua_float(vm, "lifetime", -3);
    auto dynamic = utils::read_lua_bool(vm, "dynamic", -3);

    world.AddComponent<Animation>(entity, lifetime, dynamic);
    auto* animation = world.GetComponent<Animation>(entity);
    
    lua_pushstring(vm.get(), "frames"); // lua stack -> "frames" | KEY(copy) | VALUE | KEY | TABLE
    lua_gettable(vm.get(), -3); // lua stack -> FRAMES_TABLE | KEY(copy) | VALUE | KEY | TABLE
    if (!lua_istable(vm.get(), -1)) {
        printf("Animation frame table expected!\n");
        return;
    }
    auto num_frames = lua_rawlen(vm.get(), -1); // gets the number of frames in the FRAMES_TABLE
    for (int i = 1; i < num_frames + 1; i++) {
        lua_pushnumber(vm.get(), i); // lua stack -> i | FRAMES_TABLE | KEY(copy) | VALUE | KEY | TABLE
        lua_gettable(vm.get(), -2); // retrieve sub-table 

        if (!lua_istable(vm.get(), -1)) {
            printf("Animation frame table expected!\n");
            return;
        }

        auto tilesheet = utils::read_lua_string(vm, "tilesheet", -2);
        std::string path = "./Resources/" + tilesheet;
        auto id = texture_manager.LoadTexture(path);
        auto clip_x = utils::read_lua_int(vm, "clip_x", -2);
        auto clip_y = utils::read_lua_int(vm, "clip_y", -2);
        auto width = utils::read_lua_int(vm, "width", -2);
        auto height = utils::read_lua_int(vm, "height", -2);
        
        animation->animations.at(state::IDLE).push_back(AnimFrame(id, clip_x, clip_y, width, height));

        lua_pop(vm.get(), 1);
    }
    lua_pop(vm.get(), 1);
}

void EntityFactory::create_particle(SmartLuaVM& vm, uint32_t& entity)
{

}

void EntityFactory::create_script(SmartLuaVM& vm, uint32_t& entity)
{
    world.AddComponent<Scriptable>(entity);
    auto* script = world.GetComponent<Scriptable>(entity);
    
    auto init = utils::read_lua_string(vm, "OnInit", -3);
    if (init != " ") {
        script->OnInit = init;
    }
    auto update = utils::read_lua_string(vm, "OnUpdate", -3);
    if (update != " ") {
        script->OnUpdate = update;
    }
    auto bump = utils::read_lua_string(vm, "OnBump", -3);
    if (bump != " ") {
        script->OnBump = bump;
    }
    auto death = utils::read_lua_string(vm, "OnDeath", -3);
    if (death != " ") {
        script->OnDeath = death;
    }
    auto equip = utils::read_lua_string(vm, "OnEquip", -3);
    if (equip != " ") {
        script->OnEquip = equip;
    }
    auto unequip = utils::read_lua_string(vm, "OnUnequip", -3);
    if (unequip != " ") {
        script->OnUnequip = unequip;
    }
    auto use = utils::read_lua_string(vm, "OnUse", -3);
    if (use != " ") {
        script->OnUse = use;
    }
    auto hit = utils::read_lua_string(vm, "OnHit", -3);
    if (hit != " ") {
        script->OnHit = hit;
    }
}

void EntityFactory::create_item(SmartLuaVM& vm, uint32_t& entity)
{
    auto name = utils::read_lua_string(vm, "name", -3);
    auto description = utils::read_lua_string(vm, "description", -3);
    auto weight = utils::read_lua_int(vm, "weight", -3);
    world.AddComponent<Item>(entity, name, description, weight);
}

void EntityFactory::create_equipable(SmartLuaVM& vm, uint32_t& entity)
{
    auto slot = static_cast<Slot>(utils::read_lua_int(vm, "slot", -3));
    world.AddComponent<Equipable>(entity);
    auto* equipable = world.GetComponent<Equipable>(entity);
    equipable->slot = slot;
}

void EntityFactory::create_weapon(SmartLuaVM& vm, uint32_t& entity)
{
    auto num_dice = utils::read_lua_int(vm, "num_dice", -3);
    auto sides = utils::read_lua_int(vm, "sides", -3);
    auto ranged = utils::read_lua_bool(vm, "ranged", -3);
    std::string ammo = utils::read_lua_string(vm, "ammo", -3);
    world.AddComponent<Weapon>(entity, num_dice, sides);
    auto* weapon = world.GetComponent<Weapon>(entity);
    weapon->ranged = ranged;
    weapon->ammo = ammo;
}

void EntityFactory::create_container(SmartLuaVM& vm, uint32_t& entity)
{
    auto weight_capacity = utils::read_lua_int(vm, "weight_capacity", -3);
    world.AddComponent<Container>(entity, weight_capacity);
    
    lua_pushstring(vm.get(), "starting_equipment");
    lua_gettable(vm.get(), -3);
    if (!lua_istable(vm.get(), -1)) {
        lua_pop(vm.get(), 1);
        return;
    } 

    // parse the starting equipment and add it to the inventory 
    auto* container = world.GetComponent<Container>(entity);
    auto num_items = lua_rawlen(vm.get(), -1);
    for (int i = 1; i < num_items + 1; i++) {
        auto item_name = utils::read_lua_string(vm, i, -2);
        auto item = create_item(item_name);
        auto* item_component = world.GetComponent<Item>(item);
        container->inventory.push_back(item);
        container->weight += item_component->weight;
    }
    lua_pop(vm.get(), 1);
}

void EntityFactory::create_stackable(SmartLuaVM& vm, uint32_t& entity)
{
    world.AddComponent<Stackable>(entity);
}

void EntityFactory::create_body(SmartLuaVM& vm, uint32_t& entity)
{
    world.AddComponent<Body>(entity);
}

void EntityFactory::create_id(SmartLuaVM& vm, uint32_t& entity)
{
    auto uid = utils::read_lua_string(vm, "uid", -3);
    world.AddComponent<ID>(entity, uid);
}

void EntityFactory::create_ai(SmartLuaVM& vm, uint32_t& entity)
{
    auto uid = static_cast<Attitude>(utils::read_lua_int(vm, "state", -3));
    auto blind = utils::read_lua_bool(vm, "blind", -3);
    auto scent = utils::read_lua_bool(vm, "scent", -3);
    auto smart = utils::read_lua_bool(vm, "smart", -3);
    auto xp = utils::read_lua_int(vm, "xp", -3);
    world.AddComponent<AI>(entity, uid, blind, scent, smart, xp);
}

void EntityFactory::create_useable(SmartLuaVM& vm, uint32_t& entity)
{
    auto type = static_cast<UseableType>(utils::read_lua_int(vm, "type", -3));
    auto charges = utils::read_lua_int(vm, "charges", -3);
    world.AddComponent<Useable>(entity, type, charges);
}

void EntityFactory::create_fighter(SmartLuaVM& vm, uint32_t& entity)
{
    auto hp = utils::read_lua_int(vm, "hp", -3);
    auto defence = utils::read_lua_int(vm, "defence", -3);
    auto str = utils::read_lua_int(vm, "str", -3);
    auto dex = utils::read_lua_int(vm, "dex", -3);
    auto con = utils::read_lua_int(vm, "con", -3);
    auto wis = utils::read_lua_int(vm, "wis", -3);
    auto intelligence = utils::read_lua_int(vm, "int", -3);
    auto cha = utils::read_lua_int(vm, "cha", -3);
    world.AddComponent<Fighter>(entity, hp, defence, str, dex, con, wis, intelligence, cha);
}

void EntityFactory::create_armour(SmartLuaVM& vm, uint32_t& entity)
{
    auto defence_bonus = utils::read_lua_int(vm, "defence_bonus", -3);

    world.AddComponent<Armour>(entity, defence_bonus);
}

std::vector<uint32_t> EntityFactory::load_starting_equipment()
{
    return std::vector<uint32_t>();
}
