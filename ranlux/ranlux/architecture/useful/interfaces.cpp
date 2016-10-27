#include "interfaces.hpp"

interfacefactory::globallist *interfacefactory::list = nullptr;
interfacefactory* interfacefactory::_interfacefactory = nullptr;

std::shared_ptr<interfacefactory> interfacefactory::get() {
	if( !_interfacefactory )
		_interfacefactory = new interfacefactory();

	return std::shared_ptr<interfacefactory>( _interfacefactory );
}

typedef void* ( *CreateInterfaceFn )( const char *pName, int *pReturnCode );

interfacefactory::interfacefactory( std::string module ) {
	void *CI = GetProcAddress( GetModuleHandleA( module.c_str() ), "CreateInterface" );
	uintptr_t jmp = ( uintptr_t ) CI + 4;
	uintptr_t func = jmp + *( uintptr_t* ) ( jmp + 1 ) + 5;
	list = **( globallist*** ) ( func + 6 );
}

template<typename classname> classname interfacefactory::grabinterface( std::string interfacename, bool createfn ) {
	//if( createfn )
	//	return ( classname ) ( ( CreateInterfaceFn ) GetProcAddress( GetModuleHandleA( module.c_str() ), "CreateInterface" ) ) ( interfacename.c_str(), nullptr );
	
	templist = list;

	while( templist ) {
		if( !strncmp( interfacename.c_str(), templist->name, interfacename.length() ) ) {
			int ver = atoi( templist->name + interfacename.length() );

			if( ver > 0 )
				if( createfn )
					return ( classname ) templist->next->createfnnew( templist->name, nullptr );
				else
					return ( classname ) templist->instinterfacefn();
				
		}

		templist = templist->next;
	}

	return nullptr;
}

void interfacefactory::setup() {
	interfacefactory engine( "engine.dll" );
	globals::engine = engine.grabinterface<vengineclient*>( "VEngineClient" );
	globals::engineserver = engine.grabinterface<IVEngineServer*>( "VEngineServer" ); // so, let's try it.
	// just get the interface, idk actually
	globals::debugoverlay = engine.grabinterface<IVDebugOverlay*>( "VDebugOverlay" );
	//globals::gameevents = engine.grabinterface<IGameEventManager2*>( "GAMEEVENTSMANAGER", true );

	interfacefactory client( "client.dll" );
	globals::client = client.grabinterface<ibaseclient*>( "VClient" );
	globals::entitylist = client.grabinterface<CEntityList*>( "VClientEntityList" );

	interfacefactory vguimatsurface( "vguimatsurface.dll" );
	globals::surface = vguimatsurface.grabinterface<vgui::isurface*>( "VGUI_Surface" );

	interfacefactory vgui2( "vgui2.dll" );
	globals::panel = vgui2.grabinterface<vgui::ipanel*>( "VGUI_Panel" );
}