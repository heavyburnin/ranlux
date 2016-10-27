#ifndef CREATEMOVE_HPP
#define CREATEMOVE_HPP

#pragma once

#include "../main.hpp"

class createmove {
	createmove() {};
	static createmove *_createmove;

public:
	static std::shared_ptr<createmove> get();
	~createmove() { _createmove = nullptr; }

	static bool __fastcall hkcreatemove( void *ecx, int edx, float frametime, CUserCmd *cmd );
};

#endif // !CREATEMOVE_HPP