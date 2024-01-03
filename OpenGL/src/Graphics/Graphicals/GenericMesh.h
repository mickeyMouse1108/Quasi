#pragma once
#include "NumTypes.h"
#include <utility>
#include "opengl.h"

namespace Graphics {
	template <class T> class Mesh;

	class GenericMesh {
		// used so that 'singlebyte' is a custom defined type, unlike char
		struct singlebyte { char _; };
		Mesh<singlebyte>* ptr; // using the fact that mesh<T> is always 104 bytes
	public:
		GenericMesh(void* m) : ptr((Mesh<singlebyte>*)m) {}
		OPENGL_API ~GenericMesh();

		GenericMesh(const GenericMesh&) = delete;
		GenericMesh& operator=(const GenericMesh&) = delete;

		OPENGL_API static GenericMesh& Transfer(GenericMesh& dest, GenericMesh&& from);
		GenericMesh(GenericMesh&& mesh) noexcept { Transfer(*this, std::move(mesh)); }
		GenericMesh& operator=(GenericMesh&& mesh) noexcept { return Transfer(*this, std::move(mesh)); }

		template <class T> static GenericMesh From(Mesh<T>* m) {
			return GenericMesh{ (Mesh<singlebyte>*)m };
		}

		template <class T> Mesh<T>& As() {
			return *reinterpret_cast<Mesh<T>*>(ptr);
		}

		OPENGL_API void Set(void* ptr);
		template <class T> void Set(Mesh<T>* ptr) {
			Set((void*)ptr);
		}

		OPENGL_API uint& deviceIndex();
		OPENGL_API uint deviceIndex() const;
	};
}