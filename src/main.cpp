

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

#include <imgui.h>
#include <backend/imgui_impl_glfw.h>
#include <backend/imgui_impl_opengl3.h>

#include "shaders.hpp"
#include "stb_image.h"


/*---------------------------------------------------------------
 * CAMERA DECLARATIONS
 *---------------------------------------------------------------
 */

glm::vec3  cameraPos         =  glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3  cameraFront       =  glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3  cameraUp          =  glm::vec3(0.0f, 1.0f, 0.0f);

bool       cameraFirstMouse  =  true;
float      cameraYaw         = -90.0f;
float      cameraPitch       =  0.0f;
float      cameraLastX       =  800.0f / 2.0;
float      cameraLastY       =  600.0 / 2.0;
float      cameraFov         =  45.0f;
float      cameraSensivity   =  0.1f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


/*---------------------------------------------------------------
 * INPUT DECLARATIONS
 *---------------------------------------------------------------
 */

bool mouseCaptured = true;
bool tabWasDown = false;
void process_input(GLFWwindow* window, float deltaTime);


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

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    
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
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";

    Shaders shaderProgram("../shaders/base.vert.glsl", "../shaders/base.frag.glsl");


    /*---------------------------------------------------------------
     * INITIALIZE IMGUI
     *---------------------------------------------------------------
     */

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 460");


    /*---------------------------------------------------------------
     * CREATE MODEL
     *---------------------------------------------------------------
     */

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);  

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glEnable(GL_DEPTH_TEST);  


    /*---------------------------------------------------------------
     * CREATE LIGHTING
     *---------------------------------------------------------------
     */

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    /*---------------------------------------------------------------
     * LOAD TEXTURE
     *---------------------------------------------------------------
     */

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("../textures/800 Wood Crate.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);


    /*---------------------------------------------------------------
     * MAIN LOOP
     *---------------------------------------------------------------
     */


    while(!glfwWindowShouldClose(window))
    {

        /*---------------------------------------------------------------
         * CALCULATE FRAMETIME, DELTATIME, FPS
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
            ImGui::End();
        }
        
        ImGui::Render();


        /*---------------------------------------------------------------
         * MATRIX OPERATIONS
         *---------------------------------------------------------------
         */
        
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(cameraFov), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f); 
        //model = glm::translate(model, glm::vec3(planeXPos, planeZPos, planeYPos));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.3f)); 

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


        /*---------------------------------------------------------------
         * OPENGL DRAW
         *---------------------------------------------------------------
         */

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shaderProgram.use();
        
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        unsigned int modelLoc = glGetUniformLocation(shaderProgram.ID, "lightColor");
        glUniform3f(modelLoc, 1, GL_FALSE, glm::value_ptr(1.0f, 1.0f, 1.0f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glBindVertexArray(0);
        
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
    
    shaderProgram.destroy();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


/*---------------------------------------------------------------
 * CAMERA IMPLEMENTATIONS
 *---------------------------------------------------------------
 */

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (cameraFirstMouse)
    {
        cameraLastX = xpos;
        cameraLastY = ypos;
        cameraFirstMouse = false;
    }
  
    float xoffset = xpos - cameraLastX;
    float yoffset = cameraLastY - ypos; 
    
    cameraLastX = xpos;
    cameraLastY = ypos;

    xoffset *= cameraSensivity;
    yoffset *= cameraSensivity;

    cameraYaw   += xoffset;
    cameraPitch += yoffset;

    if(cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if(cameraPitch < -89.0f)
        cameraPitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    direction.y = sin(glm::radians(cameraPitch));
    direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(direction);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cameraFov -= (float)yoffset;
    if (cameraFov < 1.0f)
        cameraFov = 1.0f;
    if (cameraFov > 45.0f)
        cameraFov = 45.0f;
}


/*---------------------------------------------------------------
 * INPUT IMPLEMENTATIONS
 *---------------------------------------------------------------
 */

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
            cameraFirstMouse = true;
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

    const float cameraSpeed = 0.05f * deltaTime;
        
    if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && mouseCaptured)
        cameraPos += cameraSpeed * cameraFront;
    if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && mouseCaptured)
        cameraPos -= cameraSpeed * cameraFront;
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && mouseCaptured)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && mouseCaptured)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}