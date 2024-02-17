#pragma once
#include "Mesh.h"
#include "Test.h"
#include "Fonts/Font.h"

namespace Test {
    class TestFontRender : Test {
    public:
        struct Vertex {
            Maths::fvec3  Position;
            Maths::colorf Color;
            Maths::fvec2  TextureCoordinate;
            int isText = 0;
    
            GL_VERTEX_T(Vertex);
            GL_VERTEX_FIELD((Position)(Color)(TextureCoordinate)(isText));
            GL_VERTEX_TRANSFORM_FIELDS((Position))
        };
    private:
        Graphics::RenderObject<Vertex> render;
        Graphics::Mesh<Vertex> meshStr, meshAtlas, meshBg;
        bool showAtlas = false;
        int alignX = 0, alignY = 0, wrapMethod = 0;
        bool cropX = false, cropY = false;
        float letterSpace = 0, lineSpace = 1;
        Maths::rect2f textBox = { -200, 200, -200, 200 };

        float fontSize = 48;
        float thickness = 0.5f, softness = 0.05f;
        
        Maths::colorf shadowColor = 0;
        float shadowSoftness = 0.3f;
        Maths::fvec2 shadowOffset = { 5, 5 };

        std::string string = R"(`Features include: <br>
New Line with `<br>`<br>
**bold text** with `**`<br>
*italic text* with `*`<br>
***bold & italic*** with `***``<br>
~~Strike trough like dis~~ with `~~`)";

        Graphics::Font font;
        Maths::colorf color = 1;

        Maths::mat3D projection = Maths::mat3D::ortho_projection({ -320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f });
        Maths::fvec3 modelTranslation = 0;
        Maths::fvec3 modelScale = 1;
        Maths::fvec3 modelRotation = 0;

        DEFINE_TEST_T(TestFontRender, ADVANCED)
    public:
        TestFontRender() {}
        ~TestFontRender() override {}

        void OnInit(Graphics::GraphicsDevice& gdevice) override;
        void OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) override {}
        void OnRender(Graphics::GraphicsDevice& gdevice) override;
        void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        void OnDestroy(Graphics::GraphicsDevice& gdevice) override;
    };
}