#pragma once
#include "Vector.h"
#include "Color.h"
#include "Variant.h"

namespace Quasi::Graphics {
    struct SunLight {
        Math::fVector3 direction;
    };

    struct PointLight {
        Math::fVector3 position;
        float constant, linear, quadratic;
    };

    struct FlashLight {
        Math::fVector3 position;
        float yaw, pitch, innerCut, outerCut;
    };

    class Light : public Variant<SunLight, PointLight, FlashLight> {
    public:
        Math::fColor color;
    public:
        [[nodiscard]] Math::fVector3 Position() const;
        [[nodiscard]] Math::fVector3 Direction() const;
    };
}
