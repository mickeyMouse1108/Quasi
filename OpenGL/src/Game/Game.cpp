#include "Game.h"

#include "imgui.h"

namespace Game
{
    Game::Game()
    {
        GraphicsDevice = Graphics::GraphicsDevice::Initialize();

        GraphicsDevice->UseShader("res/shaders/basic.glsl");
        // https://en.wikipedia.org/wiki/Isometric_projection#Overview for calculations
        constexpr float ISO_X_ROT = 0.78539816339744830961566084581988f; // simply 45° ( 2π / 8 )
        constexpr float ISO_Y_ROT = 0.61547970867038734106746458912399f; /* since sinθ = 1 (top corner y),
                                                                          and cosθ = √2 (length of corner to corner x),
                                                                        sinθ / cosθ = tanθ = 1 / √2. θ = atan1/√2.
                                                                         */
        constexpr float ISO_Z_ROT = 1.57079632679489661923132169163980f; // rotate 90°. ( 2π / 4 )
        rot = {ISO_X_ROT, ISO_Y_ROT, ISO_Z_ROT};

        world.DisplayTo(*GraphicsDevice);
        // cube.Bind(*GraphicsDevice);
        // LOG(cube.IsBound());
        GraphicsDevice->RegisterElements();
    }
    
    void Game::Update()
    {
        // Time.Update();
    }

    void Game::Render()
    {
        GraphicsDevice->BeginRender();
        
        GraphicsDevice->ClearRegistered();
        GraphicsDevice->RegisterElements();
        GraphicsDevice->EnableShader();
        GraphicsDevice->RenderRegistered();

        GraphicsDevice->SetCamera(Maths::Matrix3D::Transform(trans, scale, rot));
        ImGui::SliderFloat3("Rot", (float*)&rot, 0, 5);
        ImGui::SliderFloat3("Sca", (float*)&scale, 0, 200);
        ImGui::SliderFloat3("Tra", (float*)&trans, -100, 100);
        
        GraphicsDevice->DebugMenu();
        
        GraphicsDevice->EndRender();
    }
    
    Game::~Game()
    {
        // cube.Unbind();
        // cube.Delete();
    }
}
