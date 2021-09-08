#pragma once

#define makeStr(x) #x
#define concat(x, y) makeStr(x ## y)

#define setUpMetatable(name) \
	do { \
		luaL_newmetatable(vm, #name); \
		lua_pushvalue(vm, -1); \
		lua_setfield(vm, -2, "__index"); \
	} while (false)

#define registerMethod(name) \
	do { \
		lua_pushcfunction(vm, lua_##name);\
		lua_setfield(vm, -2, #name); \
	} while (false)

#define registerObject(type, object) \
	do {\
		type** c = (type**)lua_newuserdata(vm, sizeof(type*)); \
		*c = object; \
		luaL_newmetatable(vm, concat(type, MetaTable)); \
		lua_pushcfunction(vm, __get##type); \
		lua_setfield(vm, -2, "__index"); \
		lua_pushcfunction(vm, __set##type); \
		lua_setfield(vm, -2, "__newindex"); \
		lua_pushstring(vm, #type); \
		lua_setfield(vm, -2, "__metatable"); \
		lua_setmetatable(vm, -2); \
	} while (false)

#define getVarInt(component, lua_var, var) \
	do {\
		if(!strcmp(lua_var, #var)){\
			lua_pushinteger(vm, (*component)->var); \
			return 1; \
		} \
	} while (false)

#define setVarInt(component, lua_var, var) \
	do {\
		if (!strcmp(lua_var, #var)) { \
			(*component)->var = luaL_checknumber(vm, 3); \
			return 0; \
		} \
	} while (false)


#define getVarStr(component, lua_var, var) \
	do {\
		if(!strcmp(lua_var, #var)){\
			lua_pushstring(vm, (*component)->var.c_str()); \
			return 1; \
		} \
	} while (false)

#define setVarStr(component, lua_var, var) \
	do {\
		if (!strcmp(lua_var, #var)) { \
			(*component)->var = luaL_checkstring(vm, 3); \
			return 0; \
		} \
	} while (false)

#define getVarBool(component, lua_var, var) \
	do {\
		if(!strcmp(lua_var, #var)){\
			lua_pushboolean(vm, (*component)->var); \
			return 1; \
		} \
	} while (false)

#define setVarBool(component, lua_var, var) \
	do {\
		if (!strcmp(lua_var, #var)) { \
			if (lua_isboolean(vm, 3)){ \
				(*component)->var = lua_toboolean(vm, 3); \
			}\
			return 0; \
		} \
	} while (false)

#define getTable(component, lua_var, var, type) \
	do { \
		if (!strcmp(lua_var, #var)) { \
			register ## type(vm, (*component)->var); \
		} \
		return 1; \
	} while (false)
