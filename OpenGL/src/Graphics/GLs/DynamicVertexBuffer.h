#pragma once
#include "Debugging.h"
#include "NumTypes.h"
#include "opengl.h"

#include <typeindex>
#include <vector>
#include <initializer_list>

namespace Graphics {
    class DynamicVertexBuffer {
        private:
            uint rendererID = 0;
            uint dataOffset = 0;
            uint bufferSize = 0;

            uint vertSize = 1;
            std::type_index vertType = typeid(void);

            template <class T> bool IsValidVert() { 
                if (typeid(T) == vertType) return true;
                LOG("ERR: C++ Type " << typeid(T).name() << " doesn't fit with " << vertType.name());
                return false;
            }
        public:
            DynamicVertexBuffer() {}
            OPENGL_API DynamicVertexBuffer(uint size, uint typeSize, std::type_index type);
            OPENGL_API ~DynamicVertexBuffer();

            DynamicVertexBuffer(const DynamicVertexBuffer&) = delete;
            DynamicVertexBuffer& operator=(const DynamicVertexBuffer&) = delete;
            OPENGL_API static DynamicVertexBuffer& Transfer(DynamicVertexBuffer& dest, DynamicVertexBuffer&& from);
            DynamicVertexBuffer(DynamicVertexBuffer&& vbo) noexcept { Transfer(*this, std::move(vbo)); }
            DynamicVertexBuffer& operator=(DynamicVertexBuffer&& vbo) noexcept { return Transfer(*this, std::move(vbo)); }

            template <class T> static DynamicVertexBuffer of(uint size) { 
                return DynamicVertexBuffer(size, sizeof(T), typeid(T));
            }

            OPENGL_API void Bind() const;
            OPENGL_API void Unbind() const;

            [[nodiscard]] uint GetLength() const { return bufferSize; }
            [[nodiscard]] std::type_index GetType() const { return vertType; }

            OPENGL_API void SetDataUnchecked(const void* data, uint vertSize, uint count);
            template <class T> void SetData(const T* data, uint size);
            template <class T> void SetData(const T& arr) { SetData(arr.data(), (uint)(arr.size())); }
        
            OPENGL_API void ClearData(bool shallowClear = true);
        
            OPENGL_API void AddDataUnchecked(const void* data, uint vertSize, uint count);
            template <class T> void AddData(const T* data, uint size = 1);
            template <class T> void AddData(const T& arr) { AddData(arr.data(), (uint)(arr.size())); }
    };

    template <class T>
    void DynamicVertexBuffer::SetData(const T* data, uint size) {
        if (IsValidVert<T>()) SetDataUnchecked((const void*)data, sizeof(T), size);
    }

    template <class T>
    void DynamicVertexBuffer::AddData(const T* data, uint size) {
        if (IsValidVert<T>()) AddDataUnchecked((const void*)data, sizeof(T), size);
    }
}
