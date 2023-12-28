#pragma once

#include "DynamicIndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"

#include <GLFW/glfw3.h>

#include "stdu/ref.h"

namespace Graphics {
    using Vertex = VertexColor3D;
    template <class T = Vertex> class Mesh;
    
    // TODO: make this changeable
    class GraphicsDevice {
        static constexpr unsigned int MAX_VERTEX_COUNT = 1000;
        static constexpr unsigned int MAX_INDEX_COUNT = 1000;
    private:
        std::vector<stdu::ref<Mesh<>>> _meshes;
        
        VertexArray                 _vertexArray;
        DynamicVertexBuffer<Vertex> _vertexBuffer;
        DynamicIndexBuffer          _indexBuffer;
        Shader                      _currentShader = {}; // shader can be null if renderId is 0
        
        Renderer                    _renderer;

        // Maths::Matrix3D::PerspectiveProjectionFOV(45.0f, 4.0f / 3, 0.1f, 100.0f);
        Maths::Matrix3D _projection = Maths::Matrix3D::OrthoProjection(-4, 4, -3, 3, 0.1f, 100);
        Maths::Matrix3D _camera;

        const Maths::Vec2Int _windowSize;
        GLFWwindow* _mainWindow;

        inline static stdu::ref<GraphicsDevice> Instance = nullptr;
    // public:
        public:
            OPENGL_API GraphicsDevice(GLFWwindow* window, Maths::Vec2Int winSize);
            using stringr = const std::string&;

            OPENGL_API ~GraphicsDevice();
            OPENGL_API void BeginRender();
            OPENGL_API void EndRender();

            OPENGL_API void ClearRegistered();
            OPENGL_API void RegisterMeshes();
            OPENGL_API void RegisterNewMeshes(const Mesh<>* meshes, uint count);
            template <class U> void RegisterNewMeshes(const U& ms) { RegisterNewMeshes(ms.data(), ms.size()); }
            OPENGL_API void RenderRegistered() const;
            OPENGL_API void AddMeshes(Mesh<>* meshes, uint count);
            template <class U> void AddMeshes(const U& ms) { AddMeshes(ms.data(), ms.size()); }
            OPENGL_API void Delete(int index);
            OPENGL_API void Delete(int indexStart, int indexEnd);
            OPENGL_API void UpdateMeshIndicies();

            OPENGL_API void ClearColor(const Maths::Vector3& color);

            [[nodiscard]] bool WindowIsOpen() const { return !glfwWindowShouldClose(_mainWindow); }

            [[nodiscard]] const Renderer& GetRenderer() const { return _renderer; }
            Renderer& GetRenderer() { return _renderer; }
            [[nodiscard]] Maths::Vec2Int GetWindowSize() const { return _windowSize; }
            [[nodiscard]] const GLFWwindow* GetWindow() const { return _mainWindow; }
            GLFWwindow* GetWindow() { return _mainWindow; }
            Shader& GetShader() { return _currentShader; }
            [[nodiscard]] const Shader& GetShader() const { return _currentShader; }

            OPENGL_API void EnableShader();
            OPENGL_API void DisableShader();
            void UseShader(stringr code) { _currentShader = std::move(Shader { code }); }
            void UseShaderFromFile(stringr file) { _currentShader = std::move(Shader::FromFile(file)); }
            /*#define SHADER_UNIF(x) _currentShader->Bind(); _currentShader->SetUniform##x; _currentShader->Unbind()
            void SetUniform1Int(stringr unifName, int val)                    const { SHADER_UNIF(1I(unifName, val)); }
            void SetUniform2Int(stringr unifName, const Maths::Vec2Int& val)  const { SHADER_UNIF(2I(unifName, (const int*)&val)); }
            void SetUniform3Int(stringr unifName, const Maths::Vec3Int& val)  const { SHADER_UNIF(3I(unifName, (const int*)&val)); }
            void SetUniform4Int(stringr unifName, const Maths::Vec4Int& val)  const { SHADER_UNIF(4I(unifName, (const int*)&val)); }

            void SetUniform1IntArr(stringr unifName, int vals[], unsigned num)   const { SHADER_UNIF(1IVec(unifName, vals, num)); }

            void SetUniform1Float(stringr unifName, float val)                  const { SHADER_UNIF(1F(unifName, val)); }
            void SetUniform2Float(stringr unifName, const Maths::Vector2& val)  const { SHADER_UNIF(2F(unifName, (const float*)&val)); }
            void SetUniform3Float(stringr unifName, const Maths::Vector3& val)  const { SHADER_UNIF(3F(unifName, (const float*)&val)); }
            void SetUniform4Float(stringr unifName, const Maths::Vector4& val)  const { SHADER_UNIF(4F(unifName, (const float*)&val)); }

            void SetUniform4x4Matrix(stringr unifName, const Maths::Matrix3D& val) const { SHADER_UNIF(Matrix4x4(unifName, val)); }
            #undef SHADER_UNIF*/

            void SetCamera(const Maths::Matrix3D& camera) { _camera = camera; }
            void SetProjection(const Maths::Matrix3D& proj) { _projection = proj; }

            void SetRenderWireframe(bool usewire) { glPolygonMode(GL_FRONT_AND_BACK, usewire ? GL_LINE, GL_FILL); }

            OPENGL_API void DebugMenu();

            static GraphicsDevice& GetDeviceInstance() { return *Instance; }
            static GLFWwindow* GetMainWindow() { return Instance->_mainWindow; }

            //void Render(bool autoSort = true);

            OPENGL_API static std::unique_ptr<GraphicsDevice> Initialize(Maths::Vec2Int winSize = { 640, 480 });

        friend class Mesh<>;
    };
}
