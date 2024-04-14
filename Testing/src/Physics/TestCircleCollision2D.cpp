#include "TestCircleCollision2D.h"

#include <imgui.h>

#include "Meshes/Circle.h"
#include "Physics/CircleCollider2D.h"

namespace Test {
    void TestCircleCollision2D::OnInit(Graphics::GraphicsDevice& gdevice) {
        scene = gdevice.CreateNewRender<Vertex>();
        viewport = { 0, 800, 0, 600 };

        using namespace Graphics::VertexBuilder;
        circleMesh = Graphics::MeshUtils::Circle({ 32 }, Vertex::Blueprint {
            .Position = GetPosition {}
        });

        scene.BindMeshes(circleMesh);

        scene.UseShaderFromFile(res("circle.vert"), res("circle.frag"));
        lineShader = Graphics::Shader::FromFile(res("line.vert"), res("line.frag"), res("line.geom"));

        scene.SetProjection(Maths::mat3D::ortho_projection({ 0, 800, 0, 600, -1, 1 }));

        ResetBalls(gdevice);
    }

    void TestCircleCollision2D::OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) {
        totalLineMesh.Clear();
        uint line = 0, drawForceLine = 0;

        for (Physics2D::CircleCollider& circle : circles) {
            circle.acceleration = -circle.velocity * 0.8f;
            circle.Update(deltaTime);
        }

        const auto& mouse = gdevice.GetIO().Mouse;
        const Maths::fvec2 mousePos = mouse.GetMousePos().as<float>().map({ -1, 1, 1, -1 }, viewport);
        if (mouse.LeftOnPress() && !selected) {
            for (Physics2D::CircleCollider& circ : circles) {
                if (circ.CollidePoint(mousePos)) {
                    selected = &circ;
                    selectOffset = mousePos - circ.position;
                    break;
                }
            }
        }

        if (mouse.LeftPressed() && selected) {
            const Maths::fvec2 newPos = mousePos - selectOffset;
            selected->position = newPos;
        }

        if (mouse.LeftOnRelease()) selected = nullptr;

        if (mouse.MiddleOnPress()) lastDragPosition = mousePos;
        if (mouse.MiddlePressed()) {
            for (Physics2D::CircleCollider& c : circles) c.position -= lastDragPosition - mousePos;
            lastDragPosition = mousePos;
        }

        if (mouse.RightOnPress() && !selected) {
            for (Physics2D::CircleCollider& circ : circles) {
                if (circ.CollidePoint(mousePos)) {
                    selected = &circ;
                    break;
                }
            }
        }

        if (mouse.RightPressed() && selected) {
            totalLineMesh.GetVertices().emplace_back(selected->position);
            totalLineMesh.GetVertices().emplace_back(mousePos);
            totalLineMesh.GetIndices().emplace_back(0, 1, 1);
            line += 2;
            drawForceLine = 1;
        }

        if (mouse.RightOnRelease() && selected) {
            selected->velocity -= 3 * (mousePos - selected->position);
            selected = nullptr;
        }

        for (uint i = 0; i < circles.size(); ++i) {
            for (uint j = i + 1; j < circles.size(); ++j) {
                Physics2D::CircleCollider& base = circles[i],
                                         & target = circles[j];
                if (base.CollidesWith(target)) {
                    base.StaticResolve(target);
                    base.DynamicResolve(target);
                    totalLineMesh.GetVertices().emplace_back(base.position);
                    totalLineMesh.GetVertices().emplace_back(target.position);
                    totalLineMesh.GetIndices().emplace_back(line, line + 1, line + 1);
                    line += 2;
                }
            }
        }

        lineShader.Bind();
        lineShader.SetUniformInt("blueThreshold", (int)drawForceLine);
    }

    void TestCircleCollision2D::OnRender(Graphics::GraphicsDevice& gdevice) {
        scene.Shader().Bind();
        for (uint i = 0; i < 20; ++i)
            scene.Shader().SetUniformFvec2(std::format("offsets[{}]", i), circles[i].position);

        scene.ResetData();
        scene.RenderInstanced(20, {
            { "u_projection", scene.ProjectionMat() },
            { "selected", selected ? (int)(selected - circles.data()) : -1 }
        }, false);

        scene.ClearData();
        scene.AddNewMeshes(totalLineMesh);
        scene.Render(lineShader, {
            { "u_projection", scene.ProjectionMat() }
        }, false);
    }

    void TestCircleCollision2D::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        if (ImGui::Button("Reset Balls")) ResetBalls(gdevice);
        ImGui::Text("Current Selected: %s", selected ? std::format("#{}", selected - circles.data()).data() : "None");
    }

    void TestCircleCollision2D::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        scene.Destroy();
    }

    void TestCircleCollision2D::AddRandomBall(Graphics::GraphicsDevice& gdevice) {
        auto& rand = gdevice.GetRand();

        const Maths::fvec2 position = Maths::fvec2::random(rand, viewport);
        const float radius = rand.getf(30.0f, 80.0f);
        circles.emplace_back(position, radius * radius, radius);
        colors.emplace_back(Maths::colorf::random(rand, rand.getf(0.3f, 0.7f), rand.getf(0.8f, 1.0f)));
        scales.emplace_back(radius);
    }

    void TestCircleCollision2D::ResetBalls(Graphics::GraphicsDevice& gdevice) {
        circles.clear();
        circles.reserve(20);
        colors.clear();
        colors.reserve(20);
        scales.clear();
        scales.reserve(20);

        for (int i = 0; i < 20; ++i)
            AddRandomBall(gdevice);

        scene.Shader().Bind();
        scene.Shader().SetUniformFvec4Arr("colors", stdu::span_cast<const Maths::fvec4>(std::span(colors)));
        scene.Shader().SetUniformFloatArr("scales", scales);
    }
} // Test