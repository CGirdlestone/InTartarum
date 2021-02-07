#include "Level.hpp"

Level::Level() : grid(nullptr), width(0), height(0)
{
	SmartLuaVM vm(nullptr, &lua_close);
	vm.reset(luaL_newstate());
	auto result = luaL_dofile(vm.get(), "./Config/window.lua");

	if (result == LUA_OK) {
		lua_getglobal(vm.get(), "map_width");
		if (lua_isnumber(vm.get(), -1)) {
			width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "map_height");
		if (lua_isnumber(vm.get(), -1)) {
			height = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
	}
	else {
		std::string error_msg = lua_tostring(vm.get(), -1);
		printf(error_msg.c_str());
	}

	auto _grid = std::make_unique<Grid>(width, height);
	grid.swap(_grid);
}
