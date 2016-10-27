#include "draw.hpp"

draw* draw::_draw = nullptr;

std::shared_ptr<draw> draw::get() {
	if( !_draw )
		_draw = new draw();

	return std::shared_ptr<draw>( _draw );
}

/*void drawtext( int x, int y, color color, const wchar_t* text ) {
	Surface->DrawSetTextFont( font );
	Surface->DrawSetTextColor( color );
	Surface->DrawSetTextPos( x, y );
	Surface->DrawPrintText( text, wcslen( text ) );
}

void drawtext( int x, int y, color color, const char* fmt, ... ) {
	if( !fmt ) return; //if the passed string is null return
	if( strlen( fmt ) < 2 ) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[ 256 ] = { 0 };

	//Do sprintf with the parameters
	va_start( va_alist, fmt );
	_vsnprintf_s( logBuf + strlen( logBuf ), 256 - strlen( logBuf ), sizeof( logBuf ) - strlen( logBuf ), fmt, va_alist );
	va_end( va_alist );

	Text( x, y, color, font, logBuf );
}*/