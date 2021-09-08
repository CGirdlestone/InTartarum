#include "LuaComponents.hpp"
#include "LuaUtilMacros.hpp"

void registerPosition(lua_State* vm, Position* pos)
{
	registerObject(Position, pos);
}

int __getPosition(lua_State* vm)
{
	Position** p = (Position**)luaL_checkudata(vm, 1, "PositionMetaTable");
	const char* k = luaL_checkstring(vm, 2);
	
	getVarInt(p, k, x); // if k == "x", push the value of p->x and return 1;
	getVarInt(p, k, y);
	getVarInt(p, k, z);
	getVarInt(p, k, world_x);
	getVarInt(p, k, world_y);

	// If we reach here, the lua string k doesn't match of p's members
	lua_pushnil(vm);
	return 1;
}

int __setPosition(lua_State* vm)
{
	Position** p = (Position**)luaL_checkudata(vm, 1, "PositionMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	setVarInt(p, k, x);
	setVarInt(p, k, y);
	setVarInt(p, k, z);
	setVarInt(p, k, world_x);
	setVarInt(p, k, world_y);

	return luaL_argerror(vm, 2, lua_pushfstring(vm, "invalid option '%s'", k));
}
void registerStat(lua_State* vm, Stat& stat)
{
	registerObject(Stat, &stat);
}

int __getStat(lua_State* vm)
{
	Stat** p = (Stat**)luaL_checkudata(vm, 1, "StatMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	getVarInt(p, k, base); // if k == "x", push the value of p->x and return 1;
	getVarInt(p, k, mod);
	getVarInt(p, k, buff);

	// If we reach here, the lua string k doesn't match of p's members
	lua_pushnil(vm);
	return 1;
}

int __setStat(lua_State* vm)
{
	Stat** p = (Stat**)luaL_checkudata(vm, 1, "StatMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	setVarInt(p, k, base);
	setVarInt(p, k, mod);
	setVarInt(p, k, buff);

	return luaL_argerror(vm, 2, lua_pushfstring(vm, "invalid option '%s'", k));
}

void registerFighter(lua_State* vm, Fighter* fighter)
{
	registerObject(Fighter, fighter);
}

int __getFighter(lua_State* vm)
{
	Fighter** f = (Fighter**)luaL_checkudata(vm, 1, "FighterMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	getVarInt(f, k, hp); // if k == "x", push the value of p->x and return 1;
	getVarInt(f, k, max_hp);
	getVarInt(f, k, defence);
	getVarInt(f, k, crit_mod);
	getTable(f, k, strength, Stat);
	getTable(f, k, dexterity, Stat);
	getTable(f, k, constitution, Stat);
	getTable(f, k, intelligence, Stat);
	getTable(f, k, wisdom, Stat);
	getTable(f, k, charisma, Stat);

	// If we reach here, the lua string k doesn't match of p's members
	lua_pushnil(vm);
	return 1;
}

int __setFighter(lua_State* vm)
{
	Fighter** p = (Fighter**)luaL_checkudata(vm, 1, "FighterMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	setVarInt(p, k, hp);
	setVarInt(p, k, max_hp);
	setVarInt(p, k, defence);
	setVarInt(p, k, crit_mod);

	return luaL_argerror(vm, 2, lua_pushfstring(vm, "invalid option '%s'", k));
}

void registerSprite(lua_State* vm, Sprite* sprite)
{
	registerObject(Sprite, sprite);
}

int __getSprite(lua_State* vm)
{
	Sprite** s = (Sprite**)luaL_checkudata(vm, 1, "SpriteMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	getVarInt(s, k, id); 
	getVarInt(s, k, clip_x);
	getVarInt(s, k, clip_y);
	getVarInt(s, k, width);
	getVarInt(s, k, height);
	getVarInt(s, k, depth);
	getVarInt(s, k, r);
	getVarInt(s, k, g);
	getVarInt(s, k, b);

	// If we reach here, the lua string k doesn't match of p's members
	lua_pushnil(vm);
	return 1;
}

int __setSprite(lua_State* vm)
{
	Sprite** s = (Sprite**)luaL_checkudata(vm, 1, "SpriteMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	setVarInt(s, k, id); 
	setVarInt(s, k, clip_x);
	setVarInt(s, k, clip_y);
	setVarInt(s, k, width);
	setVarInt(s, k, height);
	setVarInt(s, k, depth);
	setVarInt(s, k, r);
	setVarInt(s, k, g);
	setVarInt(s, k, b);

	return luaL_argerror(vm, 2, lua_pushfstring(vm, "invalid option '%s'", k));
}

void registerInteractable(lua_State* vm, Interactable* interactable)
{
	registerObject(Interactable, interactable);
}

int __getInteractable(lua_State* vm)
{
	Interactable** s = (Interactable**)luaL_checkudata(vm, 1, "InteractableMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	getVarBool(s, k, repeatable);
	getVarBool(s, k, triggered);

	// If we reach here, the lua string k doesn't match of p's members
	lua_pushnil(vm);
	return 1;
}

int __setInteractable(lua_State* vm)
{
	Interactable** s = (Interactable**)luaL_checkudata(vm, 1, "InteractableMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	setVarBool(s, k, repeatable);
	setVarBool(s, k, triggered);

	return 0;
}

void registerBlocker(lua_State* vm, Blocker* blocker)
{
	registerObject(Blocker, blocker);
}

int __getBlocker(lua_State* vm)
{
	Blocker** s = (Blocker**)luaL_checkudata(vm, 1, "BlockerMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	getVarBool(s, k, blocks_view);

	// If we reach here, the lua string k doesn't match of the members
	lua_pushnil(vm);
	return 1;
}

int __setBlocker(lua_State* vm)
{
	Blocker** s = (Blocker**)luaL_checkudata(vm, 1, "BlockerMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	setVarBool(s, k, blocks_view);

	return 0;
}

void registerItem(lua_State* vm, Item* item)
{
	registerObject(Item, item);
}

int __getItem(lua_State* vm)
{
	Item** s = (Item**)luaL_checkudata(vm, 1, "ItemMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	getVarStr(s, k, name);
	getVarStr(s, k, description);
	getVarInt(s, k, weight);

	// If we reach here, the lua string k doesn't match of the members
	lua_pushnil(vm);
	return 1;
}

int __setItem(lua_State* vm)
{
	Item** s = (Item**)luaL_checkudata(vm, 1, "ItemMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	setVarStr(s, k, name);
	setVarStr(s, k, description);
	setVarInt(s, k, weight);

	return 0;
}

void registerActor(lua_State* vm, Actor* actor)
{
	registerObject(Actor, actor);
}

int __getActor(lua_State* vm)
{
	Actor** s = (Actor**)luaL_checkudata(vm, 1, "ActorMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	getVarStr(s, k, name);
	getVarStr(s, k, description);

	// If we reach here, the lua string k doesn't match of the members
	lua_pushnil(vm);
	return 1;
}

int __setActor(lua_State* vm)
{
	Actor** s = (Actor**)luaL_checkudata(vm, 1, "ActorMetaTable");
	const char* k = luaL_checkstring(vm, 2);

	setVarStr(s, k, name);
	setVarStr(s, k, description);

	return 0;
}