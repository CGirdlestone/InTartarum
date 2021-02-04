#pragma once

#include "Common.hpp"
#include "Level.hpp"

namespace Path {

    struct Node
    {
        Node(int _x, int _y, int _g, int _h, Node* _parent = nullptr) :
            x(_x), y(_y), g(_g), h(_h), parent(_parent) { };
        Node(const Node&) = delete;
        ~Node() {};
        int x;
        int y;
        int g;
        int h;
        Node* parent;

        bool operator==(const Node& other) {
            return other.x == x && other.y == y;
        }

        bool operator<(const Node& other) {
            return other.g < g;
        }
    };

	static int heuristic(int xi, int yi, int xf, int yf);
	static int calculate_g(const Node& node, int x, int y);
	std::vector<std::tuple<int, int> > a_star(Level& level, int xi, int yi, int xf, int yf);
}