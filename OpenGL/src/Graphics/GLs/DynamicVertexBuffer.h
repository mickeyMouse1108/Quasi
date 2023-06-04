#pragma once
#include "Debugging.h"
#include "NumTypes.h"
#include <vector>

namespace Graphics {
    template <class T>
    class DynamicVertexBuffer {
        private:
            uint rendererID = 0;
            uint dataOffset = 0;
            uint bufferSize = 0;
        public:
            DynamicVertexBuffer(uint size);
            ~DynamicVertexBuffer();

            void Bind() const;
            void Unbind() const;
            
            void SetData(const T* data, uint size);
            template <class U> void SetData(const U& arr) { this->SetData(arr.data(), arr.size()); }
            void SetDataWhole(const T* data);
            template <class U> void SetDataWhole(const U& arr) { if (arr.size() <= bufferSize) this->SetDataWhole(arr.data()); }
            void ClearData(bool shallowClear = true);
            void AddData(const T* data, uint size = 1);
            template <class U> void AddData(const U& arr) { this->AddData(arr.data(), arr.size()); }
            void AddData(const T& data);
    };

    template<class T>
    DynamicVertexBuffer<T>::DynamicVertexBuffer(uint size) : bufferSize(size) {
        GLCALL(glGenBuffers(1, &rendererID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(T) * size, nullptr, GL_DYNAMIC_DRAW));
    }
    
    template <class T>
    DynamicVertexBuffer<T>::~DynamicVertexBuffer() {
        GLCALL(glDeleteBuffers(1, &rendererID));
    }
    
    template <class T>
    void DynamicVertexBuffer<T>::Bind() const {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    }
    
    template <class T>
    void DynamicVertexBuffer<T>::Unbind() const {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    template <class T>
    void DynamicVertexBuffer<T>::SetData(const T* data, unsigned int size) {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(T), data);
    }

    template <class T>
    void DynamicVertexBuffer<T>::SetDataWhole(const T* data) {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(T), data);
    }
    
    template <class T>
    void DynamicVertexBuffer<T>::ClearData(bool shallowClear) {
        Bind();
        dataOffset = 0;
        if (shallowClear) return;
        const std::vector<T> clear{ bufferSize, T{} };
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(T), clear.data());
    }

    template <class T>
    void DynamicVertexBuffer<T>::AddData(const T* data, unsigned size) {
        glBufferSubData(GL_ARRAY_BUFFER, dataOffset * sizeof(T), size * sizeof(T), data);
        dataOffset += size;
    }

    template <class T>
    void DynamicVertexBuffer<T>::AddData(const T& data) {
        glBufferSubData(GL_ARRAY_BUFFER, dataOffset * sizeof(T), sizeof(T), &data);
        ++dataOffset;
    }
}
