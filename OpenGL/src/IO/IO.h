#pragma once
// ! THIS INCLUDES GLEW.H !

namespace Graphics {
    class GraphicsDevice;
}

namespace IO {
    // inline extern MouseT Mouse;
    // inline extern KeyboardT Keyboard;

    void Init(Graphics::GraphicsDevice& gd);
    void Update();
}
