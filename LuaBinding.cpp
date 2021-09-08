#include "LuaBinding.hpp"
#include "LuaComponents.hpp"
#include "LuaUtilMacros.hpp"
#include <cstdio>

void registerCombatSystem(lua_State* vm, CombatSystem& combatSystem)
{
	CombatSystem** combatSystemPtr = (CombatSystem**)lua_newuserdata(vm, sizeof(CombatSystem*));
	*combatSystemPtr = &combatSystem;

	setUpMetatable(CombatSystem);

	registerMethod(checkAlive);

	lua_setmetatable(vm, -2);
	lua_setglobal(vm, "CombatSystem");
}

int lua_checkAlive(lua_State* vm)
{
	CombatSystem** combatSystem = (CombatSystem**)luaL_checkudata(vm, 1, "CombatSystem");
	uint32_t attacker = luaL_checknumber(vm, 2);
	uint32_t defender = luaL_checknumber(vm, 3);

	(*combatSystem)->check_alive(attacker, defender);

	return 0;
}

void registerInventorySystem(lua_State* vm, InventorySystem& inventorySystem)
{
	InventorySystem** inventorySystemPtr = (InventorySystem**)lua_newuserdata(vm, sizeof(InventorySystem*));
	*inventorySystemPtr = &inventorySystem;

	setUpMetatable(ItemManager);

	registerMethod(decreaseCharge);

	lua_setmetatable(vm, -2);
	lua_setglobal(vm, "ItemManager");
}

int lua_decreaseCharge(lua_State* vm)
{
	InventorySystem** inventorySystem = (InventorySystem**)luaL_checkudata(vm, 1, "ItemManager");
	int item = luaL_checknumber(vm, 2);
	int owner = luaL_checknumber(vm, 3);

	(*inventorySystem)->decrease_charges((uint32_t)owner, (uint32_t)item);

	
	return 0;
}

void registerEntityFactory(lua_State* vm, EntityFactory& entityFactory)
{
	EntityFactory** entityFactoryPtr = (EntityFactory**)lua_newuserdata(vm, sizeof(EntityFactory*));
	*entityFactoryPtr = &entityFactory;

	setUpMetatable(EntityFactory);

	registerMethod(createMobAt);
	registerMethod(createMob);
	registerMethod(createItemAt);
	registerMethod(createItem);

	lua_setmetatable(vm, -2);
	lua_setglobal(vm, "EntityFactory");
}

int lua_createMobAt(lua_State* vm)
{
	int nargs = lua_gettop(vm);
	if (nargs != 7) {
		// error
	}

	EntityFactory** entityFactory = (EntityFactory**)luaL_checkudata(vm, 1, "EntityFactory");
	std::string name = luaL_checkstring(vm, 2);
	int x = luaL_checknumber(vm, 3);
	int y = luaL_checknumber(vm, 4);
	int z = luaL_checknumber(vm, 5);
	int world_x = luaL_checknumber(vm, 6);
	int world_y = luaL_checknumber(vm, 7);

	uint32_t mob = (*entityFactory)->create_mob_at(name, x, y, z, world_x, world_y);
	lua_pushnumber(vm, mob);

	return 1;
}

int lua_createMob(lua_State* vm)
{
	int nargs = lua_gettop(vm);
	if (nargs != 2) {
		// error
	}

	EntityFactory** entityFactory = (EntityFactory**)luaL_checkudata(vm, 1, "EntityFactory");
	std::string name = luaL_checkstring(vm, 2);

	uint32_t mob = (*entityFactory)->create_mob(name);
	lua_pushnumber(vm, mob);

	return 1;
}

int lua_createItemAt(lua_State* vm)
{
	int nargs = lua_gettop(vm);
	if (nargs != 7) {
		// error
	}

	EntityFactory** entityFactory = (EntityFactory**)luaL_checkudata(vm, 1, "EntityFactory");
	std::string name = luaL_checkstring(vm, 2);
	int x = luaL_checknumber(vm, 3);
	int y = luaL_checknumber(vm, 4);
	int z = luaL_checknumber(vm, 5);
	int world_x = luaL_checknumber(vm, 6);
	int world_y = luaL_checknumber(vm, 7);

	uint32_t mob = (*entityFactory)->create_item_at(name, x, y, z, world_x, world_y);
	lua_pushnumber(vm, mob);

	return 1;
}

int lua_createItem(lua_State* vm)
{
	
	int nargs = lua_gettop(vm);
	if (nargs != 2) {
		// error
	}

	EntityFactory** entityFactory = (EntityFactory**)luaL_checkudata(vm, 1, "EntityFactory");
	std::string name = luaL_checkstring(vm, 2);

	uint32_t mob = (*entityFactory)->create_item(name);
	lua_pushnumber(vm, mob);

	return 1;
}

void registerSoundSystem(lua_State* vm, SoundSystem& soundSystem)
{
	SoundSystem** soundSystemPtr = (SoundSystem**)lua_newuserdata(vm, sizeof(SoundSystem*));
	*soundSystemPtr = &soundSystem;

	setUpMetatable(SoundSystem);

	registerMethod(playMusic);
	registerMethod(playSFX);
	registerMethod(fadeIn);
	registerMethod(fadeOut);

	lua_setmetatable(vm, -2);
	lua_setglobal(vm, "SoundSystem");
}

int lua_playMusic(lua_State* vm)
{
	SoundSystem** soundSystem = (SoundSystem**)luaL_checkudata(vm, 1, "SoundSystem");
	std::string music = luaL_checkstring(vm, 2);

	(*soundSystem)->play_music(music);

	return 0;
}

int lua_playSFX(lua_State* vm)
{
	SoundSystem** soundSystem = (SoundSystem**)luaL_checkudata(vm, 1, "SoundSystem");
	std::string sfx = luaL_checkstring(vm, 2);

	(*soundSystem)->play_chunk(sfx);

	return 0;
}

int lua_fadeIn(lua_State* vm)
{
	SoundSystem** soundSystem = (SoundSystem**)luaL_checkudata(vm, 1, "SoundSystem");
	std::string music = luaL_checkstring(vm, 2);
	int ms = luaL_checknumber(vm, 3);

	(*soundSystem)->fade_in(music, ms);

	return 0;
}

int lua_fadeOut(lua_State* vm)
{
	SoundSystem** soundSystem = (SoundSystem**)luaL_checkudata(vm, 1, "SoundSystem");
	int ms = luaL_checknumber(vm, 2);

	(*soundSystem)->fade_out(ms);

	return 0;
}

void registerMessageLog(lua_State* vm, MessageLog& messageLog)
{
	MessageLog** messageLogPtr = (MessageLog**)lua_newuserdata(vm, sizeof(MessageLog*));
	*messageLogPtr = &messageLog;

	luaL_newmetatable(vm, "MessageLog");

	lua_pushvalue(vm, -1);
	lua_setfield(vm, -2, "__index");

	registerMethod(addMessage);

	lua_setmetatable(vm, -2);
	lua_setglobal(vm, "MessageLog");
}

int lua_addMessage(lua_State* vm)
{
	MessageLog** messageLog = (MessageLog**)luaL_checkudata(vm, 1, "MessageLog");
	int num_args = lua_gettop(vm);
	std::string message = luaL_checkstring(vm, 2);

	if (num_args == 2) {
		(*messageLog)->add_message(Message(message));
	}
	else if (num_args == 5) {
		uint8_t r = luaL_checknumber(vm, 3);
		uint8_t g = luaL_checknumber(vm, 4);
		uint8_t b = luaL_checknumber(vm, 5);
		(*messageLog)->add_message(Message(message, r, g, b));
	}
	
	return 0;
}

void registerWorldMap(lua_State* vm, WorldMap& worldMap)
{
	WorldMap** worldMapPtr = (WorldMap**)lua_newuserdata(vm, sizeof(WorldMap*));
	*worldMapPtr = &worldMap;

	luaL_newmetatable(vm, "WorldMap");

	lua_pushvalue(vm, -1);
	lua_setfield(vm, -2, "__index");

	registerMethod(getGrid);
	registerMethod(getEntityGrid);

	lua_setmetatable(vm, -2);
	lua_setglobal(vm, "WorldMap");
}

int lua_getGrid(lua_State* vm) 
{
	WorldMap** worldMap = (WorldMap**)luaL_checkudata(vm, 1, "WorldMap");

	auto& grid = (*worldMap)->get_level().get_grid();

	registerGrid(vm, grid);
	
	return 1;
}

int lua_getEntityGrid(lua_State* vm)
{
	WorldMap** worldMap = (WorldMap**)luaL_checkudata(vm, 1, "WorldMap");

	auto& entityGrid = (*worldMap)->get_entity_grid();

	registerEntityGrid(vm, entityGrid);

	return 1;
}

void registerEntityGrid(lua_State* vm, EntityGrid& entityGrid)
{
	EntityGrid** entityGridPtr = (EntityGrid**)lua_newuserdata(vm, sizeof(EntityGrid*));
	*entityGridPtr = &entityGrid;

	luaL_newmetatable(vm, "EntityGrid");

	lua_pushvalue(vm, -1);
	lua_setfield(vm, -2, "__index");

	lua_pushstring(vm, "EntityGrid");
	lua_setfield(vm, -2, "__metatable");

	registerMethod(isEmpty);
	registerMethod(addEntity);
	registerMethod(removeEntity);
	registerMethod(clear);
	registerMethod(getEntitiesAt);

	lua_setmetatable(vm, -2);
}

int lua_isEmpty(lua_State* vm)
{
	EntityGrid** entityGrid = (EntityGrid**)luaL_checkudata(vm, 1, "EntityGrid");
	int x = luaL_checknumber(vm, 2);
	int y = luaL_checknumber(vm, 3);

	bool isEmpty = (*entityGrid)->is_empty(x, y);
	lua_pushboolean(vm, isEmpty);
	
	return 1;
}

int lua_addEntity(lua_State* vm)
{
	EntityGrid** entityGrid = (EntityGrid**)luaL_checkudata(vm, 1, "EntityGrid");
	uint32_t entity = luaL_checknumber(vm, 2);
	int x = luaL_checknumber(vm, 3);
	int y = luaL_checknumber(vm, 4);

	(*entityGrid)->add_entity(entity, x, y);

	return 0;
}

int lua_removeEntity(lua_State* vm)
{
	EntityGrid** entityGrid = (EntityGrid**)luaL_checkudata(vm, 1, "EntityGrid");
	uint32_t entity = luaL_checknumber(vm, 2);
	int x = luaL_checknumber(vm, 3);
	int y = luaL_checknumber(vm, 4);

	(*entityGrid)->remove_entity(entity, x, y);

	return 0;
}

int lua_clear(lua_State* vm)
{
	EntityGrid** entityGrid = (EntityGrid**)luaL_checkudata(vm, 1, "EntityGrid");
	(*entityGrid)->clear();
	return 0;
}

int lua_getEntitiesAt(lua_State* vm)
{
	EntityGrid** entityGrid = (EntityGrid**)luaL_checkudata(vm, 1, "EntityGrid");
	int x = luaL_checknumber(vm, 2);
	int y = luaL_checknumber(vm, 3);

	auto& entities = (*entityGrid)->get(x, y);

	registerEntities(vm, entities);

	return 1;
}

void registerEntities(lua_State* vm, std::vector<uint32_t>& entities)
{
	std::vector<uint32_t>** entityArray = (std::vector<uint32_t>**)lua_newuserdata(vm, sizeof(std::vector<uint32_t>*));
	*entityArray = &entities;

	luaL_newmetatable(vm, "EntityArray");

	lua_pushvalue(vm, -1);
	lua_setfield(vm, -2, "__index");

	lua_pushstring(vm, "EntityArray");
	lua_setfield(vm, -2, "__metatable");

	registerMethod(size);
	registerMethod(get);

	lua_setmetatable(vm, -2);
}

int lua_size(lua_State* vm)
{
	std::vector<uint32_t>** entities = (std::vector<uint32_t>**)luaL_checkudata(vm, 1, "EntityArray");
	uint32_t size = (*entities)->size();

	lua_pushnumber(vm, size);

	return 1;
}

int lua_get(lua_State* vm)
{
	std::vector<uint32_t>** entities = (std::vector<uint32_t>**)luaL_checkudata(vm, 1, "EntityArray");
	int index = luaL_checknumber(vm, 2);

	uint32_t entity = (*entities)->at(index - 1);
	lua_pushnumber(vm, entity);

	return 1;
}

void registerGrid(lua_State* vm, Grid& grid)
{
	Grid** gridPtr = (Grid**)lua_newuserdata(vm, sizeof(Grid*));
	*gridPtr = &grid;

	luaL_newmetatable(vm, "Grid");

	lua_pushvalue(vm, -1);
	lua_setfield(vm, -2, "__index");

	lua_pushstring(vm, "Grid");
	lua_setfield(vm, -2, "__metatable");

	registerMethod(getTile);
	registerMethod(inBounds);
	registerMethod(getWidth);
	registerMethod(getHeight);

	lua_setmetatable(vm, -2);
}

int lua_getTile(lua_State* vm)
{
	Grid** grid = (Grid**)luaL_checkudata(vm, 1, "Grid");
	int x = luaL_checknumber(vm, 2);
	int y = luaL_checknumber(vm, 3);

	auto& tile = (*grid)->get_tile(x, y);

	registerTile(vm, tile);

	return 1;
}

int lua_inBounds(lua_State* vm)
{
	Grid** grid = (Grid**)luaL_checkudata(vm, 1, "Grid");
	int x = luaL_checknumber(vm, 2);
	int y = luaL_checknumber(vm, 3);

	bool inBounds = (*grid)->in_bounds(x, y);

	lua_pushboolean(vm, inBounds);

	return 1;
}

int lua_getWidth(lua_State* vm)
{
	Grid** grid = (Grid**)luaL_checkudata(vm, 1, "Grid");
	int width = (*grid)->get_width();
	lua_pushnumber(vm, width);
	return 1;
}

int lua_getHeight(lua_State* vm)
{
	Grid** grid = (Grid**)luaL_checkudata(vm, 1, "Grid");
	int height = (*grid)->get_height();
	lua_pushnumber(vm, height);
	return 1;
}

void registerTile(lua_State* vm, Tile& tile)
{
	Tile** tilePtr = (Tile**)lua_newuserdata(vm, sizeof(Tile*));
	*tilePtr = &tile;

	luaL_newmetatable(vm, "Tile");

	lua_pushvalue(vm, -1);
	lua_setfield(vm, -2, "__index");

	lua_pushstring(vm, "Tile");
	lua_setfield(vm, -2, "__metatable");

	registerMethod(isTileWalkable);
	registerMethod(doesTileBlockView);
	registerMethod(isTileVisible);

	lua_setmetatable(vm, -2);
}

int lua_isTileWalkable(lua_State* vm)
{
	Tile** tile = (Tile**)luaL_checkudata(vm, 1, "Tile");

	bool isWalkable = (*tile)->walkable;
	lua_pushboolean(vm, isWalkable);

	return 1;
}

int lua_doesTileBlockView(lua_State* vm)
{
	Tile** tile = (Tile**)luaL_checkudata(vm, 1, "Tile");

	bool blocksView = (*tile)->blocks_view;
	lua_pushboolean(vm, blocksView);

	return 1;
}

int lua_isTileVisible(lua_State* vm)
{
	Tile** tile = (Tile**)luaL_checkudata(vm, 1, "Tile");

	bool isVisible = (*tile)->visible;
	lua_pushboolean(vm, isVisible);

	return 1;
}

void registerWorld(lua_State* vm, World& world)
{
	World** worldPtr = (World**)lua_newuserdata(vm, sizeof(World*));
	*worldPtr = &world;

	luaL_newmetatable(vm, "World");

	lua_pushvalue(vm, -1);
	lua_setfield(vm, -2, "__index");

	registerMethod(createEntity);
	registerMethod(killEntity);

	registerMethod(addPosition);
	registerMethod(getPosition);
	registerMethod(removePosition);
	registerMethod(getEntitiesWithPosition);

	registerMethod(addFighter);
	registerMethod(getFighter);
	registerMethod(removeFighter);
	registerMethod(getEntitiesWithFighter);

	registerMethod(addSprite);
	registerMethod(getSprite);
	registerMethod(removeSprite);
	registerMethod(getEntitiesWithSprite);

	registerMethod(addInteractable);
	registerMethod(getInteractable);
	registerMethod(removeInteractable);
	registerMethod(getEntitiesWithInteractable);

	registerMethod(addBlocker);
	registerMethod(getBlocker);
	registerMethod(removeBlocker);
	registerMethod(getEntitiesWithBlocker);

	registerMethod(addItem);
	registerMethod(getItem);
	registerMethod(removeItem);
	registerMethod(getEntitiesWithItem);

	lua_setmetatable(vm, -2);
	lua_setglobal(vm, "World");
}

int lua_createEntity(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	auto entity = (*world)->CreateEntity();

	lua_pushnumber(vm, entity);
	return 1;
}

int lua_killEntity(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");
	uint32_t entity = luaL_checknumber(vm, 2);
	(*world)->KillEntity(entity);
	return 0;
}



int lua_addPosition(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	int x = luaL_checknumber(vm, 3);
	int y = luaL_checknumber(vm, 4);
	int z = luaL_checknumber(vm, 5);
	int world_x = luaL_checknumber(vm, 6);
	int world_y = luaL_checknumber(vm, 7);

	(*world)->AddComponent<Position>(entity, x, y, z, world_x, world_y);

	return 0;
}

int lua_getPosition(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	Position* pos = (*world)->GetComponent<Position>(entity);
	if (pos) {
		registerPosition(vm, pos);
	}
	else {
		lua_pushnil(vm);
	}
	return 1;
}

int lua_removePosition(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	(*world)->RemoveComponent<Position>(entity);
	return 0;
}

int lua_getEntitiesWithPosition(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");
	std::vector<uint32_t> entities = (*world)->GetEntitiesWith<Position>();

	if (entities.empty()) {
		lua_pushnil(vm);
	}
	else {
		registerEntities(vm, entities);
	}
	return 1;
}

int lua_addFighter(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	int hp = luaL_checknumber(vm, 3);
	int defence = luaL_checknumber(vm, 4);
	int strength = luaL_checknumber(vm, 5);
	int dexterity = luaL_checknumber(vm, 6);
	int constitution = luaL_checknumber(vm, 7);
	int intelligence = luaL_checknumber(vm, 8);
	int wisdom = luaL_checknumber(vm, 9);
	int charisma = luaL_checknumber(vm, 10);

	(*world)->AddComponent<Fighter>(entity, hp, defence, strength, dexterity, constitution, intelligence, wisdom, charisma);

	return 0;
}

int lua_getFighter(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	Fighter* fighter = (*world)->GetComponent<Fighter>(entity);
	if (fighter) {
		registerFighter(vm, fighter);
	}
	else {
		lua_pushnil(vm);
	}
	return 1;
}

int lua_removeFighter(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	(*world)->RemoveComponent<Fighter>(entity);
	return 0;
}

int lua_getEntitiesWithFighter(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");
	std::vector<uint32_t> entities = (*world)->GetEntitiesWith<Fighter>();

	if (entities.empty()) {
		lua_pushnil(vm);
	}
	else {
		registerEntities(vm, entities);
	}
	return 1;
}

int lua_addSprite(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	int id = luaL_checknumber(vm, 3);
	int clip_x = luaL_checknumber(vm, 4);
	int clip_y = luaL_checknumber(vm, 5);
	int width = luaL_checknumber(vm, 6);
	int height = luaL_checknumber(vm, 7);
	int depth = luaL_checknumber(vm, 8);
	uint8_t r = luaL_checknumber(vm, 9);
	uint8_t g = luaL_checknumber(vm, 10);
	uint8_t b = luaL_checknumber(vm, 11);

	(*world)->AddComponent<Sprite>(entity, id, clip_x, clip_y, width, height, depth, r, g, b);

	return 0;
}

int lua_getSprite(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	Sprite* sprite = (*world)->GetComponent<Sprite>(entity);
	if (sprite) {
		registerSprite(vm, sprite);
	}
	else {
		lua_pushnil(vm);
	}
	return 1;
}

int lua_removeSprite(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	(*world)->RemoveComponent<Sprite>(entity);
	return 0;
}

int lua_getEntitiesWithSprite(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");
	std::vector<uint32_t> entities = (*world)->GetEntitiesWith<Sprite>();

	if (entities.empty()) {
		lua_pushnil(vm);
	}
	else {
		registerEntities(vm, entities);
	}
	return 1;
}

int lua_addInteractable(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	bool repeat{ false };
	if (lua_isboolean(vm, 3)) {
		repeat = lua_toboolean(vm, 3);
	}

	(*world)->AddComponent<Interactable>(entity, repeat);

	return 0;
}

int lua_getInteractable(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	Interactable* interactable = (*world)->GetComponent<Interactable>(entity);
	if (interactable) {
		registerInteractable(vm, interactable);
	}
	else {
		lua_pushnil(vm);
	}
	return 1;
}

int lua_removeInteractable(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	(*world)->RemoveComponent<Interactable>(entity);
	return 0;
}

int lua_getEntitiesWithInteractable(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");
	std::vector<uint32_t> entities = (*world)->GetEntitiesWith<Interactable>();

	if (entities.empty()) {
		lua_pushnil(vm);
	}
	else {
		registerEntities(vm, entities);
	}
	return 1;
}

int lua_addBlocker(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	bool blocks_view{ false };
	if (lua_isboolean(vm, 3)) {
		blocks_view = lua_toboolean(vm, 3);
	}

	(*world)->AddComponent<Blocker>(entity, blocks_view);

	return 0;
}

int lua_getBlocker(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	Blocker* blocker = (*world)->GetComponent<Blocker>(entity);
	if (blocker) {
		registerBlocker(vm, blocker);
	}
	else {
		lua_pushnil(vm);
	}
	return 1;
}

int lua_removeBlocker(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	(*world)->RemoveComponent<Blocker>(entity);
	return 0;
}

int lua_getEntitiesWithBlocker(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");
	std::vector<uint32_t> entities = (*world)->GetEntitiesWith<Blocker>();

	if (entities.empty()) {
		lua_pushnil(vm);
	}
	else {
		registerEntities(vm, entities);
	}
	return 1;
}

int lua_addItem(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	std::string name = luaL_checkstring(vm, 3);
	std::string desc = luaL_checkstring(vm, 4);
	int weight = luaL_checknumber(vm, 4);

	(*world)->AddComponent<Item>(entity, name, desc, weight);

	return 0;
}

int lua_getItem(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	Item* item = (*world)->GetComponent<Item>(entity);
	if (item) {
		registerItem(vm, item);
	}
	else {
		lua_pushnil(vm);
	}
	return 1;
}

int lua_removeItem(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");

	uint32_t entity = luaL_checknumber(vm, 2);
	(*world)->RemoveComponent<Item>(entity);
	return 0;
}

int lua_getEntitiesWithItem(lua_State* vm)
{
	World** world = (World**)luaL_checkudata(vm, 1, "World");
	std::vector<uint32_t> entities = (*world)->GetEntitiesWith<Item>();

	if (entities.empty()) {
		lua_pushnil(vm);
	}
	else {
		registerEntities(vm, entities);
	}
	return 1;
}