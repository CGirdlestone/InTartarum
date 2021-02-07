#include "Pathfinding.hpp"

namespace Path{

std::pair<int, int> ScentMap::track(int x, int y) {
    auto compare = [](const ScentTuple& t1, const ScentTuple& t2) {
        return t1.scent > t2.scent;
    };

    std::priority_queue<ScentTuple, std::vector<ScentTuple>, decltype(compare)> tile_queue(compare);

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            auto _x{ x + i };
            auto _y{ y + j };
            if (!is_marked(_x, _y)) {
                continue;
            }
            auto _scent = scent(_x, _y);
            auto tile = ScentTuple(_x, _y, _scent);
            tile_queue.push(tile);
        }
    }

    auto& destination = tile_queue.top();
    auto coords = std::make_pair(destination.x, destination.y);
    return coords;
};


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
    
    return (node.x - x) * (node.y - y) == 0 ? node.g + SIDE_STEP : node.g + DIAGONAL_STEP;
}
static double gradient(int xi, int yi, int xf, int yf)
{
    return static_cast<double>(yf - yi) / static_cast<double>(xf - xi);
}


std::vector<std::tuple<int, int> > a_star(Level& level, int xi, int yi, int xf, int yf)
{
    auto& grid = level.get_grid();
    auto cmp_f = [](const std::shared_ptr<Node>& n1, const std::shared_ptr<Node>& n2) {
        return n1.get()->g + n1.get()->h > n2.get()->g + n2.get()->h;
    };

    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node> >, decltype(cmp_f)> open_queue(cmp_f);
    std::unordered_map<int, std::shared_ptr<Node> > closed;

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

std::vector<std::tuple<int, int>> dijkstra(Level& level, int xi, int yi, int xf, int yf)
{
    return std::vector<std::tuple<int, int>>();
}

std::vector<std::tuple<int, int>> breadth_first(Level& level, int xi, int yi, int xf, int yf)
{
    return std::vector<std::tuple<int, int>>();
}

std::vector<std::tuple<int, int>> bresenham(Level& level, int xi, int yi, int xf, int yf)
{
    auto& grid = level.get_grid();
    
    int x{ xi };
    int y{ yi };
    auto dx{ xf - xi };
    auto dy{ yf - yi };

    std::vector<std::tuple<int, int> > path;
    path.push_back(std::make_tuple(x, y));

    // handle special case of vertical lines to avoid division by zero in the gradient calculation.
    if (xi == xf) {
        for (int i = 0; i < std::abs(dy); i++) {
            path.push_back(std::make_tuple(x, y + sign(dy) * i));
        }
        path.push_back(std::make_tuple(xf, yf));
        return path;
    }

    auto m{ gradient(xi, yi, xf, yf) };
    float error{ 0 };

    // In octiles 2, 3, 6 and 7, so we do unit steps in x. 
    if (m >= -1.0 && m <= 1.0) {
        for (int i = 0; i < std::abs(dx); i++) {
            error += std::abs(m);
            if (error > 0.5) {
                y += sign(dy);
                error -= 1.0;
            }
            path.push_back(std::make_tuple(x + sign(dx) * i, y));
        }
    }
    // In octiles 1, 4, 5 and 8, so we do unit steps in y.
    else {
        m = 1.0 / m;
        for (int i = 0; i < std::abs(dy); i++) {
            error += std::abs(m);
            if (error > 0.5) {
                x += sign(dx);
                error -= 1.0;
            }
            path.push_back(std::make_tuple(x, y + sign(dy) * i));
        }
    }
    path.push_back(std::make_tuple(xf, yf));

    return path;
}

Path::ScentMap scent_map(Level& level, int xi, int yi, int num_steps)
{
    /* This generates a scent map (in effect a vector field) with values cascading downwards towards the point xi, yi.
    *  Values of -1 reflect tiles which are not possible to occupy OR tiles in which the scent has dropped off to untraceable levels.
    */

    auto& grid = level.get_grid();
    std::deque<std::shared_ptr<Node> > open_queue;

    ScentMap scent_map;

    auto start = std::make_shared<Node>(xi, yi, 0, 0, nullptr);
    open_queue.push_back(start);

    while (!open_queue.empty()) {
        auto current = open_queue.front();
        open_queue.pop_front();

        scent_map.mark_tile(current.get()->x, current.get()->y, current.get()->g);

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
                    scent_map.mark_tile(x, y, -1);
                    continue;
                }
                auto g = calculate_g(*current.get(), x, y);
                if (g > num_steps * SIDE_STEP) {
                    continue;
                }
                if (scent_map.is_marked(x, y)) {
                    if (g > scent_map.scent(x, y)) {
                        continue;
                    }
                }
                scent_map.mark_tile(x, y, g);
                auto child = std::make_shared<Node>(x, y, g, 0);
                open_queue.push_back(child);
            }
        }
    }

    return scent_map;
}

bool raycast(Level& level, int xi, int yi, int xf, int yf)
{
    /* This method casts a single ray from point (xi, yi) towards point (xf, yf), returning true if the ray reaches the final destination. 
    *  If there is an obstruction, defined as a tile which blocks view, it returns false. 
    */
    auto& grid = level.get_grid();
    double pi = 3.141592;

    double	ray_dir_x{ std::cos(std::atan2(static_cast<double>(yf - yi), static_cast<double>(xf - xi))) };
    double	ray_dir_y{ std::sin(std::atan2(static_cast<double>(yf - yi), static_cast<double>(xf - xi))) };

    int		ray_pos_x{ xi };
    int		ray_pos_y{ yi };

    double	delta_x{ 1 / std::abs(ray_dir_x) };
    double	delta_y{ 1 / std::abs(ray_dir_y) };

    int		step_x{ ray_dir_x < 0 ? -1 : 1 };
    int		step_y{ ray_dir_y < 0 ? -1 : 1 };

    double	dist_x{ delta_x };
    double	dist_y{ delta_y };

    bool	in_range{ true };

    int dx{ 0 };
    int dy{ 0 };

    while (ray_pos_x != xf && ray_pos_y != yf) {

        if (dist_x <= dist_y) {
            dist_x += delta_x;
            ray_pos_x += step_x;
            dx++;
        }
        else {
            dist_y += delta_y;
            ray_pos_y += step_y;
            dy++;
        }

        if (grid.in_bounds(ray_pos_x, ray_pos_y)) {
            if (grid.get_tile(ray_pos_x, ray_pos_y).blocks_view) {
                return false;
            }
        }
    }
    return true;
}

ScentMap::ScentMap()
{
    SmartLuaVM vm(nullptr, &lua_close);
    vm.reset(luaL_newstate());
    auto result = luaL_dofile(vm.get(), "./Config/window.lua");

    if (result == LUA_OK) {
        lua_getglobal(vm.get(), "map_width");
        if (lua_isnumber(vm.get(), -1)) {
            width = static_cast<int>(lua_tonumber(vm.get(), -1));
        }
        lua_getglobal(vm.get(), "map_height");
        if (lua_isnumber(vm.get(), -1)) {
            height = static_cast<int>(lua_tonumber(vm.get(), -1));
        }
    }
    else {
        std::string error_msg = lua_tostring(vm.get(), -1);
        printf(error_msg.c_str());
    }

    for (int i = 0; i < width * height; i++) {
        // -1 represents either a wall OR a tile with no scent - either way, for the purposes of this, -1 is a tile which cannot be moved into or out of.
        map.push_back(-1);
    }
}

} //end Path
