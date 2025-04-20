#pragma once

#include "Utils/Type.h"

namespace Quasi::Graphics {
    using GraphicsID = u32;
    constexpr GraphicsID GraphicsNoID = 0;

    template <class G>
    class GLObject {
    public:
        GraphicsID rendererID = GraphicsNoID;

        explicit GLObject() = default;
        explicit GLObject(GraphicsID id) : rendererID(id) {}

        ~GLObject() { Destroy(); }

        GLObject(const GLObject&) = delete;
        GLObject& operator=(const GLObject&) = delete;
        GLObject(GLObject&& obj) noexcept : rendererID(obj.rendererID) { obj.rendererID = GraphicsNoID; }
        GLObject& operator=(GLObject&& obj) noexcept {
            G::DestroyObject(rendererID);
            rendererID = obj.rendererID;
            obj.rendererID = GraphicsNoID;
            return *this;
        }

        void Bind() const { G::BindObject(rendererID); }
        void Unbind() const { G::UnbindObject(); }

        void Destroy() { G::DestroyObject(rendererID); rendererID = GraphicsNoID; }

        bool IsNull() const { return rendererID == GraphicsNoID; }

        explicit operator bool() const { return !IsNull(); }
        explicit operator GraphicsID() const { return rendererID; }
    };
}
