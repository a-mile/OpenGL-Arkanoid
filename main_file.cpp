#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderprogram.h"

const GLuint WIDTH = 800, HEIGHT = 600;
ShaderProgram *shaderProgram;
GLuint bufVertices; 
GLuint bufColors;
GLuint vao;
int vertexCount = 3;

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
}; 

GLfloat colors[] = {
    1.0f, 1.0f, 0.0f,
     1.0f, 0.0f, 1.0f,
     0.0f,  1.0f, 0.0f
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{    
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
} 

GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
	GLuint handle;
	
	glGenBuffers(1,&handle);
	glBindBuffer(GL_ARRAY_BUFFER,handle); 
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);
	
	return handle;
}

void assignVBOtoAttribute(ShaderProgram *shaderProgram,char* attributeName, GLuint bufVBO, int vertexSize) {
	GLuint location=shaderProgram->getAttribLocation(attributeName); 
	glBindBuffer(GL_ARRAY_BUFFER,bufVBO);   
	glEnableVertexAttribArray(location); 
	glVertexAttribPointer(location,vertexSize,GL_FLOAT, GL_FALSE, 0, NULL); 
}

int main(){
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL-Arkanoid", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    shaderProgram=new ShaderProgram("vshader.txt",NULL,"fshader.txt");

    bufVertices=makeBuffer(vertices, vertexCount, sizeof(float)*3);
	bufColors=makeBuffer(colors, vertexCount, sizeof(float)*3);

    glGenVertexArrays(1,&vao); 
	glBindVertexArray(vao);

    assignVBOtoAttribute(shaderProgram,"position",bufVertices,3); 
	assignVBOtoAttribute(shaderProgram,"color",bufColors,3);
   
    glBindVertexArray(0);    

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram->use();

        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = shaderProgram->getUniformLocation("ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(vao);
	    glDrawArrays(GL_TRIANGLES,0,vertexCount);
	    glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}