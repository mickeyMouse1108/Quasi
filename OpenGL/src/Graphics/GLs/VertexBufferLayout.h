#pragma once
#include <vector>
#include <GL/glew.h>
#include "opengl.h"

#include "NumTypes.h"

namespace Graphics {
    template <class T> constexpr uint GLTypeOf = 0;
    template <> inline constexpr uint GLTypeOf<float>  = GL_FLOAT;
    template <> inline constexpr uint GLTypeOf<double> = GL_DOUBLE;
    template <> inline constexpr uint GLTypeOf<int>    = GL_INT;
    template <> inline constexpr uint GLTypeOf<uint>   = GL_UNSIGNED_INT;
    template <> inline constexpr uint GLTypeOf<char>   = GL_BYTE;
    template <> inline constexpr uint GLTypeOf<uchar>  = GL_UNSIGNED_BYTE;
    template <> inline constexpr uint GLTypeOf<int16>  = GL_SHORT;
    template <> inline constexpr uint GLTypeOf<uint16> = GL_UNSIGNED_SHORT;
    
    struct VertexBufferComponent {
        uint type;
        uint count;
        uchar normalized;

        VertexBufferComponent() = default;
        VertexBufferComponent(uint type, uint count, uchar norm)
            : type(type), count(count), normalized(norm) {}
        
        static VertexBufferComponent Float()  { return { GLTypeOf<float>,  1, false }; }
        static VertexBufferComponent Double() { return { GLTypeOf<double>, 1, false }; }
        static VertexBufferComponent Int()    { return { GLTypeOf<int>,    1, false }; }
        static VertexBufferComponent Uint()   { return { GLTypeOf<uint>,   1, false }; }
        static VertexBufferComponent SByte()  { return { GLTypeOf<char>,   1, false }; }
        static VertexBufferComponent Byte()   { return { GLTypeOf<uchar>,  1, false }; }
        static VertexBufferComponent Vec2()   { return { GLTypeOf<float>,  2, false }; }
        static VertexBufferComponent Vec3()   { return { GLTypeOf<float>,  3, false }; }
        static VertexBufferComponent Vec4()   { return { GLTypeOf<float>,  4, false }; }
        static VertexBufferComponent IVec2()  { return { GLTypeOf<int>,    2, false }; }
        static VertexBufferComponent IVec3()  { return { GLTypeOf<int>,    3, false }; }
        static VertexBufferComponent IVec4()  { return { GLTypeOf<int>,    4, false }; }
    };
    
    // ReSharper disable once IdentifierTypo
    inline uint GLSizeOf(uint GLtype) {
#define MATCH_TYPE(T) case GLTypeOf<##T>: return sizeof(T)
        switch (GLtype) {
            MATCH_TYPE(float);
            MATCH_TYPE(double);
            MATCH_TYPE(int);
            MATCH_TYPE(uint);
            MATCH_TYPE(char);
            MATCH_TYPE(uchar);
            MATCH_TYPE(int16);
            MATCH_TYPE(uint16);
            default: return 0;
        }
    }
    
    class VertexBufferLayout {
        private:
            std::vector<VertexBufferComponent> _components;
            uint stride = 0;
        public:
            OPENGL_API VertexBufferLayout() = default;
            OPENGL_API VertexBufferLayout(std::initializer_list<VertexBufferComponent> comps);

            template <class T> void Push(uint count, bool normalized = false);
            OPENGL_API void Push(const VertexBufferComponent& comp);
            OPENGL_API void PushLayout(const VertexBufferLayout& layout);

#define VBE VertexBufferComponent::
        
            void PushFloat()      { Push(VBE Float());  }
            void PushDouble()     { Push(VBE Double()); }
            void PushInt()        { Push(VBE Int());    }
            void PushChar()       { Push(VBE SByte());  }
            void PushByte()       { Push(VBE Byte());   }
            void PushVector2()    { Push(VBE Vec2());   }
            void PushVector3()    { Push(VBE Vec3());   }
            void PushVector4()    { Push(VBE Vec4());   }
            void PushVector2Int() { Push(VBE IVec2());  }
            void PushVector3Int() { Push(VBE IVec3());  }
            void PushVector4Int() { Push(VBE IVec4());  }

#undef VBE

            [[nodiscard]] const std::vector<VertexBufferComponent>& GetComponents() const { return _components; }
            [[nodiscard]] uint GetStride() const { return stride; }
    };

    template <class T>
    void VertexBufferLayout::Push(uint count, bool normalized) {
        constexpr auto GLType = GLTypeOf<T>;
        _components.emplace_back(GLType, count, normalized);
        stride += count * Graphics::GLSizeOf(GLType);
    }
}
