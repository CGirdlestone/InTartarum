#pragma once

#include "Common.hpp"

class Camera
{
private:
	int x, y;
	int width, height;
	int map_width, map_height;
public:
	Camera(int _x, int _y, int _width, int _height, int _map_width, int _map_height) :
		x(_x), y(_y), width(_width), height(_height), map_width(_map_width), map_height(_map_height) { };
	~Camera() { };

	inline const int get_width() const { return width; };
	inline const int get_height() const { return height; };
	inline const int get_map_width() const { return map_width; };
	inline const int get_map_height() const { return map_height; };
	inline const std::tuple<int, int> get_position() const { return std::make_tuple(x, y); };
	inline const std::tuple<int, int> viewport(int _x, int _y) const { return std::make_tuple(_x - x, _y - y); };
	inline void follow(int _x, int _y) {
		x = std::min(std::max(_x - width / 2, 0), map_width - width);
		y = std::min(std::max(_y - height / 2, 0), map_height - height);
	};
};

