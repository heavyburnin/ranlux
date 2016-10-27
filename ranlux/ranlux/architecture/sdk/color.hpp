#pragma once

class color {
public:
	color() : colred( 0 ), colgreen( 0 ), colblue( 0 ), colalpha( 0 ){}
	color( int r, int g, int b, int a ) : colred( r ), colgreen( g ), colblue( b ), colalpha( a ){}
	color( int r, int g, int b ) : colred( r ), colgreen( g ), colblue( b ), colalpha( 255 ){}

	static color white() { return color( 255, 255, 255, 255 ); }
	static color black() { return color( 0, 0, 0, 255 ); }
	static color red() { return color( 255, 30, 30, 255 ); }
	static color green() { return color( 0, 255, 0, 255 ); }
	static color orange() { return color( 255, 130, 0, 255 ); }
	static color grey() { return color( 80, 80, 80, 255 ); }
	static color darkgrey() { return color( 35, 35, 35, 255 ); }
	static color blue() { return color( 0, 180, 255, 255 ); }
	static color cyan() { return color( 0, 255, 255, 255 ); }
	static color yellow() { return color( 255, 255, 0, 255 ); }
	static color transparent() { return color( 0, 0, 0, 0 ); }

	int getred() { return colred; }
	int getgreen() { return colgreen; }
	int getblue() { return colblue; }
	int getalpha() { return colalpha; }

private:
	int colred, colgreen, colblue, colalpha;

};