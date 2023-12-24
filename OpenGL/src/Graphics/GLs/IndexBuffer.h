#pragma once
#include "NumTypes.h"
#include "opengl.h"

namespace Graphics {
	class IndexBuffer {
		private:
			uint rendererID = 0;
			uint length = 0;
		public:
			OPENGL_API IndexBuffer(const void* data, uint length);
			OPENGL_API ~IndexBuffer();

			OPENGL_API void Bind() const;
			OPENGL_API void Unbind() const;

			OPENGL_API [[nodiscard]] uint GetLength() const { return length; }
	};
}
