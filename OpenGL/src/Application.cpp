#define TEST_MODE 0

#if TEST_MODE == 1
#include "GraphicsDevice.h"
#include "TestingFramework.h"
#else
#include "Game/Game.h"
#endif

int main()
{
#if TEST_MODE == 1
    std::unique_ptr<Graphics::GraphicsDevice> GraphicsDevice = Graphics::GraphicsDevice::Initialize();
    Test::TestManager testm;
    // renderer.SetRenderMode('w');
    
    /* Loop until the user closes the window */
    while (GraphicsDevice->WindowIsOpen())
    {
        testm.OnUpdate();
        
        GraphicsDevice->BeginRender();
        
        testm.OnImGuiRender();
        testm.OnRender(GraphicsDevice->GetRenderer());

        GraphicsDevice->EndRender();
    }
    return 0;
#else
    Game::Game game{};
    /* Loop until the user closes the window */
    while (game.IsWindowOpen())
    {
        game.Update();
        
        game.Render();
    }
    return 0;
#endif
}
