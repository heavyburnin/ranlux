#include "main.hpp" // i do .hpp, because this is the correct extension for c++.

// here's where im going to put all my global objects, stated as such: FUCKING PARKINSONS
// IVEngine *g_pEngine;

// now it should work.
namespace globals {
	uintptr_t				clientmodule;
	ibaseclient				*client;
	vengineclient			*engine;
	vgui::isurface			*surface;
	vgui::ipanel			*panel;
	IClientMode				*clientmode;
	CEntityList				*entitylist;
	IVEngineServer			*engineserver;
	IVDebugOverlay			*debugoverlay;
	IGameEventManager2		*gameevents;
	local_player			_localplayer;

	std::vector<c_csplayer*>	playersinserver;

	Vector2D				screensize;
}

bool setup() {
	// here's where you initialize everything, interfaces, netvars, etc..
	// if you're running a console in your cheat you can initialize configs.
	if( !globals::clientmodule )
		globals::clientmodule = ( uintptr_t ) ( GetModuleHandle( "client.dll" ) );

	// init the shit here
	interfacefactory::get()->setup();
	netvar::get()->setup(); // im fucking stupid..
	offsets::get()->setup();
	hooks::get()->setup();

	// still a nasty trick?
	while( globals::screensize.x == 0 && globals::screensize.y == 0 ) {
		globals::engine->get_screensize( globals::screensize.x, globals::screensize.y );
		Sleep( 500 );
	}

	return true;
}

// i'll make two modes of entrypoint, so one for debug purposes, where you can load and unload your module multiple times
// second for release purposes, so you don't get the debug symbols.
#ifdef _DEBUG
bool pressed( int key ) {
	return ( GetAsyncKeyState( key ) & ( 1 << 15 ) ) != 0;
} 

DWORD __stdcall startthread( void *param ) { // passing this dll, so we can shut down this thread if we choose to unload our dll.
	HMODULE module = static_cast< HMODULE >( param );

	// if StartUp succeeds, it will check if you're pressing ctrl and the numpad * and when you're not, it just sleeps a little. 
	// if you do press those two simultaneously, it will exit the while loop
	// and continue to exit the thread, if you've hooked something, you should destroy those hooks, and then you can exit the thread safely.
	if( setup() )
		while( !( pressed( 0xA2 ) && pressed( 0x6A ) ) ) // CTRL + NUMPAD *
			Sleep( 10 );

	hooks::get()->unhook();
	utility::get()->printtoconsole( "------------------" );
	utility::get()->printtoconsole( "\t\tunhooked\n" );
	utility::get()->printtoconsole( "------------------\n" );
	FreeLibraryAndExitThread( module, 0 );
}

// when you make a .dll project, you need to state dllmain as your entrypoint, otherwise it will not compile.
BOOL WINAPI DllMain( HINSTANCE hInst, uintptr_t dwReason, LPVOID pReserved ) {
	if( dwReason == DLL_PROCESS_ATTACH ) {
																		// if you're making a cheats' config system based on the cheats directory, you should grab the directory path here.
		::CreateThread( nullptr, 0, &startthread, hInst, 0, nullptr );	// here we create the thread our cheat will run on
		
	}

	return TRUE; // always return true.
}
#else
// when you make a .dll project, you need to state dllmain as your entrypoint, otherwise it will not compile.
BOOL WINAPI DllMain( HINSTANCE hInst, uintptr_t dwReason, LPVOID pReserved ) {
	if( dwReason == DLL_PROCESS_ATTACH ) { // i don't bother creating a thread n such. that's if you're gonna be trU int3rn4l :) this is ofc if you have a manual mapper, you can otherwise too, idk what i was thinking..
		// if you're making a cheats' config system based on the cheats directory, you should grab the directory path here.
		//globals::file->log( "suh" ); // first crash everr in this project :DD
		
		return setup();
	}

	return TRUE; // always return true.
}
#endif // _DEBUG