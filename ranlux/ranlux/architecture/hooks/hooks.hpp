#pragma once

#ifndef HOOKS_HPP
#define HOOKS_HPP

#include "../main.hpp"

class hooks {

	hooks() {};
	static hooks *_hooks;

public:
	static std::shared_ptr<hooks> get();
	~hooks() { _hooks = nullptr; }

	static void __fastcall hkplaysound( void *ecx, void *edx, const char *sound );

	void setup();
	void unhook();
};

namespace globalhooks {
	extern vmthook::vmt panelhook;
	extern vmthook::vmt clientmodehook;
	extern vmthook::vmt surfacehook;
}

#endif // !HOOKS_HPP