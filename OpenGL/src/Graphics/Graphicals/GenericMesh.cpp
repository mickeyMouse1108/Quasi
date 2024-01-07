#include "GenericMesh.h"
#include "Mesh.h"

namespace Graphics {
	GenericMesh::~GenericMesh() {
		if (ptr)
			ptr->render = nullptr;
	}
    
	void GenericMesh::Transfer(GenericMesh& dest, GenericMesh&& from) {
		dest.ptr = from.ptr;
		from.ptr = nullptr;
	}

	void GenericMesh::Set(void* mesh) {
		ptr = (Mesh<singlebyte>*)mesh;
	}

	uint& GenericMesh::deviceIndex() { return ptr->deviceIndex; }
	uint GenericMesh::deviceIndex() const { return ptr->deviceIndex; }
}