#include "Transform2D.h"

#include "Transform3D.h"

namespace Quasi::Math {
    void Transform2D::Translate(const fVector2& p) { position += p; }
    void Transform2D::Scale(const fVector2& s) { scale *= s; }
    void Transform2D::Rotate(float r) { Rotate(fComplex::rotate(r)); }
    void Transform2D::Rotate(const fComplex& r) { rotation *= r; }

    Transform2D Transform2D::Translated(const fVector2& p) const {
        return { position + p, scale, rotation };
    }
    Transform2D Transform2D::Scaled(const fVector2& s) const {
        return { position, scale * s, rotation };
    }
    Transform2D Transform2D::Rotated(float r) const {
        return Rotated(fComplex::rotate(r));
    }
    Transform2D Transform2D::Rotated(const fComplex& r) const {
        return { position, scale, rotation * r };
    }

    Transform2D Transform2D::Translation(const fVector2& p) { return { p }; }
    Transform2D Transform2D::Scaling(const fVector2& s)     { return { 0, s }; }
    Transform2D Transform2D::Rotation(float r)              { return Rotation(fComplex::rotate(r)); }
    Transform2D Transform2D::Rotation(const fComplex& q)    { return { 0, 1, q }; }

    Transform2D Transform2D::NormalTransform() const {
        return { 0, 1.0f / scale, rotation };
    }

    fVector2 Transform2D::Transform(const fVector2& point) const {
        return (point * scale).rotated_by(rotation) + position;
    }

    void Transform2D::TransformInplace(fVector2& point) const {
        point *= scale;
        point.rotate_by(rotation);
        point += position;
    }

    fVector2 Transform2D::TransformInverse(const fVector2& point) const {
        return rotation.invrotate(point - position) / scale;
    }

    void Transform2D::TransformInverseInplace(fVector2& point) const {
        point -= position;
        point = rotation.invrotate(point);
        point /= scale;
    }

    fVector2 Transform2D::TransformNormal(const fVector2& normal) const {
        return (normal / scale).rotated_by(rotation).norm();
    }

    void Transform2D::TransformNormalInplace(fVector2& normal) const {
        normal /= scale;
        normal.rotate_by(rotation);
        normal /= normal.len();
    }

    fVector2 Transform2D::TransformInverseNormal(const fVector2& normal) const {
        return (rotation.invrotate(normal) * scale).norm();
    }

    void Transform2D::TransformInverseNormalInplace(fVector2& normal) const {
        normal = rotation.invrotate(normal);
        normal *= scale;
        normal /= normal.len();
    }

    Transform2D Transform2D::Applied(const Transform2D& transformer) const {
        return { transformer.Transform(position), scale * transformer.scale, rotation * transformer.rotation };
    }

    Transform2D& Transform2D::Apply(const Transform2D& transformer) {
        position.transform_by(transformer);
        scale *= transformer.scale;
        rotation *= transformer.rotation;
        return *this;
    }

    Transform3D Transform2D::As3D() const {
        return { position.with_z(0), scale.with_z(1), Quaternion::rotate_axis({ 0, 0, 1 }, rotation) };
    }

    void Transform2D::Reset() {
        position = 0;
        scale = 1;
        rotation = 1;
    }

    Matrix2x2 Transform2D::LinearMatrix() const {
        return { fVector2 {  rotation.re * scale.x, rotation.im * scale.x },
                 fVector2 { -rotation.im * scale.y, rotation.re * scale.y }};
    }

    Matrix2D Transform2D::TransformMatrix() const {
        return { fVector3 {  rotation.re * scale.x, rotation.im * scale.x, 0 },
                 fVector3 { -rotation.im * scale.y, rotation.re * scale.y, 0 },
                 fVector3 {  position.x,            position.y           , 1 } };
    }
} // Quasi