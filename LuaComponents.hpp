#pragma once

#include "Lua542/include/lua.hpp"
#include "Components.hpp"

#define component(type) void register ## type(lua_State* vm, type* c);\
						int __get ## type(lua_State* vm); \
						int __set ## type(lua_State* vm); \

component(Position)
component(Stat)
component(Fighter)
component(Sprite)
component(Interactable)
component(Blocker)
component(Item)
component(Actor)