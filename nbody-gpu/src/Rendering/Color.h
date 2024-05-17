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

	DECL_COLOR(YELLOW,	1.0f, 1.0f, 0.0f, 1.0f);
	DECL_COLOR(CYAN,	0.0f, 1.0f, 1.0f, 1.0f);
	DECL_COLOR(MAGENTA, 1.0f, 0.0f, 1.0f, 1.0f);

	DECL_COLOR(LIGHT_GRAY, 0.8f, 0.8f, 0.8f, 1.0f);
	DECL_COLOR(DARK_GRAY, 0.2f, 0.2f, 0.2f, 1.0f);
}