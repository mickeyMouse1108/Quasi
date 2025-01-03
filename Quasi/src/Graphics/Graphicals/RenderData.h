#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GLDebug.h"
#include "Shader.h"
#include "VertexElement.h"

#include <cstring>

namespace Quasi::Graphics {
	class GraphicsDevice;

	template <IVertex Vtx> class Mesh;

    template <class>
	class RenderObject;
    
	class RenderData {
	public:
		VertexArray varray;
		VertexBuffer vbo;
		IndexBuffer ibo;

	    Math::Matrix3D projection = Math::Matrix3D::ortho_projection({ -4, 4, -3, 3, 0.1f, 100 });
	    Math::Matrix3D camera {};
	    Shader shader = {}; // shader can be null if renderId is 0
	private:
		UniqueRef<byte[]> vertexData;
		u32 vertexOffset = 0;
		UniqueRef<u32[]> indexData;
		u32 indexOffset = 0;

		OptRef<GraphicsDevice> device;
		u32 deviceIndex = 0;

		friend class GraphicsDevice;
	public:
		explicit RenderData(GraphicsDevice& gd, u32 vsize, u32 isize, u32 vertSize, const VertexBufferLayout& layout) :
			varray(VertexArray::New()), vbo(VertexBuffer::New(vsize * vertSize)), ibo(IndexBuffer::New(isize)),
			vertexData(new byte[vertSize * vsize]), indexData(new u32[isize]), device(gd) {
			varray.Bind();
			varray.AddBuffer(layout);
		}

		RenderData(const RenderData&) = delete;
		RenderData& operator=(const RenderData&) = delete;

		static void Transfer(RenderData& dest, RenderData&& from);
		RenderData(RenderData&& rd) noexcept : device(rd.device) { Transfer(*this, std::move(rd)); }
		RenderData& operator=(RenderData&& rd) noexcept { Transfer(*this, std::move(rd)); return *this; }

		~RenderData();

		template <class T> void PushVertex(const T& vertex);
		void PushIndex(TriIndices index);
		void PushIndicesOffseted(Span<const TriIndices> indices, usize objectSize);

		void Bind() const;
		void Unbind() const;

		void BufferUnload();
		void BufferLoad();

		void Clear();
		template <class T> void Add(const Mesh<T>& mesh) { mesh.AddTo(*this); }
		void Add(const CollectionAny auto& arr) { for (const auto& m : arr) Add(m); }

		void Destroy();

		void Render(Shader& replaceShader, const ShaderArgs& args = {}, bool setDefaultShaderArgs = true);
		void Render(const ShaderArgs& args = {}, bool setDefaultShaderArgs = true) { Render(shader, args, setDefaultShaderArgs); }

		void RenderInstanced(Shader& replaceShader, int instances, const ShaderArgs& args = {}, bool setDefaultShaderArgs = true);
		void RenderInstanced(int instances, const ShaderArgs& args = {}, bool setDefaultShaderArgs = true) { RenderInstanced(shader, instances, args, setDefaultShaderArgs); }

	    void SetCamera(const Math::Matrix3D& cam) { camera = cam; }
	    void SetProjection(const Math::Matrix3D& proj) { projection = proj; }
	    
	    void UseShader(Str code) { shader = Shader::New(code); }
	    void UseShaderFromFile(Str file) { shader = Shader::FromFile(file); }
	    void UseShaderFromFile(Str vert, Str frag, Str geom = {}) { shader = Shader::FromFile(vert, frag, geom); }

		friend class GraphicsDevice;
		template <IVertex T> friend class Mesh;
	};

	template <class T> void RenderData::PushVertex(const T& vertex) {
		const byte* rawbytes = Memory::TransmutePtr<const byte>(&vertex);
		std::memcpy(vertexData.get() + vertexOffset, rawbytes, sizeof(T));
		vertexOffset += sizeof(T);
	}
}
