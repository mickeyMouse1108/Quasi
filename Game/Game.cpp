#include <iostream>
#include "Graphics/Graphicals/GraphicsDevice.h"
#include "test/TestingFramework.h"

int main() {
    std::unique_ptr<Graphics::GraphicsDevice> GraphicsDevice = Graphics::GraphicsDevice::Initialize();
    Test::TestManager testm;
    /* Loop until the user closes the window */
    while (GraphicsDevice->WindowIsOpen()) {
        testm.OnUpdate();

        GraphicsDevice->BeginRender();

        testm.OnImGuiRender();
        testm.OnRender(GraphicsDevice->GetRenderer());

        GraphicsDevice->EndRender();
    }
}
