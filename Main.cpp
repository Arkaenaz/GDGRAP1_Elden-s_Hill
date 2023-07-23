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

#include "Model/Tank/TankBody.hpp"
#include "Model/Tank/TankTurret.hpp"

#include "Model/Skybox.hpp"

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

double xoldpos = 0.f;
double yoldpos = 0.f;
double currentmousexpos = 0.f;
double currentmouseypos = 0.f;
float sensitivity = 0.f;

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    xoldpos = currentmousexpos;
    yoldpos = currentmouseypos;
    currentmousexpos = xpos;
    currentmouseypos = ypos;
    sensitivity = 150.0f;
}

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

    glfwSetCursorPosCallback(window, mouse_pos_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    double currentFrame = glfwGetTime();
    double lastFrame = currentFrame;
    double deltaTime;

    Shaders CShaders = Shaders("Shaders/sample.vert", "Shaders/sample.frag");
    Shaders CSkyboxShaders = Shaders("Shaders/skybox.vert", "Shaders/skybox.frag");

    Skybox* pSkybox = new Skybox("Skybox/px.png",
                                 "Skybox/nx.png",
                                 "Skybox/py.png",
                                 "Skybox/ny.png",
                                 "Skybox/pz.png",
                                 "Skybox/nz.png");

    PerspectiveCamera* pPerspectiveCamera = new PerspectiveCamera(glm::vec3(400.f, 400.f, 500.f), glm::vec3(0.f, 3.0f, 0.f), glm::normalize(glm::vec3(0.f, 1.0f, 0.f)), 60.0f, 1000.0f);
    TankBody *pTankBody = new TankBody("3D/T-34/T-34/T-34.obj", glm::vec3(0.0f, 0.f, 0.f), glm::vec3(.5f));
    TankTurret *pTankTurret = new TankTurret("3D/T-34/T-34/T-34.obj", glm::vec3(0.0f, 0.f, 0.f), glm::vec3(.5f));
    pTankBody->addTexture("3D/T-34/T-34/tex/T-34_Body.jpg");
    pTankTurret->addTexture("3D/T-34/T-34/tex/T-34_Body.jpg");

    pTankTurret->rotate(glm::vec3(0,90,0));

    DirectionLight* pDirectionLight = new DirectionLight(glm::vec3(4, 11, -3), glm::vec3(1, 1, 1), 1.f, glm::vec3(1, 1, 1), 0.5f, 16);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        float cam_y_mod = -(currentmousexpos - xoldpos) * sensitivity;
        float cam_x_mod = -(currentmouseypos - yoldpos) * sensitivity;
        float cam_z_mod = 0.f;

        pPerspectiveCamera->rotateAround(pTankBody->getPosition(), glm::vec3(cam_x_mod, cam_y_mod, cam_z_mod) * (float)deltaTime);

        sensitivity = 0.f;

        CSkyboxShaders.use();

        glm::mat4 skyView = glm::mat4(1.f);
        skyView = glm::mat4(glm::mat3(pPerspectiveCamera->getViewMatrix()));

        CSkyboxShaders.setFloatMat4("projection", pPerspectiveCamera->getProjection());
        CSkyboxShaders.setFloatMat4("view", skyView);

        pSkybox->draw(CSkyboxShaders);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        CShaders.use();

        CShaders.setFloatVec3("cameraPos", pPerspectiveCamera->getPosition());
        CShaders.setFloatMat4("view", pPerspectiveCamera->getViewMatrix());
        CShaders.setFloatMat4("projection", pPerspectiveCamera->getProjection());

        CShaders.setFloatVec3("lightPos", pDirectionLight->getPosition());
        CShaders.setFloatVec3("lightColor", pDirectionLight->getColor());
        CShaders.setFloat("ambientStr", pDirectionLight->getAmbientStrength());
        CShaders.setFloatVec3("ambientColor", pDirectionLight->getAmbientColor());
        CShaders.setFloat("specStr", pDirectionLight->getSpecStrength());
        CShaders.setFloat("specPhong", pDirectionLight->getSpecPhong());

        pTankBody->draw(CShaders);
        pTankTurret->draw(CShaders);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}