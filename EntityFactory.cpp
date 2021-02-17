#include "EntityFactory.hpp"

EntityFactory::EntityFactory(World& _world, TextureManager& _texture_manager)
    : world(_world), texture_manager(_texture_manager), vm(nullptr, &lua_close)
{
    vm.reset(luaL_newstate());
}

uint32_t EntityFactory::create_mob(std::string& entity_name, int x, int y, int z)
{
    /* Create the position component and push the corret table onto the lua stack. */
    vm.reset(luaL_newstate());
    std::string mob_file = "./Resources/Data/Monsters/mobs.lua";
    load_file(mob_file);

    auto entity = world.CreateEntity();
    world.AddComponent<Position>(entity, x, y, z);

    lua_getglobal(vm.get(), "mobs"); // push mobs table
    lua_pushstring(vm.get(), entity_name.c_str()); // push mob name
    lua_gettable(vm.get(), -2); // get mob sub-table on top of stack

    create_entity(entity);
    return entity;
}

uint32_t EntityFactory::create_player()
{
    vm.reset(luaL_newstate());
    std::string player_file = "./Resources/Data/PlayerData/Player.lua";
    load_file(player_file);

    auto entity = world.CreateEntity();
    
    lua_getglobal(vm.get(), "player_table"); // get player_table
    if (!lua_istable(vm.get(), -1)) {
        printf("Expected table!");
    }
    
    create_entity(entity);
    return entity;
}

uint32_t EntityFactory::create_item(std::string& entity_name, int x, int y, int z)
{
    /* Create the position component and push the correct table onto the lua stack. */
    vm.reset(luaL_newstate());
    std::string item_file = "./Resources/Data/Objects/objects.lua";
    load_file(item_file);

    auto entity = world.CreateEntity();
    world.AddComponent<Position>(entity, x, y, z);

    lua_getglobal(vm.get(), entity_name.c_str()); // push object name
    if (!lua_istable(vm.get(), -1)) {
        printf("Expected table!");
    }

    create_entity(entity);
    return entity;
}

uint32_t EntityFactory::create_item(std::string& entity_name)
{
    /* Push the corret table onto the lua stack. */
    vm.reset(luaL_newstate());
    std::string item_file = "./Resources/Data/Objects/objects.lua";
    load_file(item_file);

    auto entity = world.CreateEntity(); 
    
    lua_getglobal(vm.get(), "objects"); // push objects table
    lua_pushstring(vm.get(), entity_name.c_str()); // push object name
    lua_gettable(vm.get(), -2); // get object sub-table on top of stack
    
    create_entity(entity);
    return entity;
}

uint32_t EntityFactory::create_npc(std::string& entity_name, int x, int y, int z)
{
    vm.reset(luaL_newstate());
    std::string item_file = "./Resources/Data/NPCs/npcs.lua";
    load_file(item_file);

    auto entity = world.CreateEntity();
    world.AddComponent<Position>(entity, x, y, z);

    lua_getglobal(vm.get(), entity_name.c_str()); // push object name
    if (!lua_istable(vm.get(), -1)) {
        printf("Expected table!");
    }

    create_entity(entity);
    return entity;
}



bool EntityFactory::load_file(const std::string& lua_file)
{
    auto result = luaL_dofile(vm.get(), lua_file.c_str());
    if (result != LUA_OK) {
        std::string error_msg = lua_tostring(vm.get(), -1);
        printf(error_msg.c_str());
        return false;
    }
    return true;
}

void EntityFactory::create_entity(uint32_t& entity)
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
            create_position(entity);
        }
        else if (component == "sprite") {
            create_sprite(entity);
        }
        else if (component == "player") {
            create_player(entity);
        }
        else if (component == "blocker") {
            create_blocker(entity);
        }
        else if (component == "item") {
            create_item(entity);
        }
        else if (component == "equipable") {
            create_equipable(entity);
        }
        else if (component == "script") {
            create_script(entity);
        }
        else if (component == "weapon") {
            create_weapon(entity);
        }
        else if (component == "animation") {
            create_animation(entity);
        }
        else if (component == "interactable") {
            create_interactable(entity);
        }
        else if (component == "container") {
            create_container(entity);
        }
        else if (component == "stackable") {
            create_stackable(entity);
        }
        lua_pop(vm.get(), 2);
    }
    lua_pop(vm.get(), 1);
}

void EntityFactory::create_position(uint32_t& entity)
{
    auto x = utils::read_lua_int(vm, "x", -3);
    auto y = utils::read_lua_int(vm, "y", -3);
    auto z = utils::read_lua_int(vm, "z", -3);

    world.AddComponent<Position>(entity, x, y, z);
}

void EntityFactory::create_sprite(uint32_t& entity)
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

void EntityFactory::create_player(uint32_t& entity)
{
    auto vision = utils::read_lua_int(vm, "vision", -3);

    world.AddComponent<Player>(entity, vision);
}

void EntityFactory::create_actor(uint32_t& entity)
{
    world.AddComponent<Actor>(entity);
}

void EntityFactory::create_blocker(uint32_t& entity)
{
    auto blocks_view = utils::read_lua_bool(vm, "blocks_view", -3);
    world.AddComponent<Blocker>(entity, blocks_view);
}

void EntityFactory::create_interactable(uint32_t& entity)
{
    auto repeatable = utils::read_lua_bool(vm, "repeatable", -3);
    world.AddComponent<Interactable>(entity, repeatable);
}

void EntityFactory::create_animation(uint32_t& entity)
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

void EntityFactory::create_particle(uint32_t& entity)
{

}

void EntityFactory::create_script(uint32_t& entity)
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

void EntityFactory::create_item(uint32_t& entity)
{
    auto name = utils::read_lua_string(vm, "name", -3);
    auto description = utils::read_lua_string(vm, "description", -3);
    auto weight = utils::read_lua_int(vm, "weight", -3);
    world.AddComponent<Item>(entity, name, description, weight);
}

void EntityFactory::create_equipable(uint32_t& entity)
{
    auto slot = static_cast<Slot>(utils::read_lua_int(vm, "slot", -3));
    world.AddComponent<Equipable>(entity);
    auto* equipable = world.GetComponent<Equipable>(entity);
    equipable->slot = slot;
}

void EntityFactory::create_weapon(uint32_t& entity)
{
    auto num_dice = utils::read_lua_int(vm, "num_dice", -3);
    auto sides = utils::read_lua_int(vm, "sides", -3);
    world.AddComponent<Weapon>(entity, num_dice, sides);
}

void EntityFactory::create_container(uint32_t& entity)
{
    auto weight_capacity = utils::read_lua_int(vm, "weight_capacity", -3);
    world.AddComponent<Container>(entity, weight_capacity);
}

void EntityFactory::create_stackable(uint32_t& entity)
{
    world.AddComponent<Stackable>(entity);
}
