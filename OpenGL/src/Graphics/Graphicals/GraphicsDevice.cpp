#include "GraphicsDevice.h"
#include "MeshObject.h"

#include <algorithm>
#include <numeric>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Graphics
{
    GraphicsDevice::GraphicsDevice(GLFWwindow* window) : 
        _vertexArray  (new VertexArray()),
        _vertexBuffer (new DynamicVertexBuffer<Vertex>(MAX_VERTEX_COUNT)),
        _indexBuffer  (new DynamicIndexBuffer(MAX_INDEX_COUNT)),
        _renderer     (new Renderer()),
        _mainWindow   { window }
    {
        _vertexArray->AddBuffer(*_vertexBuffer);
    }

    GraphicsDevice::~GraphicsDevice()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(_mainWindow);
        glfwTerminate();
    }

    void GraphicsDevice::BeginRender()
    {
        _renderer->Clear();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    
    void GraphicsDevice::EnableShader()
    {
        _currentShader->Bind();
        _currentShader->SetUniformMatrix4x4("u_MVP", _projection * _camera);
        _currentShader->Unbind();
    }

    void GraphicsDevice::EndRender()
    {
        ImGui::Render();
        // glClear(GL_COLOR_BUFFER_BIT);
        
        glfwPollEvents();
            
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
        glfwSwapBuffers(_mainWindow);
    }

    void GraphicsDevice::ClearRegistered() const
    {
        _vertexBuffer->ClearData();
        _indexBuffer ->ClearData();
    }

    void GraphicsDevice::RegisterElements() const
    {
        for (unsigned i = 0; i < _meshObjs.size(); ++i) _meshObjs[i].get().AddTo(*_vertexBuffer, *_indexBuffer);
    }

    void GraphicsDevice::RegisterNewElements(MeshObject* meshObjs, unsigned int objCount) const
    {
        for (unsigned int i = 0; i < objCount; ++i) meshObjs[i].AddTo(*_vertexBuffer, *_indexBuffer);
    }

    void GraphicsDevice::RenderRegistered() const
    {
        _renderer->Draw(*_vertexArray, *_indexBuffer, *_currentShader);
    }

    void GraphicsDevice::AddMeshObject(MeshObject* objs, unsigned int count)
    {
        for (unsigned i = 0; i < count; ++i)
        {
            objs[i].device = this;
            objs[i].deviceIndex = _meshObjs.size();
            _meshObjs.emplace_back(objs[i]);
        }
    }

    void GraphicsDevice::Delete(int index) {
        _meshObjs.erase(_meshObjs.begin() + index);
        UpdateMeshIndices();
    }

    void GraphicsDevice::Delete(int indexStart, int indexEnd) {
        _meshObjs.erase(_meshObjs.begin() + indexStart, _meshObjs.begin() + indexEnd);
        UpdateMeshIndices();
    }

    void GraphicsDevice::UpdateMeshIndices() const {
        for (unsigned i = 0; i < _meshObjs.size(); ++i)
            _meshObjs[i].get().deviceIndex = i;
    }

    void GraphicsDevice::ClearColor(const Maths::Vector3& color)
    {
        glClearColor(color.x, color.y, color.z, 1.0f);
    }

    void GraphicsDevice::DebugMenu()
    {
        ImGui::Text("Application Averages %fms/frame (~%f FPS)", 1000.0 / (double)ImGui::GetIO().Framerate, (double)ImGui::GetIO().Framerate);
    }

    std::unique_ptr<GraphicsDevice> GraphicsDevice::Initialize()
    {
        GLFWwindow* window;
        /* Initialize the library */
        if (!glfwInit())
            ASSERT(false);

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
        
        if (!window)
        {
            glfwTerminate();
            ASSERT(false);
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);

        /* SETTING UP GLEW W/ GLEWINIT*/
        if (glewInit() != GLEW_OK) {
            LOG("ERR: ERROR SETTING UP glewInit()");
        }

        LOG(glGetString(GL_VERSION));

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCALL(glEnable(GL_DEPTH_TEST));
        glDepthFunc(GL_LEQUAL);

        // IMGUI INIT
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((const char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
        
        return std::make_unique<GraphicsDevice>(window);
    }
}
