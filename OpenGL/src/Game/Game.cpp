#include "Game.h"

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
        GraphicsDevice->SetCamera(Maths::Matrix3D::Transform({0, 0, 0}, {50.0f, 50.0f, 50.0f}, {ISO_X_ROT, ISO_Y_ROT, ISO_Z_ROT}));
        block.GetMeshObjectForm().Bind(*GraphicsDevice);
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
        
        GraphicsDevice->DebugMenu();
        
        GraphicsDevice->EndRender();
    }
    
    Game::~Game()
    {
        // cube.Unbind();
        // cube.Delete();
    }
}
