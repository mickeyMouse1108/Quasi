#pragma once

#include "Render.h"
#include "Texture.h"

#include <GLFW/glfw3.h>

#include "stdu/ref.h"

namespace Graphics {
    // TODO: make this changeable
    class GraphicsDevice {
        static constexpr unsigned int MAX_VERTEX_COUNT = 1000;
        static constexpr unsigned int MAX_INDEX_COUNT = 1000;
    private:
        std::vector<RenderData> renders;
        // Maths::Matrix3D::PerspectiveProjectionFOV(45.0f, 4.0f / 3, 0.1f, 100.0f);

        Maths::ivec2 windowSize;
        GLFWwindow* mainWindow;
        bool useWireRender = false;

        inline static stdu::ref<GraphicsDevice> Instance = nullptr;
    public:
        OPENGL_API explicit GraphicsDevice(GLFWwindow* window, Maths::ivec2 winSize);
        using stringr = const std::string&;

        OPENGL_API void Quit();
        OPENGL_API ~GraphicsDevice();
        
        OPENGL_API void BeginRender();
        OPENGL_API void EndRender();

        template <class T> stdu::ref<RenderData> CreateNewRender(uint vsize = MAX_VERTEX_COUNT, uint isize = MAX_INDEX_COUNT);
        OPENGL_API void BindRender(RenderData& render);
        OPENGL_API void DeleteRender(uint index);
        OPENGL_API void DeleteAllRenders();
        OPENGL_API RenderData& GetRender(uint index);
        OPENGL_API void Render(RenderData& r);
        void Render(uint index) { Render(GetRender(index)); }

        OPENGL_API void BindTexture(Texture& texture, uint slot);

        OPENGL_API void ClearColor(const Maths::colorf& color);

        [[nodiscard]] bool IsClosed() const { return !mainWindow; } // quit() sets mainwindow to nullptr 
        OPENGL_API [[nodiscard]] bool WindowIsOpen() const;
    
        [[nodiscard]] Maths::ivec2 GetWindowSize() const { return windowSize; }
        GLFWwindow* GetWindow() { return mainWindow; }
        [[nodiscard]] const GLFWwindow* GetWindow() const { return mainWindow; }

        static void SetRenderWireframe(bool usewire) { Render::SetRenderWireframe(usewire); }

        // WARNING! this may close the application, so be prepared to handle that situation
        OPENGL_API void DebugMenu();

        static GraphicsDevice& GetDeviceInstance() { return *Instance; }
        static GLFWwindow* GetMainWindow() { return Instance->mainWindow; }

        //void Render(bool autoSort = true);

        OPENGL_API static GraphicsDevice Initialize(Maths::ivec2 winSize = { 640, 480 });
    };

    template <class T>
    stdu::ref<RenderData> GraphicsDevice::CreateNewRender(uint vsize, uint isize) {
        renders.push_back(RenderData::Create<T>(vsize, isize));
        BindRender(renders.back());
        return renders.back();
    }
}
