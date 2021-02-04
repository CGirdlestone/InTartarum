#include "Pathfinding.hpp"

namespace Path{

static int heuristic(int xi, int yi, int xf, int yf)
{
    // octile heuristic
    int D = 10;
    int D2 = 14;
    auto dx = std::abs(xi - xf);
    auto dy = std::abs(yi - yf);
    return D * (dx + dy) + (D2 - 2 * D) * std::min(dx, dy);
};

static int calculate_g(const Node& node, int x, int y)
{
    const int SIDE_STEP = 10;
    const int DIAGONAL_STEP = 14;
    return (node.x - x) * (node.y - y) == 0 ? node.g + SIDE_STEP : node.g + DIAGONAL_STEP;
};

std::vector<std::tuple<int, int> > a_star(Level& level, int xi, int yi, int xf, int yf)
{
    auto& grid = level.get_grid();
    auto cmp_f = [](const std::shared_ptr<Node>& n1, const std::shared_ptr<Node>& n2) {
        return n1.get()->g + n1.get()->h > n2.get()->g + n2.get()->h;
    };

    std::vector<std::shared_ptr<Node> > closed_array(grid.get_width() * grid.get_height(), nullptr);

    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node> >, decltype(cmp_f)> open_queue(cmp_f);
    std::map<int, std::shared_ptr<Node> > closed;

    auto start = std::make_shared<Node>(xi, yi, 0, heuristic(xi, yi, xf, yf));

    open_queue.push(std::move(start));
    while (!open_queue.empty()) {
        auto current = open_queue.top();
        closed.insert({ current->x + current->y * grid.get_width(), current });
        open_queue.pop();

        if (current.get()->x == xf && current.get()->y == yf) {
            // arrived at target
            std::vector<std::tuple<int, int> > path;
            auto* node = current.get();
            while (node->parent != nullptr) {
                path.push_back(std::make_tuple(node->x, node->y));
                node = node->parent;
            }
            return path;
        }

        // run through neighbours
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                auto x = current.get()->x + i;
                auto y = current.get()->y + j;
                if (i == 0 && j == 0) {
                    continue;
                }
                if (!grid.in_bounds(x, y)) {
                    continue;
                }
                if (!(grid.get_tile(x, y).walkable)) {
                    continue;
                }

                auto g = calculate_g(*(current.get()), x, y);
                auto h = heuristic(x, y, xf, yf);
                auto child = std::make_shared<Node>(x, y, g, h, current.get());
                auto key = child.get()->x + child.get()->y * grid.get_width();

                auto it = closed.find(key);

                if (it == closed.end()) {
                    open_queue.push(child);
                }
                else {
                    if (child.get()->g < it->second->g) {
                        open_queue.push(child);
                        closed.at(key) = child;
                    }
                }
            }
        }
    }
}

} //end Path
