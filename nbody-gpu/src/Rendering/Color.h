#pragma once

#include <imgui.h>

#define DECL_COLOR(name, r, g, b, a) static const ImU32 name = ImColor(r, g, b, a)

namespace Color {
	DECL_COLOR(WHITE,	1.0f, 1.0f, 1.0f, 1.0f);
	DECL_COLOR(BLACK,	0.0f, 0.0f, 0.0f, 1.0f);
	DECL_COLOR(TRANS,	0.0f, 0.0f, 0.0f, 0.0f);

	DECL_COLOR(RED,		1.0f, 0.0f, 0.0f, 1.0f);
	DECL_COLOR(GREEN,	0.0f, 1.0f, 0.0f, 1.0f);
	DECL_COLOR(BLUE,	0.0f, 0.0f, 1.0f, 1.0f);
}