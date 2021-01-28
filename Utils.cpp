#pragma once

#include "Utils.hpp"

namespace utils {
	int roll(std::string& die_roll, bool drop_lowest) {
        /* This calculates the total score for rolling 'x' number of 'm'-sided dice. */

        auto pos_d = die_roll.find("d");
        auto num_rolls = std::stoi(die_roll.substr(0, pos_d));
        auto die_size = std::stoi(die_roll.substr(pos_d + 1));

        std::vector<int> rolls;
        do {
            auto r = rand() % die_size + 1;
            rolls.push_back(r);
            num_rolls--;
        } while (num_rolls > 0);

        if (drop_lowest) {
            std::sort(rolls.begin(), rolls.end());
            rolls.erase(rolls.begin());
        }

        auto result{ 0 };
        for (auto n : rolls) {
            result += n;
        }

        return result;
	}
}