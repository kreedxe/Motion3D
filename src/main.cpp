

/*---------------------------------------------------------------
 * INCLUDES AND DEFINITIONS
 *---------------------------------------------------------------
 */

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include <stb_image.h>

#include <imgui.h>
#include <backend/imgui_impl_glfw.h>
#include <backend/imgui_impl_opengl3.h>

#include "shaders.hpp"
#include "camera.hpp"
#include "cube.hpp"


/*---------------------------------------------------------------
 * INPUT DECLARATIONS
 *---------------------------------------------------------------
 */

bool mouseCaptured = true;
bool tabWasDown = false;
void process_input(GLFWwindow* window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera;
Cube cubeOne; 


int main()
{

    /*---------------------------------------------------------------
     * DECLARE VARIABLES
     *---------------------------------------------------------------
     */

    const int WIDTH = 1200;
    const int HEIGHT = 800;
    const char* TITLE = "OpenGL 4.6 | GLFW 3";

    float currentTime = 0.0f;
    float frameTime = 0.0f;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    float fps = 0.0f;

    int success;
    char infoLog[512];

    
    /*---------------------------------------------------------------
     * INITIALIZE GLFW, WINDOW AND GLAD
     *---------------------------------------------------------------
     */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    if (mouseCaptured)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";

    cubeOne.load("../textures/800 Wood Crate.jpg", glm::mat4(1.0f));


    /*---------------------------------------------------------------
     * INITIALIZE IMGUI
     *---------------------------------------------------------------
     */

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 460");

    if (!mouseCaptured)
    {
        glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
        glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
        glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
        glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
        glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
    }


    /*---------------------------------------------------------------
     * MAIN LOOP
     *---------------------------------------------------------------
     */


    while(!glfwWindowShouldClose(window))
    {

        /*---------------------------------------------------------------
         * CALCULATE METRICS AND PROCESS INPUT
         *---------------------------------------------------------------
         */
        
        currentTime = glfwGetTime();
        frameTime = currentTime - lastTime;
        lastTime = currentTime;
        
        deltaTime = frameTime * 60.0f;
        fps = 1.0f / frameTime;
        
        glfwPollEvents();
        process_input(window, deltaTime);
        
        
        /*---------------------------------------------------------------
         * IMGUI DRAW
         *---------------------------------------------------------------
         */
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Metrics"))
        {
            ImGui::Text("FPS: %f", fps);
            ImGui::Text("Frame Time: %f", frameTime);
            ImGui::Text("Delta time: %f", deltaTime);
            ImGui::Separator();
            ImGui::End();
        }
        
        ImGui::Render();


        /*---------------------------------------------------------------
         * OPENGL DRAW
         *---------------------------------------------------------------
         */

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        cubeOne.draw(&camera);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }  


    /*---------------------------------------------------------------
     * DEINITIALIZATION
     *---------------------------------------------------------------
     */

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    cubeOne.destroy();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


/*---------------------------------------------------------------
 * INPUT IMPLEMENTATIONS
 *---------------------------------------------------------------
 */

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.mouseCallback(xpos, ypos);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.scrollCallback(xoffset, yoffset);
}


void process_input(GLFWwindow* window, float deltaTime)
{

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabWasDown)
    {
        if (mouseCaptured)
        {
            if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
            glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
            glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
            glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
            glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
            mouseCaptured = false;
            camera.firstMouse = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            glfwSetScrollCallback(window, scroll_callback);
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetMouseButtonCallback(window, NULL);
            glfwSetKeyCallback(window, NULL);
            glfwSetCharCallback(window, NULL);
            mouseCaptured = true;
        }
    }

    tabWasDown = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;

    if (mouseCaptured)
    {
        camera.keyboardCallback(window, deltaTime);
    }
}