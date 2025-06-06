#include "PolygonShape2D.h"

#include "SeperatingAxisSolver.h"

namespace Quasi::Physics2D {
    template <class T> void BasicPolygonShape<T>::FixPolygon() {
        for (u32 i = 0; i < Size(); ++i) {
            UpdateLenBtwn(i);
        }
        FixCenterOfMass();
    }

    template <class T> void BasicPolygonShape<T>::FixCenterOfMass() {
        const fv2 centroid = CenterOfMass();
        for (u32 i = 0; i < Size(); ++i) {
            PointAt(i) -= centroid;
        }
    }

    template <class T> float BasicPolygonShape<T>::ComputeArea() const {
        float area = 0;

        u32 i = 0;
        for (; i < Size() - 1; ++i)
            area += PointAt(i).x * PointAt(i + 1).y - PointAt(i + 1).x * PointAt(i).y;
        // split modulo branch
        area += PointAt(i).x * PointAt(0).y - PointAt(0).x * PointAt(i).y;

        return std::abs(area) / 2;
    }

    template <class T> fRect2D BasicPolygonShape<T>::ComputeBoundingBox() const {
        fRect2D rect = fRect2D::AntiDomain();
        for (u32 i = 0; i < Size(); ++i) rect.ExpandToFit(PointAt(i));
        return rect;
    }

    template <class T> fv2 BasicPolygonShape<T>::CenterOfMass() const {
        float x = 0.0f, y = 0.0f, area = 0.0f;
        u32 i = 0;
        for (; i < Size() - 1; i++) {
            const fv2& p0 = PointAt(i), &p1 = PointAt(i + 1);
            const float areaSum = (p0.x * p1.y) - (p1.x * p0.y);

            x += (p0.x + p1.x) * areaSum;
            y += (p0.y + p1.y) * areaSum;
            area += areaSum;
        }
        // split modulo branch
        const fv2& p0 = PointAt(i), &p1 = PointAt(0);
        const float areaSum = (p0.x * p1.y) - (p1.x * p0.y);

        x += (p0.x + p1.x) * areaSum;
        y += (p0.y + p1.y) * areaSum;
        area += areaSum;

        const float invArea = 1.0f / 3.0f / area;
        return { x * invArea, y * invArea };
    }

    template <class T> float BasicPolygonShape<T>::Inertia() const {
        float inertNum = 0.0f, inertDen = 0.0f;
        u32 i = 0;
        for (; i < Size() - 1; ++i) {
            const float z = PointAt(i + 1).CrossZ(PointAt(i));
            inertDen += z;
            const float uSquared = PointAt(i).LenSq(), vSquared = PointAt(i + 1).LenSq();
            inertNum += z * (uSquared + vSquared + std::sqrt(uSquared * vSquared));
        }
        return inertNum / (inertDen * 6);
    }

    template <class T> fv2 BasicPolygonShape<T>::NearestPointTo(const fv2& point) const {
        u32 nearest = 0;
        float mindist = point.DistSq(PointAt(0));
        for (u32 i = 1; i < Size(); ++i) {
            if (const float d = point.DistSq(PointAt(i)); d < mindist) {
                mindist = d;
                nearest = i;
            }
        }
        return PointAt(nearest);
    }

    template <class T> fv2 BasicPolygonShape<T>::FurthestAlong(const fv2& normal) const {
        u32 furthest = 0;
        float m = normal.Dot(PointAt(0));
        for (u32 i = 1; i < Size(); ++i) {
            if (const float d = normal.Dot(PointAt(i)); d > m) {
                m = d;
                furthest = i;
            }
        }
        return PointAt(furthest);
    }

    template <class T> fLine2D BasicPolygonShape<T>::BestEdgeFor(const fv2& normal) const {
        float maxDepth = normal.Dot(PointAt(0));
        i32 furthest = 0;
        for (i32 i = 1; i < Size(); ++i) {
            if (const float d = normal.Dot(PointAt(i)); d > maxDepth) {
                maxDepth = d;
                furthest = i;
            }
        }

        const fv2 &p  = PointAt(furthest),
                   f0 = PointAtWrap(furthest + 1) - p,
                   f1 = PointAtWrap(furthest - 1) - p;
        if (std::abs(f0.Dot(normal)) * InvLenBtwn(furthest) >
            std::abs(f1.Dot(normal)) * InvLenBtwnWrap(furthest - 1))
            return { p, f1 };
        return { p, f0 };
    }

    template <class T> fRange BasicPolygonShape<T>::ProjectOntoAxis(const fv2& axis) const {
        fRange range = fRange::AntiDomain();
        for (u32 i = 0; i < Size(); ++i) {
            const float d = axis.Dot(PointAt(i));
            range.ExpandToFit(d);
        }
        return range;
    }

    template <class T> fRange BasicPolygonShape<T>::ProjectOntoOwnAxis(u32 axisID, const fv2& axis) const {
        fRange range = fRange::On(axis.Dot(PointAt(axisID)));
        for (u32 i = 0; i < Size(); ++i) {
            if (i == axisID || i == WrapIndex(axisID + 1)) continue;
            const float d = axis.Dot(PointAt(i));
            range.ExpandToFit(d);
        }
        return range;
    }

    template <class T> bool BasicPolygonShape<T>::AddSeperatingAxes(SeperatingAxisSolver& sat) const {
        bool success = false;
        u32 i = 0;
        for (; i < Size() - 1; ++i) {
            success |= sat.CheckAxis((PointAt(i + 1) - PointAt(i)).PerpendLeft() * InvLenBtwn(i));
        }
        success |= sat.CheckAxis((PointAt(0) - PointAt(i)).PerpendLeft() * InvLenBtwn(i));
        return success;
    }

    template class StaticPolygonShape<3>;
    template class StaticPolygonShape<4>;
    template class BasicPolygonShape<StaticPolygonShape<3>>;
    template class BasicPolygonShape<StaticPolygonShape<4>>;
    template class BasicPolygonShape<DynPolygonShape>;

    template <u32 N> StaticPolygonShape<N>::StaticPolygonShape(Span<const fv2> ps) {
        for (u32 i = 0; i < N; ++i) {
            points[i]   = ps[i];
            invDists[i] = 1 / ps[(i + 1) % N].Dist(ps[i]);
        }
        this->FixCenterOfMass();
    }


    DynPolygonShape::DynPolygonShape(Span<const fv2> points) : data(Vec<PointWithInvDist>::WithCap(points.Length())) {
        u32 i = 0;
        for (; i < points.Length() - 1; ++i) {
            data.Push({ points[i], 1 / points[i + 1].Dist(points[i]) });
        }
        data.Push({ points[i], 1 / points[0].Dist(points[i]) });
        FixCenterOfMass();
    }

    u32 DynPolygonShape::Size() const {
        return data.Length();
    }

    void DynPolygonShape::AddPoint(const fv2& p) {
        // needs to update last and first distances
        data.Push({ p, 1 / p.Dist(PointAt(0)) });
        UpdateLenBtwn((i32)(data.Length() - 2));
    }

    void DynPolygonShape::AddPoint(const fv2& p, u32 i) {
        // doesnt need i + 1 because length is not updated
        data.Insert({ p, 1 / p.Dist(PointAtWrap((i32)i)) }, i);
        UpdateLenBtwnWrap((i32)i - 1);
    }

    void DynPolygonShape::RemovePoint(u32 i) {
        data.Pop(i);
        UpdateLenBtwnWrap((i32)i);
    }

    void DynPolygonShape::PopPoint() {
        data.Pop();
        UpdateLenBtwn(0);
    }

    void DynPolygonShape::SetPoint(const fv2& p, i32 i) {
        PointAt(i) = p;
        InvLenBtwn(i) = 1 / p.Dist(PointAtWrap(i + 1));
    }
}
