#pragma once

enum class TileType {
	EMPTY,
	STONE,
	STAIRS,
	DIRT_LOOSE,
	DIRT_MED,
	DIRT_HEAVY,
	DIRT_PACKED,
	GRASS_THIN,
	GRASS_MED,
	GRASS_THICK,
	WATER,
	TREE_01,
	TREE_02,
	TREE_03,
	TREE_04,
};

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



