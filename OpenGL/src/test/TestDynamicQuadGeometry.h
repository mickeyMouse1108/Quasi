#pragma once

#include "Test.h"

namespace Test
{
    class TestDynamicQuadGeometry : public Test
    {
    private:
        static constexpr int MAX_QUAD = 8;
        static constexpr int MAX_VERTEX = MAX_QUAD * 4;
        static constexpr int MAX_INDEX = MAX_QUAD * 6;
        
        unsigned int quads = 0;
        bool isMin = false;
        bool isMax = false;
        
        VertexColor3D vertexes[MAX_VERTEX];
        
        Graphics::DynamicVertexBuffer<VertexColor3D>* vb;
        Graphics::VertexArray* va;
        Graphics::IndexBuffer* ib;
        Graphics::Shader* shader;

        Maths::Matrix3D projection;
    public:
        TestDynamicQuadGeometry();
        ~TestDynamicQuadGeometry() override;
        
        void OnUpdate(float deltaTime) override;
        void OnRender(Graphics::Renderer& renderer) override;
        void OnImGuiRender() override;
        
        static Maths::Vector4 COLORS[8];
    };
}
