#include "GenericMesh.h"
#include "Mesh.h"

namespace Graphics {
	GenericMesh::~GenericMesh() {
		if (ptr)
			ptr->render = nullptr;
	}

	GenericMesh& GenericMesh::Transfer(GenericMesh& dest, GenericMesh&& from) {
		dest.ptr = from.ptr;
		from.ptr = nullptr;
		return dest;
	}

	void GenericMesh::Set(void* ptr) {
		this->ptr = (Mesh<singlebyte>*)ptr;
	}

	uint& GenericMesh::deviceIndex() { return ptr->deviceIndex; }
	uint GenericMesh::deviceIndex() const { return ptr->deviceIndex; }
}