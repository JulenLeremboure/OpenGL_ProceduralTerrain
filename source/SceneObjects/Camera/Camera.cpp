#include "Camera.h"

#include <iostream>

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"


void Camera::moveCameraForInput(const sf::Event& inputEvent, float deltaTime)
{
    if (inputEvent.type == sf::Event::KeyPressed)
    {
		Point3Df offsetVecResult{0.f, 0.f, 0.f};

        switch (inputEvent.key.code)
        {
	        case sf::Keyboard::Z:
				offsetVecResult.z -= 1.0f;
	            break;
	        case sf::Keyboard::S:
				offsetVecResult.z += 1.0f;
	            break;
	        case sf::Keyboard::Q:
				offsetVecResult.x -= 1.0f;
	            break;
	        case sf::Keyboard::D:
				offsetVecResult.x += 1.0f;
	            break;
	        case sf::Keyboard::Space:
				offsetVecResult.y += 1.0f;
	            break;
	        case sf::Keyboard::LControl:
				offsetVecResult.y -= 1.0f;
	            break;
        }

		//if (offsetVecResult == Point3Df(0.f, 0.f, 0.f))
		//	return;

		//m_cameraMovement =  offsetVecResult * CAMERA_SPEED * deltaTime;

		Mat4<float> matTranslationByRot = Mat4<float>::rotationX(-m_cameraBeta) *
							 Mat4<float>::rotationY(-m_cameraAlpha) * 
							Mat4<float>::translation(offsetVecResult * CAMERA_SPEED * deltaTime);

		std::cout << "x: " << m_cameraBeta << " | y: " << m_cameraAlpha << std::endl;

		Point3Df offset = { matTranslationByRot(0, 3),
							matTranslationByRot(1, 3),
							matTranslationByRot(2, 3) };

		m_cameraPos = m_cameraPos + offset;
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
