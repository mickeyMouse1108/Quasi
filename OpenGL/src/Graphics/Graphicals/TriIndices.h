#pragma once

#include "NumTypes.h"

namespace Graphics {
	struct TriIndices {
		uint a, b, c;

		TriIndices operator+(uint i) const { return { a + i, b + i, c + i }; }
		TriIndices operator+=(uint i) { a += i; b += i; c += i; return *this; }
	};
}