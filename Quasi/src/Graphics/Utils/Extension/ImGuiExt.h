#pragma once
#include <imgui.h>
#include "Utils/Str.h"
#include "Utils/Type.h"
#include "Math/Rect.h"
#include "Math/Color.h"

namespace Quasi::Graphics {
    class CameraController;
    class Light;
}

namespace ImGui {
#define Q Quasi::
#define Q_IMGUI_EDITOR(NAME, ...) void NAME(Q Str title, __VA_ARGS__, float width = Q floats::NAN)

    template <class T> ImGuiDataType ImGuiDataEnum();

    float GetUsableWidth();
    float GetRemWidth(float width = Q floats::NAN);
    float GetLastItemWidth();
    float GetSpacingWidth();
    float GetItemDefaultWidth(float totalWidth = Q floats::NAN);
    float GetItemRemainingWidth(float totalWidth = Q floats::NAN);

    void DisplaySimpleIcon(const char* text, const Q Math::Color& bgColor);

    void DisplayTextCropped(Q Str text, float width);

    void EditString(Q Str label, Q String& string);

    template <class T> requires std::is_arithmetic_v<T>
    Q_IMGUI_EDITOR(EditScalar, T& value, float speed = 1, Q NoInfer<Q Option<Q Math::Range<T>>> range = nullptr);

    template <class T> requires std::is_arithmetic_v<T>
    Q_IMGUI_EDITOR(EditScalarWithIcon, const Q Math::Color& color,
        T& value, float speed = 1, Q NoInfer<Q Option<Q Math::Range<T>>> range = nullptr, Q Str fmt = {}) {
        PushID(title.Data());
        DisplaySimpleIcon(title.Data(), color);

        SetNextItemWidth(GetRemWidth(width) - GetLastItemWidth() - GetSpacingWidth());
        SameLine();
        DragScalar("##value", ImGuiDataEnum<T>(), &value, speed, range ? &range->min : nullptr, range ? &range->max : nullptr, fmt.Data());
        PopID();
    }

    template <Q u32 N, class T>
    Q_IMGUI_EDITOR(EditVector, Q Math::VectorN<N, T>& vector, float speed = 1, Q NoInfer<const Q Option<Q Math::RectN<N, T>>&> range = nullptr);

    template <class T>
    Q_IMGUI_EDITOR(EditRange, Q Math::Range<T>& range, float speed = 1, Q NoInfer<const Q Option<Q Math::Range<T>>&> constraint = nullptr);

    template <Q u32 N, class T>
    Q_IMGUI_EDITOR(EditRect, Q Math::RectN<N, T>& range, float speed = 1, Q NoInfer<const Q Option<Q Math::RectN<N, T>>&> constraint = nullptr);

    Q_IMGUI_EDITOR(EditComplexRotation, Q Math::Complex<float>& complex);
    Q_IMGUI_EDITOR(EditQuatRotation, Q Math::Quaternion& quaternion);

    Q_IMGUI_EDITOR(EditRotation, float& yaw, float& pitch);

    template <Q Math::ColorLike C>
    Q_IMGUI_EDITOR(EditColor, C& color);

    Q_IMGUI_EDITOR(EditTransform, Q Math::Transform2D& transform, float speed = 1);
    Q_IMGUI_EDITOR(EditTransform, Q Math::Transform3D& transform, float speed = 1);

    Q_IMGUI_EDITOR(EditCameraController, Q Graphics::CameraController& camera);
    Q_IMGUI_EDITOR(EditLight, Q Graphics::Light& light);

#undef Q
#undef Q_IMGUI_EDITOR
} // ImGui