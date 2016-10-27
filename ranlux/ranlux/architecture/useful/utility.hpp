#ifndef UTILITY_HPP
#define UTILITY_HPP

#pragma once

#include "..\main.hpp"

class utility { // first/second attempt at using smart singletons
	utility() {};
	static utility *_utility;

public:
	static std::shared_ptr<utility> get();
	~utility() { _utility = nullptr; }

	// findpattern that accepts mask within pattern shown as such:
	// 8B E8 ? ? ? ?
	template<typename rettype = uintptr_t> rettype findpattern( std::string module, const char *pattern );

	void printtoconsole( const char* szMsg, ... );

	bool pressed( int key );
};

#endif // !UTILITY_HPP