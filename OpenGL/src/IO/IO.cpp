#include "IO.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace IO {
    void Init() {
        Mouse = {};
        Keyboard = {};
    }

    void Update() {
        Mouse.Update();
        Keyboard.Update();
    }
}
