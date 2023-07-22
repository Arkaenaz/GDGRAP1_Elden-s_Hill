#include <iostream>
#include <string>
#include <cmath>

#include "Config/Settings.hpp"

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "Model/Model3D.hpp"

using namespace models;

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Elden's Hill", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    double currentFrame = glfwGetTime();
    double lastFrame = currentFrame;
    double deltaTime;

    glActiveTexture(GL_TEXTURE0);

    Model3D* pModel = new Model3D("3D/octopus_toy.obj", "3D/octopus_toy_texture.png", glm::vec3(0.0f), glm::vec3(1.0f));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glBegin(GL_POLYGON);
        for (float i = 0; i < 5; i++) {
            float angle1 = (0 + 72 * i) * 3.14 / 180;
            float angle2 = (72 + 72 * i) * 3.14 / 180;
            glVertex2f(0.3f * cos(angle1), 0.3f * sin(angle1));
        }
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}