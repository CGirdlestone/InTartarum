#pragma once

#include "TileType.hpp"

struct Tile {
	bool walkable{ true };
	bool explored{ false };
	bool blocks_view{ false };
	bool visible{ false };
	TileType type{ TileType::EMPTY };

	inline void set(bool _walkable, bool _blocks_view, TileType _type) {
		walkable = _walkable;
		blocks_view = _blocks_view;
		type = _type;
	};
};



