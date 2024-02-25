#include "TestLightCasters.h"

#include "imgui.h"
#include "Model Loading/OBJModelLoader.h"

namespace Test {
    void TestLightCasters::OnInit(Graphics::GraphicsDevice& gdevice) {
        scene = gdevice.CreateNewRender<Vertex>();
        lightScene = gdevice.CreateNewRender<VertexColor3D>();

        Graphics::OBJModelLoader mloader;
        mloader.LoadFile(res("lights.obj"));
        Graphics::OBJModel model = mloader.RetrieveModel();

        materials = std::move(model.materials);
        meshes.reserve(model.objects.size());
        for (const Graphics::OBJObject& obj : model.objects) {
            meshes.emplace_back(
                obj.mesh.Convert<Vertex>(
                    [&](Graphics::OBJVertex v) {
                        return Vertex { v.Position, v.Normal, obj.materialIndex };
                    }));
        }
        scene.BindMeshes(meshes);

        scene.UseShaderFromFile(res("shader.vert"), res("shader.frag"));
        scene.SetProjection(Maths::mat3D::perspective_fov(90.0f, 4.0f / 3.0f, 0.01f, 100.0f));

        camera.position = { 10.506737, 11.603662, 6.2335258 };
        camera.yaw = -3.6930802f; camera.pitch = -0.64090014f;
        camera.speed = 5;
        camera.sensitivity = 0.12f;
        camera.fov = 90;
        camera.fovRange = { 1, 90 };
        camera.zoomRatio = 0.5f;
        camera.smoothZoom = 120;

        lights.reserve(MAX_LIGHTS);
        AddPointLight({
            .position = { -0, 7, 0 },
            .constant = 1,
            .linear = 0,
            .quadratic = 0
        }, { 1, 1, 1 });

        AddPointLight({
            .position = { 12, 3, -14 },
            .constant = 1,
            .linear = 0,
            .quadratic = 0.001f,
         }, { 1, 0, 0 });

        AddPointLight({
            .position = { -9, 3, 6 },
            .constant = 1,
            .linear = 0,
            .quadratic = 0.001f,
         }, { 0, 0.6176472f, 1 });

        AddPointLight({
            .position = { 12, 3, 16 },
            .constant = 1,
            .linear = 0,
            .quadratic = 0.001f,
         }, { 0.2647059f, 1, 0 });

        lightScene.UseShader(Graphics::Shader::StdColored);
    }

    void TestLightCasters::OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) {
        camera.Update(gdevice, deltaTime);
    }

    void TestLightCasters::OnRender(Graphics::GraphicsDevice& gdevice) {
        lightScene.SetProjection(camera.GetProjMat());
        lightScene.SetCamera(camera.GetViewMat());
        lightScene.ResetData();
        lightScene.Render();

        scene.GetShader().Bind();
        scene.GetShader().SetUniform1F("ambientStrength", ambientStrength);
        scene.GetShader().SetUniform3F("viewPosition", camera.position.begin());
        scene.GetShader().SetUniform1F("specularIntensity", specularStrength);

        for (uint i = 0; i < materials.size(); ++i) {
            UniformMaterial(std::format("materials[{}]", i), materials[i]);
        }

        for (uint i = 0; i < lights.size(); ++i) {
            UniformLight(std::format("lights[{}]", i), lights[i]);
        }

        scene.SetProjection(camera.GetProjMat());
        scene.SetCamera(camera.GetViewMat());
        scene.ResetData();
        scene.Render();
    }

    void TestLightCasters::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        ImGui::DragFloat("Ambient Strength", &ambientStrength, 0.01f);
        ImGui::DragFloat("Specular Strength", &specularStrength, 0.01f);

        camera.ImGuiEdit();

        if (ImGui::TreeNode("Lights")) {
            if (ImGui::Button("+") && lights.size() < MAX_LIGHTS) {
                AddPointLight({
                    .position = 0,
                    .constant = 1, .linear = 0, .quadratic = 0
                }, 1);
            }
            ImGui::SameLine();
            if (ImGui::Button("-") && !lights.empty()) {
                lights.pop_back();
            }

            for (uint i = 0; i < lights.size(); ++i) {
                lights[i].ImGuiEdit(std::format("Light {}", i + 1).c_str());
                lightMeshes[i].ApplyMaterial(&VertexColor3D::Color, lights[i].color);
                lightMeshes[i].SetTransform(Maths::mat3D::translate_mat(lights[i].Position()));
            }
            ImGui::TreePop();
        }
    }

    void TestLightCasters::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        scene.Destroy();
    }

    void TestLightCasters::UniformMaterial(const std::string& name, const Graphics::MTLMaterial& material) {
        Graphics::Shader& shader = scene.GetShader();
        shader.SetUniform3F(name + ".ambient", material.Ka.begin());
        shader.SetUniform3F(name + ".diffuse", material.Kd.begin());
        shader.SetUniform3F(name + ".specular", material.Ks.begin());
        shader.SetUniform1F(name + ".shininess", material.Ns);
    }

    void TestLightCasters::UniformLight(const std::string& name, const Graphics::Light& light) {
        Graphics::Shader& shader = scene.GetShader();
        Maths::fvec3 top, bottom;
        switch (light.type) {
            case Graphics::LightType::SUNLIGHT:
                top = light.As<Graphics::SunLight>()->direction;
                break;
            case Graphics::LightType::POINTLIGHT: {
                const Graphics::PointLight& p = light.As<Graphics::PointLight>();
                top = p.position;
                bottom = { p.constant, p.linear, p.quadratic };
                break;
            }
            case Graphics::LightType::FLASHLIGHT: {
                const Graphics::FlashLight& f = light.As<Graphics::FlashLight>();
                top = f.position;
                bottom = { f.yaw, f.pitch, f.innerCut };
                break;
            }
            case Graphics::LightType::NONE:
                break;
        }
        shader.SetUniform1I(name + ".lightId", (int)light.type + 1);
        shader.SetUniform3F(name + ".d1", top.begin());
        shader.SetUniform3F(name + ".d2", bottom.begin());
        shader.SetUniform1F(name + ".d3", light.Is<Graphics::FlashLight>() ? light.As<Graphics::FlashLight>()->outerCut : 0);
        shader.SetUniform4F(name + ".color", light.color.begin());
    }

    void TestLightCasters::AddPointLight(const Graphics::PointLight& point, const Maths::colorf& color) {
        lights.emplace_back();
        lights.back() = point;
        lights.back().color = color;

        lightMeshes.emplace_back(
            Graphics::MeshUtils::CubeMesh(0, 0.75f, 0.75f, 0.75f)
            .ApplyMaterial(&VertexColor3D::Color, color)
        );
        lightMeshes.back().SetTransform(Maths::mat3D::translate_mat(point.position));
        lightMeshes.back().Bind(lightScene);
    }
}