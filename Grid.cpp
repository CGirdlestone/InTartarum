
#include "Grid.hpp"

void Grid::set_tile(int x, int y, bool _walkable, bool _blocks_view, TileType _type)
{
	auto& tile = get_tile(x, y);
	tile.set(_walkable, _blocks_view, _type);
}

