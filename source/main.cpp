#include <gl/glew.h>

#include "SFML/Graphics.hpp"

#include "Utility/Matrix.h"
#include "Rendering/Texture.h"

#include "SceneObjects/Camera/Camera.h"
#include "Utility/Maths.h"
#include "ProceduralGeneration/HeightMap/HeightMap.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"

constexpr float WINDOW_BASE_WIDTH = 800.f;
constexpr float WINDOW_BASE_HEIGHT = 600.f;

constexpr float WINDOW_ASPECT_RATIO = WINDOW_BASE_WIDTH / WINDOW_BASE_HEIGHT;

int main()
{
    // Set version of opengl to 4.6
    const sf::ContextSettings contextSettings(24, 8, 4, 4, 6);

    sf::Window window(sf::VideoMode(WINDOW_BASE_WIDTH, WINDOW_BASE_HEIGHT), "OpenGL", sf::Style::Default, contextSettings);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    glewExperimental = GL_TRUE;
    if (glewInit())
        throw std::runtime_error("Error init glew");

    glClearColor(GameColors::sky.r, GameColors::sky.g, GameColors::sky.b, GameColors::sky.a);

    // ---- INIT RESOURCES
    Camera camera;
    HeightMap heightMap;
    sf::Clock dtClock;

    // ---- GAME LOOP
    bool isProgramRunning = true;

    while (isProgramRunning)
    {
        float deltaTime = dtClock.restart().asSeconds();

        // ---- EVENTS
        sf::Event sfmlEvent;
        while (window.pollEvent(sfmlEvent))
        {
            if (sfmlEvent.type == sf::Event::Closed
             || sfmlEvent.type == sf::Event::KeyPressed && sfmlEvent.key.code == sf::Keyboard::Escape)
            {
                isProgramRunning = false;
            }
            else if (sfmlEvent.type == sf::Event::Resized)
            {
                glViewport(0, 0, sfmlEvent.size.width, sfmlEvent.size.height);
            }
            else if (sfmlEvent.type == sf::Event::MouseMoved)
            {
                sf::Mouse::setPosition(sf::Vector2i(WINDOW_BASE_WIDTH / 2.f, WINDOW_BASE_HEIGHT / 2.f), window);
            }
            else if (sfmlEvent.type == sf::Event::KeyPressed && sfmlEvent.key.code == sf::Keyboard::A)
            {
                heightMap.load(10);
            }

            camera.moveCameraForInput(sfmlEvent, deltaTime);
            camera.rotateCameraForInput(sfmlEvent, WINDOW_BASE_WIDTH, WINDOW_BASE_HEIGHT);
        }

        // ---- DRAWINGS

        // effacement les tampons de couleur/profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto matView = glm::lookAt(camera.m_cameraPos, camera.m_cameraPos + camera.m_cameraFront, camera.m_cameraUp);

        const auto matProj = glm::perspective(CAMERA_FOV, WINDOW_ASPECT_RATIO, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);
        const auto matViewByProj = matProj * matView;

        heightMap.render(matViewByProj);

        glFlush();

        window.display();
    }

    // lib�ration des ressources...

    return 0;
}