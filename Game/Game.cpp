#include "Graphics/Graphicals/GraphicsDevice.h"
#include "test/TestingFramework.h"
#include "Mesh.h"
#include "imgui.h"

int main() {
    Graphics::GraphicsDevice GraphicsDevice = Graphics::GraphicsDevice::Initialize();
    // Test::TestManager testm;
    //
    // testm.OnInit(GraphicsDevice);
    // while (GraphicsDevice.WindowIsOpen()) {
    //     testm.OnUpdate();
    //
    //     GraphicsDevice.BeginRender();
    //
    //     testm.OnRender(GraphicsDevice);
    //     testm.OnImGuiRender(GraphicsDevice);
    //
    //     GraphicsDevice.EndRender();
    // }
    // testm.OnDestroy(GraphicsDevice);
    Maths::fvec3 trans = { 0, 0, -2 }, scale = 1, rot;
    Graphics::Mesh<VertexColor3D> cube = Graphics::MeshUtils::CubeMesh(0, 1, 1, 1);
    stdu::ref render = GraphicsDevice.CreateNewRender<VertexColor3D>(); 
    render->BindMeshes(&cube, 1);
    
    render->SetProjection(Maths::mat3D::perspective_fov(90.0f, 4.0f / 3, 0.1f, 100));
    render->UseShaderFromFile("basic_tex.glsl");
    // GraphicsDevice->RegisterMeshes();
    // * Loop until the user closes the window */
    while (GraphicsDevice.WindowIsOpen()) {
        GraphicsDevice.BeginRender();
    
        render->ResetData<VertexColor3D>();
        render->Render();
    
        Maths::mat3D mat = Maths::mat3D::transform(trans, scale, rot);
        render->SetCamera(mat.inv());
        ImGui::SliderFloat3("Rot", &rot.x,   -3.14f, 3.14f);
        ImGui::SliderFloat3("Sca", &scale.x,  0,     3    );
        ImGui::SliderFloat3("Tra", &trans.x, -10,    10   );
    
        GraphicsDevice.DebugMenu();
    
        GraphicsDevice.EndRender();
    }
    return 0;
}
