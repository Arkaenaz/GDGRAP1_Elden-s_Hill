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
#include "Model/Tank/TankTracks.hpp"
#include "Model/TempModels/Octopus.hpp"

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

const float MAX_SPEED = 10.0f;

float x_mod = 0;
float y_mod = 0;
float z_mod = 0;

float ortho_x_mod = 0;
float ortho_y_mod = 0;

float zoom_mod = 0.f;

bool wPress;
bool sPress;
bool aPress;
bool dPress;
bool ePress;
bool qPress;
bool bCamera = true;
bool bZoom = false;

double xoldpos = 0.f;
double yoldpos = 0.f;
double currentmousexpos = 0.f;
double currentmouseypos = 0.f;
float sensitivity = 0.f;

void Key_Callback(
    GLFWwindow* window,
    int key,
    int scanCode,
    int action, // pressed / released
    int mod
) {
    if (action == GLFW_PRESS)
        switch (key) {
        case GLFW_KEY_W:
            wPress = true;
            sPress = false;
            dPress = false;
            aPress = false;
            break;
        case GLFW_KEY_A:
            aPress = true;
            dPress = false;
            sPress = false;
            wPress = false;
            break;
        case GLFW_KEY_S:
            sPress = true;
            wPress = false;
            dPress = false;
            aPress = false;
            break;
        case GLFW_KEY_D:
            dPress = true;
            aPress = false;
            wPress = false;
            sPress = false;
            break;
        case GLFW_KEY_E:
            ePress = true;
            break;
        case GLFW_KEY_Q:
            qPress = true;
            break;
        case GLFW_KEY_1:
            if (bCamera)
                bZoom = !bZoom;
            else
                bZoom = false;
            bCamera = true;
            
            break;

        case GLFW_KEY_2:
            bZoom = false;
            bCamera = false;
            break;
        }
    if (action == GLFW_RELEASE)
        switch (key) {
        case GLFW_KEY_W:
            wPress = false;
            break;
        case GLFW_KEY_A:
            aPress = false;
            break;
        case GLFW_KEY_S:
            sPress = false;
            break;
        case GLFW_KEY_D:
            dPress = false;
            break;
        case GLFW_KEY_E:
            ePress = false;
            break;
        case GLFW_KEY_Q:
            qPress = false;
            break;
        }
}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    xoldpos = currentmousexpos;
    yoldpos = currentmouseypos;
    currentmousexpos = xpos;
    currentmouseypos = ypos;
    sensitivity = 50.0f;
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

    glfwSetKeyCallback(window, Key_Callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    

    TankBody *pTankBody = new TankBody("3D/T-34/T-34/T-34.obj", glm::vec3(0.0f, 0.f, 0.f), glm::vec3(.5f));
    TankTurret *pTankTurret = new TankTurret("3D/T-34/T-34/T-34.obj", glm::vec3(0.0f, 0.f, 0.f), glm::vec3(.5f));
    TankTracks *pTankTracks = new TankTracks("3D/T-34/T-34/T-34.obj", glm::vec3(0.0f, 0.f, 0.f), glm::vec3(.5f));
    pTankBody->addTexture("3D/T-34/T-34/tex/T-34_Body.jpg");
    pTankBody->addTexture("3D/T-34/T-34/tex/T-34_Body_norm.jpg");
    pTankTurret->addTexture("3D/T-34/T-34/tex/T-34_Body.jpg");
    pTankTurret->addTexture("3D/T-34/T-34/tex/T-34_Body_norm.jpg");
    pTankTracks->addTexture("3D/T-34/T-34/tex/T-34_Tracks.jpg");
    pTankTracks->addTexture("3D/T-34/T-34/tex/T-34_Tracks_norm.jpg");
    
    PerspectiveCamera* pPerspectiveCamera = new PerspectiveCamera(glm::vec3(0.f,  105.f, -800.f), glm::vec3(0.f, 3.0f, 0.f), glm::normalize(glm::vec3(0.f, 1.0f, 0.f)), 60.0f, 2500.f);
    OrthoCamera* pOrthoCamera = new OrthoCamera(glm::vec3(0.f, -90.f, -1.f), glm::vec3(0.f, 3.0f, 0.f), glm::normalize(glm::vec3(0.f, 1.0f, 0.0f)), glm::vec3(-1920.0f, -1080.0f, -1000.f), glm::vec3(1920.0f, 1080.0f, 1000.f));
    Camera* pCurrentCamera = pPerspectiveCamera;

    DirectionLight* pMoonLight = new DirectionLight(glm::vec3(4, 500, -3), glm::vec3(80.f / 255.f, 104.f / 255.f, 134.f / 255.f), 1.f, glm::vec3(1, 1, 1), 0.5f, 16);

    Model3D* pOctopus = new Octopus("3D/octopus_toy.obj", glm::vec3(0.0f, -300.f, 0.f), glm::vec3(100.f));
    pOctopus->addTexture("3D/octopus_toy_texture.png");




    /*float cam_y_mod;
    float cam_x_mod;
    float cam_z_mod = 0.f;*/

    TankTurret trueTurretRotation = TankTurret("3D/T-34/T-34/T-34.obj", glm::vec3(0.0f, 0.f, 0.f), glm::vec3(.5f));

    float yaw_mod = 0.f;
    float pitch_mod = 0.f;


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

        if(wPress && x_mod + 0.3f * (float)deltaTime < MAX_SPEED) {
            x_mod += 0.3f * (float)deltaTime;
            
            ortho_y_mod += BASE_SPEED * (float)deltaTime;
 
        }
        else if(!wPress && x_mod > 0){
            x_mod -= 1.f * (float)deltaTime;
        }

        if(sPress && x_mod - 0.3f * (float)deltaTime > -MAX_SPEED) {
            x_mod -= 0.2f * (float)deltaTime;
            ortho_y_mod -= BASE_SPEED * (float)deltaTime;
        }
        else if(!sPress && x_mod < 0) {
            x_mod += 1.f * (float)deltaTime;
        }

        if(aPress && y_mod + 0.1f * (float)deltaTime < MAX_SPEED) {
            y_mod += 0.1f * (float)deltaTime;
            ortho_x_mod -= BASE_SPEED * (float)deltaTime;
        }
        else if(!aPress && y_mod > 0) {
            y_mod -= 0.6f * (float)deltaTime;
        }

        if(dPress && y_mod - 0.1f * (float)deltaTime > -MAX_SPEED) {
            y_mod -= 0.1f * (float)deltaTime;
            ortho_x_mod += BASE_SPEED * (float)deltaTime;
        }
        else if(!dPress && y_mod < 0) {
            y_mod += 0.6f * (float)deltaTime;
        }

        if (bCamera)
            pCurrentCamera = pPerspectiveCamera;
        else pCurrentCamera = pOrthoCamera;
        // temp stuff
        if (pCurrentCamera == pPerspectiveCamera) {
            if (!bZoom) {
                ortho_x_mod = 0.f;
                ortho_y_mod = 0.f;
                pPerspectiveCamera->setFOV(60.f);
                pPerspectiveCamera->setZoom(-750.f);
                if (pPerspectiveCamera->getPitch() >= -45.0f && pPerspectiveCamera->getPitch() <= -10.0f) {
                    //pPerspectiveCamera->rotateAround(pTankBody->getPosition(), glm::vec3(cam_x_mod, cam_y_mod, 0.f) * (float)deltaTime);
                    trueTurretRotation.rotate(glm::vec3(0.f, cam_y_mod, 0.f) * (float)deltaTime);
                }
                //std::cout << pPerspectiveCamera->getYaw();
                pPerspectiveCamera->addPitch(cam_x_mod * (float)deltaTime);
                pPerspectiveCamera->addYaw(cam_y_mod * (float)deltaTime);   
                if (pPerspectiveCamera->getPitch() <= -45.0f)
                    pPerspectiveCamera->setPitch(-45.0f);
                if (pPerspectiveCamera->getPitch() >= -10.0f)
                    pPerspectiveCamera->setPitch(-10.0f);
                pPerspectiveCamera->setCenter(pTankBody->getPosition());
                pPerspectiveCamera->updateTP(pTankBody->getRotationAngles(), pTankBody->getPosition());
                pTankBody->move(glm::vec3(x_mod * -pTankBody->getTransformation()[0][2] / 0.5, 0, x_mod * pTankBody->getTransformation()[0][0] / 0.5));
                pTankTurret->setPosition(pTankBody->getPosition());
                pTankTracks->setPosition(pTankBody->getPosition());

                pTankTurret->rotateTurret(trueTurretRotation, deltaTime);
                pTankBody->rotate(glm::vec3(0, y_mod, 0));
                pTankTracks->rotate(glm::vec3(0, y_mod, 0));
            }
            else {
                if (qPress) {
                    if (zoom_mod < 0.f)
                        zoom_mod = 0.f;
                    else
                        zoom_mod -= 500.f * (float)deltaTime;
                }
                if (ePress) {
                    zoom_mod += 500.f * (float)deltaTime;
                }
                if (wPress) {
                    pitch_mod += BASE_SPEED * (float)deltaTime;
                    if (pitch_mod > 89.0f)
                        pitch_mod = 89.f;
                }

                if (sPress) {
                    pitch_mod -= BASE_SPEED * (float)deltaTime;
                    if (pitch_mod < -10.f)
                        pitch_mod = -10.f;
                }

                if (aPress) {
                    yaw_mod -= BASE_SPEED * (float)deltaTime;
                }

                if (dPress) {
                    yaw_mod += BASE_SPEED * (float)deltaTime;
                }
                pPerspectiveCamera->setZoom(50.f + zoom_mod);
                pPerspectiveCamera->setFOV(90.f);
                pPerspectiveCamera->updateFP(pTankBody->getRotationAngles(), glm::vec3(pTankBody->getPosition().x, pTankBody->getPosition().y + 175.f, pTankBody->getPosition().z));
                pPerspectiveCamera->setPitch(pitch_mod);
                pPerspectiveCamera->setYaw(yaw_mod);
                
            }
        }

        if (wPress) {
            ortho_y_mod += BASE_SPEED * (float)deltaTime;
        }

        if (sPress) {
            ortho_y_mod -= BASE_SPEED * (float)deltaTime;
        }

        if (aPress) {
            ortho_x_mod -= BASE_SPEED * (float)deltaTime;
        }

        if (dPress) {
            ortho_x_mod += BASE_SPEED * (float)deltaTime;
        }
        pOrthoCamera->setPosition(glm::vec3(pTankBody->getPosition().x, 90.f, pTankBody->getPosition().z - 1.f));
        pOrthoCamera->setCenter(pTankBody->getPosition());
        if (pCurrentCamera == pOrthoCamera) {
            pOrthoCamera->setPosition(glm::vec3(pTankBody->getPosition().x - ortho_x_mod, 90.f, pTankBody->getPosition().z - 1.f + ortho_y_mod));
            pOrthoCamera->setCenter(glm::vec3(pTankBody->getPosition().x - ortho_x_mod, pTankBody->getPosition().y, pTankBody->getPosition().z + ortho_y_mod));
        }

        sensitivity = 0.f;

        CSkyboxShaders.use();
        if (bZoom)
            CSkyboxShaders.setFloatVec3("viewColor", glm::vec3(0.f, 0.2f, 0.f));
        else
            CSkyboxShaders.setFloatVec3("viewColor", glm::vec3(0.f));
        glm::mat4 skyView = glm::mat4(1.f);
        skyView = glm::mat4(glm::mat3(pPerspectiveCamera->getViewMatrix()));

        CSkyboxShaders.setFloatMat4("projection", pCurrentCamera->getProjection());
        CSkyboxShaders.setFloatMat4("view", skyView);

        pSkybox->draw(CSkyboxShaders);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        CShaders.use();
        CShaders.setFloatVec3("cameraPos", pCurrentCamera->getPosition());
        CShaders.setFloatMat4("view", pCurrentCamera->getViewMatrix());
        CShaders.setFloatMat4("projection", pCurrentCamera->getProjection());

        CShaders.setFloatVec3("lightPos", pMoonLight->getPosition());
        CShaders.setFloatVec3("lightColor", pMoonLight->getColor());
        CShaders.setFloat("ambientStr", pMoonLight->getAmbientStrength());
        CShaders.setFloatVec3("ambientColor", pMoonLight->getAmbientColor());
        CShaders.setFloat("specStr", pMoonLight->getSpecStrength());
        CShaders.setFloat("specPhong", pMoonLight->getSpecPhong());
        CShaders.setFloat("intensity", 0.1f);


        if (bZoom) {
            CShaders.setFloatVec3("viewColor", glm::vec3(0.f, 0.2f, 0.f));
            CShaders.setFloat("intensity", 1.0f);
        }
        else {
            CShaders.setFloatVec3("viewColor", glm::vec3(0.f));
        }
        pTankBody->draw(CShaders);
        pTankTurret->draw(CShaders);
        pTankTracks->draw(CShaders);
        pOctopus->draw(CShaders);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}