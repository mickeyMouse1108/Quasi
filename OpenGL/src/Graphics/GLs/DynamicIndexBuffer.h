#pragma once
#include <initializer_list>

#include "NumTypes.h"

namespace Graphics {
    class DynamicIndexBuffer {
        private:
            uint rendererID = 0;
            uint bufferSize = 0;
            uint dataOffset = 0;
            uint indexOffset = 0;
        public:
            DynamicIndexBuffer(uint size);
            ~DynamicIndexBuffer();

            void Bind() const;
            void Unbind() const;

            void SetData(const uint* data, uint size);
            template <class T> void SetData(const T& arr) { this->SetData(arr.data(), arr.size()); }
        
            void SetDataWhole(const uint* data);
            template <class T> void SetDataWhole(const T& arr) { if (arr.size() >= bufferSize) this->SetDataWhole(arr.data()); }
        
            void ClearData(bool shallowClear = true);
        
            void AddData(const uint* data, uint size = 0, int maxIndex = -1);
            void AddData(std::initializer_list<uint> data) { AddData(data.begin(), data.size()); } 
            void AddData(uint data);
            template <class T> void AddData(const T& arr, int maxIndex = -1) { this->AddData(arr.data(), arr.size(), maxIndex); }

            [[nodiscard]] uint GetLength() const { return bufferSize; }
    };
}
