#include "TestMenu.h"

TestMenu::TestMenu(GLFWwindow *window, Test *&currentTest)
    : Test(window), m_Window(window), m_CurrentTest(currentTest)
{
}

TestMenu::~TestMenu()
{
}

void TestMenu::OnImguiRender()
{
    for (auto test : m_AllTests)
    {
        if (ImGui::Button(test.first.c_str()))
            m_CurrentTest = test.second();
    }
}
