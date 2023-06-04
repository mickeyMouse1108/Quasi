#pragma once
#include "NumTypes.h"

namespace Graphics {
	class VertexBuffer {
	    private:
		    uint rendererID = 0;
	    public:
		    VertexBuffer(const void* data, uint size);
		    ~VertexBuffer();

		    void Bind() const;
		    void Unbind() const;
	};
}
