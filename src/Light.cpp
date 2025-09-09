#include "Light.h"

// ---------------- Directional Light ----------------
DirectionalLight::DirectionalLight(const glm::vec3 &pos, const glm::vec4 &color, const glm::vec3 &dir,  float intensity)
    : Light(pos, color, intensity), m_Direction(glm::normalize(dir)) {}

void DirectionalLight::upload(Shader &shader, const std::string &name) const
{
    shader.SetVec3(name + ".color", m_Color * m_Intensity);
    shader.SetVec3(name + ".direction", m_Direction);
    shader.SetInt1(name + ".enabled", m_Enabled ? 1 : 0);
}

// ---------------- Point Light ----------------
PointLight::PointLight(const glm::vec3 &pos, const glm::vec4 &color, float intensity,
                       float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f)
    : Light(pos, color, intensity),
      m_Constant(constant), m_Linear(linear), m_Quadratic(quadratic) {}

void PointLight::upload(Shader &shader, const std::string &name) const
{
    shader.SetVec3(name + ".color", m_Color * m_Intensity);
    shader.SetVec3(name + ".position", m_Position);
    shader.SetFloat1(name + ".constant", m_Constant);
    shader.SetFloat1(name + ".linear", m_Linear);
    shader.SetFloat1(name + ".quadratic", m_Quadratic);
    shader.SetInt1(name + ".enabled", m_Enabled ? 1 : 0);
}

// ---------------- Spot Light ----------------
SpotLight::SpotLight(const glm::vec3 &pos, const glm::vec4 &color, const glm::vec3 &dir, 
                     float intensity, float cutOff, float outerCutOff)
    : Light(pos, color, intensity), m_Direction(glm::normalize(dir)),
      m_CutOff(cutOff), m_OuterCutOff(outerCutOff) {}

void SpotLight::upload(Shader &shader, const std::string &name) const
{
    shader.SetVec3(name + ".color", m_Color * m_Intensity);
    shader.SetVec3(name + ".position", m_Position);
    shader.SetVec3(name + ".direction", m_Direction);
    shader.SetFloat1(name + ".cutOff", m_CutOff);
    shader.SetFloat1(name + ".outerCutOff", m_OuterCutOff);
    shader.SetInt1(name + ".enabled", m_Enabled ? 1 : 0);
}