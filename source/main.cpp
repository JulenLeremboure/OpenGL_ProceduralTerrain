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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(glWindow, mouseMovedCallback);

    // ---- INIT RESOURCES
    srand(time(NULL));
    Camera camera(glWindow, glm::vec3(0.f, 50.f, 0.f));
    HeightMap heightMap;

    // for fps
    int mFrame = 0;
    int mFps = 0;
    

    float lastFrame = 0; // Time of last frame

    // ---- INIT IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glWindow, true);
	ImGui_ImplOpenGL3_Init("#version 460");

    // ---- Test ImGui
    float igFloat = 1.5f;
    int igInt = 50;
    const char* igItems[]
    {
	    "Algo1",
	    "Algo2",
	    "Algo3",
	    "Algo4"
    };
    int igSelected = 0;

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
        

        ImGui::Begin("Terrain generation settings");

        ImGui::Text("General:");
        ImGui::SliderFloat("Frequency", &igFloat, 0.f, 2.f);
        ImGui::SliderInt("Seed", &igInt, 0, 100);
        ImGui::Text("Fractal:");
        ImGui::SliderFloat("Octaves", &igFloat, 0.f, 2.f);
        ImGui::SliderFloat("Lacunarity", &igFloat, 0.f, 2.f);
        ImGui::SliderFloat("Gain", &igFloat, 0.f, 2.f);
        ImGui::Text("Algo:");
        ImGui::ListBox("Algo", &igSelected, igItems, 4);

        
        if (ImGui::Button("Reload"))
        {
	        // DoFunction();
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glWindow);
        glfwPollEvents();
    }

    // liberation des ressources...
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