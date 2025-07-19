#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include "Mesh.h"

class Test
{
public:
    Test(GLFWwindow *window) {};
    virtual ~Test() {};

    virtual void OnUpdate(float deltaTime) {};
    virtual void OnRender() {};
    virtual void OnImguiRender() {};
    virtual void OnWindowResize(GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height);};
};

#endif
