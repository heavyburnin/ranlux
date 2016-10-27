#ifndef PAINTTRAVERSE_HPP
#define PAINTTRAVERSE_HPP

#pragma once

#include "../main.hpp"

class painttraverse {
	painttraverse() {};
	static painttraverse *_painttraverse;

public:
	static std::shared_ptr<painttraverse> get();
	~painttraverse() { _painttraverse = nullptr; }

	static void __fastcall hkpainttraverse( vgui::ipanel *ecx, void *edx, vgui::vpanel vguipanel, bool forcerepaint, bool allowforce );

	void drawesp( c_csplayer *local, c_csplayer *entity, int idx );

	static bool pressed( int key );
};

#endif // !PAINTTRAVERSE_HPP