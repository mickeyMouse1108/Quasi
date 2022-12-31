#pragma once

namespace Graphics
{
	class IndexBuffer {
	private:
		unsigned int rendererID;
		unsigned int length;
	public:
		IndexBuffer(const void* data, unsigned int length);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetLength() const { return length; }
	};
}