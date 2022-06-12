#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "shader.hpp"
#include "stb_image.hpp"
#include "illixr_loader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* sceneWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Scene View", NULL, NULL);
    GLFWwindow* headsetWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Headset View", NULL, NULL);
    Camera sceneCamera(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), 225.0f, -30.0f);

    Camera headsetCamera(glm::vec3(0.0f, 1.0f, 0.5f));
    if (sceneWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(sceneWindow, framebuffer_size_callback);
    glfwSetFramebufferSizeCallback(headsetWindow, framebuffer_size_callback);

    //attach camera to window
    assocWindowAndCamera(sceneWindow,&sceneCamera);
    assocWindowAndCamera(headsetWindow,&headsetCamera);
    setMouseCallback(sceneWindow, CAMERA_CLICK_DRAG);
    setMouseCallback(headsetWindow, CAMERA_CLICK_DRAG);
    glfwSetScrollCallback(sceneWindow, scroll_callback);
    glfwSetScrollCallback(headsetWindow, scroll_callback);

    glewExperimental = true;

    glfwMakeContextCurrent(sceneWindow);
    glewInit();

    GLuint sceneVAO;
    glGenVertexArrays(1, &sceneVAO);
    glBindVertexArray(sceneVAO);
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glfwSwapInterval(1);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    Shader sceneShader("illixr.vs", "illixr.fs");
    // load models
    // -----------
    ILLIXR::ObjScene scene_sceneObj("object_data/","scene.obj");
    ILLIXR::ObjScene scene_headsetObj("object_data/","headset.obj");

    glfwMakeContextCurrent(headsetWindow);
    glewInit();

    GLuint headsetVAO;
    glGenVertexArrays(1, &headsetVAO);
    glBindVertexArray(headsetVAO);
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glfwSwapInterval(1);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    Shader headsetShader("illixr.vs", "illixr.fs");
    // load models
    // -----------
    ILLIXR::ObjScene headset_sceneObj("object_data/","scene.obj");
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(sceneWindow) && !glfwWindowShouldClose(headsetWindow))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(sceneWindow);
        processInput(headsetWindow);

        glfwMakeContextCurrent(sceneWindow);
        glewInit();
        glBindVertexArray(sceneVAO);
        sceneShader.use();
        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(sceneCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = sceneCamera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(sceneShader.ID, "u_projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(sceneShader.ID, "u_modelview"), 1, GL_FALSE, glm::value_ptr(view*model));
        scene_sceneObj.Draw();

        model = glm::translate(model, headsetCamera.Position);
        model = glm::rotate(model,glm::radians(headsetCamera.Yaw+90.0f),glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model,glm::radians(headsetCamera.Pitch),glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(sceneShader.ID, "u_modelview"), 1, GL_FALSE, glm::value_ptr(view*model));
        scene_headsetObj.Draw();

        glfwSwapBuffers(sceneWindow);

        glfwMakeContextCurrent(headsetWindow);
        glewInit();
        glBindVertexArray(headsetVAO);
        headsetShader.use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model = glm::mat4(1.0f);
        view = headsetCamera.GetViewMatrix();
        projection = glm::perspective(glm::radians(headsetCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(headsetShader.ID, "u_modelview"), 1, GL_FALSE, glm::value_ptr(view*model));
        glUniformMatrix4fv(glGetUniformLocation(headsetShader.ID, "u_projection"), 1, GL_FALSE, glm::value_ptr(projection));
        headset_sceneObj.Draw();
        glfwSwapBuffers(headsetWindow);

        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    Camera *cameraPtr = (Camera *)glfwGetWindowUserPointer(window);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPtr->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPtr->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPtr->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPtr->ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera *cameraPtr = (Camera *)glfwGetWindowUserPointer(window);
    cameraPtr->ProcessMouseScroll(static_cast<float>(yoffset));
}
