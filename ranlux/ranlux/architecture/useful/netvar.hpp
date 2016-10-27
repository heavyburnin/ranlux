#ifndef NETVAR_HPP
#define NETVAR_HPP

#pragma once

#include "../main.hpp"

class recvtable;

class netvar {
	netvar() {};
	static netvar *_netvar;

public:
	static std::shared_ptr<netvar> get();
	~netvar() { _netvar = nullptr; }

	void setup();

	//static uintptr_t recurse( recvtable *table, const char *propname, uintptr_t addoffset );
	//uintptr_t get_netvar( const std::string& table, const char *prop );
};

#endif // !NETVAR_HPP