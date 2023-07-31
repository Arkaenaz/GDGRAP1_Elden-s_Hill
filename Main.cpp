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

#include "Model/Plane/Plane.hpp"

#include "Model/Plane/NoNormal.hpp"

#include "Model/Camera/OrthoCamera.hpp"
#include "Model/Camera/PerspectiveCamera.hpp"

#include "Model/Light.hpp"

#include "Model/Light/DirectionLight.hpp"
#include "Model/Light/PointLight.hpp"

#include "Model/Shaders.hpp"

using namespace models;
using namespace cameras;
using namespace lights;

const float MAX_SPEED = .5f;

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

int intensityState = 0;

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
        case GLFW_KEY_F:
            intensityState += 1;
            intensityState = intensityState % 3;
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

    Skybox* pSkybox = new Skybox("Skybox/night_right.png",
                                 "Skybox/night_left.png",
                                 "Skybox/night_up.png",
                                 "Skybox/night_down.png",
                                 "Skybox/night_front.png",
                                 "Skybox/night_back.png");
    

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
    
    PointLight* pPointLight = new PointLight(glm::vec3(pTankBody->getPosition().x, pTankBody->getPosition().y + 30.f, pTankBody->getPosition().z + 150.f), glm::vec3(1, 1, 1), 0.1f, glm::vec3(1, 1, 1), 0.5f, 16);
    DirectionLight* pMoonLight = new DirectionLight(glm::vec3(0, -1000, 500), glm::vec3(80.f / 255.f, 104.f / 255.f, 134.f / 255.f), 0.5f, glm::vec3(80.f / 255.f, 104.f / 255.f, 134.f / 255.f), 3.f, 16);

    Model3D* pOctopus = new Octopus("3D/octopus_toy.obj", glm::vec3(100.0f, 0.f, 0.f), glm::vec3(50.f));
    pOctopus->addTexture("3D/octopus_toy_texture.png");

    Model3D *pPlane = new Plane("3D/plane.obj", glm::vec3(0.0f, 0.f, 0.f), glm::vec3(100.f));
    pPlane->rotate(glm::vec3(90, 0, 0));
    //pPlane->scale(glm::vec3(10,10,10));
    pPlane->addTexture("3D/grass.jpg");

    Model3D *Fridge = new Plane("3D/POIs/Fridge/Fridge.obj", glm::vec3(0.0f, 0.f, 0.f), glm::vec3(100.f));
    Fridge->addTexture("3D/POIs/Fridge/Fridge.png");

    Model3D *Millie = new Plane("3D/POIs/Millie/Millie.obj", glm::vec3(400.0f, 0.f, 0.f), glm::vec3(100.f));
    Millie->scale(glm::vec3(-99.5, -99.5, -99.5));
    Millie->rotate(glm::vec3(0, 45, 0));
    Millie->addTexture("3D/POIs/Millie/Millie.png");

    Model3D *Tree = new Plane("3D/POIs/Tree/Tree.obj", glm::vec3(0.0f, -300.f, 600.f), glm::vec3(100.f));
    Tree->scale(glm::vec3(500, 500, 500));
    Tree->addTexture("3D/POIs/Tree/Tree.png");

    Model3D *Titan = new Plane("3D/POIs/Titan/Titan.obj", glm::vec3(-500.0f, 0.f, 500.f), glm::vec3(100.f));
    Titan->scale(glm::vec3(-99.5, -99.5, -99.5));
    Titan->addTexture("3D/POIs/Titan/Titan.png");

    Model3D *Makima = new NoNormal("3D/POIs/Makima/Makima.obj", glm::vec3(-500.0f, 0.f, 700.f), glm::vec3(100.f));
    Makima->addTexture("3D/POIs/Makima/Makima.png");

    Model3D *Noob = new Plane("3D/POIs/Noob/Noob.obj", glm::vec3(800.0f, 200.f, 300.f), glm::vec3(100.f));
    Noob->scale(glm::vec3(-70, -70, -70));
    Noob->addTexture("3D/POIs/Noob/Noob.png");
    

    /*float cam_y_mod;
    float cam_x_mod;
    float cam_z_mod = 0.f;*/

    TankTurret trueTurretRotation = TankTurret("3D/T-34/T-34/T-34.obj", glm::vec3(0.0f, 0.f, 0.f), glm::vec3(.5f));
    trueTurretRotation.rotate(glm::vec3(0.f, 180.f, 0.f));
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

        std::cout << x_mod << std::endl;
        
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
                pPerspectiveCamera->setFar(2500.f);
                if (pPerspectiveCamera->getPitch() >= -45.0f && pPerspectiveCamera->getPitch() <= -10.0f) {
                    //pPerspectiveCamera->rotateAround(pTankBody->getPosition(), glm::vec3(cam_x_mod, cam_y_mod, 0.f) * (float)deltaTime);
                    trueTurretRotation.rotate(glm::vec3(0.f, cam_y_mod, 0.f) * (float)deltaTime);
                }
                //std::cout << pPerspectiveCamera->getYaw();
                pitch_mod += cam_x_mod * (float)deltaTime;
                yaw_mod += cam_y_mod * (float)deltaTime;
                pPerspectiveCamera->setPitch(pitch_mod);
                pPerspectiveCamera->setYaw(yaw_mod);
                if (pPerspectiveCamera->getPitch() <= -45.0f) {
                    pitch_mod = -44.9f;
                    pPerspectiveCamera->setPitch(-45.f);
                }
                if (pPerspectiveCamera->getPitch() >= -10.0f) {
                    pitch_mod = -10.1f;
                    pPerspectiveCamera->setPitch(-10.f);
                }
                pPerspectiveCamera->setCenter(pTankBody->getPosition());
                pPerspectiveCamera->updateTP(pTankBody->getRotationAngles(), pTankBody->getPosition());
                pPointLight->move(glm::vec3(x_mod * -pTankBody->getTransformation()[0][2] / 0.5, 0, x_mod * pTankBody->getTransformation()[0][0] / 0.5));
                pTankBody->move(glm::vec3(x_mod * -pTankBody->getTransformation()[0][2] / 0.5, 0, x_mod * pTankBody->getTransformation()[0][0] / 0.5));
                pTankTurret->setPosition(pTankBody->getPosition());
                pTankTracks->setPosition(pTankBody->getPosition());

                pTankTurret->rotateTurret(trueTurretRotation, deltaTime);

               // pPointLight->setPosition(glm::vec3(pTankBody->getTransformation()[0][3], pTankBody->getTransformation()[1][3], pTankBody->getTransformation()[2][3]));
                pPointLight->rotateAround(pTankBody->getPosition(), glm::vec3(0, y_mod, 0));
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
                    if (zoom_mod > 1850.f)
                        zoom_mod = 1850.f;
                    else
                        zoom_mod += 500.f * (float)deltaTime;
                }
                if (wPress) {
                    pitch_mod += BASE_SPEED * (float)deltaTime;
                    
                }

                if (sPress) {
                    pitch_mod -= BASE_SPEED * (float)deltaTime;
                    
                }

                if (aPress) {
                    yaw_mod -= BASE_SPEED * (float)deltaTime;
                }

                if (dPress) {
                    yaw_mod += BASE_SPEED * (float)deltaTime;
                }
                if (pitch_mod > 45.0f)
                    pitch_mod = 45.f;
                if (pitch_mod < -5.f)
                    pitch_mod = -5.f;
                pPerspectiveCamera->setFar(5000.f);
                pPerspectiveCamera->setZoom(50.f + zoom_mod);
                pPerspectiveCamera->setFOV(90.f);
                pPerspectiveCamera->updateFP(pTankBody->getRotationAngles(), glm::vec3(pTankBody->getPosition().x, pTankBody->getPosition().y + 175.f, pTankBody->getPosition().z));
                pPerspectiveCamera->setPitch(pitch_mod);
                pPerspectiveCamera->setYaw(yaw_mod);
                
            }
        }
        //pPointLight->setPosition(glm::vec3(pTankBody->getPosition().x, pTankBody->getPosition().y + 30.f, pTankBody->getPosition().z + 100.f));
        //pPointLight->rotateAround(pTankBody->getPosition(), pTankBody->getRotationAngles());
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
        std::cout << "Intensity State :" << intensityState << std::endl;
        if (intensityState == 0)
            pPointLight->setIntensity(LOW);
        else if (intensityState == 1)
            pPointLight->setIntensity(MEDIUM);
        else if (intensityState == 2)
            pPointLight->setIntensity(HIGH);
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

        CShaders.setFloatVec3("pointLightPos", pPointLight->getPosition());
        CShaders.setFloatVec3("pointLightPosLightColor", pPointLight->getColor());
        CShaders.setFloat("pointAmbientStr", pPointLight->getAmbientStrength());
        CShaders.setFloatVec3("pointAmbientColor", pPointLight->getAmbientColor());
        CShaders.setFloat("pointSpecStr", pPointLight->getSpecStrength());
        CShaders.setFloat("pointSpecPhong", pPointLight->getSpecPhong());
        CShaders.setFloat("pointIntensity", pPointLight->getIntensity());

        CShaders.setFloatVec3("dirLightPos", pMoonLight->getPosition());
        CShaders.setFloatVec3("dirLightColor", pMoonLight->getColor());
        CShaders.setFloat("dirAmbientStr", pMoonLight->getAmbientStrength());
        CShaders.setFloatVec3("dirAmbientColor", pMoonLight->getAmbientColor());
        CShaders.setFloat("dirSpecStr", pMoonLight->getSpecStrength());
        CShaders.setFloat("dirSpecPhong", pMoonLight->getSpecPhong());
        CShaders.setFloat("dirIntensity", pMoonLight->getIntensity());


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
        //pOctopus->draw(CShaders);
        pPlane->draw(CShaders);
        Fridge->draw(CShaders);
        Millie->draw(CShaders);
        Tree->draw(CShaders);
        Titan->draw(CShaders);
        Makima->draw(CShaders);
        Noob->draw(CShaders);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}