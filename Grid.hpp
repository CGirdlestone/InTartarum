#pragma once

#include "Common.hpp"
#include "Tile.hpp"


class Grid {
private:
	std::unique_ptr<Tile[]> grid;
	int width{ 0 };
	int height{ 0 };

public:
	Grid(int _width, int _height) :grid(nullptr), width(_width), height(_height) { 
		grid = std::make_unique<Tile[]>(width * height);
		for (int i = 0; i < _width; i++) {
			for (int j = 0; j < _height; j++) {
				set_tile(i, j, true, false, TileType::EMPTY);
			}
		}
	}

	inline Tile& get_tile(int x, int y) { return grid.get()[x + y * width];};
	void set_tile(int x, int y, bool _walkable, bool _blocks_view, TileType _type);

	inline const int get_width() const { return width; };

	inline const int get_height() const { return height; };

	inline const bool in_bounds(int x, int y) const { if ((x < 0 || x >= width) || (y < 0 || y >= height)) { return false; } else { return true; } };
};