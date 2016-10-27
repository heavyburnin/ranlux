#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#pragma once

#include "../main.hpp"

class interfacefactory {
	interfacefactory() {};
	static interfacefactory *_interfacefactory;

	// global list of all interfaces in the game.
	struct globallist {
	public:
		void					*( *instinterfacefn )( );	// returns a ptr to the interface
		const char*				name;						// name of interface
		
		globallist*				next;						// next interface in global list

		void					*( *createfnnew ) ( const char *interfacename, int *returncode );
	};

	static globallist*		list;				// For the global list.
	globallist*				templist;

public:
	static std::shared_ptr<interfacefactory> get();
	~interfacefactory() { _interfacefactory = nullptr; }

	// constructor, also gets createinterface address of module
	interfacefactory( std::string modulename );

	// gets the actual interface
	template<typename classname> classname grabinterface( std::string interfacename, bool createfn = false );
	void									setup();
};

#endif // !INTERFACES_HPP