#pragma once

namespace Graphics
{
    class DynamicIndexBuffer
    {
    private:
        unsigned int rendererID;
        unsigned int bufferSize;
        unsigned int dataOffset = 0;
        unsigned int indexOffset = 0;
    public:
        DynamicIndexBuffer(unsigned int size);
        ~DynamicIndexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const unsigned int* data, unsigned int size);
        void SetDataWhole(const unsigned int* data);
        void ClearData(bool shallowClear = true);
        void AddData(const unsigned int* data, unsigned int size = 0, unsigned int indexMax = 0);
        void AddData(const unsigned int& data);

        unsigned int GetLength() const { return bufferSize; }
    };
}
