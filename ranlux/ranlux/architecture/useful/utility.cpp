#include "utility.hpp"

#define in_range(x,a,b)		(x >= a && x <= b)
#define get_bits( x )		(in_range(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xA))
#define get_byte( x )		(get_bits(x[0]) << 4 | get_bits(x[1]))

std::shared_ptr<utility> utility::get() {
	if( !_utility )
		_utility = new utility();

	return std::shared_ptr<utility>( _utility );
}

inline bool is_match( const PBYTE addr, const PBYTE pat, const PBYTE msk ) {
	size_t n = 0;
	while( addr[ n ] == pat[ n ] || msk[ n ] == ( BYTE )'?' ) {
		if( !msk[ ++n ] ) {
			return true;
		}
	}
	return false;
}

template<typename rettype > rettype utility::findpattern( std::string module, const char *ccPattern ) {
	HMODULE hModule = GetModuleHandleA( module.c_str() );

	if( !hModule )
		return 0x0;

	PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER( hModule );
	PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS( LONG( hModule ) + pDsHeader->e_lfanew );
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

	ULONG uCB = ( ULONG ) hModule + pOptionalHeader->BaseOfCode;
	ULONG uSOC = pOptionalHeader->SizeOfCode;

	PBYTE base = ( PBYTE ) uCB;

	size_t l = strlen( ccPattern );
	PBYTE patt_base = static_cast< PBYTE >( _alloca( l >> 1 ) );
	PBYTE msk_base = static_cast< PBYTE >( _alloca( l >> 1 ) );
	PBYTE pat = patt_base;
	PBYTE msk = msk_base;
	l = 0;

	while( *ccPattern ) {
		if( *( PBYTE ) ccPattern == ( BYTE )'\?' ) {
			*pat++ = 0;
			*msk++ = '?';
			ccPattern += ( ( *( PWORD ) ccPattern == ( WORD )'\?\?' ) ? 3 : 2 );
		} else {
			*pat++ = get_byte( ccPattern );
			*msk++ = 'x';
			ccPattern += 3;
		}
		l++;
	}

	*msk = 0;
	pat = patt_base;
	msk = msk_base;
	for( DWORD n = 0; n < ( uSOC - l ); ++n ) {
		if( is_match( base + n, patt_base, msk_base ) ) {
			return base + n;
		}
	}
	return 0x0;
}

void utility::printtoconsole( const char* szMsg, ... ) {
	typedef void( __cdecl* MsgFn )( const char* szMsg, va_list );
	static MsgFn Msg = ( MsgFn ) GetProcAddress( GetModuleHandleA( "tier0.dll" ), "Msg" );

	char buf[ 512 ];
	va_list vlist;
	va_start( vlist, szMsg );
	_vsnprintf( buf, sizeof( buf ), szMsg, vlist );
	va_end( vlist );
	sprintf( buf, "%s", buf );

	Msg( buf, vlist );
}

utility* utility::_utility = nullptr;