#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <stdint.h>
#include <string>
#include <map>
#include <array>
#include <vector>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <fstream>
#include <math.h>
#include <deque>
#include <set>
#include <unordered_map>
#include <queue>
#include <thread>
#include <random>

#include "Lua542/include/lua.hpp"

const int TILE_SIZE{ 16 };
const int MAX_ENTITIES{ 16382 }; // (2^14 - 1) - 1
using SmartLuaVM = std::unique_ptr<lua_State, decltype(&lua_close)>;

inline const bool is_null(uint32_t entity) { return entity == MAX_ENTITIES + 1; };