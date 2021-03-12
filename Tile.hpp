#pragma once

#include "TileType.hpp"

struct Tile {
	bool walkable{ true };
	bool explored{ false };
	bool blocks_view{ false };
	bool visible{ false };
	TileType type{ TileType::GRASS };

	inline void set(bool _walkable, bool _blocks_view, TileType _type) {
		walkable = _walkable;
		blocks_view = _blocks_view;
		type = _type;
	};

	inline void set(bool _walkable, bool _blocks_view, bool _explored, TileType _type) {
		walkable = _walkable;
		blocks_view = _blocks_view;
		explored = _explored;
		type = _type;
	};
};



