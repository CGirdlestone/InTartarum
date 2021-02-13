#pragma once

#include "Common.hpp"

namespace utils {

	int roll(std::string& die_roll, bool drop_lowest = false);
	inline size_t advance(size_t num_bytes) { return 8 * num_bytes; };
	void serialiseUint8(std::ofstream& file, uint8_t x);
	void serialiseUint16(std::ofstream& file, uint16_t x);
	void serialiseUint32(std::ofstream& file, uint32_t x);
	void serialiseUint64(std::ofstream& file, uint64_t x);
	void serialiseString(std::ofstream& file, std::string data);
	void serialiseVector(std::ofstream& file, std::vector<uint32_t>& data);
	uint8_t					deserialiseUint8(const char* buffer, size_t& offset);
	uint16_t				deserialiseUint16(const char* buffer, size_t& offset);
	uint32_t				deserialiseUint32(const char* buffer, size_t& offset);
	uint64_t				deserialiseUint64(const char* buffer, size_t& offset);
	std::string				deserialiseString(const char* buffer, size_t& offset);
	std::vector<uint32_t>	deserialiseVector(const char* buffer, size_t& offset);

	int read_lua_int(SmartLuaVM& vm, const char* key, int index);
	int read_lua_int(SmartLuaVM& vm, int key, int index);
	std::string read_lua_string(SmartLuaVM& vm, const char* key, int index);
	std::string read_lua_string(SmartLuaVM& vm, int key, int index);
}