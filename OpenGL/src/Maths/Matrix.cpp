#include "Matrix.h"

#include <iomanip>
#include <string>

namespace Maths
{
    Matrix3D::Matrix3D(float vals[16], bool horizontalPacking)
    {
        if (horizontalPacking) {
            m11 = vals[0 +0]; m12 = vals[0 +1]; m13 = vals[0 +2]; m14 = vals[0 +3];
            m21 = vals[4 +0]; m22 = vals[4 +1]; m23 = vals[4 +2]; m24 = vals[4 +3];
            m31 = vals[8 +0]; m32 = vals[8 +1]; m33 = vals[8 +2]; m34 = vals[8 +3];
            m41 = vals[12+0]; m42 = vals[12+1]; m43 = vals[12+2]; m44 = vals[12+3];
        } else {
            m11 = vals[0 +0]; m21 = vals[0 +1]; m31 = vals[0 +2]; m41 = vals[0 +3];
            m12 = vals[4 +4]; m22 = vals[4 +5]; m32 = vals[4 +6]; m42 = vals[4 +7];
            m13 = vals[8 +4]; m23 = vals[8 +5]; m33 = vals[8 +6]; m43 = vals[8 +7];
            m14 = vals[12+4]; m24 = vals[12+5]; m34 = vals[12+6]; m44 = vals[12+7];
        }
    }

    Matrix3D::Matrix3D(float _m11, float _m12, float _m13, float _m14,
                       float _m21, float _m22, float _m23, float _m24,
                       float _m31, float _m32, float _m33, float _m34,
                       float _m41, float _m42, float _m43, float _m44, bool horizontalPacking)
    {
        if (horizontalPacking) {
            m11 = _m11; m12 = _m12; m13 = _m13; m14 = _m14;
            m21 = _m21; m22 = _m22; m23 = _m23; m24 = _m24;
            m31 = _m31; m32 = _m32; m33 = _m33; m34 = _m34;
            m41 = _m41; m42 = _m42; m43 = _m43; m44 = _m44;
        } else {
            m11 = _m11; m21 = _m21; m31 = _m31; m41 = _m41;
            m12 = _m12; m22 = _m22; m32 = _m32; m42 = _m42;
            m13 = _m13; m23 = _m23; m33 = _m33; m43 = _m43;
            m14 = _m14; m24 = _m24; m34 = _m34; m44 = _m44;
        }
    }

    Matrix3D::Matrix3D(Vector4 arrs[4], bool verticalPacking)
    {
        if (!verticalPacking)
        {
            m11 = arrs[0].x; m12 = arrs[0].y; m13 = arrs[0].z; m14 = arrs[0].w;
            m21 = arrs[1].x; m22 = arrs[1].y; m23 = arrs[1].z; m24 = arrs[1].w;
            m31 = arrs[2].x; m32 = arrs[2].y; m33 = arrs[2].z; m34 = arrs[2].w;
            m41 = arrs[3].x; m42 = arrs[3].y; m43 = arrs[3].z; m44 = arrs[3].w;
        } else {
            m11 = arrs[0].x; m21 = arrs[0].y; m31 = arrs[0].z; m41 = arrs[0].w;
            m12 = arrs[1].x; m22 = arrs[1].y; m32 = arrs[1].z; m42 = arrs[1].w;
            m13 = arrs[2].x; m23 = arrs[2].y; m33 = arrs[2].z; m43 = arrs[2].w;
            m14 = arrs[3].x; m24 = arrs[3].y; m34 = arrs[3].z; m44 = arrs[3].w;
        }
    }

    Matrix3D::Matrix3D(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4, bool verticalPacking)
    {
        if (!verticalPacking)
        {
            m11 = r1.x; m12 = r1.y; m13 = r1.z; m14 = r1.w;
            m21 = r2.x; m22 = r2.y; m23 = r2.z; m24 = r2.w;
            m31 = r3.x; m32 = r3.y; m33 = r3.z; m34 = r3.w;
            m41 = r4.x; m42 = r4.y; m43 = r4.z; m44 = r4.w;
        } else {
            m11 = r1.x; m21 = r1.y; m31 = r1.z; m41 = r1.w;
            m12 = r2.x; m22 = r2.y; m32 = r2.z; m42 = r2.w;
            m13 = r3.x; m23 = r3.y; m33 = r3.z; m43 = r3.w;
            m14 = r4.x; m24 = r4.y; m34 = r4.z; m44 = r4.w;
        }
    }

    const float* Matrix3D::GetInRow() const
    {
        return (const float*)this;
    }

    const float* Matrix3D::GetInCol() const
    {
        const float* list = new float[]{ m11, m21, m31, m41, m12, m22, m32, m42, m13, m23, m33, m43, m14, m24, m34, m44 };
        return list;
    }

    const Vector4* Matrix3D::GetRows() const
    {
        Vector4* vecs = new Vector4[] { Vector4 {m11, m12, m13, m14},
                                        Vector4 {m11, m12, m13, m14},
                                        Vector4 {m11, m12, m13, m14},
                                        Vector4 {m11, m12, m13, m14} };
        return vecs;
    }

    const Vector4* Matrix3D::GetCols() const
    {
        Vector4* vecs = new Vector4[] { Vector4 {m11, m21, m31, m41},
                                        Vector4 {m12, m22, m32, m42},
                                        Vector4 {m13, m23, m33, m43},
                                        Vector4 {m14, m24, m34, m44} };
        return vecs;
    }

    void Matrix3D::Translate(const Vector3& translation)
    {
        //{{1, 0, 0, t x}, {0, 1, 0, t y}, {0, 0, 1, t z}, {0, 0, 0, 1}} . {{a, b, c, d}, {e, f, g, h}, {I, j, k, l}, {0, 0, 0, 1}}
        m14 += translation.x;
        m24 += translation.y;
        m34 += translation.z;
    }

    Matrix3D Matrix3D::TranslateMat(const Vector3& translation, const Matrix3D& mat)
    {
        return { mat.m11, mat.m12, mat.m13, mat.m14 + translation.x,
                   mat.m21, mat.m22, mat.m23, mat.m24 + translation.y,
                   mat.m31, mat.m32, mat.m33, mat.m34 + translation.z,
                   0,       0,       0,       1                       };
    }

    void Matrix3D::Scale(const Vector3& scale)
    {
        m11 *= scale.x; m12 *= scale.x; m13 *= scale.x; m14 *= scale.x;
        m21 *= scale.y; m22 *= scale.y; m23 *= scale.y; m24 *= scale.y;
        m31 *= scale.z; m32 *= scale.z; m33 *= scale.z; m34 *= scale.z;
    }

    Matrix3D Matrix3D::ScaleMat(const Vector3& scale, const Matrix3D& mat)
    {
        return { mat.m11 * scale.x, mat.m12 * scale.x, mat.m13 * scale.x, mat.m14 * scale.x,
                   mat.m21 * scale.y, mat.m22 * scale.y, mat.m23 * scale.y, mat.m24 * scale.y,
                   mat.m31 * scale.z, mat.m32 * scale.z, mat.m33 * scale.z, mat.m34 * scale.z,
                   0,                 0                , 0                , 1                 };
    }

    void Matrix3D::Rotate(const Vector3& rotation)
    {
        //{{Cos[z] Cos[y], Cos[z] Sin[y] Sin[x] - Sin[z] Cos[x], Cos[z] Sin[y] Sin[x] - Sin[z] Cos[x], 0},
        // {Sin[z] Cos[y], Sin[z] Sin[y] Sin[x] + Cos[z] Cos[x], Sin[z] Sin[y] Sin[x] - Cos[z] Cos[x], 0},
        // {-Sin[y], Cos[y] Sin[x], Cos[y] Sin[x], 0},
        // {0, 0, 0, 1}} . {{a, b, c, d}, {e, f, g, h}, {i, j, k, l}, {0, 0, 0, 1}}
        const float cx = cos(rotation.x);
        const float sx = sin(rotation.x);
        const float cy = cos(rotation.y);
        const float sy = sin(rotation.y);
        const float cz = cos(rotation.z);
        const float sz = sin(rotation.z);
        Matrix3D rotm = { cz * cy, cz * sy * sx - sz * cx, cz * sy * sx - sz * cx, 0,
                          sz * cy, sz * sy * sx + cz * cx, sz * sy * sx - cz * cx, 0,
                         -sy     , cy * sx               , cy * sx               , 0,
                          0      , 0                     , 0                     , 1};
        *this = rotm * *this;
    }

    Matrix3D Matrix3D::RotateMat(const Vector3& rotation, const Matrix3D& mat)
    {
        const float cx = cos(rotation.x);
        const float sx = sin(rotation.x);
        const float cy = cos(rotation.y);
        const float sy = sin(rotation.y);
        const float cz = cos(rotation.z);
        const float sz = sin(rotation.z);
        Matrix3D rotm = { cy * cz, sx * sy * cz - cx * sz, cx * sy * cz + sx * sz, 0,
                          cy * sz, sx * sy * sz + cx * cz, cx * sy * sz - sx * cz, 0,
                         -sy     , sx * cy               , cx * cy               , 0,
                          0      , 0                     , 0                     , 1};
        return rotm * mat;
    }

    Matrix3D Matrix3D::OrthoProjection(float lf, float rh, float dw, float up, float bc, float fr)
    {
        const float x = 2.0f / (rh - lf);
        const float y = 2.0f / (up - dw);
        const float z = 2.0f / (fr - bc);
        return {
            x   , 0.0f, 0.0f, -(lf * x + 1),
            0.0f, y   , 0.0f, -(dw * y + 1),
            0.0f, 0.0f, z   , -(bc * z + 1),
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    Matrix3D Matrix3D::Transform(const Vector3& translate, const Vector3& scale, const Vector3& rotate)
    {
        return TranslateMat(translate) * RotateMat(rotate) * ScaleMat(scale);
    }

    Vector3 operator*(const Matrix3D& t, const Vector3& v)
    {
        return { t.m11 * v.x + t.m12 * v.y + t.m13 * v.z + t.m14,
                 t.m21 * v.x + t.m22 * v.y + t.m23 * v.z + t.m24,
                 t.m31 * v.x + t.m32 * v.y + t.m33 * v.z + t.m34};
    }

    Vector4 operator*(const Matrix3D& t, const Vector4& v)
    {
        return { t.m11 * v.x + t.m12 * v.y + t.m13 * v.z + t.m14 * v.w,
                 t.m21 * v.x + t.m22 * v.y + t.m23 * v.z + t.m24 * v.w,
                 t.m31 * v.x + t.m32 * v.y + t.m33 * v.z + t.m34 * v.w,
                 t.m41 * v.x + t.m42 * v.y + t.m43 * v.z + t.m44 * v.w};
    }

    Matrix3D operator*(const Matrix3D& t, const Matrix3D& m)
    {
        const Vector4* c = m.GetCols();
        return { t * c[0], t * c[1], t * c[2], t * c[3] };
    }

    std::ostream& operator<<(std::ostream& stream, const Matrix3D& mat)
    {
        stream << std::fixed << std::setprecision(3);
        stream << '[' << mat.m11 << ',' << mat.m12 << ',' << mat.m13 << ',' << mat.m14 << ']' << '\n';
        stream << '[' << mat.m21 << ',' << mat.m22 << ',' << mat.m23 << ',' << mat.m24 << ']' << '\n';
        stream << '[' << mat.m31 << ',' << mat.m32 << ',' << mat.m33 << ',' << mat.m34 << ']' << '\n';
        stream << '[' << mat.m41 << ',' << mat.m42 << ',' << mat.m43 << ',' << mat.m44 << ']';// << '\n'
        stream << std::setprecision(-1);
        return stream;
    }
}
