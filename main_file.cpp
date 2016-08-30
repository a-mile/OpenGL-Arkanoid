#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include "shaderprogram.h"

const GLuint WIDTH = 800, HEIGHT = 600;

ShaderProgram *shaderProgram;

GLuint bufVertices; 
GLuint bufColors;
GLuint bufNormals;
GLuint vao;

int vertexCount = 36;
float padSpeed = 0;
float ballSpeedX = 40;
float ballSpeedY = 40;

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
glm::mat4 padModel = glm::mat4(1.0f);
glm::mat4 ballModel = glm::mat4(1.0f);
glm::mat4 P = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.2f, 200.0f); 		
glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, -30.0f, -100.0f),
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
float leftWallX;
float rightWallX;	
float upperWallY;
float padY;
float bounceAngle = 3.1415;
float directionX = 1;
float directionY = 1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{    
    if(action == GLFW_PRESS)
    {   if(key == GLFW_KEY_ESCAPE)
    	    glfwSetWindowShouldClose(window, GL_TRUE);
        if(key == GLFW_KEY_LEFT)
		{		
        	padSpeed = 6;
		}
        if(key == GLFW_KEY_RIGHT)
		{			
            padSpeed = -6;
		}
    }
    if(action == GLFW_RELEASE)
    {
        padSpeed = 0;
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

void initOpenGLProgram(GLFWwindow* window)
{
	glClearColor(0, 0, 1, 1); 	
	glEnable(GL_DEPTH_TEST); 
	glfwSetKeyCallback(window, key_callback);

	shaderProgram=new ShaderProgram("vshader.txt",NULL,"fshader.txt");

    bufVertices=makeBuffer(vertices, vertexCount, sizeof(float)*4);	
	bufColors=makeBuffer(colors, vertexCount, sizeof(float)*4);
    bufNormals=makeBuffer(normals, vertexCount, sizeof(float)*4);

    glGenVertexArrays(1,&vao); 
	glBindVertexArray(vao);

    assignVBOtoAttribute(shaderProgram,"position",bufVertices,4); 	
    assignVBOtoAttribute(shaderProgram,"normal",bufNormals,4);
   
    glBindVertexArray(0); 

	upperWallModel = glm::translate(upperWallModel, glm::vec3(0.0f,70.0f,0.0f));
	upperWallModel = glm::scale(upperWallModel, glm::vec3(50.0f,1.0f,1.0f));	
	leftWallModel = glm::translate(leftWallModel, glm::vec3(49.0f,20.0f,0.0f));
    leftWallModel = glm::scale(leftWallModel, glm::vec3(1.0f,50.0f,1.0f)); 	
	rightWallModel = glm::translate(rightWallModel, glm::vec3(-49.0f,20.0f,0.0f));
    rightWallModel = glm::scale(rightWallModel, glm::vec3(1.0f,50.0f,1.0f)); 	
	padModel = glm::scale(padModel, glm::vec3(6.0f,1.0f,1.0f));
	padModel = glm::translate(padModel, glm::vec3(0.0f,-29.0f,0.0f));	

	leftWallX = (leftWallModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 position = leftWallModel*vertex;
		if(position.x < leftWallX)
		{
			leftWallX = position.x;
		}
	}		

	rightWallX = (rightWallModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 position = rightWallModel*vertex;
		if(position.x > rightWallX)
		{
			rightWallX = position.x;
		}
	}	 

	upperWallY = (upperWallModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).y;
	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 position = upperWallModel*vertex;
		if(position.y < upperWallY)
		{
			upperWallY = position.y;
		}
	}	

	padY = (padModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).y;
	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 position = padModel*vertex;
		if(position.y > padY)
		{
			padY = position.y;
		}
	}	
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
    glUniform4f(shaderProgram->getUniformLocation("lightPos0"), 0,-30,-50,1);
	glUniform4fv(shaderProgram->getUniformLocation("color"),1, glm::value_ptr(color));		
	
	glBindVertexArray(vao);
	
	glDrawArrays(GL_TRIANGLES,0,vertexCount);
		
	glBindVertexArray(0);
}

void drawScene(GLFWwindow* window, float padDeltaX, float ballDeltaX, float ballDeltaY) {	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			
	padModel = glm::translate(padModel, glm::vec3(padDeltaX,0.0f,0.0f));
	ballModel = glm::translate(ballModel, glm::vec3(ballDeltaX,ballDeltaY,0.0f));
		
	float padLeftEdgeX = (padModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
	float padRightEdgeX = (padModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;

	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 position = padModel*vertex;
		if(position.x > padLeftEdgeX)
			padLeftEdgeX = position.x;
		if(position.x < padRightEdgeX)
			padRightEdgeX = position.x;
	}	

	if(padLeftEdgeX > leftWallX || padRightEdgeX < rightWallX)
	{		
		padModel = glm::translate(padModel, glm::vec3(-padDeltaX,0.0f,0.0f));		
	}

	float ballLeftEdgeX = (ballModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
	float ballRightEdgeX = (ballModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
	float ballUpperEdgeY = (ballModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).y;
	float ballBottomEdgeY = (ballModel*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).y;

	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 position = ballModel*vertex;
		if(position.x > ballLeftEdgeX)
			ballLeftEdgeX = position.x;
		if(position.x < ballRightEdgeX)
			ballRightEdgeX = position.x;
		if(position.y > ballUpperEdgeY)
			ballUpperEdgeY = position.y;
		if(position.y < ballBottomEdgeY)
			ballBottomEdgeY = position.y;
	}

	if(ballLeftEdgeX > leftWallX || ballRightEdgeX < rightWallX)
	{
		ballModel = glm::translate(ballModel, glm::vec3(-ballDeltaX,0.0f,0.0f));	
		directionX = -directionX;	
	}
	if(ballUpperEdgeY > upperWallY)
	{
		ballModel = glm::translate(ballModel, glm::vec3(0.0f,-ballDeltaY,0.0f));
		directionY = -directionY;
	}
	if(ballBottomEdgeY < padY)
	{
		if(ballRightEdgeX <= padLeftEdgeX && ballLeftEdgeX >= padRightEdgeX)
		{
			float ballMiddle = (ballLeftEdgeX + ballRightEdgeX)/2;
			float factor = ((ballMiddle - padRightEdgeX) / (padLeftEdgeX - padRightEdgeX))*2 - 1;
			factor = roundf(factor * 100) / 100;			
			
			bounceAngle = factor * 5 * 3.1415 / 12;

			directionX = 1;
			directionY = 1;
			
			ballModel = glm::translate(ballModel, glm::vec3(0.0f,-ballDeltaY,0.0f));			
		}
	}

	

	drawObject(vao,shaderProgram,P,V,ballModel, glm::vec4(1.0f,0.0f,1.0f,1.0f));	
	drawObject(vao,shaderProgram,P,V,padModel, glm::vec4(1.0f,0.0f,0.0f,1.0f));
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

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
        
    glViewport(0, 0, WIDTH, HEIGHT);

    initOpenGLProgram(window);  
	float padDeltaX = 0; 
	float ballDeltaX = 0;     
	float ballDeltaY = 0; 	    		

    while(!glfwWindowShouldClose(window))
    {        	
		padDeltaX = padSpeed * glfwGetTime();	
		ballDeltaX = ballSpeedX * glfwGetTime() * std::sin(bounceAngle) * directionX;		
		ballDeltaY = ballSpeedY * glfwGetTime() * std::cos(bounceAngle) * directionY;
        glfwSetTime(0);
        drawScene(window,padDeltaX,ballDeltaX, ballDeltaY);		
        glfwPollEvents();
    }

    freeOpenGLProgram();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}