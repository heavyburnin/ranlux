#include "painttraverse.hpp"

painttraverse* painttraverse::_painttraverse = nullptr;

std::shared_ptr<painttraverse> painttraverse::get() {
	if( !_painttraverse )
		_painttraverse = new painttraverse();

	return std::shared_ptr<painttraverse>( _painttraverse );
}

bool painttraverse::pressed( int key ) {
	return ( GetAsyncKeyState( key ) & ( 1 << 15 ) ) != 0;
}

void __fastcall painttraverse::hkpainttraverse( vgui::ipanel *ecx, void *edx, vgui::vpanel vguipanel, bool forcerepaint, bool allowforce ) {

	const char* panelname = globals::panel->GetName( vguipanel );

	if( strstr( panelname, "FocusOverlayPanel" ) != 0 ) {
		// sorry, snapchat..
		// i was thinking, to optimize my hack, i can loop through all entities that are actually on the server, look now, i'll see if i can do it.

		c_csplayer *localplayer = globals::entitylist->get_cliententity( globals::engine->get_localplayer() );

		if( globals::engine->in_game() ){
			for( int i = 1; i <= 64; ++i ) {

				c_csplayer *entity = globals::entitylist->get_cliententity( i );

				if( entity == nullptr )
					continue;

				if( entity == localplayer )
					continue;

				if( entity->is_dormant() )
					continue;

				if( !entity->is_alive() )
					continue;

				int entteamnum = entity->teamnum();

				if( entteamnum == 1 )
					continue;

				if( entteamnum == 2 )
					globals::debugoverlay->AddBoxOverlayNew( entity->origin(), entity->bbmins(), entity->bbmaxs(), entity->get_absangles(), 255, 0, 0, 50 );
				else if( entteamnum == 3 )
					globals::debugoverlay->AddBoxOverlayNew( entity->origin(), entity->bbmins(), entity->bbmaxs(), entity->get_absangles(), 0, 50, 255, 50 );
			}
		}
	}

	globalhooks::panelhook.get<void( __thiscall* )( vgui::ipanel *, vgui::vpanel, bool, bool ) >( 41 )( ecx, vguipanel, forcerepaint, allowforce );
}

void painttraverse::drawesp( c_csplayer *local, c_csplayer *entity, int idx ) {

}

/*void get_actualmaxclients() {
	static bool once = false;

	if( !once ||  ) {
		for( int i = 1; i <= 64; ++i ) { // i'll just do it here.
			c_csplayer *entity = globals::entitylist->get_cliententity( i );
			if( entity == nullptr )
				continue;

			globals::playersinserver.push_back( entity );
		}
		once = true;
	}
}*/