#pragma once

#include "NumTypes.h"

namespace Graphics {
	struct TriIndicies {
		uint a, b, c;

		TriIndicies operator+(uint i) { return { a + i, b + i, c + i }; }
		TriIndicies operator+=(uint i) { a += i; b += i; c += i; return *this; }
	};
}