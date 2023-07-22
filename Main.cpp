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

#include "Model/Camera.hpp"

#include "Model/Camera/OrthoCamera.hpp"
#include "Model/Camera/PerspectiveCamera.hpp"

#include "Model/Light.hpp"

#include "Model/Light/DirectionLight.hpp"
#include "Model/Light/PointLight.hpp"

#include "Model/Shaders.hpp"

using namespace models;
using namespace cameras;
using namespace lights;


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
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    double currentFrame = glfwGetTime();
    double lastFrame = currentFrame;
    double deltaTime;

    Shaders* pShaders = new Shaders("Shaders/sample.vert", "Shaders/sample.frag");
    PerspectiveCamera* pPerspectiveCamera = new PerspectiveCamera(glm::vec3(0.f, 30.f, 100.f), glm::vec3(0.f, 3.0f, 0.f), glm::normalize(glm::vec3(0.f, 1.0f, 0.f)), 60.0f, 1000.0f);
    Model3D* pModel = new Model3D("3D/octopus_toy.obj", glm::vec3(0.0f, 0.f, 10.f), glm::vec3(5.0f));
    pModel->addTexture("3D/octopus_toy_texture.png");

    DirectionLight* pDirectionLight = new DirectionLight(glm::vec3(4, 11, -3), glm::vec3(1, 1, 1), 0.1f, glm::vec3(1, 1, 1), 0.5f, 16);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        pShaders->use();

        pShaders->setFloatVec3("cameraPos", pPerspectiveCamera->getPosition());
        pShaders->setFloatMat4("view", pPerspectiveCamera->getViewMatrix());
        pShaders->setFloatMat4("projection", pPerspectiveCamera->getProjection());

        pShaders->setFloatVec3("lightPos", pDirectionLight->getPosition());
        pShaders->setFloatVec3("lightColor", pDirectionLight->getColor());
        pShaders->setFloat("ambientStr", pDirectionLight->getAmbientStrength());
        pShaders->setFloatVec3("ambientColor", pDirectionLight->getAmbientColor());
        pShaders->setFloat("specStr", pDirectionLight->getSpecStrength());
        pShaders->setFloat("specPhong", pDirectionLight->getSpecPhong());

        pShaders->setFloatMat4("transform", pModel->getTransformation());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pModel->getTexture(0));
        pShaders->setInt("tex0", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pModel->getTexture(1));
        pShaders->setInt("norm_tex", 0);

        pShaders->setFloatVec3("objColor", pModel->getColor());

        pModel->draw(*pShaders);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}