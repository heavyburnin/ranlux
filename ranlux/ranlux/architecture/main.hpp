#ifndef MAIN_HPP
#define MAIN_HPP

#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>
#include <float.h>
#include <fstream>
#include <memory>
#include <vector>

#include "sdk\color.hpp"
#include "sdk\enums.hpp"
#include "tools\vmthook.hpp"
#include "tools\foi.hpp"
#include "useful\utility.hpp"
#include "sdk\vector.hpp"
#include "useful\netvar.hpp"
#include "sdk\sdk.hpp"
#include "useful\draw.hpp"
#include "useful\interfaces.hpp"
#include "hooks\hooks.hpp"
#include "sdk\angle.hpp"
#include "hooks\painttraverse.hpp"
#include "sdk\crc.hpp"
#include "hooks\createmove.hpp"
#include "useful\offsets.hpp"

// here's the external globals gonna be, so we don't get unresolved external error, stated as such: im way to used to using hungarian notation..
// extern IVEngine *g_pEngine; let's fuck it?

// needa move externals that doesn't belong here to their parent header file. That's when using namespaces, not using smart singletons like im doing now.
namespace globals {
	extern uintptr_t				clientmodule;
	extern vengineclient			*engine;
	extern vgui::isurface			*surface;
	extern vgui::ipanel				*panel;
	extern ibaseclient				*client;
	extern IClientMode				*clientmode;
	extern CEntityList				*entitylist;
	extern IVEngineServer			*engineserver;
	extern IVDebugOverlay			*debugoverlay;
	extern IGameEventManager2		*gameevents;
	extern local_player				_localplayer;

	extern std::vector<c_csplayer*>	playersinserver;

	extern Vector2D					screensize;
}
#endif // !MAIN_HPP