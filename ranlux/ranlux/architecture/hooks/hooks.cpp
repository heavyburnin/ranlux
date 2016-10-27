#include "hooks.hpp"

namespace globalhooks {
	vmthook::vmt panelhook;
	vmthook::vmt clientmodehook;
	vmthook::vmt surfacehook;
}

hooks* hooks::_hooks = nullptr;

std::shared_ptr<hooks> hooks::get() {
	if( !_hooks )
		_hooks = new hooks();

	return std::shared_ptr<hooks>( _hooks );
}

void __fastcall hooks::hkplaysound( void *ecx, void *edx, const char *sound ) {
	globalhooks::surfacehook.get< void( __thiscall* ) ( void *ecx, const char *sound ) >( 82 )( ecx, sound );

	if( strstr( sound, "weapons/hegrenade/beep.wav" ) != 0 && !globals::engine->in_game() )
		offsets::get()->IsReadyCallback();
}

void hooks::setup() {
	std::nullptr_t *nullptrone;

	globalhooks::clientmodehook.init( globals::clientmode );
	globalhooks::clientmodehook.hook( createmove::hkcreatemove, 24 );
	globalhooks::clientmodehook.rehook();

	globalhooks::surfacehook.init( globals::surface );
	globalhooks::surfacehook.hook( hooks::hkplaysound, 82 );
	globalhooks::surfacehook.rehook();

	globalhooks::panelhook.init( globals::panel );
	globalhooks::panelhook.hook( painttraverse::hkpainttraverse, 41 );
	globalhooks::panelhook.rehook();
}

__declspec( noinline ) void destroyhook( vmthook::vmt hook ) {
	if( !hook.hooked() || !hook.is_initialized() )
		return;

	hook.kill();
}

void hooks::unhook() {
	destroyhook( globalhooks::panelhook );
	destroyhook( globalhooks::surfacehook );
	destroyhook( globalhooks::clientmodehook );
}