#include "Camera.h"

#include "SFML/Window/Event.hpp"

void Camera::moveCameraForInput(const sf::Event& inputEvent, float deltaTime)
{
    if (inputEvent.type == sf::Event::KeyPressed)
    {
        switch (inputEvent.key.code)
        {
	        case sf::Keyboard::Z:
	            m_cameraPos.z -= CAMERA_SPEED * deltaTime;
	            break;
	        case sf::Keyboard::S:
	            m_cameraPos.z += CAMERA_SPEED * deltaTime;
	            break;
	        case sf::Keyboard::Q:
	            m_cameraPos.x -= CAMERA_SPEED * deltaTime;
	            break;
	        case sf::Keyboard::D:
	            m_cameraPos.x += CAMERA_SPEED * deltaTime;
	            break;
	        case sf::Keyboard::Space:
	            m_cameraPos.y += CAMERA_SPEED * deltaTime;
	            break;
	        case sf::Keyboard::W: // TODO : Understand why LShift or LControl doesn't work
	            m_cameraPos.y -= CAMERA_SPEED * deltaTime;
	            break;
        }
    }
}

void Camera::rotateCameraForInput(const sf::Event& inputEvent, float windowWidth, float windowHeight)
{
    if (inputEvent.type == sf::Event::MouseMoved)
    {
        m_cameraAlpha += (inputEvent.mouseMove.x - windowWidth / 2.f) * -0.001f;
        m_cameraBeta += (inputEvent.mouseMove.y - windowHeight / 2.f) * 0.001f;
    }
}
