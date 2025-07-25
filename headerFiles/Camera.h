#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include"ShaderClass.h"

class Camera
{
    public:
        Camera(int width, int height, glm::vec3 position, glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f));

        void UpdateMatrix(float fovDeg, float nearPlane, float farPlane);
        void OnWindowResize(int width, int height);
        void SetUniforms(Shader& shader);
        void Inputs(GLFWwindow* window);

    private:
        int width;
        int height;
        glm::vec3 position;
        glm::vec3 orientation;
        glm::mat4 cameraMatrix = glm::mat4(1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        float speed = 0.05f;
        float sensitivity = 300.0f;
        bool firstClick = true;
};

#endif