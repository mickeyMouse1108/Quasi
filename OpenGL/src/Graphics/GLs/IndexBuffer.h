#pragma once
#include "NumTypes.h"

namespace Graphics {
	class IndexBuffer {
		private:
			uint rendererID = 0;
			uint length = 0;
		public:
			IndexBuffer(const void* data, uint length);
			~IndexBuffer();

			void Bind() const;
			void Unbind() const;

			[[nodiscard]] uint GetLength() const { return length; }
	};
}
