#include "Body2D.h"

#include "Collision2D.h"
#include "World2D.h"

namespace Quasi::Physics2D {
    void Body::AddMomentum(const fv2& newtonSeconds) {
        velocity += newtonSeconds * invMass;
    }

    // void Body::AddForce(const fv2& newton) {
    //     acceleration += newton * invMass;
    // }

    void Body::AddAngularMomentum(float angMomentum) {
        angularVelocity += angMomentum * invInertia;
    }

    // void Body::AddTorque(float torque) {
    //     angularAcceleration += torque * invInertia;
    // }

    void Body::AddRelativeVelocity(const fv2& relPosition, const fv2& vel) {
        AddMomentum(vel);
        AddAngularMomentum(relPosition.CrossZ(vel));
    }

    void Body::AddVelocityAt(const fv2& absPosition, const fv2& vel) {
        return AddRelativeVelocity(absPosition - position, vel);
    }

    void Body::SetMass(float newMass) {
        inertia *= newMass / mass;
        invInertia = inertia > 0 ? 1 / inertia : 0;
        mass = newMass;
        invMass = mass > 0 ? 1 / mass : 0;
    }

    Manifold Body::CollideWith(const Body& target) const {
        return CollideWith(target.shape, target.GetTransform());
    }

    Manifold Body::CollideWith(const Shape& target, const PhysicsTransform& xf) const {
        return CollideShapes(shape, GetTransform(), target, xf);
    }

    bool Body::OverlapsWith(const Body& target) const {
        return OverlapsWith(target.shape, target.GetTransform());
    }

    bool Body::OverlapsWith(const Shape& target, const PhysicsTransform& xf) const {
        return OverlapShapes(shape, GetTransform(), target, xf);
    }

    PhysicsTransform Body::GetTransform() const {
        return { position, rotation };
    }

    void Body::Update(float dt) {
        position += velocity * dt;
        rotation += Radians(angularVelocity * dt);
        TryUpdateTransforms();
    }

    void Body::TryUpdateTransforms() {
        if (shapeHasChanged) {
            baseBoundingBox = shape.ComputeBoundingBox();
            inertia = shape.Inertia() * mass;
            invInertia = inertia > 0 ? 1 / inertia : 0;
            shapeHasChanged = false;
        }
        boundingBox = GetTransform().TransformRect(baseBoundingBox);
    }

    void Body::SetShapeHasChanged() {
        shapeHasChanged = true;
    }

    fRect2D Body::BoundingBox() const {
        return boundingBox;
    }
} // Physics2D