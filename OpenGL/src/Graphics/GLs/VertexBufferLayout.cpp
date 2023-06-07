#include "VertexBufferLayout.h"

namespace Graphics {
#define VBE VertexBufferComponent VertexBufferComponent::
    VBE Float()     { return { GLTypeOf<float>,  1, false }; }
    VBE Double()    { return { GLTypeOf<double>, 1, false }; }
    VBE Int()       { return { GLTypeOf<int>,    1, false }; }
    VBE Uint()      { return { GLTypeOf<uint>,   1, false }; }
    VBE SByte()     { return { GLTypeOf<char>,   1, false }; }
    VBE Byte()      { return { GLTypeOf<uchar>,  1, false }; }
    VBE Vec2()      { return { GLTypeOf<float>,  2, false }; }
    VBE Vec3()      { return { GLTypeOf<float>,  3, false }; }
    VBE Vec4()      { return { GLTypeOf<float>,  4, false }; }
    VBE IVec2()     { return { GLTypeOf<int>,    2, false }; }
    VBE IVec3()     { return { GLTypeOf<int>,    3, false }; }
    VBE IVec4()     { return { GLTypeOf<int>,    4, false }; }
#undef VBE
    
    VertexBufferLayout::VertexBufferLayout(std::initializer_list<VertexBufferComponent> comps) {
        for (const auto& component : comps) Push(component);
    }

    void VertexBufferLayout::Push(const VertexBufferComponent& comp) {
        _components.push_back(comp);
        stride += comp.count * GLSizeOf(comp.type);
    }

    void VertexBufferLayout::PushLayout(const VertexBufferLayout& layout) {
        for (const auto& component : layout.GetComponents()) Push(component);
    }
}
