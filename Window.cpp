#include "Window.hpp"

Window::Window() :
	context(nullptr, SDL_DestroyWindow), renderer(nullptr, SDL_DestroyRenderer), title(""), width(0), height(0),
	default_width(0), default_height(0), tile_width(0), tile_height(0), background({ 0,0,0 })
{
	SmartLuaVM vm(nullptr, &lua_close);
	vm.reset(luaL_newstate());
	auto result = luaL_dofile(vm.get(), "./Config/window.lua");

	if (result == LUA_OK) {

		lua_getglobal(vm.get(), "title");
		if (lua_isstring(vm.get(), -1)) {
			title = std::string(lua_tostring(vm.get(), -1));
		}

		lua_getglobal(vm.get(), "width");
		if (lua_isnumber(vm.get(), -1)) {
			width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}

		lua_getglobal(vm.get(), "height");
		if (lua_isnumber(vm.get(), -1)) {
			height = static_cast<int>(lua_tonumber(vm.get(), -1));
		}

		lua_getglobal(vm.get(), "tile_width");
		if (lua_isnumber(vm.get(), -1)) {
			tile_width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}

		lua_getglobal(vm.get(), "tile_height");
		if (lua_isnumber(vm.get(), -1)) {
			tile_height = static_cast<int>(lua_tonumber(vm.get(), -1));
		}

		lua_getglobal(vm.get(), "colour_r");
		if (lua_isnumber(vm.get(), -1)) {
			auto r = static_cast<uint8_t>(lua_tonumber(vm.get(), -1));
			background.r = r;
		}

		lua_getglobal(vm.get(), "colour_g");
		if (lua_isnumber(vm.get(), -1)) {
			auto g = static_cast<uint8_t>(lua_tonumber(vm.get(), -1));
			background.g = g;
		}

		lua_getglobal(vm.get(), "colour_b");
		if (lua_isnumber(vm.get(), -1)) {
			auto b = static_cast<uint8_t>(lua_tonumber(vm.get(), -1));
			background.b = b;
		}
		

	}
	else {
		std::string error_msg = lua_tostring(vm.get(), -1);
		printf(error_msg.c_str());
	}
	context.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * tile_width, height * tile_height, SDL_WINDOW_SHOWN));
	renderer.reset(SDL_CreateRenderer(context.get(), -1, SDL_RENDERER_ACCELERATED));

	SDL_SetRenderDrawColor(renderer.get(), background.r, background.g, background.b, 0);
}

Window::~Window()
{

}

