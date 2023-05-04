#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Utility/Matrix.h"
#include "Rendering/Texture.h"

#include "SceneObjects/Camera/Camera.h"
#include "Utility/Maths.h"
#include "ProceduralGeneration/HeightMap/HeightMap.h"
#include <iostream>

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"

constexpr float WINDOW_BASE_WIDTH = 1600.f;
constexpr float WINDOW_BASE_HEIGHT = 800.f;
constexpr float WINDOW_ASPECT_RATIO = WINDOW_BASE_WIDTH / WINDOW_BASE_HEIGHT;

void mouseMovedCallback(GLFWwindow* window, double xPos, double yPos);

static float s_mousePosX{ 0.f }, s_mousePosY { 0.f };

int main()
{
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //const sf::ContextSettings contextSettings(24, 8, 4, 4, 6);
    //window.setVerticalSyncEnabled(true);

    GLFWwindow* glWindow = glfwCreateWindow(WINDOW_BASE_WIDTH, WINDOW_BASE_HEIGHT, "Insane procedural terrain", nullptr, nullptr);
    if (!glWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	int windowWidth = WINDOW_BASE_WIDTH;
	int windowHeight = WINDOW_BASE_HEIGHT;

    glfwMakeContextCurrent(glWindow);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit())
        throw std::runtime_error("Error init glew");

    glClearColor(GameColors::sky.r, GameColors::sky.g, GameColors::sky.b, GameColors::sky.a);
    glViewport(0, 0, WINDOW_BASE_WIDTH, WINDOW_BASE_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(glWindow, mouseMovedCallback);

    // ---- INIT RESOURCES
    srand(time(NULL));
    sf::Clock dtClock;
    sf::Clock fpsClock;
    Camera camera(glWindow, glm::vec3(0.f, 50.f, 0.f));
    HeightMap heightMap;

    // for fps
    int mFrame = 0;
    int mFps = 0;
    

    float lastFrame = 0; // Time of last frame

    // ---- GAME LOOP
    while (!glfwWindowShouldClose(glWindow))
    {
        const auto currentFrameTime = static_cast<float>(glfwGetTime());
        const float deltaTime = currentFrameTime - lastFrame;
        lastFrame = currentFrameTime;

        glfwGetFramebufferSize(glWindow, &windowWidth, &windowHeight);

        // ---- EVENTS
        if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(glWindow, GLFW_TRUE);
        }
        else if (glfwGetKey(glWindow, GLFW_KEY_R) == GLFW_PRESS)
        {
            heightMap.clear();
            heightMap.load(rand());
        }

        camera.moveCameraForInput(glWindow, deltaTime);
        camera.rotateCameraForInput(s_mousePosX, s_mousePosY);

        
        //window.clear();
        // ---- DRAWINGS
        // effacement les tampons de couleur/profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        heightMap.render(camera, WINDOW_ASPECT_RATIO);

        glFlush();
        

        // show fps
        if (fpsClock.getElapsedTime().asSeconds() >= 1.f)
        {
            mFps = mFrame;
            mFrame = 0;
            fpsClock.restart();
        }

        ++mFrame;

        std::ostringstream ss;
        ss << mFps;

        window.setTitle(ss.str());

        glfwSwapBuffers(glWindow);
        glfwPollEvents();
    }

    // liberation des ressources...
    glfwDestroyWindow(glWindow);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

void mouseMovedCallback(GLFWwindow* window, double xPos, double yPos)
{
    s_mousePosX = static_cast<float>(xPos);
    s_mousePosY = static_cast<float>(yPos);
}