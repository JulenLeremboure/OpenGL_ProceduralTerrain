#pragma once

#include "SFML/Window/Event.hpp"
#include "../../Utility/Maths.h"
#include "../../Utility/Matrix.h"

constexpr float CAMERA_FOV = 80.0f / 180.0f * PI;
constexpr float CAMERA_NEAR_PLANE = 0.1f;
constexpr float CAMERA_FAR_PLANE = 1000.0f;
constexpr float CAMERA_SPEED = 300.f;
constexpr float CAMERA_SENSITIVITY = 0.05f;

/**
 * Source : https://learnopengl.com/Getting-started/Camera
 */
class Camera
{
public:
    Camera(const glm::vec3& initialPos = glm::vec3(0.f, 200.f, 0.f)) :
		m_cameraPos(initialPos)
	{ }

    void moveCameraForInput(const sf::Event& inputEvent, float deltaTime);
    void rotateCameraForInput(const sf::Event& inputEvent, float windowWidth, float windowHeight);

public:

    float m_cameraAlpha { 0.f };
    float m_cameraBeta { 0.f };

    glm::vec3 m_cameraPos{ 0.f, 0.f, 0.f };
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_lastX{ 0.f }, m_lastY{ 0.f }, m_yaw{ 0.f }, m_pitch{ 0.f };
    bool m_isFirstMouseInput{ false };
};