#pragma once
#include "opengl.h"
#include "NumTypes.h"

namespace Graphics {
	class VertexBuffer {
	    private:
		    glID rendererID = 0;
	    public:
		    OPENGL_API VertexBuffer(const void* data, uint size);
		    OPENGL_API ~VertexBuffer();

		    OPENGL_API void Bind() const;
		    OPENGL_API void Unbind() const;
	};
}
