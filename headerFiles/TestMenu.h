#ifndef TEST_MENU_CLASS_H
#define TEST_MENU_CLASS_H

#include "Test.h"
#include "imgui/imgui.h"
#include <vector>
#include <functional>


class TestMenu : public Test
{
public:
    TestMenu(GLFWwindow *window, Test*& currentTest);
    ~TestMenu();

    void OnImguiRender() override;

    template <typename T>
    void Register(const std::string &name)
    {
        std::cout << "Registered : " << name << std::endl;
        m_AllTests.push_back(std::make_pair(name, [this]() { return new T(m_Window); }));
    }

private:
    GLFWwindow *m_Window;
    Test*& m_CurrentTest;
    std::vector<std::pair<std::string, std::function<Test *()>>> m_AllTests;
};

#endif
