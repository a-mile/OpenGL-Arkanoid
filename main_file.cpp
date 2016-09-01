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
#include "block.h"

const GLuint WIDTH = 800, HEIGHT = 600;

ShaderProgram *shaderProgram;

GLuint bufCubeVertices; 
GLuint bufCubeNormals;
GLuint bufSphereVertices; 
GLuint bufSphereNormals;
GLuint vao;

int cubeVertexCount = 36;

float padVelocityX = 0;
float ballVelocityX = 0;
float ballVelocityY = 60;

float cubeVertices[]={
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

float cubeNormals[]={

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

glm::mat4 leftWallModel = glm::mat4(1.0f);
glm::mat4 rightWallModel = glm::mat4(1.0f);
glm::mat4 upperWallModel = glm::mat4(1.0f);
glm::mat4 padModel = glm::mat4(1.0f);
glm::mat4 ballModel = glm::mat4(1.0f);
glm::mat4 P = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.2f, 200.0f); 		
glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, -50.0f, -90.0f),
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
float leftWallX;
float rightWallX;	
float upperWallY;
float padY;
Block* level1[9*9];

void generateLevel1()
{

	for(int j=0; j<9; j++)
	{
		for(int i=0; i<9; i++)
		{
			glm::mat4 blockModel = glm::mat4(1.0f);
			blockModel = glm::scale(blockModel, glm::vec3(5.1f,1.0f,1.0f));
			blockModel = glm::translate(blockModel, glm::vec3(8.4f,(float)(68.0f-3.0f*j),0.0f));
			blockModel = glm::translate(blockModel, glm::vec3((float)(-i*2.1f),0.0f,0.0f));			
			Block* levelBlock = new Block(2, blockModel);
			levelBlock->calculateEdges();
			level1[9*j + i] = levelBlock;
		}
	}
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{    
    if(action == GLFW_PRESS)
    {   if(key == GLFW_KEY_ESCAPE)
    	    glfwSetWindowShouldClose(window, GL_TRUE);
        if(key == GLFW_KEY_LEFT)
		{		
        	padVelocityX = 10;
		}
        if(key == GLFW_KEY_RIGHT)
		{			
            padVelocityX = -10;
		}
    }
    if(action == GLFW_RELEASE)
    {
        padVelocityX = 0;
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
	glUniform4f(shaderProgram->getUniformLocation("lightPos0"), 0,0,-40,1);
	glGenVertexArrays(1,&vao); 

    bufCubeVertices=makeBuffer(cubeVertices, cubeVertexCount, sizeof(float)*4);		
    bufCubeNormals=makeBuffer(cubeNormals, cubeVertexCount, sizeof(float)*4);  	

	upperWallModel = glm::translate(upperWallModel, glm::vec3(0.0f,70.0f,0.0f));
	upperWallModel = glm::scale(upperWallModel, glm::vec3(50.0f,1.0f,1.0f));	
	leftWallModel = glm::translate(leftWallModel, glm::vec3(49.0f,20.0f,0.0f));
    leftWallModel = glm::scale(leftWallModel, glm::vec3(1.0f,50.0f,1.0f)); 	
	rightWallModel = glm::translate(rightWallModel, glm::vec3(-49.0f,20.0f,0.0f));
    rightWallModel = glm::scale(rightWallModel, glm::vec3(1.0f,50.0f,1.0f)); 	
	padModel = glm::scale(padModel, glm::vec3(6.0f,1.0f,1.0f));
	padModel = glm::translate(padModel, glm::vec3(0.0f,-29.0f,0.0f));
	ballModel = glm::translate(ballModel, glm::vec3(0.0f,-27.0f,0.0f));	

	generateLevel1();

	leftWallX = (leftWallModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).x;
	rightWallX = (rightWallModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).x;
	upperWallY = (upperWallModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).y;
	padY = (padModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).y;

	for(int i=4; i<cubeVertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(cubeVertices[i], cubeVertices[i+1], cubeVertices[i+2], cubeVertices[i+3]);
		glm::vec4 leftWallPosition = leftWallModel*vertex;
		glm::vec4 rightWallPosition = rightWallModel*vertex;
		glm::vec4 upperWallPosition = upperWallModel*vertex;
		glm::vec4 padPosition = padModel*vertex;

		if(leftWallPosition.x < leftWallX)
		{
			leftWallX = leftWallPosition.x;
		}		
		if(rightWallPosition.x > rightWallX)
		{
			rightWallX = rightWallPosition.x;
		}		
		if(upperWallPosition.y < upperWallY)
		{
			upperWallY = upperWallPosition.y;
		}		
		if(padPosition.y > padY)
		{
			padY = padPosition.y;
		}
	}						
}

void freeOpenGLProgram() {
	delete shaderProgram; 
	
	glDeleteVertexArrays(1,&vao); 
	glDeleteBuffers(1,&bufCubeVertices); 	
    glDeleteBuffers(1,&bufCubeNormals);				
}

void drawObject(GLuint bufVertices, GLuint bufNormals, int vertexCount, glm::mat4 mP, glm::mat4 mV, glm::mat4 mM, glm::vec4 color) {	
	shaderProgram->use();
		
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(mM));	
    
	glUniform4fv(shaderProgram->getUniformLocation("color"),1, glm::value_ptr(color));		
	
	glBindVertexArray(vao);

	assignVBOtoAttribute(shaderProgram,"position",bufVertices,4); 	
    assignVBOtoAttribute(shaderProgram,"normal",bufNormals,4);
	
	glDrawArrays(GL_TRIANGLES,0,vertexCount);
		
	glBindVertexArray(0);
}

void drawScene(GLFWwindow* window, float padDeltaX, float ballDeltaX, float ballDeltaY) {	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			
	padModel = glm::translate(padModel, glm::vec3(padDeltaX,0.0f,0.0f));
	ballModel = glm::translate(ballModel, glm::vec3(ballDeltaX,ballDeltaY,0.0f));			

	float padLeftEdgeX = (padModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).x;
	float padRightEdgeX = (padModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).x;
	float ballLeftEdgeX = (ballModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).x;
	float ballRightEdgeX = (ballModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).x;
	float ballUpperEdgeY = (ballModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).y;
	float ballBottomEdgeY = (ballModel*glm::vec4(cubeVertices[0], cubeVertices[1], cubeVertices[2], cubeVertices[3])).y;

	for(int i=4; i<cubeVertexCount*4 - 3; i+=4)
	{
		glm::vec4 vertex = glm::vec4(cubeVertices[i], cubeVertices[i+1], cubeVertices[i+2], cubeVertices[i+3]);
		glm::vec4 padPosition = padModel*vertex;
		glm::vec4 ballPosition = ballModel*vertex;
		if(padPosition.x > padLeftEdgeX)
			padLeftEdgeX = padPosition.x;
		if(padPosition.x < padRightEdgeX)
			padRightEdgeX = padPosition.x;				
		if(ballPosition.x > ballLeftEdgeX)
			ballLeftEdgeX = ballPosition.x;
		if(ballPosition.x < ballRightEdgeX)
			ballRightEdgeX = ballPosition.x;
		if(ballPosition.y > ballUpperEdgeY)
			ballUpperEdgeY = ballPosition.y;
		if(ballPosition.y < ballBottomEdgeY)
			ballBottomEdgeY = ballPosition.y;
	}		

	if(padLeftEdgeX > leftWallX || padRightEdgeX < rightWallX)
	{		
		padModel = glm::translate(padModel, glm::vec3(-padDeltaX,0.0f,0.0f));		
	}		

	if(ballLeftEdgeX > leftWallX || ballRightEdgeX < rightWallX)
	{
		ballModel = glm::translate(ballModel, glm::vec3(-ballDeltaX,0.0f,0.0f));	
		ballVelocityX = -ballVelocityX;	
	}
	if(ballUpperEdgeY > upperWallY)
	{
		ballModel = glm::translate(ballModel, glm::vec3(0.0f,-ballDeltaY,0.0f));
		ballVelocityY = -ballVelocityY;
	}
	if(ballBottomEdgeY < padY)
	{
		if(ballRightEdgeX < padLeftEdgeX && ballLeftEdgeX > padRightEdgeX)
		{
			float ballMiddle = (ballLeftEdgeX + ballRightEdgeX)/2;
			float factor = ((ballMiddle - padRightEdgeX) / (padLeftEdgeX - padRightEdgeX))*2 - 1;
			factor = roundf(factor * 100) / 100;			
									
			ballVelocityY = -ballVelocityY;		
			ballVelocityX = ballVelocityY * factor;	

			ballModel = glm::translate(ballModel, glm::vec3(0.0f,-ballDeltaY,0.0f));
		}
	}	

	for(int i=0; i<9*9; i++)
	{
		if(level1[i]->canDraw())
		{
			if(ballRightEdgeX < level1[i]->getLeftEdgeX() && ballLeftEdgeX > level1[i]->getRightEdgeX())
			{
				if(ballUpperEdgeY > level1[i]->getBottomEdgeY() && ballUpperEdgeY < level1[i]->getUpperEdgeY())
				{
					//std::cout << i << std::endl;
					ballModel = glm::translate(ballModel, glm::vec3(0.0f,-ballDeltaY,0.0f));
					ballVelocityY = -ballVelocityY;
					
					level1[i]->hit();
					break;
				}
			}
		}
	}	

	drawObject(bufCubeVertices, bufCubeNormals, cubeVertexCount, P,V,ballModel, glm::vec4(1.0f,0.0f,1.0f,1.0f));		
	drawObject(bufCubeVertices, bufCubeNormals, cubeVertexCount,P,V,padModel, glm::vec4(1.0f,0.0f,0.0f,1.0f));
    drawObject(bufCubeVertices, bufCubeNormals, cubeVertexCount,P,V,upperWallModel, glm::vec4(1.0f,1.0f,0.0f,1.0f));	 
    drawObject(bufCubeVertices, bufCubeNormals, cubeVertexCount,P,V,leftWallModel, glm::vec4(1.0f,1.0f,0.0f,1.0f));	
    drawObject(bufCubeVertices, bufCubeNormals, cubeVertexCount,P,V,rightWallModel, glm::vec4(1.0f,1.0f,0.0f,1.0f));

	for(int i=0; i<9*9; i++)
	{
		if(level1[i]->canDraw())
		{			
			drawObject(bufCubeVertices, bufCubeNormals, cubeVertexCount,P,V,level1[i]->getModel(), level1[i]->getColor());
			
		}
	}
		
	glfwSwapBuffers(window);
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

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
		padDeltaX = glfwGetTime() * padVelocityX;	
		ballDeltaX = glfwGetTime() * ballVelocityX;		
		ballDeltaY = glfwGetTime() * ballVelocityY;

        glfwSetTime(0);

        drawScene(window,padDeltaX,ballDeltaX, ballDeltaY);		

        glfwPollEvents();
    }

    freeOpenGLProgram();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}