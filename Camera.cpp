#include "Camera.hpp"

Camera::Camera(): x(0), y(0), width(0), height(0), map_width(0), map_height(0), zoom(0)
{
	SmartLuaVM vm(nullptr, &lua_close);
	vm.reset(luaL_newstate());
	auto result = luaL_dofile(vm.get(), "./Config/window.lua");

	if (result == LUA_OK) {
		lua_getglobal(vm.get(), "camera_x");
		if (lua_isnumber(vm.get(), -1)) {
			x = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "camera_y");
		if (lua_isnumber(vm.get(), -1)) {
			y = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "camera_width");
		if (lua_isnumber(vm.get(), -1)) {
			width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "camera_height");
		if (lua_isnumber(vm.get(), -1)) {
			height = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "map_width");
		if (lua_isnumber(vm.get(), -1)) {
			map_width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "map_height");
		if (lua_isnumber(vm.get(), -1)) {
			map_height = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "camera_zoom");
		if (lua_isnumber(vm.get(), -1)) {
			zoom = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
	}
	else {
		std::string error_msg = lua_tostring(vm.get(), -1);
		printf(error_msg.c_str());
	}
}

void Camera::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(x));
	utils::serialiseUint32(file, static_cast<uint32_t>(y));
}

void Camera::deserialise(const char* buffer, size_t offset)
{
	x = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	y = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}
