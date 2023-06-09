#pragma once

#include "../../Utility/Maths.h"
#include "GLFW/glfw3.h"

constexpr float CAMERA_FOV = 80.0f / 180.0f * PI;
constexpr float CAMERA_NEAR_PLANE = 0.1f;
constexpr float CAMERA_FAR_PLANE = 1000.0f;
constexpr float CAMERA_SPEED = 120.f;
constexpr float CAMERA_SENSITIVITY = 0.1f;

/**
 * Source : https://learnopengl.com/Getting-started/Camera
 */
class Camera
{
public:
    Camera(GLFWwindow* window, const glm::vec3& initialPos = glm::vec3(0.f, 0.f, 0.f));

    void moveCameraForInput(GLFWwindow* window, float deltaTime);
	void rotateCameraForInput(float xPos, float yPos);

public:

    float m_cameraAlpha { 0.f };
    float m_cameraBeta { 0.f };

    glm::vec3 m_cameraPos{ 0.f, 0.f, 0.f };
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_lastX{ 0.f }, m_lastY{ 0.f }, m_yaw{ 0.f }, m_pitch{ 0.f };
    bool m_isFirstMouseInput{ false };
    bool m_rotationIsActive = true;
};