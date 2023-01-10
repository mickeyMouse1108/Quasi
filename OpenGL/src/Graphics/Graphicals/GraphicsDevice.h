#pragma once

#include "DynamicIndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"

#include <GLFW/glfw3.h>

namespace Graphics
{
    class MeshObject;
    
    // TODO: make this changeable
    class GraphicsDevice
    {
        using                    Vertex = VertexColor3D;
        template <class T> using ref = std::reference_wrapper<T>;
        static constexpr unsigned int MAX_VERTEX_COUNT = 1000;
        static constexpr unsigned int MAX_INDEX_COUNT = 1000;
    private:
        std::vector<ref<MeshObject>> _meshObjs;
        
        std::unique_ptr<VertexArray>                 _vertexArray;
        std::unique_ptr<DynamicVertexBuffer<Vertex>> _vertexBuffer;
        std::unique_ptr<DynamicIndexBuffer>          _indexBuffer;
        std::unique_ptr<Shader>                      _currentShader;
        
        std::unique_ptr<Renderer>                    _renderer;

        Maths::Matrix3D _projection = Maths::Matrix3D::OrthoProjection(-320, 320, -240, 240, -100, 100);
        Maths::Matrix3D _camera;

        GLFWwindow* _mainWindow;
    // public:
        public:
        GraphicsDevice(GLFWwindow* window);
        using string = const std::string&;

        ~GraphicsDevice();
        void BeginRender();
        void EndRender();
        
        void ClearRegistered() const;
        void RegisterElements() const;
        void RegisterNewElements(MeshObject* meshObjs, unsigned int objCount) const;
        void RenderRegistered() const;
        void AddMeshObject(MeshObject* meshObjs, unsigned int objCount);
        void Delete(int index);
        void Delete(int indexStart, int indexEnd);
        void UpdateMeshIndices() const;

        void ClearColor(const Maths::Vector3& color);
        
        bool WindowIsOpen() const { return !glfwWindowShouldClose(_mainWindow); }

        Renderer& GetRenderer() { return *_renderer; }
        void EnableShader();
        void UseShader          (string shaderPath)                                 { _currentShader = std::make_unique<Shader>(shaderPath); }
        void SetUniform1Int     (string unifName, int val)                    const { _currentShader->SetUniform1I(unifName, val);          }
        void SetUniform1Float   (string unifName, float val)                  const { _currentShader->SetUniform1F(unifName, val);          }
        void SetUniform4Float   (string unifName, const Maths::Vector4& val)  const { _currentShader->SetUniform4F(unifName, (float*)&val); }
        void SetUniform4x4Matrix(string unifName, const Maths::Matrix3D& val) const { _currentShader->SetUniformMatrix4x4(unifName, val);   }
        
        void SetCamera(const Maths::Matrix3D& camera) { _camera = camera; }

        void DebugMenu();

        //void Render(bool autoSort = true);
        
        static std::unique_ptr<GraphicsDevice> Initialize();

        friend class MeshObject;
    };
}
