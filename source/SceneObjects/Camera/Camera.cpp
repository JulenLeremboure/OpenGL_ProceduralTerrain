#include "Camera.h"

#include <iostream>

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(GLFWwindow* window, const glm::vec3& initialPos)
	:m_cameraPos(initialPos)
{ }

void Camera::moveCameraForInput(GLFWwindow* window, float deltaTime)
{
    glm::vec3 offsetVecResult{ 0.f, 0.f, 0.f };

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        offsetVecResult += m_cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        offsetVecResult -= m_cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        offsetVecResult -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp));

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        offsetVecResult += glm::normalize(glm::cross(m_cameraFront, m_cameraUp));

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        offsetVecResult.y += 1.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        offsetVecResult.y -= 1.0f;

    m_cameraPos += offsetVecResult * CAMERA_SPEED * deltaTime;
}

void Camera::rotateCameraForInput(float xPos, float yPos)
{
    if (m_isFirstMouseInput)
    {
        m_lastX = xPos;
        m_lastY = yPos;
        m_isFirstMouseInput = false;
    }

    const float xOffset = xPos - m_lastX;
    const float yOffset = m_lastY - yPos;
    m_lastX = xPos;
    m_lastY = yPos;

    m_yaw += xOffset * CAMERA_SENSITIVITY;
    m_pitch += yOffset * CAMERA_SENSITIVITY;

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
