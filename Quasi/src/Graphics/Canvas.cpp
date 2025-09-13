#include "Canvas.h"
#include "GraphicsDevice.h"

namespace Quasi::Graphics {
    Gradient Gradient::Vertical(float s, float e, const Math::fColor& sc, const Math::fColor& ec) {
        return { { 0, s }, { 0, e - s }, sc, ec };
    }

    Gradient Gradient::Horizontal(float s, float e, const Math::fColor& sc, const Math::fColor& ec) {
        return { { s, 0 }, { e - s, 0 }, sc, ec };
    }

    Math::fColor Gradient::At(const Math::fv2& p) const {
        return startColor.Lerp(endColor, (p - startPoint).Dot(direction) / direction.LenSq());
    }

    void UIMesh::SetTextureFill() {
        // TODO
    }

    void UIMesh::FillGradient(const Gradient& g) {
        for (UIVertex& v : vertices) {
            v.Color = g.At(v.Position);
        }
    }

    void UIMesh::OverlayGradient(const Gradient& g) {
        for (UIVertex& v : vertices) {
            v.Color *= g.At(v.Position);
        }
    }

    Canvas::Canvas() {}
    Canvas::Canvas(GraphicsDevice& gd) : renderCanvas(gd.CreateNewRender<UIVertex>(16384, 16384)) {
        const Math::fv2 screenSize = gd.GetWindowSize().As<float>();
        renderCanvas.SetProjection(Math::Matrix3D::OrthoProjection({ 0, screenSize.AddZ(1) }));

        renderCanvas.UseShader(
            "#shader vertex\n"
            "#version 330 core\n"
            "layout (location = 0) in vec2 position;"
            "layout (location = 1) in vec2 texCoord;"
            "layout (location = 2) in vec4 color;"
            "layout (location = 3) in vec4 stuv;"
            "layout (location = 4) in int renderPrim;"
            ""
            "out vec4 vColor;"
            "out vec4 vSTUV;"
            "flat out int vRenderPrim;"
            ""
            "uniform mat4 u_projection, u_view;"
            ""
            "void main() {"
            "    gl_Position = u_projection * u_view * vec4(position, 0.0, 1.0);"
            "    vColor = color;"
            "    vSTUV = stuv;"
            "    vRenderPrim = renderPrim;"
            "}\n"
            "#shader fragment\n"
            "#version 330 core\n"
            ""
            "layout (location = 0) out vec4 FragColor;"
            ""
            "in vec4 vColor;"
            "in vec4 vSTUV;"
            "flat in int vRenderPrim;"
            ""
            "void main() {"
            "    bool invert = (vRenderPrim & 8) == 8; "
            "    bool shouldDiscard = false;"
            "    switch (vRenderPrim & 7) {"
            "        case 1: {"
            "            float distSq  = vSTUV.x * vSTUV.x + vSTUV.y * vSTUV.y;"
            "            shouldDiscard = (distSq > 1.0) || (vSTUV.z > distSq);"
            "            break;"
            "        }"
            "        case 2: {"
            "           shouldDiscard = vSTUV.x * vSTUV.x > vSTUV.y;"
            "           break;"
            "        }"
            "    }"
            "    if (shouldDiscard ^^ invert) discard;"
            "    FragColor = vColor;"
            "}"
        );
    }

    void Canvas::DrawTriangle(const Math::fv2& p1, const Math::fv2& p2, const Math::fv2& p3) {
        if (NeedDrawFill()) {
            Batch batch = NewBatch();
            batch.SetFill();
            batch.Point(p1);
            batch.Point(p2);
            batch.Point(p3);
            batch.Tri(0, 1, 2);
        }
        if (NeedDrawStroke()) {
            Path p = NewPath(CLOSED_CURVE);
            p.AddPoint(p1);
            p.AddPoint(p2);
            p.AddPoint(p3);
        }
    }

    void Canvas::DrawQuad(const Math::fv2& p1, const Math::fv2& p2, const Math::fv2& p3, const Math::fv2& p4) {
        if (NeedDrawFill()) {
            Batch batch = NewBatch();
            batch.Point(p1);
            batch.Point(p2);
            batch.Point(p3);
            batch.Point(p4);
            batch.Quad(0, 1, 2, 3);
        }
        if (NeedDrawStroke()) {
            Path p = NewPath(CLOSED_CURVE);
            p.AddPoint(p1);
            p.AddPoint(p2);
            p.AddPoint(p3);
            p.AddPoint(p4);
        }
    }

    void Canvas::DrawSquare(const Math::fv2& center, float size) {
        DrawRect(Math::fRect2D::FromCenter(center, size));
    }

    void Canvas::DrawRect(const Math::fRect2D& rect) {
        if (NeedDrawFill()) {
            DrawSimpleRect(rect, drawAttr.fillColor);
        }
        if (NeedDrawStroke()) {
            DrawRectStroke(rect);
        }
    }

    void Canvas::DrawRoundedSquare(const Math::fv2& center, float size, float radius) {
        return DrawRoundedRect(Math::fRect2D::FromSize(center, size), radius);
    }

    void Canvas::DrawVarRoundedSquare(const Math::fv2& center, float size, float tr, float br, float tl, float bl) {
        return DrawVarRoundedRect(Math::fRect2D::FromSize(center, size), tr, br, tl, bl);
    }

    void Canvas::DrawRoundedRect(const Math::fRect2D& rect, float radius) {
        radius = std::min(radius, std::min(rect.Width(), rect.Height()) * 0.5f);
        if (NeedDrawFill())
            DrawSimpleRoundedRect(rect, radius, drawAttr.fillColor);
        if (NeedDrawStroke()) {
            const Math::fRect2D inner = rect.Inset(radius);
            Batch batch = NewBatch();
            batch.SetStroke();
            const float r = drawAttr.strokeWeight;
            // sides
            batch.Point({ rect.max.x - r, inner.max.y });
            batch.Point({ rect.max.x + r, inner.max.y });
            batch.Point({ rect.max.x + r, inner.min.y });
            batch.Point({ rect.max.x - r, inner.min.y });
            batch.Quad(0, 1, 2, 3);
            batch.Point({ rect.min.x + r, inner.max.y });
            batch.Point({ rect.min.x - r, inner.max.y });
            batch.Point({ rect.min.x - r, inner.min.y });
            batch.Point({ rect.min.x + r, inner.min.y });
            batch.Quad(4, 5, 6, 7);
            batch.Point({ inner.max.x, rect.max.y - r });
            batch.Point({ inner.max.x, rect.max.y + r });
            batch.Point({ inner.min.x, rect.max.y + r });
            batch.Point({ inner.min.x, rect.max.y - r });
            batch.Quad(8, 9, 10, 11);
            batch.Point({ inner.max.x, rect.min.y - r });
            batch.Point({ inner.max.x, rect.min.y + r });
            batch.Point({ inner.min.x, rect.min.y + r });
            batch.Point({ inner.min.x, rect.min.y - r });
            batch.Quad(12, 13, 14, 15);
            // corners
            DrawQuarterArc(inner.max,                    Math::Rotor2D::FromComplex({ +1, 0 }), radius, r, drawAttr.strokeColor);
            DrawQuarterArc({ inner.max.x, inner.min.y }, Math::Rotor2D::FromComplex({ 0, -1 }), radius, r, drawAttr.strokeColor);
            DrawQuarterArc({ inner.min.x, inner.max.y }, Math::Rotor2D::FromComplex({ 0, +1 }), radius, r, drawAttr.strokeColor);
            DrawQuarterArc(inner.min,                    Math::Rotor2D::FromComplex({ -1, 0 }), radius, r, drawAttr.strokeColor);
        }
    }

    void Canvas::DrawVarRoundedRect(const Math::fRect2D& rect, float tr, float br, float tl, float bl) {
        if (NeedDrawFill())
            DrawSimpleVarRoundRect(rect, tr, br, tl, bl, drawAttr.fillColor);
        if (NeedDrawStroke()) {
            Batch batch = NewBatch();
            batch.SetStroke();
            const float r = drawAttr.strokeWeight;
            // sides
            batch.Point({ rect.max.x - r, rect.max.y - tr });
            batch.Point({ rect.max.x + r, rect.max.y - tr });
            batch.Point({ rect.max.x + r, rect.min.y + br });
            batch.Point({ rect.max.x - r, rect.min.y + br });
            batch.Quad(0, 1, 2, 3);
            batch.Point({ rect.min.x + r, rect.max.y - tl });
            batch.Point({ rect.min.x - r, rect.max.y - tl });
            batch.Point({ rect.min.x - r, rect.min.y + bl });
            batch.Point({ rect.min.x + r, rect.min.y + bl });
            batch.Quad(4, 5, 6, 7);
            batch.Point({ rect.max.x - tr, rect.max.y - r });
            batch.Point({ rect.max.x - tr, rect.max.y + r });
            batch.Point({ rect.min.x + tl, rect.max.y + r });
            batch.Point({ rect.min.x + tl, rect.max.y - r });
            batch.Quad(8, 9, 10, 11);
            batch.Point({ rect.max.x - br, rect.min.y - r });
            batch.Point({ rect.max.x - br, rect.min.y + r });
            batch.Point({ rect.min.x + bl, rect.min.y + r });
            batch.Point({ rect.min.x + bl, rect.min.y - r });
            batch.Quad(12, 13, 14, 15);
            // corners
            DrawQuarterArc({ rect.max.x - tr, rect.max.y - tr }, Math::Rotor2D::FromComplex({ +1, 0 }), tr, r, drawAttr.strokeColor);
            DrawQuarterArc({ rect.max.x - br, rect.min.y + br }, Math::Rotor2D::FromComplex({ 0, -1 }), br, r, drawAttr.strokeColor);
            DrawQuarterArc({ rect.min.x + tl, rect.max.y - tl }, Math::Rotor2D::FromComplex({ 0, +1 }), tl, r, drawAttr.strokeColor);
            DrawQuarterArc({ rect.min.x + bl, rect.min.y + bl }, Math::Rotor2D::FromComplex({ -1, 0 }), bl, r, drawAttr.strokeColor);
        }
    }

    void Canvas::DrawCircle(const Math::fv2& center, float radius) {
        if (NeedDrawFill())
            DrawSimpleCircle(center, radius, drawAttr.fillColor);
        if (NeedDrawStroke())
            DrawCircleOutline(center, radius, drawAttr.strokeWeight, drawAttr.strokeColor);
    }

    void Canvas::DrawEllipse(const Math::fRect2D& bounds) {
        // TODO: this is not a proper offset ellipse;
        //       the stroke of an ellipse is an order-8 algaebraic curve
        //       this is a temporary solution
        //       use cubic beziers next time
        if (NeedDrawFill())
            DrawSimpleEllipse(bounds, drawAttr.fillColor);
        // if (NeedDrawStroke()) {
        //     const Math::fRect2D rect = bounds.Extrude(drawAttr.strokeWeight);
        //     Batch batch = NewBatch();
        //     static constexpr float R2 = Math::ROOT_2;
        //     const float diagonal = R2 * (radius + thickness);
        //     float inner = (radius - thickness) / (radius + thickness);
        //     inner *= inner;
        //     batch.PushV(UIVertex { bounds.min,                     0, drawAttr.strokeColor, { -1, -1, inner, 0 }, UIRender::CIRCLE });
        //     batch.PushV(UIVertex { { bounds.max.x, bounds.min.y }, 0, drawAttr.strokeColor, { +1, -1, inner, 0 }, UIRender::CIRCLE });
        //     batch.PushV(UIVertex { { bounds.min.x, bounds.max.y }, 0, drawAttr.strokeColor, { -1, +1, inner, 0 }, UIRender::CIRCLE });
        //     batch.PushV(UIVertex { bounds.max,                     0, drawAttr.strokeColor, { +1, +1, inner, 0 }, UIRender::CIRCLE });
        //     batch.Tri(0, 1, 2);
        //     batch.Tri(2, 0, 3);
        // }
    }

    void Canvas::DrawArc(const Math::fv2& center, float radius, const Math::Rotor2D& startAngle, const Math::Rotor2D& endAngle, ArcDirection direction, ArcMode mode) {
        switch (direction) {
            case CCW:
                return DrawArcCCW(center, radius, startAngle, endAngle, mode);
            case CW:
                return DrawArcCCW(center, radius, endAngle, startAngle, mode);
            case MAJOR: {
                const bool alreadyMajor = (endAngle - startAngle).Sin() < 0;
                return DrawArcCCW(
                    center, radius,
                    alreadyMajor ? startAngle : endAngle,
                    alreadyMajor ? endAngle : startAngle,
                    mode
                );
            }
            case MINOR: {
                const bool alreadyMinor = (endAngle - startAngle).Sin() > 0;
                return DrawArcCCW(
                    center, radius,
                    alreadyMinor ? startAngle : endAngle,
                    alreadyMinor ? endAngle : startAngle,
                    mode
                );
            }
            default:;
        }
    }

    void Canvas::DrawArcCCW(const Math::fv2& center, float radius, const Math::Rotor2D& startAngle, const Math::Rotor2D& endAngle, ArcMode mode) {
        Math::fComplex turn = (endAngle - startAngle).AsComplex();
        Math::fv2 start = startAngle.IHat() * radius;
        if (NeedDrawFill()) {
            const bool isMajorArc = turn.im < 0;
            if (isMajorArc) { // needs to split into two sections
                DrawHalfCircle(center, start.PerpendLeft(), drawAttr.fillColor);
                turn = -turn;
                start = -start;
            }
            const Math::fComplex halfTurn = Math::Rotor2D::FromComplex(turn).Halved().AsComplex();
            DrawHalfCircularChord(center, start.ComplexMul(halfTurn.AsVec2D()), halfTurn.re, std::abs(halfTurn.im), drawAttr.fillColor);

            if (mode != CHORD || isMajorArc) {
                Batch batch = NewBatch();
                batch.SetFill();
                batch.Point(center + start);
                batch.Point(center + radius * endAngle.IHat());
                if (mode == CHORD) {
                    batch.Point(center - start);
                } else {
                    batch.Point(center);
                }
                batch.Tri(0, 1, 2);
            }

            if (isMajorArc) {
                // restore fixes
                turn = -turn;
                start = -start;
            }
        }
        if (NeedDrawStroke()) {
            Path p = NewPath(mode == OPEN ? OPEN_CURVE : CLOSED_CURVE);
            p.AddPoint(center + start);
            p.AddCircularArc(center, Math::Rotor2D::FromComplex(turn), CCW);
            if (mode == CLOSED) {
                p.AddPoint(center);
            }
        }
    }

    // void Canvas::DrawEllArc(const Math::fv2& center, const Math::fv2& size, float startAngle, float stopAngle,
    //                         ArcMode mode) {
    //     // TODO
    // }

    void Canvas::DrawPoint(const Math::fv2& position) {
        if (NeedDrawStroke())
            DrawSimpleCircle(position, drawAttr.strokeWeight, drawAttr.strokeColor);
    }

    void Canvas::DrawLine(const Math::fv2& start, const Math::fv2& end) {
        const Math::fv2 tangent = start.Tangent(end).Norm();
        DrawSimpleLine(start, end, tangent);
        DrawLineCap(start, tangent);
        DrawLineCap(end,  -tangent);
    }

    void Canvas::DrawMesh(const UIMesh& mesh) {
        DestinationMesh().Add(mesh);
    }

    UIMesh& Canvas::DestinationMesh() {
        return drawMesh.UnwrapOr(worldMesh);
    }

    void Canvas::Batch::PushV(UIVertex v) {
        v.Position = canvas.TransformToWorldSpace(v.Position);
        canvas.renderCanvas->PushVertex(v);
    }
    UIVertex& Canvas::Batch::VertAt(u32 i)             { return mesh.vertices[i]; }
    const UIVertex& Canvas::Batch::VertAt(u32 i) const { return mesh.vertices[i]; }
    u32 Canvas::Batch::VertCount() const { return mesh.vertices.Length(); }
    void Canvas::Batch::PushI(u32 i, u32 j, u32 k) { return Tri(i, j, k); }
    TriIndices* Canvas::Batch::IndexData() { return mesh.indices.Data(); }

    void Canvas::Batch::SetColor(const Math::fColor& color) { storedPoint.Color = color; }
    void Canvas::Batch::SetFill()   { storedPoint.Color = canvas.drawAttr.fillColor; }
    void Canvas::Batch::SetStroke() { storedPoint.Color = canvas.drawAttr.strokeColor; }
    void Canvas::Batch::SetPrim(u32 prim) {
        storedPoint.RenderPrim &= ~UIRender::PRIMITIVE_TYPE;
        storedPoint.RenderPrim |= prim;
    }
    void Canvas::Batch::SetPrimAndStyle(u32 primAndStyle) { storedPoint.RenderPrim = primAndStyle; }
    void Canvas::Batch::SetPosition(const Math::fv2& point) { storedPoint.Position = canvas.TransformToWorldSpace(point); }
    void Canvas::Batch::SetUV(float u, float v) { storedPoint.STUV = { u, v, 0, 0 }; }
    void Canvas::Batch::SetSTUV(float s, float t, float u, float v) { storedPoint.STUV = { s, t, u, v }; }

    void Canvas::Batch::Point(const Math::fv2& position) {
        storedPoint.Position = canvas.TransformToWorldSpace(position);
        SetPrim(UIRender::PLAIN);
        Push();
    }

    void Canvas::Batch::PointCirc(const Math::fv2& position, float u, float v, float radiusRatio) {
        SetPosition(position);
        storedPoint.STUV = { u, v, radiusRatio, 0 };
        SetPrim(UIRender::CIRCLE);
        Push();
    }

    void Canvas::Batch::PointQBezGeneric(const Math::fv2& position, float u, float v, bool flip) {
        SetPosition(position);
        storedPoint.STUV = { u, v, 0, 0 };
        SetPrim((UIRender::RenderPrimitive)(UIRender::QBEZ | (flip << 3)));
        Push();
    }

    void Canvas::Batch::PointQBezA(const Math::fv2& position, bool flip) { PointQBezGeneric(position, 0,    0, flip); }
    void Canvas::Batch::PointQBezB(const Math::fv2& position, bool flip) { PointQBezGeneric(position, 0.5f, 0, flip); }
    void Canvas::Batch::PointQBezC(const Math::fv2& position, bool flip) { PointQBezGeneric(position, 1,    1, flip); }
    void Canvas::Batch::PointQTriple(const Math::fv2& position, bool innerSide) {
        PointQDoubleS(position, innerSide);
        storedPoint.STUV = { 1, 1, 0, 0 };
        Push();
    }

    void Canvas::Batch::PointQDoubleS(const Math::fv2& position, bool innerSide) {
        SetPosition(position);
        storedPoint.RenderPrim = UIRender::PLAIN;
        Push();
        storedPoint.STUV = {};
        storedPoint.RenderPrim = UIRender::QBEZ | (innerSide << 3);
        Push();
    }

    void Canvas::Batch::PointQDoubleE(const Math::fv2& position, bool innerSide) {
        SetPosition(position);
        storedPoint.RenderPrim = UIRender::PLAIN;
        Push();
        storedPoint.RenderPrim = UIRender::QBEZ | (innerSide << 3);
        storedPoint.STUV = { 1, 1, 0, 0 };
        Push();
    }

    void Canvas::Batch::PointQDoubleC(const Math::fv2& position, bool innerSide) {
        SetPosition(position);
        storedPoint.RenderPrim = UIRender::PLAIN;
        Push();
        storedPoint.RenderPrim = UIRender::QBEZ | (innerSide << 3);
        storedPoint.STUV = { 0.5f, 0, 0, 0 };
        Push();
    }

    void Canvas::Batch::PointsQBez(Span<const Math::fv2> bezpoints, bool innerSide) {
        SetPosition(bezpoints[0]);
        storedPoint.RenderPrim = UIRender::QBEZ | (innerSide << 3);
        iOffset = mesh.vertices.Length();
        for (usize i = 0; i < bezpoints.Length() / 2; ++i) {
            storedPoint.STUV = { 0, 0, 0, 0 };
            Push();
            SetPosition(bezpoints[2 * i + 1]);
            storedPoint.STUV = { 0.5f, 0, 0, 0 };
            Push();
            SetPosition(bezpoints[2 * i + 2]);
            storedPoint.STUV = { 1, 1, 0, 0 };
            Push();
            Tri(3 * i, 3 * i + 1, 3 * i + 2);
        }
        iOffset = mesh.vertices.Length();
    }

    void Canvas::Batch::PointsQBezClosed(Span<const Math::fv2> outer, Span<const Math::fv2> inner) {
        mesh.vertices.Reserve(9 * (outer.Length() / 2) + 2);
        mesh.indices .Reserve(5 * (outer.Length() / 2));
        const UIVertex* outerVerts = mesh.vertices.DataEnd();
        PointsQBez(outer, false);
        const UIVertex* innerVerts = mesh.vertices.DataEnd();
        PointsQBez(inner, true);

        iOffset = mesh.vertices.Length();
        storedPoint.RenderPrim = UIRender::PLAIN;
        for (usize i = 0; i < (outer.Length() / 2); ++i) {
            //         B
            // A                C+
            //
            //         E
            //    D          F+
            // ABC: outer curve (handled)
            // DEF: inner curve (handled)
            // C, F: duplicated and will be handled next iteration
            // DAECF: flat region
            const usize j = i * 3;
            storedPoint.Position = outerVerts[j + 0].Position; Push(); // A
            storedPoint.Position = innerVerts[j + 0].Position; Push(); // D
            storedPoint.Position = innerVerts[j + 1].Position; Push(); // E
            Tri(j + 0, j + 1, j + 2); // A D E
            Tri(j + 0, j + 2, j + 3); // A E C
            Tri(j + 3, j + 2, j + 4); // C E F
        }
        storedPoint.Position = innerVerts[-1].Position; Push();
        storedPoint.Position = (innerVerts + (innerVerts - outerVerts))[-1].Position; Push();
        iOffset = mesh.vertices.Length();
    }

    void Canvas::Batch::Tri(u32 i, u32 j, u32 k) {
        mesh.PushIndex({ iOffset + i, iOffset + j, iOffset + k });
    }
    void Canvas::Batch::Quad(u32 i, u32 j, u32 k, u32 l) {
        mesh.PushIndex({ iOffset + i, iOffset + j, iOffset + l });
        mesh.PushIndex({ iOffset + j, iOffset + k, iOffset + l });
    }

    void Canvas::Batch::TriStrip(Span<const u32> strip) {
        u32 tri[3] = { iOffset + strip[0], 0, iOffset + strip[1] };
        for (u32 i = 2; i < strip.Length(); ++i) {
            tri[1 ^ (i & 1)] = tri[2];
            tri[2] = iOffset + strip[i];
            mesh.PushIndex({ tri[0], tri[1], tri[2] });
        }
    }

    void Canvas::Batch::TriFan(Span<const u32> fan) {
        const u32 center = iOffset + fan[0];
        u32 a = iOffset + fan[1];
        for (u32 i = 2; i < fan.Length(); ++i) {
            const u32 b = iOffset + fan[i];
            mesh.PushIndex({ center, a, b });
            a = b;
        }
    }

    void Canvas::Batch::Push() { mesh.PushVertex(storedPoint); }
    void Canvas::Batch::PushAsPlain() {
        storedPoint.RenderPrim = UIRender::PLAIN;
        Push();
    }

    void Canvas::Batch::Refresh() { iOffset = mesh.vertices.Length(); }

    Canvas::Batch Canvas::NewBatch() {
        UIMesh& dest = DestinationMesh();
        return { *this, dest, (u32)dest.vertices.Length() };
    }

    void Canvas::DrawSimpleLine(const Math::fv2& start, const Math::fv2& end, const Math::fv2& tangent) {
        Batch batch = NewBatch();
        batch.SetStroke();
        const Math::fv2 normal = tangent.Perpend() * drawAttr.strokeWeight;
        batch.Point(start + normal);
        batch.Point(end   + normal);
        batch.Point(end   - normal);
        batch.Point(start - normal);
        batch.Quad(0, 1, 2, 3);
    }

    void Canvas::DrawLineCap(const Math::fv2& point, const Math::fv2& tangent) {
        Batch batch = NewBatch();
        batch.SetStroke();
        // no need to draw flat caps: theyre done by default
        switch (drawAttr.drawStyle & UIRender::CAP_MASK) {
            case UIRender::ROUND_CAP: {
                const Math::fv2 tangentR2 = tangent * (Math::ROOT_2 * drawAttr.strokeWeight), normalR2 = tangentR2.Perpend();
                batch.PointCirc(point - tangentR2, +1, +1);
                batch.PointCirc(point + normalR2,  -1, +1);
                batch.PointCirc(point - normalR2,  +1, -1);
                batch.Tri(0, 1, 2);
                break;
            }
            case UIRender::SQUARE_CAP:  {
                const Math::fv2 t = tangent * drawAttr.strokeWeight;
                const Math::fv2 normal = t.Perpend(), left = point + normal, right = point - normal;
                batch.Point(left);
                batch.Point(right);
                batch.Point(right - t);
                batch.Point(left  - t);
                batch.Quad(0, 1, 2, 3);
                break;
            }
            case UIRender::ARROW_CAP: {
                const Math::fv2 tangentR2 = tangent * (2 * drawAttr.strokeWeight), normalR2 = tangentR2.Perpend();
                batch.Point(point - tangentR2);
                batch.Point(point + normalR2);
                batch.Point(point - normalR2);
                batch.Tri(0, 1, 2);
                break;
            }
            default:;
        }
    }

    void Canvas::DrawLineJoin(const Math::fv2& point, const Math::fv2& tangent1, const Math::fv2& tangent2) {
        const float cos = tangent1.Dot(tangent2), sin = tangent1.Cross(tangent2);
        const bool left = sin >= 0;
        const Math::fv2 normal1 = tangent1.Perpend(!left) * drawAttr.strokeWeight,
                        normal2 = tangent2.Perpend(!left) * drawAttr.strokeWeight;
        // when cos : [0,1], angle is small, join is minor arc
        // when cos : [-1,0], angle is large, join is major arc
        Batch batch = NewBatch();
        batch.SetStroke();

        // bevel joint by default. all joints use this
        batch.Point(point);
        batch.Point(point + normal1);
        batch.Point(point + normal2);
        batch.Tri(0, 1, 2);

        switch (drawAttr.drawStyle & UIRender::JOIN_MASK) {
            case UIRender::ROUND_JOIN: {
                // halven the angles first
                const float u = Math::HALF_ROOT_2 * std::sqrt(1 + cos),
                            v = Math::HALF_ROOT_2 * std::sqrt(1 - cos);
                DrawHalfCircularChord(
                    point,
                    drawAttr.strokeWeight * tangent1.ComplexMul(v, left ? -u : u),
                    u, v, drawAttr.strokeColor);
                break;
            }
            case UIRender::MITER_JOIN: {
                if (cos > -0.9375f) { // miter join will be too large - only render it when cos is reasonable
                    const float h = (cos - 1) / sin;
                    const Math::fv2 outward = (left ? -drawAttr.strokeWeight : drawAttr.strokeWeight) * tangent1.ComplexMul(h, 1);
                    batch.Point(point + outward);
                    batch.Tri(1, 3, 2);
                }
                break;
            }
            default:;
        }
    }

    void Canvas::DrawHalfCircularChord(const Math::fv2& center, const Math::fv2& forward, float cos, float sin, const Math::fColor& color) {
        // a v^2 + b v + c + d u + e u v
        static constexpr float HORIZ_COEFF[5] = {
            0.115607368042f,
           -0.356381124853f,
            0.241706294943f,
            1.17272151222f,
            0.0595057818955f,
        };
        // half of cos + isin: u + vi, quarter: s + ti
        const float horiz  = (HORIZ_COEFF[0] * cos + HORIZ_COEFF[1] + HORIZ_COEFF[4] * sin) * cos + HORIZ_COEFF[3] * sin + HORIZ_COEFF[2],
                    vert   = Math::ROOT_2 / std::sqrt(1 + cos);
        // horiz was supposed to be sec(x/2 + pi/4) - sin(x) tan(x/2 + pi/4),
        // but computationally at pi/2 there's a pole (nan) and there's no good way to solve it
        // banged my head against a wall for an hour with no solution in sight,
        // trignonometry ingrained into my brain at this point
        // ...
        // so i used a regression model a v^2 + b v + c + d u + e u v with desmos
        // with error < 0.0004831, using no sqrts or division, just multiplication
        // and also quite fast too, so I'm pretty happy about it.
        // ...
        // i wasted a whole day figuring this out
        // ...
        // shut up
        Batch batch = NewBatch();
        batch.SetColor(color);
        batch.PointCirc(center + forward * vert,                  vert,     0);
        batch.PointCirc(center + forward.ComplexMul(cos, +horiz), cos, +horiz);
        batch.PointCirc(center + forward.ComplexMul(cos, -horiz), cos, -horiz);
        batch.Tri(0, 1, 2);
    }

    void Canvas::DrawHalfCircle(const Math::fv2& center, const Math::fv2& forward, const Math::fColor& color) {
        Batch batch = NewBatch();
        static constexpr float R2 = Math::ROOT_2;
        const Math::fv2 f = R2 * forward;
        batch.SetColor(color);
        batch.PointCirc(center + f,                +R2, 0);
        batch.PointCirc(center + f.PerpendLeft(),  0, +R2);
        batch.PointCirc(center + f.PerpendRight(), 0, -R2);
        batch.Tri(0, 1, 2);
    }

    void Canvas::DrawSimpleCircle(const Math::fv2& center, float radius, const Math::fColor& color) {
        Batch batch = NewBatch();
        static constexpr float R2 = Math::ROOT_2;
        const float diagonal = R2 * radius;
        batch.SetColor(color);
        batch.PointCirc({ center.x + diagonal, center.y }, +R2, 0);
        batch.PointCirc({ center.x, center.y + diagonal }, 0, +R2);
        batch.PointCirc({ center.x - diagonal, center.y }, -R2, 0);
        batch.PointCirc({ center.x, center.y - diagonal }, 0, -R2);
        batch.Quad(0, 1, 2, 3);
    }

    void Canvas::DrawQuarterChord(const Math::fv2& center, const Math::fv2& rightMostSide, const Math::fColor& color) {
        Batch batch = NewBatch();
        const Math::fv2 leftMostSide = rightMostSide.PerpendLeft(), right = center + rightMostSide;
        batch.SetColor(color);
        batch.PointCirc(right,                 1, 0);
        batch.PointCirc(center + leftMostSide, 0, 1);
        batch.PointCirc(right  + leftMostSide, 1, 1);
        batch.Tri(0, 1, 2);
    }

    void Canvas::DrawSimpleEllipse(const Math::fv2& center, const Math::fv2& radii, const Math::fColor& color) {
        Batch batch = NewBatch();
        batch.SetColor(color);
        batch.PointCirc({ center.x - radii.x, center.y - radii.y }, -1, -1);
        batch.PointCirc({ center.x + radii.x, center.y - radii.y }, +1, -1);
        batch.PointCirc({ center.x + radii.x, center.y + radii.y }, +1, +1);
        batch.PointCirc({ center.x - radii.x, center.y + radii.y }, -1, +1);
        batch.Quad(0, 1, 2, 3);
    }

    void Canvas::DrawSimpleEllipse(const Math::fRect2D& bounds, const Math::fColor& color) {
        Batch batch = NewBatch();
        batch.SetColor(color);
        batch.PointCirc({ bounds.min.x, bounds.min.y }, -1, -1);
        batch.PointCirc({ bounds.max.x, bounds.min.y }, +1, -1);
        batch.PointCirc({ bounds.min.x, bounds.max.y }, -1, +1);
        batch.PointCirc({ bounds.max.x, bounds.max.y }, +1, +1);
        batch.Quad(0, 1, 2, 3);
    }

    void Canvas::DrawCircleOutline(const Math::fv2& center, float radius, float thickness, const Math::fColor& color) {
        Batch batch = NewBatch();
        const float innerRadius = radius - thickness, outerRadius = (radius + thickness) * Math::ROOT_2,
                    innerUV = innerRadius / (radius + thickness), innerSq = innerUV * innerUV;
        batch.SetColor(color);
        batch.PointCirc({ center.x + innerRadius, center.y }, +innerUV,      0, innerSq);
        batch.PointCirc({ center.x + outerRadius, center.y }, +Math::ROOT_2, 0, innerSq);
        batch.PointCirc({ center.x, center.y + innerRadius }, 0, +innerUV,      innerSq);
        batch.PointCirc({ center.x, center.y + outerRadius }, 0, +Math::ROOT_2, innerSq);
        batch.PointCirc({ center.x - innerRadius, center.y }, -innerUV,      0, innerSq);
        batch.PointCirc({ center.x - outerRadius, center.y }, -Math::ROOT_2, 0, innerSq);
        batch.PointCirc({ center.x, center.y - innerRadius }, 0, -innerUV,      innerSq);
        batch.PointCirc({ center.x, center.y - outerRadius }, 0, -Math::ROOT_2, innerSq);
        batch.TriStrip({{ 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 }});
    }

    void Canvas::DrawQuarterArc(const Math::fv2& center, const Math::Rotor2D& start, float radius, float thickness, const Math::fColor& color) {
        if (radius < thickness) { radius = thickness = (radius + thickness) * 0.5f; }
        const float innerRadius = radius - thickness,
                    innerUV = innerRadius / (radius + thickness), innerSq = innerUV * innerUV;
        Batch batch = NewBatch();
        const Math::fv2 innerForward = start.IHat() * innerRadius,
                        outerForward = start.IHat() * (Math::ROOT_2 * (radius + thickness));
        batch.SetColor(color);
        batch.PointCirc(center + innerForward,               innerUV, 0, innerSq);
        batch.PointCirc(center + innerForward.PerpendLeft(), 0, innerUV, innerSq);
        batch.PointCirc(center + outerForward.PerpendLeft(), 0, Math::ROOT_2, innerSq);
        batch.PointCirc(center + outerForward,               Math::ROOT_2, 0, innerSq);
        batch.Quad(0, 1, 2, 3);
    }

    void Canvas::DrawCircularArcCCW(const Math::fv2& center, const Math::Rotor2D& mid, Math::Rotor2D step, float radius, float thickness, const Math::fColor& color) {
        const float innerRadius = radius - thickness;
        if (step.Cos() < 0) {
            DrawQuarterArc(center, mid.RotateByInv(step), radius, thickness, color);
            step = step.RotateCW90();
            DrawQuarterArc(center, mid.RotateBy(step), radius, thickness, color);
        }
        const float secant = Math::ROOT_2 / std::sqrt(1 + step.Cos()), outerRadius = secant * (radius + thickness),
                    innerUV = innerRadius / (radius + thickness), innerSq = innerUV * innerUV;
        Batch batch = NewBatch();
        batch.SetColor(color);
        Math::fv2 fwd = mid.IHat();
        batch.PointCirc(center + fwd * innerRadius, innerUV, 0, innerSq);
        batch.PointCirc(center + fwd * outerRadius, secant,  0, innerSq);
        fwd = fwd.RotateBy(step);
        const Math::fv2 iUV = step.IHat() * innerUV, sUV = step.IHat() * secant;
        batch.PointCirc(center + fwd * innerRadius, iUV.x, iUV.y, innerSq);
        batch.PointCirc(center + fwd * outerRadius, sUV.x, sUV.y, innerSq);
        fwd = step.InvRotate(mid.IHat());
        batch.PointCirc(center + fwd * innerRadius, iUV.x, -iUV.y, innerSq);
        batch.PointCirc(center + fwd * outerRadius, sUV.x, -sUV.y, innerSq);
        batch.Quad(0, 1, 3, 2);
        batch.Quad(1, 0, 4, 5);
    }

    void Canvas::DrawSimpleRect(const Math::fRect2D& rect, const Math::fColor& color) {
        Batch batch = NewBatch();
        batch.SetColor(color);
        batch.Point(rect.TopRight());
        batch.Point(rect.TopLeft());
        batch.Point(rect.BottomLeft());
        batch.Point(rect.BottomRight());
        batch.Quad(0, 1, 2, 3);
    }

    void Canvas::DrawSimpleRoundedRect(const Math::fRect2D& outer, float radius, const Math::fColor& color) {
        const Math::fRect2D inner = outer.Inset(radius);

        Batch batch = NewBatch();
        batch.SetColor(color);
        batch.Point({ inner.max.x, outer.max.y });
        batch.Point({ inner.min.x, outer.max.y });
        batch.Point({ outer.max.x, inner.max.y });
        batch.Point({ outer.min.x, inner.max.y });
        batch.Point({ outer.max.x, inner.min.y });
        batch.Point({ outer.min.x, inner.min.y });
        batch.Point({ inner.max.x, outer.min.y });
        batch.Point({ inner.min.x, outer.min.y });
        batch.TriStrip({{ 0, 1, 2, 3, 4, 5, 6, 7 }});
        batch.PointCirc({ inner.min.x, outer.min.y }, 1, 0);
        batch.PointCirc({ outer.min.x, inner.min.y }, 0, 1);
        batch.PointCirc({ outer.min.x, outer.min.y }, 1, 1);
        batch.PointCirc({ inner.max.x, outer.min.y }, 1, 0);
        batch.PointCirc({ outer.max.x, inner.min.y }, 0, 1);
        batch.PointCirc({ outer.max.x, outer.min.y }, 1, 1);
        batch.PointCirc({ inner.min.x, outer.max.y }, 1, 0);
        batch.PointCirc({ outer.min.x, inner.max.y }, 0, 1);
        batch.PointCirc({ outer.min.x, outer.max.y }, 1, 1);
        batch.PointCirc({ inner.max.x, outer.max.y }, 1, 0);
        batch.PointCirc({ outer.max.x, inner.max.y }, 0, 1);
        batch.PointCirc({ outer.max.x, outer.max.y }, 1, 1);
        batch.Tri(8, 9, 10); batch.Tri(11, 12, 13); batch.Tri(14, 15, 16); batch.Tri(17, 18, 19);
    }

    void Canvas::DrawSimpleVarRoundRect(const Math::fRect2D& outer, float tr, float br, float tl, float bl, const Math::fColor& color) {
        Batch batch = NewBatch();
        batch.SetColor(color);
        batch.Point({ outer.max.x - tr, outer.max.y });
        batch.Point({ outer.min.x + tl, outer.max.y });
        batch.Point({ outer.max.x, outer.max.y - tr });
        batch.Point({ outer.min.x, outer.max.y - tl });
        batch.Point({ outer.max.x, outer.min.y + br });
        batch.Point({ outer.min.x, outer.min.y + bl });
        batch.Point({ outer.max.x - br, outer.min.y });
        batch.Point({ outer.min.x + bl, outer.min.y });
        batch.TriStrip({{ 0, 1, 2, 3, 4, 5, 6, 7 }});
        batch.PointCirc({ outer.min.x + bl, outer.min.y }, 1, 0);
        batch.PointCirc({ outer.min.x, outer.min.y + bl }, 0, 1);
        batch.PointCirc({ outer.min.x, outer.min.y      }, 1, 1);
        batch.PointCirc({ outer.max.x - br, outer.min.y }, 1, 0);
        batch.PointCirc({ outer.max.x, outer.min.y + br }, 0, 1);
        batch.PointCirc({ outer.max.x, outer.min.y      }, 1, 1);
        batch.PointCirc({ outer.min.x + tl, outer.max.y }, 1, 0);
        batch.PointCirc({ outer.min.x, outer.max.y - tl }, 0, 1);
        batch.PointCirc({ outer.min.x, outer.max.y      }, 1, 1);
        batch.PointCirc({ outer.max.x - tr, outer.max.y }, 1, 0);
        batch.PointCirc({ outer.max.x, outer.max.y - tr }, 0, 1);
        batch.PointCirc({ outer.max.x, outer.max.y      }, 1, 1);
        batch.Tri(8, 9, 10); batch.Tri(11, 12, 13); batch.Tri(14, 15, 16); batch.Tri(17, 18, 19);
    }

    void Canvas::DrawRectStroke(const Math::fRect2D& rect) {
        const Math::fRect2D outer = rect.Extrude(drawAttr.strokeWeight),
                            inner = rect.Inset(drawAttr.strokeWeight);

        Batch batch = NewBatch();
        batch.SetStroke();
        switch (drawAttr.drawStyle & UIRender::JOIN_MASK) {
            case UIRender::ROUND_JOIN:
                // just add the corners, the rest will be done by BEVEL_JOIN
                DrawQuarterChord(rect.min,                   { -drawAttr.strokeWeight, 0 }, drawAttr.strokeColor);
                DrawQuarterChord({ rect.max.x, rect.min.y }, { 0, -drawAttr.strokeWeight }, drawAttr.strokeColor);
                DrawQuarterChord({ rect.min.x, rect.max.y }, { 0, +drawAttr.strokeWeight }, drawAttr.strokeColor);
                DrawQuarterChord(rect.max,                   { +drawAttr.strokeWeight, 0 }, drawAttr.strokeColor);
                batch.Refresh();
                [[fallthrough]];
            case UIRender::BEVEL_JOIN: {
                /*
                 *     C          B
                 *   E              L
                 *        D    A
                 *
                 *        H    I
                 *   F              K
                 *     G          J
                 */
                batch.Point({ inner.max.x, inner.max.y });
                batch.Point({ rect.max.x,  outer.max.y });
                batch.Point({ rect.min.x,  outer.max.y });
                batch.Point({ inner.min.x, inner.max.y });
                batch.Point({ outer.min.x, rect.max.y  });
                batch.Point({ outer.min.x, rect.min.y  });
                batch.Point({ rect.min.x,  outer.min.y });
                batch.Point({ inner.min.x, inner.min.y });
                batch.Point({ inner.max.x, inner.min.y });
                batch.Point({ rect.max.x,  outer.min.y });
                batch.Point({ outer.max.x, rect.min.y  });
                batch.Point({ outer.max.x, rect.max.y  });
                batch.Quad(0, 1, 2, 3); // ABCD
                batch.TriFan({{ 2, 4, 5, 6, 7, 3 }}); // C, EFGHD
                batch.Quad(6, 9, 8, 7); // GJIH
                batch.TriFan({{ 9, 10, 11, 1, 0, 8 }}); // J, KLBAI
                break;
            }
            case UIRender::MITER_JOIN: {
                batch.Point(outer.BottomLeft());
                batch.Point(inner.BottomLeft());
                batch.Point(outer.TopLeft());
                batch.Point(inner.TopLeft());
                batch.Point(outer.TopRight());
                batch.Point(inner.TopRight());
                batch.Point(outer.BottomRight());
                batch.Point(inner.BottomRight());
                batch.TriStrip({{ 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 }});
                break;
            }
            default:;
        }
    }

    void Canvas::Path::AddPoint(const Math::fv2& point) {
        if (NoPointsYet()) return BeginLineSegment(point);

        const Math::fv2 tangent = lastPoint.Tangent(point);
        canvas.DrawSimpleLine(lastPoint, point, tangent);
        EndLineSegment(point, tangent);
    }

    bool Canvas::Path::NoPointsYet() {
        return closing == OPEN_CURVE || closing == CLOSED_CURVE;
    }

    void Canvas::Path::BeginLineSegment(const Math::fv2& p) {
        switch (closing) {
            case OPEN_CURVE:
                // no points yet, encountered first point
                lastPoint = p;
                closing = OPEN_CURVE_SECOND_POINT;
                break;
            case CLOSED_CURVE:
                // no points yet, but we need to store first point to close the curve
                firstPoint = lastPoint = p;
                closing = CLOSED_CURVE_SECOND_POINT;
                break;
            default:;
        }
    }

    void Canvas::Path::EndLineSegment(const Math::fv2& p, const Math::fv2& tangent) {
        switch (closing) {
            case OPEN_CURVE_SECOND_POINT: {
                // we cap of the first point since we just left it
                canvas.DrawLineCap(lastPoint, tangent);
                lastPoint = p;
                lastTangent = tangent;
                closing = OPEN_CURVE_MIDDLE_POINT;
                break;
            }
            case OPEN_CURVE_MIDDLE_POINT: {
                // since we're in the middle, we now have to draw joins instead of caps
                canvas.DrawLineJoin(lastPoint, lastTangent, tangent);
                lastPoint = p;
                lastTangent = tangent;
                break;
            }
            case CLOSED_CURVE_SECOND_POINT: {
                // we cant draw the join of the previous point, since we dont know the last
                lastPoint = p;
                firstTangent = lastTangent = tangent;
                closing = CLOSED_CURVE_MIDDLE_POINT;
                break;
            }
            case CLOSED_CURVE_MIDDLE_POINT: {
                // now we can draw the previous join
                canvas.DrawLineJoin(lastPoint, lastTangent, tangent);
                lastPoint = p;
                lastTangent = tangent;
                break;
            }
            default:;
        }
    }

    void Canvas::Path::DrawCircularArcCCW(const Math::fv2& startPoint, const Math::fv2& center, const Math::Rotor2D& turn) {
        const Math::Rotor2D halfTurn = turn.HalvedCCW();
        const auto [direction, radius] = (startPoint - center).NormAndLen();
        const Math::Rotor2D middle = Math::Rotor2D::FromComplex({ direction.x, direction.y }) + halfTurn;
        canvas.DrawCircularArcCCW(center, middle, halfTurn, radius, canvas.drawAttr.strokeWeight, canvas.drawAttr.strokeColor);
    }

    void Canvas::Path::AddCircularArc(const Math::fv2& center, const Math::Rotor2D& turn, ArcDirection dir) {
        if (NoPointsYet()) return;

        const Math::fv2 lastDirection = (lastPoint - center), endPoint = lastDirection.RotateBy(turn) + center;
        const bool swap = (dir & 1) ^ ((dir & 2) && turn.Sin() < 0);
        DrawCircularArcCCW(swap ? endPoint : lastPoint, center, swap ? turn.Inverse() : turn);

        EndLineSegment(endPoint, lastDirection.Norm().Perpend(!swap));
        lastTangent = lastTangent.RotateBy(turn);
    }

    void Canvas::Path::AddQuadBez(const Math::fv2& control, const Math::fv2& end) {
        if (NoPointsYet()) return;

        const Math::fv2 startPoint = lastPoint;
        const Math::fv2 v0 = control - startPoint, v1 = end - control;
        Math::fv2 startNorm = v0.Norm(),
                  endNorm   = v1.Norm();
        EndLineSegment(end, startNorm);
        lastTangent = endNorm;

        const float k11 = v1.LenSq(), k01 = v1.Dot(v0), k00 = v0.LenSq();

        startNorm = startNorm.Perpend();
        endNorm   = endNorm.Perpend();

        const bool rightwardCurving = v0.Cross(v1) < 0;
        float w = rightwardCurving ? canvas.drawAttr.strokeWeight : -canvas.drawAttr.strokeWeight;
        if (startNorm.Dot(endNorm) > -0.99f) {
            // we need arbitrary precision on this path; curves are smooth
            // and thickness limits the size of which bezier curves can be rendered

            // this is actually one iteration of newton starting at 0.5,
            // it's just that its so simplified you cant even tell.
            // its supposed to find the closest point to the control point (t value)
            // that satisfies the equation (x(t) - C) . v(t) = 0 (this is when x(t) is closest to C)
            // where x(t) is the position and v(t) is the velocity, C is the control point,
            // and this turns out to be a cubic equation,
            // in which we do one iteration to get a very nice approximation.
            // good luck to anyone that has to fix or adapt this line of code though.
            const float t = 0.5f - (k11 - k00) / (6 * (k11 + k00) + 4 * k01),
                        u = 1 - t;

            Math::fv2 left    = startPoint + v0 * t,
                      right   = control    + v1 * t,
                      closest = left * u + right * t;
            const Math::fv2 farLeft  = (startPoint + left) * 0.5f, nearLeft  = (left + closest) * 0.5f,
                            farRight = (right      + end)  * 0.5f, nearRight = (closest + right) * 0.5f;
            left  *= 0.5f;
            left  += 0.25f * (startPoint + closest);
            right *= 0.5f;
            right += 0.25f * (closest + end);
            Math::fv2 closestNorm = v0 * u + v1 * t,
                      leftNorm    = (v0 + closestNorm).Norm().Perpend(),
                      rightNorm   = (closestNorm + v1).Norm().Perpend();
            closestNorm = closestNorm.Norm().Perpend();
            Math::fv2 fln = leftNorm + startNorm, nln = leftNorm + closestNorm,
                      frn = rightNorm + endNorm,  nrn = rightNorm + closestNorm;

            startNorm   *= w;
            endNorm     *= w;
            closestNorm *= w;
            leftNorm    *= w;
            rightNorm   *= w;
            w *= 2;
            fln *= w / fln.LenSq();
            nln *= w / nln.LenSq();
            nrn *= w / nrn.LenSq();
            frn *= w / frn.LenSq();

            Vec allPoints = Vec<Math::fv2>::FromIList({
                startPoint + startNorm,
                farLeft    + fln,
                left       + leftNorm,
                nearLeft   + nln,
                closest    + closestNorm,
                nearRight  + nrn,
                right      + rightNorm,
                farRight   + frn,
                end        + endNorm,
                startPoint - startNorm,
                farLeft    - fln,
                left       - leftNorm,
                nearLeft   - nln,
                closest    - closestNorm,
                nearRight  - nrn,
                right      - rightNorm,
                farRight   - frn,
                end        - endNorm,
            });

            u32 subdivCount = 0;
            for (; subdivCount <= 4; ++subdivCount) {
                const usize upperLineCount = 4 << subdivCount;
                const Math::fv2* lowerLines = allPoints.Data() + 2 * upperLineCount + 1;
                for (usize j = 0; j < upperLineCount; ++j) {
                    const Math::fv2 upperForward = allPoints[2 * j + 2] - allPoints[2 * j],
                                    towardsLowerControl = lowerLines[2 * j + 1] - allPoints[2 * j];
                    if ((upperForward.Cross(towardsLowerControl) < 0) != rightwardCurving)
                        goto subdivide_properbranch;
                }
                break;

            subdivide_properbranch:
                allPoints.ResizeExtra(allPoints.Length() - 2);
                Math::fv2* begin = allPoints.Data() + 2 * upperLineCount + 1;
                // work backwards; so when doubling we dont run into overwriting issues
                usize back = allPoints.Length();
                // TODO: use proper interpolation with normals. this method gives inaccurate results
                for (usize section = 0; section < 2; ++section) {
                    for (usize j = upperLineCount; j --> 0; ) {
                        // P0-C-P1 original; P0-D0-M + M-D1-P1 subdivided
                        //          C
                        //
                        //    D0    M     D1
                        //
                        // P0                 P1
                        const Math::fv2 &P0 = begin[2 * j], &C = begin[2 * j + 1], &P1 = begin[2 * j + 2];
                        const Math::fv2 D0 = (P0 + C) * 0.5f, D1 = (C + P1) * 0.5f, M = (D0 + D1) * 0.5f;
                        allPoints[--back] = P1;
                        allPoints[--back] = D1;
                        allPoints[--back] = M;
                        allPoints[--back] = D0;
                    }
                    allPoints[--back] = begin[0];
                    begin -= 2 * upperLineCount + 1;
                }
            }

            const usize group = (8 << subdivCount) + 1;

            Batch batch = canvas.NewBatch();
            batch.SetStroke();
            batch.PointsQBezClosed(allPoints.Subspan(0, group), allPoints.Skip(group));
        } else {
            // very sharp curve - use circle approximation for the tip,
            // the rest can be easily approximated by simple beziers.
            // no need for extra subdivisions

            const Math::fv2 pt = 2 * v0, pt2 = v1 - v0;
            const float t = -pt2.Dot(v0) / pt2.LenSq();

            static constexpr float K = 0.04;
            const float a = t - K, b = t + K;
            Math::fv2 leftControl  = startPoint + v0 * a,
                      rightControl = control + v1 * b;
            // we'll assume the neighbors are symmetrical
            const Math::fv2 closest     = startPoint + t * (pt + t * pt2),
                            closestNorm = closest.Tangent(control),
                            neighbor    = startPoint + a * (pt + a * pt2),
                            reflect     = startPoint + b * (pt + b * pt2);

            Math::fv2 neighborNorm = v0 + pt2 * a;
            startNorm *= w;
            endNorm   *= w;
            const Math::fv2 reflectNorm = (neighborNorm + pt2 * (2 * K)).Perpend().Norm(w);
            neighborNorm = neighborNorm.Perpend().Norm(w);

            const Math::fv2 outerNeighbor = neighbor + neighborNorm, outerReflect = reflect + reflectNorm,
                            middleGround = (outerNeighbor + outerReflect) * 0.5f;
            const auto [down, y] = (middleGround - outerNeighbor).NormAndLen();
            const float r = std::max(canvas.drawAttr.strokeWeight, y), x = f32s::CopySign(std::sqrt(r * r - y * y), w);
            const Math::fv2 center = outerNeighbor + down.ComplexMul(y, -x);
            canvas.DrawHalfCircularChord(
                center, down.Perpend() * w,
                x / r, y / r,
                canvas.drawAttr.strokeColor
            );

            Batch batch = canvas.NewBatch();
            batch.SetStroke();

            batch.Point(outerNeighbor);
            batch.Point(outerReflect);
            batch.Point(closest - closestNorm * std::abs(w));
            batch.Tri(0, 1, 2);

            Math::fv2 leftNorm  = startNorm + neighborNorm,
                      rightNorm = reflectNorm + endNorm;
            w *= 2 * w;
            leftNorm  *= w / leftNorm.LenSq();
            rightNorm *= w / rightNorm.LenSq();

            Vec allPoints = Vec<Math::fv2>::FromIList({
                startPoint   + startNorm,
                leftControl  + leftNorm,
                outerNeighbor,
                outerReflect,
                rightControl + rightNorm,
                end          + endNorm,
                startPoint   - startNorm,
                leftControl  - leftNorm,
                neighbor     - neighborNorm,
                reflect      - reflectNorm,
                rightControl - rightNorm,
                end          - endNorm,
            });

            u32 subdivCount = 0;
            for (; subdivCount <= 4; ++subdivCount) {
                const usize upperLineCount = 1 << subdivCount;
                const Math::fv2* lowerLines = allPoints.Data() + 4 * upperLineCount + 2;
                for (usize j = 0; j < upperLineCount; ++j) {
                    const Math::fv2 upperForward        = allPoints[2 * j + 2] - allPoints[2 * j],
                                    towardsLowerControl = lowerLines[2 * j + 1] - allPoints[2 * j];
                    if ((upperForward.Cross(towardsLowerControl) < 0) != rightwardCurving)
                        goto subdivide_sharpbranch;
                }
                {
                    const Math::fv2* upperLines = allPoints.Data() + 2 * upperLineCount + 1;
                    lowerLines += 2 * upperLineCount + 1;
                    for (usize j = 0; j < upperLineCount; ++j) {
                        const Math::fv2 upperForward = upperLines[2 * j + 2] - upperLines[2 * j],
                                        towardsLowerControl = lowerLines[2 * j + 1] - upperLines[2 * j];
                        if ((upperForward.Cross(towardsLowerControl) < 0) != rightwardCurving)
                            goto subdivide_sharpbranch;
                    }
                }
                break;

                subdivide_sharpbranch:
                allPoints.ResizeExtra(allPoints.Length() - 4);
                Math::fv2* begin = allPoints.Data() + 6 * upperLineCount + 3;
                // work backwards; so when doubling we dont run into overwriting issues
                usize back = allPoints.Length();
                for (usize section = 0; section < 4; ++section) {
                    for (usize j = upperLineCount; j --> 0; ) {
                        const Math::fv2 &P0 = begin[2 * j], &C = begin[2 * j + 1], &P1 = begin[2 * j + 2];
                        const Math::fv2 D0 = (P0 + C) * 0.5f, D1 = (C + P1) * 0.5f, M = (D0 + D1) * 0.5f;
                        allPoints[--back] = P1;
                        allPoints[--back] = D1;
                        allPoints[--back] = M;
                        allPoints[--back] = D0;
                    }
                    allPoints[--back] = begin[0];
                    begin -= 2 * upperLineCount + 1;
                }
            }

            const usize group = (2 << subdivCount) + 1;

            batch.PointsQBezClosed(allPoints.Subspan(0,     group), allPoints.Subspan(group * 2, group));
            batch.PointsQBezClosed(allPoints.Subspan(group, group), allPoints.Subspan(group * 3, group));
        }
    }

    void Canvas::Path::ClosePath() {
        // end the path
        switch (closing) {
            case OPEN_CURVE: case CLOSED_CURVE:
                // no path was drawn. only 1 point.
                return;
            case OPEN_CURVE_SECOND_POINT: case OPEN_CURVE_MIDDLE_POINT:
                // this is the last point. cap it off
                canvas.DrawLineCap(lastPoint, -lastTangent);
                break;
            case CLOSED_CURVE_SECOND_POINT: case CLOSED_CURVE_MIDDLE_POINT: {
                // this is the last point. close it off with the first
                const Math::fv2 tangent = lastPoint.Tangent(firstPoint);
                canvas.DrawLineJoin(lastPoint, lastTangent, tangent); // last join
                canvas.DrawSimpleLine(lastPoint, firstPoint, tangent);
                canvas.DrawLineJoin(firstPoint, tangent, firstTangent); // first join
                break;
            }
            default:;
        }
        closing = COMPLETED;
    }

    Canvas::Path::~Path() { ClosePath(); }

    Canvas::Path Canvas::NewPath(CurveMode mode) {
        return Path { *this, mode };
    }

    void Canvas::StrokeWeight(float weight) {
        drawAttr.strokeWeight = weight;
        drawAttr.drawStyle &= ~UIRender::NO_STROKE;
    }

    void Canvas::StrokeCap(UIRender::RenderStyle cap) {
        drawAttr.drawStyle &= ~UIRender::CAP_MASK;
        drawAttr.drawStyle |= cap;
    }

    void Canvas::StrokeJoin(UIRender::RenderStyle join) {
        drawAttr.drawStyle &= ~UIRender::JOIN_MASK;
        drawAttr.drawStyle |= join;
    }

    void Canvas::Fill(const Math::fColor& fillColor) {
        drawAttr.fillColor = fillColor;
        drawAttr.drawStyle &= ~UIRender::NO_FILL;
    }

    void Canvas::Stroke(const Math::fColor& strokeColor) {
        drawAttr.strokeColor = strokeColor;
        drawAttr.drawStyle &= ~UIRender::NO_STROKE;
    }

    void Canvas::NoStroke() {
        drawAttr.drawStyle |= UIRender::NO_STROKE;
    }

    void Canvas::NoFill() {
        drawAttr.drawStyle |= UIRender::NO_FILL;
    }

    Canvas::DeferRenderScope::DeferRenderScope(Canvas& canvas, UIMesh& newDest)
        : canvas(canvas), prevDestination(canvas.drawMesh) {
        canvas.drawMesh = newDest;
    }

    Canvas::DeferRenderScope::~DeferRenderScope() {
        canvas.drawMesh = prevDestination;
    }

    Canvas::DeferRenderScope Canvas::RenderTo(UIMesh& mesh) {
        return { *this, mesh };
    }

    Canvas::PushStylesScope::PushStylesScope(Canvas& canvas) : canvas(canvas), originalAttr(canvas.drawAttr) {}
    Canvas::PushStylesScope::~PushStylesScope() {
        canvas.drawAttr = originalAttr;
    }

    Canvas::PushStylesScope Canvas::PushStyles() {
        return { *this };
    }

    Math::fv2 Canvas::TransformToWorldSpace(const Math::fv2& point) const {
        return transform * point;
    }

    void Canvas::BeginFrame() {
        renderCanvas.BeginContext();
        worldMesh.Clear();
    }

    void Canvas::EndFrame() {
        worldMesh.CopyTo(renderCanvas.GetRenderData());
        renderCanvas.EndContext();
        renderCanvas.DrawContext();
    }
}
