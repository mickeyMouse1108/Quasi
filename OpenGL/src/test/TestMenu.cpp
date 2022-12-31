#include "imgui.h"
#include "Test.h"

namespace Test
{
    TestMenu::TestMenu(Test*& currTest) : currentTest(&currTest) {}

    void TestMenu::OnImGuiRender()
    {
        for (TestMenuItem& testItem : menuItems)
        {
            if ( ImGui::Button(testItem.name.c_str()) )
            {
                // LOG("clicked " << testItem.name);
                // delete currentTest;
                *currentTest = testItem.testConstruct();
            }
        }
    }
}