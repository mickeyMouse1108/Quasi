#pragma once
#include "Constants.h"
#include "Mesh.h"
#include "MeshConstructor.h"
#include "VertexConverter.h"

namespace Quasi::Graphics::MeshUtils {
    struct StadiumCreator;

    template <>
    struct OptionsFor<StadiumCreator> {
        Math::fVector2 start, end;
        float radius;
        u32 subdivisions;

        using MData = VertexBuilder::MeshConstructData2D;
    };

    struct StadiumCreator : MeshConstructor<StadiumCreator> {
        Options opt;
        StadiumCreator(const Options& options) : opt(options) {}

        template <FnArgs<MData> F>
        void MergeImpl(F&& f, Mesh<ResultingV<F>>& mesh) {
            using namespace Math;
            auto& vert = mesh.vertices;
            auto& ind  = mesh.indices;
            const u32 iOffset = vert.size();

            const fVector2 X = (opt.start - opt.end).norm(opt.radius);
            const fVector2 Y = { X.y, -X.x };

            vert.emplace_back(f(MData { opt.start + Y })); // 0
            vert.emplace_back(f(MData { opt.end   + Y })); // 1
            vert.emplace_back(f(MData { opt.start - Y })); // 2
            vert.emplace_back(f(MData { opt.end   - Y })); // 3
            ind.emplace_back(iOffset + 0, iOffset + 1, iOffset + 2);
            ind.emplace_back(iOffset + 1, iOffset + 2, iOffset + 3);

            if (opt.subdivisions > 0) {
                vert.emplace_back(f(MData { opt.start + X })); // 4
                vert.emplace_back(f(MData { opt.end   - X })); // 5

                ind.emplace_back(iOffset + 0, iOffset + 2, iOffset + 4);
                ind.emplace_back(iOffset + 1, iOffset + 3, iOffset + 5);
            }

            const u32 nPoints = 1 << opt.subdivisions;

            const auto mapK2Vi = [=](u32 k) -> u32 {
                if (k == 0) return 0;
                if (k == nPoints) return 1;
                const u32 rightMostbit = k - (k & k - 1); // https://stackoverflow.com/a/21624122/19968422
                const u32 maxI = nPoints / rightMostbit;
                const u32 maxK = nPoints - rightMostbit;
                return maxI - (maxK - k) / (2 * rightMostbit);
            };

            for (u32 i = 2; i <= opt.subdivisions; ++i) {
                const u32 dist = nPoints >> i;
                for (u32 k = dist; k < nPoints; k += dist * 2) {
                    const float angle = PI * (float)k / (float)nPoints;
                    const float cos = std::cos(angle), sin = std::sin(angle);
                    vert.emplace_back(f(MData { opt.start + Y * cos + X * sin }));
                    vert.emplace_back(f(MData { opt.end   + Y * cos - X * sin }));
                    const u32 from = k - dist, to = k + dist;
                    ind.emplace_back(iOffset + mapK2Vi(from) * 2,     vert.size() - 2, iOffset + mapK2Vi(to) * 2);
                    ind.emplace_back(iOffset + mapK2Vi(from) * 2 + 1, vert.size() - 1, iOffset + mapK2Vi(to) * 2 + 1);
                }
            }
        }
    };

    inline static MeshConstructor<StadiumCreator> Stadium {};
}
