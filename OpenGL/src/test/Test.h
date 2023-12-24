#pragma once
#include <functional>

#include "Renderer.h"

namespace Test
{
    class Test
    {
    public:
        Test() {}
        virtual ~Test() {}

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender(Graphics::Renderer& renderer) {}
        virtual void OnImGuiRender() {}
    };

    class TestMenu : public Test
    {
        struct TestMenuItem
        {
            std::string name;
            std::function<Test*()> testConstruct;
        };
    private:
        Test** currentTest;
        std::vector<TestMenuItem> menuItems;
    public:
        OPENGL_API TestMenu(Test*& currTest);

        OPENGL_API void OnImGuiRender() override;

        template <typename T>
        void RegisterTest(const std::string& name)
        {
            //LOG("Registered " << name << " Test");
            menuItems.emplace_back(name, []{ return (Test*)(new T()); });
        }
    };
}
