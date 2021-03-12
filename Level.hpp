#pragma once

#include "Common.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include "EntityGrid.hpp"

class Level
{
private:
	std::unique_ptr<Grid> grid;
	int width;
	int height;
public:
	Level();
	Level(int _width, int _height): grid(std::make_unique<Grid>(_width, _height)), width(_width), height(_height) {};
	Level(int _width, int _height, bool overworld) : grid(std::make_unique<Grid>(_width, _height, true)), width(_width), height(_height) {};
	~Level() {};

	inline Grid& get_grid() { return *(grid.get()); };
	inline const int get_width() const { return width; };
	inline const int get_height() const { return height; };
};

