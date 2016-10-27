#include "offsets.hpp"

offsets *offsets::_offsets = nullptr;

std::shared_ptr<offsets> offsets::get() {
	if( !_offsets )
		_offsets = new offsets();

	return std::shared_ptr<offsets>( _offsets );
}

void offsets::setup() {

	globals::clientmode = **foi::make_ptr<IClientMode***>( foi::getvtable( globals::client )[ 10 ], 5 );
	GetClientCount = utility::get()->findpattern( "engine.dll", "3B 0D ? ? ? ? 7F 12" + 2 ) - globals::clientmodule;


}

void offsets::IsReadyCallback() {
	static uintptr_t addr = utility::get()->findpattern( "client.dll", "55 8B EC 51 56 8B 35 ? ? ? ? 8B 4E 58" );

	reinterpret_cast< void( __thiscall* ) ( ) >( addr )( );
}