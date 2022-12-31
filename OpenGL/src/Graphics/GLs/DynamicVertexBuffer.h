#pragma once
#include "Debugging.h"

namespace Graphics
{
    template <typename T>
    class DynamicVertexBuffer
    {
    private:
        unsigned int rendererID;
        unsigned int dataOffset = 0;
        unsigned int bufferSize;
    public:
        DynamicVertexBuffer(unsigned int size);
        ~DynamicVertexBuffer();

        void Bind() const;
        void Unbind() const;
        
        void SetData(const T* data, unsigned int size);
        void SetDataWhole(const T* data);
        void ClearData(bool shallowClear = true);
        void AddData(const T* data, unsigned int size = 1);
        void AddData(const T& data);
    };

    template<typename T>
    DynamicVertexBuffer<T>::DynamicVertexBuffer(unsigned int size) : bufferSize(size) {
        GLCALL(glGenBuffers(1, &rendererID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(T) * size, nullptr, GL_DYNAMIC_DRAW));
    }
    
    template <typename T>
    DynamicVertexBuffer<T>::~DynamicVertexBuffer() {
        GLCALL(glDeleteBuffers(1, &rendererID));
    }
    
    template <typename T>
    void DynamicVertexBuffer<T>::Bind() const {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    }
    
    template <typename T>
    void DynamicVertexBuffer<T>::Unbind() const {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    template <typename T>
    void DynamicVertexBuffer<T>::SetData(const T* data, unsigned int size)
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(T), data);
    }

    template <typename T>
    void DynamicVertexBuffer<T>::SetDataWhole(const T* data)
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(T), data);
    }

    
    template <typename T>
    void DynamicVertexBuffer<T>::ClearData(bool shallowClear)
    {
        Bind();
        dataOffset = 0;
        if (shallowClear) return;
        T* clear = new T[bufferSize];
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(T), clear);
        delete[] clear;
    }

    template <typename T>
    void DynamicVertexBuffer<T>::AddData(const T* data, unsigned size)
    {
        glBufferSubData(GL_ARRAY_BUFFER, dataOffset * sizeof(T), size * sizeof(T), data);
        dataOffset += size;
    }

    template <typename T>
    void DynamicVertexBuffer<T>::AddData(const T& data)
    {
        glBufferSubData(GL_ARRAY_BUFFER, dataOffset * sizeof(T), sizeof(T), &data);
        ++dataOffset;
    }
}
