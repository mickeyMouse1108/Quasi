#pragma once
#include <initializer_list>
#include <utility>
#include "opengl.h"

#include "NumTypes.h"

namespace Graphics {
    struct TriIndicies;

    class DynamicIndexBuffer {
        private:
            uint rendererID = 0;
            uint bufferSize = 0;
            uint dataOffset = 0;
            uint indexOffset = 0;
        public:
            DynamicIndexBuffer() {}
            OPENGL_API DynamicIndexBuffer(uint size);
            OPENGL_API ~DynamicIndexBuffer();

            DynamicIndexBuffer(const DynamicIndexBuffer&) = delete;
            DynamicIndexBuffer& operator=(const DynamicIndexBuffer&) = delete;
            OPENGL_API static DynamicIndexBuffer& Transfer(DynamicIndexBuffer& dest, DynamicIndexBuffer&& from);
            DynamicIndexBuffer(DynamicIndexBuffer&& ib) noexcept { Transfer(*this, std::move(ib)); }
            DynamicIndexBuffer& operator=(DynamicIndexBuffer&& ib) noexcept { return Transfer(*this, std::move(ib)); }

            OPENGL_API void Bind() const;
            OPENGL_API void Unbind() const;

            OPENGL_API void SetData(const uint* data, uint size);
            OPENGL_API void SetData(const TriIndicies* data, uint size) { SetData((const uint*)data, size * 3); }
            template <class T> void SetData(const T& arr) { SetData(arr.data(), arr.size()); }
        
            OPENGL_API void SetDataWhole(const uint* data);
            OPENGL_API void SetDataWhole(const TriIndicies* data) { SetDataWhole((const uint*)data); }
            template <class T> void SetDataWhole(const T& arr) { if (arr.size() >= bufferSize) SetDataWhole(arr.data()); }
        
            OPENGL_API void ClearData(bool shallowClear = true);
        
            OPENGL_API void AddData(const uint* data, uint size, int maxIndex = -1);
            OPENGL_API void AddData(const TriIndicies* data, uint size, int maxIndex = -1) { AddData((const uint*)data, size * 3, maxIndex); }
            template <class T> void AddData(const T& arr, int maxIndex = -1) { AddData(arr.data(), arr.size(), maxIndex); }

            [[nodiscard]] uint GetLength() const { return bufferSize; }
            [[nodiscard]] uint GetUsedLength() const { return dataOffset; }
    };
}
