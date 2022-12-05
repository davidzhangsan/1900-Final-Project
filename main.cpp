#include <iostream>
#include <stdlib.h>
#include <string>
 
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
 
#include "Shader.h"
 
int screen_width{ 1080 };
int screen_height{ 1080 };
 
int num_frames{ 0 };
float last_time{ 0.0f };
 
float vertices[] = 
{
//    x      y      z   
    -1.0f, -1.0f, -0.0f,
     1.0f,  1.0f, -0.0f,
    -1.0f,  1.0f, -0.0f,
     1.0f, -1.0f, -0.0f
};
 
unsigned int indices[] = 
{
//  2---,1
//  | .' |
//  0'---3
    0, 1, 2,
    0, 3, 1
};
 
 
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}
 
void countFPS()
{
    double current_time = glfwGetTime();
    num_frames++;
    if (current_time - last_time >= 1.0)
    {
        std::cout << 1000.0 / num_frames << "ms / frame\n";
        num_frames = 0;
        last_time += 1.0;
    }
}
 
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow * window = glfwCreateWindow(screen_width, screen_height, "Mandelbrot", NULL, NULL);
 
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
        return -1;
    }
 
    glfwMakeContextCurrent(window);
 
    if (glewInit())
    {
        std::cout << "Failed initializing GLEW\n";
    }
 
    glViewport(0, 0, screen_width, screen_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
 
    glBindVertexArray(VAO);
 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
 
    Shader our_shader("shader.vert", "shader.frag");
 
    last_time = glfwGetTime();
 
    glEnable(GL_DEPTH_TEST);
 
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        countFPS();
 
        our_shader.use_shader();
        glBindVertexArray(VAO);
         
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
 
    }
 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
 
    glfwTerminate();
    return 0;
}