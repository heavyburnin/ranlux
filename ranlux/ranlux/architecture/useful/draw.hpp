#pragma once

#ifndef _DRAW_HPP
#define _DRAW_HPP

#include "../main.hpp"
class draw {
	draw() {};
	static draw *_draw;

public:
	static std::shared_ptr<draw> get();
	~draw() { _draw = nullptr; }

	Vector2D screen;
};

#endif // !_DRAW_HPP