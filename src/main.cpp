#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <image_loader/stb_image.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <custom/shader.h>
#include <custom/camera.h>
#include <custom/model.h>

#include <iostream>
#include <string.h>


void framebuffer_size_callback(GLFWwindow *window, int width, int height); 
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastx = WINDOW_WIDTH/2.0f;
float lasty = WINDOW_HEIGHT/2.0f;
bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);  
    if (window == NULL){    
        std::cout <<"Failed to create GLFW window"<< std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); 


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){ 
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(true);

    Shader objectShader("src/shaders/basic_vert.glsl", "src/shaders/basic_frag.glsl");
    Shader lightingShader("src/shaders/lighting_vert.glsl", "src/shaders/lighting_frag.glsl");
    
    Model backpack("C:/Users/jonat/OneDrive/Documenten/Code/C/opengl/resource/backpack/backpack.obj");

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.5f, 0.6f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        objectShader.use();
        
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f); 
        objectShader.setMat4("projection", projection);
        objectShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        objectShader.setMat4("model", model);
        backpack.Draw(objectShader);

        glfwSwapBuffers(window);    
        glfwPollEvents();   
    }

    glfwTerminate();    
    return 0;
}

void processInput(GLFWwindow* window){ 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ 
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){ 
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){ 
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){ 
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){ 
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){ 
        camera.ProcessKeyboard(UPWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){ 
        camera.ProcessKeyboard(DOWNWARD, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse){
        lastx = xpos, lasty = ypos;
        firstMouse = false;
    }
    float xoffset = (xpos - lastx);
    float yoffset = (lasty - ypos);
    
    lastx = xpos, lasty = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    camera.ProcessScroll((float)yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ 
        glViewport(0, 0, width, height);
}