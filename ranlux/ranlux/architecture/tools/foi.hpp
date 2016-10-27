// Functions Of Interest

#ifndef FOI_HPP
#define FOI_HPP

#pragma once

#include <Windows.h>

namespace foi {
	inline static void**& getvtable( void* inst, unsigned int offset = 0 ) {
		return *reinterpret_cast< void*** >( ( char* ) inst + offset );
	}

	inline const void** getvtable( const void* inst, size_t offset = 0 ) {
		return *reinterpret_cast< const void*** >( ( size_t ) inst + offset );
	}

	template< typename Fn >
	inline Fn getvfunc( const void* inst, size_t index, size_t offset = 0 ) {
		return reinterpret_cast< Fn >( getvtable( inst, offset )[ index ] );
	}

	// Gets a member of a class by offset
	template< typename Ty > inline Ty& getmember( void* ptr, intptr_t offset ) {
		return *reinterpret_cast< Ty* >( ( intptr_t ) ptr + offset );
	}

	template< typename Ty > inline const Ty& getmember( const void* ptr, intptr_t offset ) {
		return *reinterpret_cast< const Ty* >( ( intptr_t ) ptr + offset );
	}

	// Make a pointer by offset
	template< typename Ty > inline Ty make_ptr( void* ptr, intptr_t offset ) {
		return reinterpret_cast< Ty >( ( intptr_t ) ptr + offset );
	}
	template< typename Ty > inline const Ty make_ptr( const void* ptr, intptr_t offset ) {
		return reinterpret_cast< const Ty >( ( intptr_t ) ptr + offset );
	}

	// Assign data to whatever pointer
	template< typename Ty > inline Ty& assign( void* ptr, const Ty& data ) {
		*reinterpret_cast< Ty* >( ptr ) = data;
	}

	// Gets the address of a virtual function, offset is the offset of the vmt pointer
	template< typename Fn > inline Fn getvfunc( void* inst, intptr_t index, intptr_t offset = 0 ) {
		return reinterpret_cast< Fn >( getmember<void**>( inst, offset )[ index ] );
	}

	// Gets the offset between 2 pointers
	inline intptr_t getoffset( const void* inst, const void* ptr ) {
		return ( intptr_t ) ptr - ( intptr_t ) inst;
	}
	inline intptr_t getoffset( const void* inst, intptr_t ptr ) {
		return ptr - ( intptr_t ) inst;
	}
}

#endif // !FOI_HPP