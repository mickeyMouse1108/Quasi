#include "IO.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace IO {
    void Init(Graphics::GraphicsDevice& gd) {
        Mouse = MouseT { gd };
        Keyboard = KeyboardT { gd };
    }

    void Update() {
        Mouse.Update();
        Keyboard.Update();
    }
}
