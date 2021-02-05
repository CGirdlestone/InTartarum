#pragma once

#include "Common.hpp"
#include "Level.hpp"

namespace Path {

    static const int SIDE_STEP = 10;
    static const int DIAGONAL_STEP = 14;

    struct ScentTuple{
    ScentTuple(int _x, int _y, int _scent): x(_x), y(_y), scent(_scent){};
    int x;
    int y;
    int scent;
    };

    class ScentMap {
    public:   
        ScentMap(int _width, int _height) : width(_width), height(_height) {
            for (int i = 0; i < width * height; i++) {
                // -1 represents either a wall OR a tile with no scent - either way, for the purposes of this, -1 is a tile which cannot be moved into or out of.
                map.push_back(-1); 
            }
        };
        inline const int scent(int x, int y) const { return map[x + y * width]; };
        inline void mark_tile(int x, int y, int scent_strength) { map[x + y * width] = scent_strength; };
        inline const bool is_marked(int x, int y) const { return map[x + y * width] != -1; };

        std::pair<int, int> track(int x, int y);
    private:
        int width;
        int height;
        std::vector<int> map;
    };

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
    static double gradient(int xi, int yi, int xf, int yf);
    static inline int sign(int x) { return x >= 0 ? 1 : -1; };
	std::vector<std::tuple<int, int> > a_star(Level& level, int xi, int yi, int xf, int yf);
    std::vector<std::tuple<int, int> > dijkstra(Level& level, int xi, int yi, int xf, int yf);
    std::vector<std::tuple<int, int> > breadth_first(Level& level, int xi, int yi, int xf, int yf);
    std::vector<std::tuple<int, int> > bresenham(Level& level, int xi, int yi, int xf, int yf);
    Path::ScentMap scent_map(Level& level, int xi, int yi, int num_steps);
    bool raycast(Level& level, int xi, int yi, int xf, int yf);

} // end Path