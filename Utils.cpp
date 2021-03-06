#pragma once

#include "Utils.hpp"

namespace utils {
    int getAttributeMod(int attrib)
    {
        return (attrib - 10) / 2;
    }

    int roll(std::string& die_roll, bool drop_lowest) 
    {
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

    void serialiseUint8(std::ofstream& file, uint8_t x)
    {
        file.write(reinterpret_cast<char*>(&x), sizeof(reinterpret_cast<char*>(&x)));
    }

    void serialiseUint16(std::ofstream& file, uint16_t x)
    {
        uint8_t bytes[2];
        for (int i = 0; i < 2; i++) {
            bytes[i] = (x >> (1 - i) * 8) & 0xFF;
        }

        for (int i = 0; i < 2; ++i) {
            file.write(reinterpret_cast<char*>(&bytes[i]), sizeof(reinterpret_cast<char*>(&bytes[i])));
        }
    }

    void serialiseUint32(std::ofstream& file, uint32_t x)
    {
        uint8_t bytes[4];
        bytes[0] = ((x >> 24) & 0xFF);
        bytes[1] = ((x >> 16) & 0xFF);
        bytes[2] = ((x >> 8) & 0xFF);
        bytes[3] = ((x >> 0) & 0xFF);

        for (int i = 0; i < 4; ++i) {
            file.write(reinterpret_cast<char*>(&bytes[i]), sizeof(reinterpret_cast<char*>(&bytes[i])));
        }
    }

    void serialiseUint64(std::ofstream& file, uint64_t x)
    {
        uint8_t bytes[8];
        for (int i = 0; i < 7; i++) {
            bytes[i] = (x >> (7 - i) * 8) & 0xFF;
        }

        for (int i = 0; i < 8; ++i) {
            file.write(reinterpret_cast<char*>(&bytes[i]), sizeof(reinterpret_cast<char*>(&bytes[i])));
        }
    }

    void serialiseString(std::ofstream& file, std::string data)
    {
        uint32_t string_length = static_cast<uint32_t>(data.length());
        serialiseUint32(file, string_length);

        for (uint32_t i = 0; i < string_length; i++) {
            serialiseUint8(file, static_cast<uint8_t>(data[i]));
        }
    }

    void serialiseVector(std::ofstream& file, const std::vector<uint32_t>& data)
    {
        uint32_t num_elements = static_cast<uint32_t>(data.size());
        serialiseUint32(file, num_elements);

        for (uint32_t i = 0; i < num_elements; i++) {
            serialiseUint32(file, data[i]);
        }
    }

    void serialiseVectorBool(std::ofstream& file, const std::vector<bool>& data)
    {
        uint32_t num_elements = static_cast<uint32_t>(data.size());
        serialiseUint32(file, num_elements);

        for (uint32_t i = 0; i < num_elements; i++) {
            serialiseUint32(file, static_cast<uint32_t>(data[i]));
        }
    }

    uint8_t deserialiseUint8(const char* buffer, size_t& offset)
    {
        uint8_t value = static_cast<unsigned char>(buffer[offset + 0]);

        offset += utils::advance(1);

        return value;
    }

    uint16_t deserialiseUint16(const char* buffer, size_t& offset)
    {
        uint16_t value = (  static_cast<unsigned char>(buffer[offset + 0]) << 8 |
                            static_cast<unsigned char>(buffer[offset + 1])
                            );
        offset += utils::advance(2);

        return value;
    }

    uint32_t deserialiseUint32(const char* buffer, size_t& offset)
    {
        uint32_t value = (  static_cast<unsigned char>(buffer[offset + 0]) << 24 |
                            static_cast<unsigned char>(buffer[offset + 1]) << 16 |
                            static_cast<unsigned char>(buffer[offset + 2]) << 8 |
                            static_cast<unsigned char>(buffer[offset + 3])
                            );
        offset += utils::advance(4);

        return value;
    }

    uint64_t deserialiseUint64(const char* buffer, size_t& offset)
    {
        uint64_t value = (  static_cast<unsigned char>(buffer[offset + 0]) << 56 |
                            static_cast<unsigned char>(buffer[offset + 1]) << 48 |
                            static_cast<unsigned char>(buffer[offset + 2]) << 40 |
                            static_cast<unsigned char>(buffer[offset + 3]) << 32 |
                            static_cast<unsigned char>(buffer[offset + 4]) << 24 |
                            static_cast<unsigned char>(buffer[offset + 5]) << 16 |
                            static_cast<unsigned char>(buffer[offset + 6]) << 8 |
                            static_cast<unsigned char>(buffer[offset + 7])
                            );
        offset += utils::advance(8);

        return value;
    }

    std::string deserialiseString(const char* buffer, size_t& offset)
    {
        std::string _str{ "" };
        uint32_t _strLength = utils::deserialiseUint32(buffer, offset);

        /* Read in the entity's name */
        char* str_arr = new char[_strLength];
        for (uint32_t k = 0; k < _strLength; ++k) {
            uint8_t letterCode = utils::deserialiseUint8(buffer, offset);
            str_arr[k] = static_cast<char>(letterCode);
        }
        _str.assign(str_arr, _strLength);
        delete[] str_arr;

        return _str;
    }

    std::vector<uint32_t> deserialiseVector(const char* buffer, size_t& offset)
    {
        std::vector<uint32_t> contents;

        uint32_t num_elements = utils::deserialiseUint32(buffer, offset);

        for (uint32_t i = 0; i < num_elements; i++) {
            uint32_t element = deserialiseUint32(buffer, offset);
            contents.push_back(element);
        }

        return contents;
    }

    std::vector<bool> deserialiseVectorBool(const char* buffer, size_t& offset)
    {
        std::vector<bool> contents;

        uint32_t num_elements = utils::deserialiseUint32(buffer, offset);

        for (uint32_t i = 0; i < num_elements; i++) {
            bool element = static_cast<bool>(deserialiseUint32(buffer, offset));
            contents.push_back(element);
        }

        return contents;
    }

    int read_lua_int(SmartLuaVM& vm, const char* key, int index)
    {
        lua_pushstring(vm.get(), key);
        lua_gettable(vm.get(), index);
        if (!lua_isnumber(vm.get(), -1)) {
            lua_pop(vm.get(), 1);
            return -1;
        }
        auto val = lua_tointeger(vm.get(), -1);
        lua_pop(vm.get(), 1);

        return static_cast<int>(val);
    }

    int read_lua_int(SmartLuaVM& vm, int key, int index)
    {
        lua_pushnumber(vm.get(), key);
        lua_gettable(vm.get(), index);
        if (!lua_isnumber(vm.get(), -1)) {
            lua_pop(vm.get(), 1);
            return -1;
        }
        auto val = lua_tointeger(vm.get(), -1);
        lua_pop(vm.get(), 1);

        return static_cast<int>(val);
    }

    float read_lua_float(SmartLuaVM& vm, const char* key, int index)
    {
        lua_pushstring(vm.get(), key);
        lua_gettable(vm.get(), index);
        if (!lua_isnumber(vm.get(), -1)) {
            lua_pop(vm.get(), 1);
            return -1;
        }
        auto val = static_cast<float>(lua_tonumber(vm.get(), -1));
        lua_pop(vm.get(), 1);

        return val;
    }

    float read_lua_float(SmartLuaVM& vm, int key, int index)
    {
        lua_pushnumber(vm.get(), key);
        lua_gettable(vm.get(), index);
        if (!lua_isnumber(vm.get(), -1)) {
            lua_pop(vm.get(), 1);
            return -1;
        }
        auto val = static_cast<float>(lua_tonumber(vm.get(), -1));
        lua_pop(vm.get(), 1);

        return val;
    }

    bool read_lua_bool(SmartLuaVM& vm, const char* key, int index)
    {
        lua_pushstring(vm.get(), key);
        lua_gettable(vm.get(), index);
        if (!lua_isboolean(vm.get(), -1)) {
            lua_pop(vm.get(), 1);
            return -1;
        }
        auto val = lua_toboolean(vm.get(), -1);
        lua_pop(vm.get(), 1);

        return val;
    }

    bool read_lua_bool(SmartLuaVM& vm, int key, int index)
    {
        lua_pushnumber(vm.get(), key);
        lua_gettable(vm.get(), index);
        if (!lua_isboolean(vm.get(), -1)) {
            lua_pop(vm.get(), 1);
            return -1;
        }
        auto val = lua_toboolean(vm.get(), -1);
        lua_pop(vm.get(), 1);

        return val;
    }

    std::string read_lua_string(SmartLuaVM& vm, const char* key, int index)
    {
        lua_pushstring(vm.get(), key);
        lua_gettable(vm.get(), index);
        if (!lua_isstring(vm.get(), -1)) {
            lua_pop(vm.get(), 1);
            return std::string(" ");
        }
        auto s = std::string(lua_tostring(vm.get(), -1));
        lua_pop(vm.get(), 1);

        return s;
    }

    std::string read_lua_string(SmartLuaVM& vm, int key, int index)
    {
        lua_pushnumber(vm.get(), key);
        lua_gettable(vm.get(), index);
        if (!lua_isstring(vm.get(), -1)) {
            lua_pop(vm.get(), 1);
            return std::string(" ");
        }
        auto val = lua_tostring(vm.get(), -1);
        lua_pop(vm.get(), 1);

        return val;
    }
};

