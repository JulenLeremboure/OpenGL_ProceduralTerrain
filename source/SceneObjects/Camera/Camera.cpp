#include "Camera.h"

#include <iostream>

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"


void Camera::moveCameraForInput(const float deltaTime)
{
	glm::vec3 offsetVecResult{0.f, 0.f, 0.f};

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	    offsetVecResult += m_cameraFront;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		offsetVecResult -= m_cameraFront;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		offsetVecResult -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        offsetVecResult += glm::normalize(glm::cross(m_cameraFront, m_cameraUp));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        offsetVecResult.y += 1.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		offsetVecResult.y -= 1.0f;

	m_cameraPos += offsetVecResult * CAMERA_SPEED * deltaTime;
}

void Camera::rotateCameraForInput(const sf::Event& inputEvent, float windowWidth, float windowHeight)
{
    if (inputEvent.type == sf::Event::MouseMoved)
    {
        const float xPos = (inputEvent.mouseMove.x - windowWidth / 2.f);
        const float yPos = -(inputEvent.mouseMove.y - windowHeight / 2.f);

        m_yaw += xPos * CAMERA_SENSITIVITY;
        m_pitch += yPos * CAMERA_SENSITIVITY;

        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        direction.y = sin(glm::radians(m_pitch));
        direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_cameraFront = glm::normalize(direction);
    }
}
