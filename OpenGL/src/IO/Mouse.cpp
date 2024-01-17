#include "GraphicsDevice.h"

#include "Mouse.h"

namespace IO {
    MouseT Mouse { nullptr };
    
    GLFWwindow* MouseT::inputWindow() { return graphicsDevice->GetWindow(); }
    const GLFWwindow* MouseT::inputWindow() const { return graphicsDevice->GetWindow(); }

    MouseT::MouseT(Graphics::GraphicsDevice& gd) : graphicsDevice(gd) {}

    void MouseT::Update() {
        prevMouseStates = mouseStates;
        mouseStates = 0;
        for (int i = 0; i <= LAST_MOUSE; ++i) {
            mouseStates |= glfwGetMouseButton(inputWindow(), i) << i;
        }
    }

    Maths::dvec2 MouseT::GetMousePosPx() {
        Maths::dvec2 pos;
        glfwGetCursorPos(inputWindow(), &pos.x, &pos.y);
        return pos;
    }

    Maths::dvec2 MouseT::GetMousePos() {
        auto r01 = GetMousePosPx() / graphicsDevice->GetWindowSize(); // range 0 - 1
        return r01 * 2.0 - 1.0;
    }

    bool MouseT::IsInWindow() {
        auto [mouseX,  mouseY ] = GetMousePosPx();
        auto [borderX, borderY] = graphicsDevice->GetWindowSize();
        return 0 <= mouseX && mouseX <= borderX &&
               0 <= mouseY && mouseY <= borderY;
    }

    int  MouseT::PressedState()         const { return mouseStates; }
    bool MouseT::LeftPressed()          const { return PressedState() & 1 << LEFT_MOUSE;   }
    bool MouseT::RightPressed()         const { return PressedState() & 1 << RIGHT_MOUSE;  }
    bool MouseT::MiddlePressed()        const { return PressedState() & 1 << MIDDLE_MOUSE; }
    bool MouseT::ButtonPressed(int btn) const { return PressedState() & 1 << btn;          }
    bool MouseT::AnyPressed()           const { return PressedState() != 0; }
    bool MouseT::NonePressed()          const { return PressedState() == 0; }
    
    int  MouseT::OnPressState()         const { return ~prevMouseStates & mouseStates; }
    bool MouseT::LeftOnPress()          const { return OnPressState() & 1 << LEFT_MOUSE;   }
    bool MouseT::RightOnPress()         const { return OnPressState() & 1 << RIGHT_MOUSE;  }
    bool MouseT::MiddleOnPress()        const { return OnPressState() & 1 << MIDDLE_MOUSE; }
    bool MouseT::ButtonOnPress(int btn) const { return OnPressState() & 1 << btn;          }
    bool MouseT::AnyOnPress()           const { return OnPressState() != 0; }
    bool MouseT::NoneOnPress()          const { return OnPressState() == 0; }
    
    int  MouseT::OnReleaseState()         const { return prevMouseStates & ~mouseStates; }
    bool MouseT::LeftOnRelease()          const { return OnReleaseState() & 1 << LEFT_MOUSE;   }
    bool MouseT::RightOnRelease()         const { return OnReleaseState() & 1 << RIGHT_MOUSE;  }
    bool MouseT::MiddleOnRelease()        const { return OnReleaseState() & 1 << MIDDLE_MOUSE; }
    bool MouseT::ButtonOnRelease(int btn) const { return OnReleaseState() & 1 << btn;          }
    bool MouseT::AnyOnRelease()           const { return OnReleaseState() != 0; }
    bool MouseT::NoneOnRelease()          const { return OnReleaseState() == 0; }
    
    bool MouseT::IsStandardMouseButton(int btn) { return LEFT_MOUSE <= btn && btn <= MIDDLE_MOUSE; }
    bool MouseT::IsValidMouseButton(int btn) { return LEFT_MOUSE <= btn && btn <= LAST_MOUSE; }

    const char* MouseT::MouseButtonToStr(int btn) {
        switch (btn) {
            case LEFT_MOUSE:   return "Left Mouse Button";
            case RIGHT_MOUSE:  return "Right Mouse Button";
            case MIDDLE_MOUSE: return "Middle Mouse Button";

            // not a great method but i want to use const char*
            case 3: return "No. 3 Mouse Button";
            case 4: return "No. 4 Mouse Button";
            case 5: return "No. 5 Mouse Button";
            case 6: return "No. 6 Mouse Button";
            case 7: return "No. 7 Mouse Button";
            default: return nullptr;
        }
    }
}
