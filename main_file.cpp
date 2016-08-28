#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderprogram.h"

const GLuint WIDTH = 800, HEIGHT = 600;
ShaderProgram *shaderProgram;
GLuint bufVertices; 
GLuint bufColors;
GLuint bufNormals;
GLuint vao;
int vertexCount = 36;
float speed = 0;

float vertices[]={
				1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				-1.0f,-1.0f,-1.0f,1.0f,
				
				1.0f,-1.0f,-1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				
				
				-1.0f,-1.0f, 1.0f,1.0f,	
				1.0f, 1.0f, 1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f, 1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				1.0f,-1.0f,-1.0f,1.0f,
				
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				-1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f,-1.0f,-1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f,-1.0f, 1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				
				-1.0f, 1.0f, 1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				
			}; 

float colors[]={
				1.0f,0.0f,0.0f,1.0f,
				1.0f,0.0f,0.0f,1.0f,
				1.0f,0.0f,0.0f,1.0f,
				
				1.0f,0.0f,0.0f,1.0f,
				1.0f,0.0f,0.0f,1.0f,
				1.0f,0.0f,0.0f,1.0f,
				
				0.0f,1.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,1.0f,
				
				0.0f,1.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,1.0f,
				
				0.0f,0.0f,1.0f,1.0f,
				0.0f,0.0f,1.0f,1.0f,
				0.0f,0.0f,1.0f,1.0f,
				
				0.0f,0.0f,1.0f,1.0f,
				0.0f,0.0f,1.0f,1.0f,
				0.0f,0.0f,1.0f,1.0f,
				
				1.0f,1.0f,0.0f,1.0f,
				1.0f,1.0f,0.0f,1.0f,
				1.0f,1.0f,0.0f,1.0f,
				
				1.0f,1.0f,0.0f,1.0f,
				1.0f,1.0f,0.0f,1.0f,
				1.0f,1.0f,0.0f,1.0f,
				
				0.0f,1.0f,1.0f,1.0f,
				0.0f,1.0f,1.0f,1.0f,
				0.0f,1.0f,1.0f,1.0f,
				
				0.0f,1.0f,1.0f,1.0f,
				0.0f,1.0f,1.0f,1.0f,
				0.0f,1.0f,1.0f,1.0f,
				
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
				1.0f,1.0f,1.0f,1.0f,
			};

float normals[]={
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				
				0.0f, 0.0f, 1.0f,0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				
				0.0f, 0.0f, 1.0f,0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				
				1.0f, 0.0f, 0.0f,0.0f,
				1.0f, 0.0f, 0.0f,0.0f,
				1.0f, 0.0f, 0.0f,0.0f,
				
				1.0f, 0.0f, 0.0f,0.0f,
				1.0f, 0.0f, 0.0f,0.0f,
				1.0f, 0.0f, 0.0f,0.0f,
				
				-1.0f, 0.0f, 0.0f,0.0f,
				-1.0f, 0.0f, 0.0f,0.0f,
				-1.0f, 0.0f, 0.0f,0.0f,
				
				-1.0f, 0.0f, 0.0f,0.0f,
				-1.0f, 0.0f, 0.0f,0.0f,
				-1.0f, 0.0f, 0.0f,0.0f,
				
				0.0f,-1.0f, 0.0f,0.0f,
				0.0f,-1.0f, 0.0f,0.0f,
				0.0f,-1.0f, 0.0f,0.0f,
				
				0.0f,-1.0f, 0.0f,0.0f,
				0.0f,-1.0f, 0.0f,0.0f,
				0.0f,-1.0f, 0.0f,0.0f,
				
				0.0f, 1.0f, 0.0f,0.0f,
				0.0f, 1.0f, 0.0f,0.0f,
				0.0f, 1.0f, 0.0f,0.0f,
				
				0.0f, 1.0f, 0.0f,0.0f,
				0.0f, 1.0f, 0.0f,0.0f,
				0.0f, 1.0f, 0.0f,0.0f,
			};
			
			float vertexNormals[]={
				1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				-1.0f,-1.0f,-1.0f,1.0f,
				
				1.0f,-1.0f,-1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				
				
				-1.0f,-1.0f, 1.0f,1.0f,	
				1.0f, 1.0f, 1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f, 1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				1.0f,-1.0f,-1.0f,1.0f,
				
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				-1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				-1.0f, 1.0f, 1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				1.0f,-1.0f,-1.0f,1.0f,
				
				-1.0f,-1.0f,-1.0f,1.0f,
				-1.0f,-1.0f, 1.0f,1.0f,
				1.0f,-1.0f, 1.0f,1.0f,
				
				-1.0f, 1.0f, 1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
				1.0f, 1.0f, 1.0f,1.0f,
				
				-1.0f, 1.0f, 1.0f,1.0f,
				-1.0f, 1.0f,-1.0f,1.0f,
				1.0f, 1.0f,-1.0f,1.0f,
			};

			float texCoords[]={
				1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
				1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
				
				1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
				1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
				
				1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
				1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
				
				1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
				1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
				
				1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
				1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
				
				1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
				1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
			};


glm::mat4 leftWallModel = glm::mat4(1.0f);
glm::mat4 rightWallModel = glm::mat4(1.0f);
glm::mat4 upperWallModel = glm::mat4(1.0f);
glm::mat4 P = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.2f, 200.0f); 		
glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, -30.0f, -100.0f),
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
float minX;
float maxX;	
bool left = true;
bool right = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{    
    if(action == GLFW_PRESS)
    {   if(key == GLFW_KEY_ESCAPE)
    	    glfwSetWindowShouldClose(window, GL_TRUE);
        if(key == GLFW_KEY_LEFT)
		{		
			right = true;
			if(left)
            	speed = 60;
		}
        if(key == GLFW_KEY_RIGHT)
		{
			left = true;
			if(right)
            	speed = -60;
		}
    }
    if(action == GLFW_RELEASE)
    {
        speed = 0;
    }
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

void freeOpenGLProgram() {
	delete shaderProgram; 
	
	glDeleteVertexArrays(1,&vao); //Usunięcie vao
	glDeleteBuffers(1,&bufVertices); //Usunięcie VBO z wierzchołkami
	glDeleteBuffers(1,&bufColors); //Usunięcie VBO z kolorami
    glDeleteBuffers(1,&bufNormals);				
}

void drawObject(GLuint vao, ShaderProgram *shaderProgram, glm::mat4 mP, glm::mat4 mV, glm::mat4 mM, glm::vec4 color) {	
	shaderProgram->use();
		
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(mM));	
    glUniform4f(shaderProgram->getUniformLocation("lightPos0"), 0,-30,-100,1);
	glUniform4fv(shaderProgram->getUniformLocation("color"),1, glm::value_ptr(color));		
	
	glBindVertexArray(vao);
	
	glDrawArrays(GL_TRIANGLES,0,vertexCount);
		
	glBindVertexArray(0);
}

void drawScene(GLFWwindow* window, float position) {	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			
	glm::mat4 M = glm::mat4(1.0f);      
	M = glm::translate(M, glm::vec3(position,-29.0f,0.0f));	
	M = glm::scale(M, glm::vec3(6.0f,1.0f,1.0f));

	float modelMinX = (M*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
	float modelMaxX = (M*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 position = M*vertex;
		if(position.x < modelMinX)
		{
			modelMinX = position.x;
		}
		if(position.x > modelMaxX)
		{
			modelMaxX = position.x;
		}
	}

	if(modelMaxX >= minX) 
	{
		M = glm::mat4(1.0f);      
		M = glm::translate(M, glm::vec3(42.0f,-29.0f,0.0f));
		M = glm::scale(M, glm::vec3(6.0f,1.0f,1.0f));
		speed = 0;			
		left = false;
	}
	if(modelMinX <= maxX)
	{
		M = glm::mat4(1.0f);      
		M = glm::translate(M, glm::vec3(-42.0f,-29.0f,0.0f));
		M = glm::scale(M, glm::vec3(6.0f,1.0f,1.0f));
		speed = 0;
		right = false;		
	}
		
	drawObject(vao,shaderProgram,P,V,M, glm::vec4(1.0f,0.0f,0.0f,1.0f));
    drawObject(vao,shaderProgram,P,V,upperWallModel, glm::vec4(1.0f,1.0f,0.0f,1.0f));	 
    drawObject(vao,shaderProgram,P,V,leftWallModel, glm::vec4(1.0f,1.0f,0.0f,1.0f));	
    drawObject(vao,shaderProgram,P,V,rightWallModel, glm::vec4(1.0f,1.0f,0.0f,1.0f));
		
	glfwSwapBuffers(window);
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
    glEnable(GL_DEPTH_TEST);

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

    bufVertices=makeBuffer(vertices, vertexCount, sizeof(float)*4);
	bufColors=makeBuffer(colors, vertexCount, sizeof(float)*4);
    bufNormals=makeBuffer(normals, vertexCount, sizeof(float)*4);

    glGenVertexArrays(1,&vao); 
	glBindVertexArray(vao);

    assignVBOtoAttribute(shaderProgram,"position",bufVertices,4); 	
    assignVBOtoAttribute(shaderProgram,"normal",bufNormals,4);
   
    glBindVertexArray(0);    

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	
    upperWallModel = glm::translate(upperWallModel, glm::vec3(0.0f,70.0f,0.0f));
	upperWallModel = glm::scale(upperWallModel, glm::vec3(50.0f,1.0f,1.0f));	
	leftWallModel = glm::translate(leftWallModel, glm::vec3(49.0f,20.0f,0.0f));
    leftWallModel = glm::scale(leftWallModel, glm::vec3(1.0f,50.0f,1.0f)); 	
	rightWallModel = glm::translate(rightWallModel, glm::vec3(-49.0f,20.0f,0.0f));
    rightWallModel = glm::scale(rightWallModel, glm::vec3(1.0f,50.0f,1.0f));   

	minX = (leftWallModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 position = leftWallModel*vertex;
		if(position.x < minX)
		{
			minX = position.x;
		}
	}
	
	std::cout<<minX<<std::endl;

	maxX = (rightWallModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 position = rightWallModel*vertex;
		if(position.x > maxX)
		{
			maxX = position.x;
		}
	}
	
	std::cout<<maxX<<std::endl;

    float position = 0;

    while(!glfwWindowShouldClose(window))
    {
        position += speed * glfwGetTime();
        glfwSetTime(0);
        drawScene(window,position);		
        glfwPollEvents();
    }

    freeOpenGLProgram();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}