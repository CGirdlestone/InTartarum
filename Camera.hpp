#pragma once

#include "Common.hpp"
#include "Window.hpp"
#include "Utils.hpp"

class Camera
{
private:
	Window& window;
	int x, y;
	int offset_x, offset_y;
	int width, height;
	int map_width, map_height;
	int zoom{ 1 };
public:
	Camera(Window& _window);
	~Camera() { };

	inline const int get_width() const { return width; };
	inline const int get_height() const { return height; };
	inline void set_width(int _width) { width = _width; };
	inline void set_height(int _height) { width = _height; };
	inline const int get_offset_x() const { return offset_x; };
	inline const int get_offset_y() const { return offset_y; };
	inline const int get_map_width() const { return map_width; };
	inline const int get_map_height() const { return map_height; };
	inline const std::tuple<int, int> get_position() const { return std::make_tuple(x, y); };
	inline const std::tuple<int, int> viewport(int _x, int _y) const { return std::make_tuple(_x - x, _y - y); };
	inline void follow(int _x, int _y) {
		x = std::min(std::max(_x - width / zoom / 2, 0), map_width - width / zoom);
		y = std::min(std::max(_y - height / zoom / 2, 0), map_height - height / zoom);
	};
	inline const int get_zoom() const { return zoom; };
	void serialise(std::ofstream& file);
	void deserialise(const char* buffer, size_t offset);
};

