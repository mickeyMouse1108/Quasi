#pragma once
#include "Constants.h"
#include "Quaternion.h"

namespace Quasi::Math {
    struct MatrixTransform3D {
        Matrix3D transform, normalMatrix;

        [[nodiscard]] fVector3 Transform(const fVector3& p) const { return transform * p; }
        [[nodiscard]] fVector3 TransformNormal(const fVector3& n) const { return (normalMatrix * n).norm(); }
    };

    template <class T> concept ITransformer3D = requires (const T& t, fVector3 vec) {
        { t.Transform(vec) } -> std::convertible_to<fVector3>;
        { t.TransformNormal(vec) } -> std::convertible_to<fVector3>;
    };

    template <class T>
    struct InverseTransform3D {
        static_assert(ITransformer3D<T>, "T should be a transformer3d"); // delayed constraint
        const T& tformer;
        [[nodiscard]] fVector3 Transform (const fVector3& point) const { return tformer.TransformInverse(point); }
                      void     TransformInplace(fVector3& point) const { tformer.TransformInverseInplace(point); }
        [[nodiscard]] fVector3 TransformInverse (const fVector3& point) const { return tformer.Transform(point); }
                      void     TransformInverseInplace(fVector3& point) const { tformer.TransformInplace(point); }
        [[nodiscard]] fVector3 TransformNormal (const fVector3& normal) const { return tformer.TransformInverseNormal(normal); }
                      void     TransformNormalInplace(fVector3& normal) const { tformer.TransformInverseNormalInplace(normal); }
        [[nodiscard]] fVector3 TransformInverseNormal (const fVector3& normal) const { return tformer.TransformNormal(normal); }
                      void     TransformInverseNormalInplace(fVector3& normal) const { tformer.TransformNormalInplace(normal); }
    };

    struct Transform3D {
        fVector3 position = 0, scale = 1;
        Quaternion rotation = 1;

        Transform3D() = default;
        Transform3D(const fVector3& pos, const fVector3& scale, const Quaternion& rot)
            : position(pos), scale(scale), rotation(rot) {}
        Transform3D(const fVector3& pos, const fVector3& scale = 1, const fVector3& rot = 0.0f)
            : Transform3D(pos, scale, Quaternion::from_euler(rot)) {}

        template <class Quat>
        struct RotationAngleProxy {
            Quat& r;

            operator fVector3() const { return r.to_euler(); }
            RotationAngleProxy& operator=(const fVector3& angle) requires (!std::is_const_v<Quat>)
            { r = Quaternion::from_euler(angle); return *this; }

            [[nodiscard]] fVector3 Radians() const { return r.to_euler(); }
            [[nodiscard]] fVector3 Degrees() const { return Radians() * RAD2DEG; }

            [[nodiscard]] const Quat& AsComplex() const { return r; }
            Quat& AsComplex() { return r; }
        };

        [[nodiscard]] RotationAngleProxy<const Quaternion> RotationAngle() const { return { rotation }; }
        RotationAngleProxy<Quaternion> RotationAngle() { return { rotation }; }

        void Translate(const fVector3& p);
        void Scale(const fVector3& s);
        void Rotate(const fVector3& r);
        void Rotate(const Quaternion& q);
        void RotateX(float r);
        void RotateY(float r);
        void RotateZ(float r);

        [[nodiscard]] Transform3D Translated(const fVector3& p) const;
        [[nodiscard]] Transform3D Scaled(const fVector3& s) const;
        [[nodiscard]] Transform3D Rotated(const fVector3& r) const;
        [[nodiscard]] Transform3D Rotated(const Quaternion& q) const;
        [[nodiscard]] Transform3D RotatedX(float r) const;
        [[nodiscard]] Transform3D RotatedY(float r) const;
        [[nodiscard]] Transform3D RotatedZ(float r) const;

        static Transform3D Translation(const fVector3& p);
        static Transform3D Scaling(const fVector3& s);
        static Transform3D Rotation(const fVector3& r);
        static Transform3D Rotation(const Quaternion& q);

        [[nodiscard]] Transform3D NormalTransform() const;

        [[nodiscard]] fVector3 Transform (const fVector3& point) const;
                      void     TransformInplace(fVector3& point) const;
        [[nodiscard]] fVector3 TransformInverse (const fVector3& point) const;
                      void     TransformInverseInplace(fVector3& point) const;
        [[nodiscard]] fVector3 TransformNormal (const fVector3& normal) const;
                      void     TransformNormalInplace(fVector3& normal) const;
        [[nodiscard]] fVector3 TransformInverseNormal (const fVector3& normal) const;
                      void     TransformInverseNormalInplace(fVector3& normal) const;

        [[nodiscard]] InverseTransform3D<Transform3D> Inverse() const { return { *this }; }

        [[nodiscard]] Transform3D Applied(const Transform3D& transformer) const; // apply transform onto self
        [[nodiscard]] Transform3D AppliedTo(const Transform3D& transformed) const { return transformed.Applied(*this); }
        Transform3D& Apply(const Transform3D& transformer); // apply transform onto self
        void ApplyTo(Transform3D& transformed) const { transformed.Apply(*this); }

        [[nodiscard]] Transform3D Then(const Transform3D& t) const { return AppliedTo(t); }

        void Reset();

        [[nodiscard]] Matrix3x3 LinearMatrix() const;
        [[nodiscard]] Matrix3D  TransformMatrix() const;

        [[nodiscard]] fVector3 operator*(const fVector3& p) const { return Transform(p); }
        [[nodiscard]] Transform3D operator*(const Transform3D& t) const { return Applied(t); }
        Transform3D& operator*=(const Transform3D& t) { return Apply(t); }
    };

    template <class T> VectorN<3, T> VectorN<3, T>::transformed_by(const Transform3D& transform) const {
        return transform.Transform(*this);
    }
    template <class T> VectorN<3, T>& VectorN<3, T>::transform_by(const Transform3D& transform) {
        return transform.TransformInplace(*this);
    }
} // Quasi