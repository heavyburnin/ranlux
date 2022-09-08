#ifndef VMT_HPP
#define VMT_HPP

#pragma once

#include <assert.h>

#include "../main.hpp"
#include "FOI.hpp"

// credits: Casual_Hacker, modified & commented (some of it) by heavyburnin 
namespace vmthook {u
	//----------------------------------------------------------------
	// Class: VMTManager
	//----------------------------------------------------------------
	// Hooks virtual functions by replacing the vtable pointer from an instance.
	// VMTManager is a RAII version
	class vmt {
	protected:
		void ***vftable;
		void **oldvmt;
		void **arrofvfuncs;
		void *userdata;

		inline static void**& getvtbl( void* inst, unsigned int offset ) {
			return *reinterpret_cast< void*** >( ( char* ) inst + offset );
		}

	public:
		// Guard identifies if an instance is hooked.
		//enum { GUARD = __COUNTER__ * 0xDE4DB3EF + 12345 }; // This should be random for each project... But is this safe?
		enum { GUARD = 0xD34DB33F }; // i won't do it now, cause i don't have a need for it

		//-----------------------------------------------------------------------------
		// CountFuncs( void** vmt )
		//-----------------------------------------------------------------------------
		// Counts the number of virtual functions in a virtual table
		inline unsigned int count_vfuncs( void **inst ) {
			unsigned int i = -1;
			do ++i; while( inst[ i ] );
			return i;
		}

		//-----------------------------------------------------------------------------
		// Init( void *inst, unsigned int offset, unsigned int vfuncs )
		//-----------------------------------------------------------------------------
		// Initializes vmt object with specified class
		// Can specify offset from class
		// Can also specify amount of vfuncs the table has.
		__declspec( noinline ) vmt& init( void *inst, unsigned int offset = 0, unsigned int vfuncs = 0 ) {
			vftable = &getvtbl( inst, offset );
			oldvmt = *vftable;
			// Count vfuncs ourself if needed
			if( !vfuncs ) {
				vfuncs = count_vfuncs( oldvmt );
#ifdef _DEBUG
				assert( vfuncs >= 1 );
#endif // _DEBUG
			}
			// Allocate room for the new vtable + 3 places before + 1 place after for a null ptr
			void** arr = arrofvfuncs = ( void** ) ::malloc( ( vfuncs + 4 ) * sizeof( void* ) );
			arr[ 0 ] = this; // Ptr back to the hook object
			arr[ 1 ] = ( void* ) GUARD; // Guard tells us if the vtable is hooked
			( arr + 3 )[ vfuncs ] = nullptr; // Marks the end of the vtable
											 // Copy over the other vfuncs + RTTI ptr
			{
				unsigned int i = -1;
				do arr[ i + 3 ] = oldvmt[ i ];
				while( ++i < vfuncs );
			}

			return *this;
		}

		//-----------------------------------------------------------------------------
		// HookPresent( void *inst, unsigned int offset )
		//-----------------------------------------------------------------------------
		// Used to see if the inst specified is hooked. inst == class object
		inline bool is_hooked( void* inst, unsigned int offset = 0 ) {
			void** vmt = getvtbl( inst, offset );
			return vmt[ -2 ] == ( void* ) GUARD;
		}

		//-----------------------------------------------------------------------------
		// Hooked()
		//-----------------------------------------------------------------------------
		// Used to check if current vtable is hooked.
		inline bool hooked() {
			return ( *vftable != oldvmt );
		}

		//-----------------------------------------------------------------------------
		// IsInitialized()
		//-----------------------------------------------------------------------------
		// Returns true if this instance has been initialized.
		// Could also be used to check if this vmt has been unhooked successfully.
		inline bool is_initialized() const {
			return ( vftable != nullptr );
		}

		//-----------------------------------------------------------------------------
		// GetMethod( unsigned int index )
		//-----------------------------------------------------------------------------
		// ~ Explanation:
		// Get the original function.
		// Use a function prototype for the template argument to make it very easy to call this function.
		// Example syntax: hook.get<bool (__thiscall*)( void*, int )>( 12 )( inst, arg );
		// Get the hook object. Does not check if this vmt has actually been hooked!
		template <typename Fn >inline Fn& get( uintptr_t idx ) {
#ifdef _DEBUG
			assert( idx < count_vfuncs( arrofvfuncs + 3 ) && is_initialized() );
#endif // _DEBUG
			return *( Fn* ) &oldvmt[ idx ];
		}

		//-----------------------------------------------------------------------------
		// HookMethod( void *newfunc, unsigned int index )
		//-----------------------------------------------------------------------------
		// Hooks a virtual function, in the this virtual table, at specified index.
		// returns the this ptr
		inline vmt& hook( void *newfunc, uintptr_t idx ) {
#ifdef _DEBUG
			assert( idx < count_vfuncs( arrofvfuncs + 3 ) && is_initialized() );
#endif // _DEBUG
			arrofvfuncs[ idx + 3 ] = newfunc;
			return *this;
		}

		//-----------------------------------------------------------------------------
		// UnHook()
		//-----------------------------------------------------------------------------
		// Restores to original vmt.
		inline vmt& unhook() {
			*vftable = oldvmt;
			return *this;
		}

		//-----------------------------------------------------------------------------
		// Kill()
		//-----------------------------------------------------------------------------
		// Completely removes hook and also "forgets" about this vmt.
		__declspec( noinline ) void kill() {
			if( is_initialized() ) {
				unhook();
				vftable = nullptr;
			}
			::free( arrofvfuncs );
			arrofvfuncs = nullptr;
		}

		//-----------------------------------------------------------------------------
		// EraseHooks()
		//-----------------------------------------------------------------------------
		// self-explanatory, restores present vfunction hooks to original.
		void erase_hooks() {
			unsigned int i = 0;
			void** vmt = arrofvfuncs + 3;
			do vmt[ i ] = oldvmt[ i ];
			while( vmt[ ++i ] );
		}

		//-----------------------------------------------------------------------------
		// ReHook()
		//-----------------------------------------------------------------------------
		// Sets current vftable to current array of vfuncs.
		inline vmt& rehook() {
			*vftable = arrofvfuncs + 3;
			return *this;
		}
	};
}

#endif // !VMT_HPP
