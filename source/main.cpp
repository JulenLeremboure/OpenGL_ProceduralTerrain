#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Utility/Matrix.h"
#include "Rendering/Texture.h"

#include "SceneObjects/Camera/Camera.h"
#include "Utility/Maths.h"
#include "ProceduralGeneration/HeightMap/HeightMap.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"

constexpr int WINDOW_BASE_WIDTH = 800;
constexpr int WINDOW_BASE_HEIGHT = 600;

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
    Camera camera(glWindow, glm::vec3(0.f, 50.f, 0.f));
    HeightMap heightMap;

    float lastFrame = 0; // Time of last frame

    // ---- INIT IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glWindow, true);
	ImGui_ImplOpenGL3_Init("version 110");

    // ---- GAME LOOP
    while (!glfwWindowShouldClose(glWindow))
    {
        const auto currentFrameTime = static_cast<float>(glfwGetTime());
        const float deltaTime = currentFrameTime - lastFrame;
        lastFrame = currentFrameTime;

        glfwGetFramebufferSize(glWindow, &windowWidth, &windowHeight);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ---- EVENTS
        if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(glWindow, GLFW_TRUE);
        }
        else if (glfwGetKey(glWindow, GLFW_KEY_R) == GLFW_PRESS)
        {
            heightMap.load(10);
        }

        camera.moveCameraForInput(glWindow, deltaTime);
        camera.rotateCameraForInput(s_mousePosX, s_mousePosY);

        // ---- DRAWINGS

        // effacement les tampons de couleur/profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto matView = glm::lookAt(camera.m_cameraPos, camera.m_cameraPos + camera.m_cameraFront, camera.m_cameraUp);

        const auto matProj = glm::perspective(CAMERA_FOV, WINDOW_ASPECT_RATIO, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);
        const auto matViewByProj = matProj * matView;

        heightMap.render(matViewByProj);

        glFlush();

        ImGui::Begin("Generation terrain");
        ImGui::Text("Test");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glWindow);
        glfwPollEvents();
    }

    // lib?ration des ressources...
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(glWindow);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

void mouseMovedCallback(GLFWwindow* window, double xPos, double yPos)
{
    s_mousePosX = static_cast<float>(xPos);
    s_mousePosY = static_cast<float>(yPos);
}