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
    /* Create the position component and push the corret table onto the lua stack. */
    vm.reset(luaL_newstate());
    std::string item_file = "./Resources/Data/Objects/objects.lua";
    load_file(item_file);

    auto entity = world.CreateEntity();
    world.AddComponent<Position>(entity, x, y, z);

    lua_getglobal(vm.get(), "objects"); // push objects table
    lua_pushstring(vm.get(), entity_name.c_str()); // push object name
    lua_gettable(vm.get(), -2); // get object sub-table on top of stack

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
    world.AddComponent<Blocker>(entity);
}

void EntityFactory::create_interactable(uint32_t& entity)
{
    world.AddComponent<Interactable>(entity);
}

void EntityFactory::create_animation(uint32_t& entity)
{

}

void EntityFactory::create_particle(uint32_t& entity)
{

}

void EntityFactory::create_script(uint32_t& entity)
{

}
