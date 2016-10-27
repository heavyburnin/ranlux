#ifndef OFFSETS_HPP
#define OFFSETS_HPP

#pragma once

#include "../main.hpp"

class offsets {
	offsets() {};
	static offsets *_offsets;

public:
	static std::shared_ptr<offsets> get();
	~offsets() { _offsets = nullptr; }

	void setup();

	uintptr_t GetClientCount;

	void IsReadyCallback();

	//void Host_Error( const char *error, ... );

};

#endif // !OFFSETS_HPP