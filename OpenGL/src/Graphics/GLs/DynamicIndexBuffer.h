#pragma once
#include <initializer_list>
#include "opengl.h"

#include "NumTypes.h"

namespace Graphics {
    class DynamicIndexBuffer {
        private:
            uint rendererID = 0;
            uint bufferSize = 0;
            uint dataOffset = 0;
            uint indexOffset = 0;
        public:
            OPENGL_API DynamicIndexBuffer(uint size);
            OPENGL_API ~DynamicIndexBuffer();

            OPENGL_API void Bind() const;
            OPENGL_API void Unbind() const;

            OPENGL_API void SetData(const uint* data, uint size);
            template <class T> void SetData(const T& arr) { this->SetData(arr.data(), arr.size()); }
        
            OPENGL_API void SetDataWhole(const uint* data);
            template <class T> void SetDataWhole(const T& arr) { if (arr.size() >= bufferSize) this->SetDataWhole(arr.data()); }
        
            OPENGL_API void ClearData(bool shallowClear = true);
        
            OPENGL_API void AddData(const uint* data, uint size = 0, int maxIndex = -1);
            void AddData(std::initializer_list<uint> data) { AddData(data.begin(), (uint)(data.size())); } 
            OPENGL_API void AddData(uint data);
            template <class T> void AddData(const T& arr, int maxIndex = -1) { this->AddData(arr.data(), arr.size(), maxIndex); }

            [[nodiscard]] uint GetLength() const { return bufferSize; }
    };
}
