#pragma once

#include "SFML/Window/Event.hpp"
#include "../../Utility/Maths.h"
#include "../../Utility/Matrix.h"

constexpr float CAMERA_FOV = 80.0f / 180.0f * PI;
constexpr float CAMERA_NEAR_PLANE = 0.1f;
constexpr float CAMERA_FAR_PLANE = 1000.0f;
constexpr float CAMERA_SPEED = 300.f;

class Camera
{
public:
    Camera(const Point3Df& initialPos = Point3Df(0.f, 200.f, 0.f)) :
		m_cameraPos(initialPos)
	{ }

    void moveCameraForInput(const sf::Event& inputEvent, float deltaTime);
    void rotateCameraForInput(const sf::Event& inputEvent, float windowWidth, float windowHeight);

public:
    Point3Df m_cameraPos{ 0.f, 0.f, 0.f };
    Point3Df m_cameraMovement{ 0.f, 0.f, 0.f };
    float m_cameraAlpha { 0.f };
    float m_cameraBeta { 0.f };
};