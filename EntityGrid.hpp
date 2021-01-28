#pragma once

#include "Common.hpp"

class EntityGrid {
private:
	int width;
	int height;
	std::map<int, std::vector<uint32_t> > grid;
public:
	EntityGrid(int _width, int _height) : width(_width), height(_height) {
		for (int i = 0; i < _width * _height; i++) {
			grid.insert({ i, {} });
		}
	}

	inline std::vector<uint32_t>& get(int x, int y) {
		return grid.at(x + y * width);
	}

	inline bool is_empty(int x, int y) const {
		return grid.at(x + y * width).empty();
	}

	inline void add_entity(uint32_t entity, int x, int y) {
		grid.at(x + y * width).push_back(entity);
	}

	inline void remove_entity(uint32_t entity, int x, int y) {
		auto& g = grid.at(x + y * width);
		g.erase(std::remove_if(g.begin(), g.end(), [entity](const uint32_t e) {return e == entity; }), g.end());
	}

	void clear() {
		for (auto& [i, vec] : grid) {
			vec.clear();
		}
	}
};