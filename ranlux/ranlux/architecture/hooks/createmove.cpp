#include "createmove.hpp"

createmove* createmove::_createmove = nullptr;

std::shared_ptr<createmove> createmove::get() {
	if( !_createmove )
		_createmove = new createmove();

	return std::shared_ptr<createmove>( _createmove );
}

bool __fastcall createmove::hkcreatemove( void *ecx, int edx, float frametime, CUserCmd *cmd ) {
	if( cmd == nullptr ) // im so stupid..
		return true;

	if( !cmd->command_number )
		return true;

	globals::_localplayer.set_frametime( frametime + 0.001f );

	/*if( globalhooks::clientmodehook.get< bool( __thiscall* ) ( void *, float, CUserCmd * ) >( 24 )( ecx, frametime, cmd ) )
		globals::engine->set_viewangles( cmd->viewangles );*/

	c_csplayer *localplayer = globals::entitylist->get_cliententity( globals::engine->get_localplayer() );

	if( localplayer == nullptr || !localplayer->is_alive() )
		return true;


	if( cmd->buttons&IN_JUMP ) // bhop
		if( !( localplayer->flags() & FL_ONGROUND ) )
			cmd->buttons &= ~IN_JUMP;
	
	return false;
}
