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
#include "gameobject.h"
#include "block.h"

const GLuint WIDTH = 1024, HEIGHT = 768;
const int ballCount = 3;
const int levelColumns = 7;
const int levelRows = 12;

ShaderProgram *shaderProgram;

float padVelocityX = 0.0f;
float ballVelocityX[ballCount];
float ballVelocityY[ballCount];

GameObject *leftWall;
GameObject *rightWall;
GameObject *upperWall;
GameObject *ground;
GameObject *pad;
GameObject *balls[ballCount];

Block* levelBlocks[levelColumns * levelRows];

glm::mat4 P = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.2f, 200.0f);
glm::mat4 V = glm::lookAt(
    glm::vec3(0.0f, -43.0f, -32.0f),
    glm::vec3(0.0f, -9.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
	if (key == GLFW_KEY_ESCAPE)
	    glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_LEFT)
	{
	    padVelocityX = 30;
	}
	if (key == GLFW_KEY_RIGHT)
	{
	    padVelocityX = -30;
	}
    }
    if (action == GLFW_RELEASE)
    {
	padVelocityX = 0;
    }
}
void freeOpenGLProgram()
{
    delete shaderProgram;
	delete leftWall;
	delete rightWall;
	delete upperWall;
	delete ground;
	delete pad;
	for(int i=0; i< ballCount; i++)
	{
		delete balls[i];
	}
}
void generateLevelBlocks()
{
	GameObjectVertices* cube = new GameObjectVertices("cube.obj");
	GLuint metal = readTexture("metal.png");
	GLuint metalSpec = readTexture("metal_spec.png");

	for(int i=0; i< levelRows; i++)
	{
		for(int j=0; j< levelColumns; j++)
		{
			Block* levelBlock = new Block(cube, metal, metalSpec,
			      glm::vec3(21.0f - 7.0f * j,23.8f - 2.0f * i,0.0f), glm::vec3(6.5f, 1.0f, 1.0f), V, P, shaderProgram);		
			levelBlocks[levelColumns * i + j] = levelBlock;	
		}
	}
}
void initOpenGLProgram(GLFWwindow *window)
{
    glClearColor(0, 0, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, key_callback);

    shaderProgram = new ShaderProgram("vshader.txt", NULL, "fshader.txt");	

	GameObjectVertices* cube = new GameObjectVertices("cube.obj");	
	GameObjectVertices* sphere = new GameObjectVertices("sphere.obj");	

    for (int i = 0; i < ballCount; i++)
    {
		ballVelocityX[i] = 0.0f;
		ballVelocityY[i] = 10.0f;

		balls[i] = new GameObject(sphere, readTexture("metal.png"), readTexture("metal_spec.png"),
					glm::vec3(-20.0f + 3.0f * i, -19.0f , 0.0f), glm::vec3(0.7f, 0.7f, 0.7f), V, P, shaderProgram);
    }	

	GLuint titles = readTexture("tiles.png");
	GLuint titlesSpec = readTexture("tiles_spec.png");
    leftWall = new GameObject(cube, titles, titlesSpec,
			      glm::vec3(25.0f, 0.0f, 0.0f), glm::vec3(1.0f, 50.0f, 1.0f), V, P, shaderProgram);
    rightWall = new GameObject(cube, titles, titlesSpec,
			       glm::vec3(-25.0f, 0.0f, 0.0f), glm::vec3(1.0f, 50.0f, 1.0f), V, P, shaderProgram);
    upperWall = new GameObject(cube, titles, titlesSpec,
			       glm::vec3(0.0f, 24.5f, 0.0f), glm::vec3(50.0f, 1.0f, 1.0f), V, P, shaderProgram);
    ground = new GameObject(cube, readTexture("plaster.png"), readTexture("plaster_spec.png"),
			    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(51.0f, 50.0f, 1.0f), V, P, shaderProgram);
    pad = new GameObject(cube, readTexture("boards.png"), readTexture("boards_spec.png"),
			 glm::vec3(0.0f, -24.5f, 0.0f), glm::vec3(6.0f, 1.0f, 1.0f), V, P, shaderProgram);

	generateLevelBlocks();
}
void drawScene(GLFWwindow *window, float padDeltaX, float ballDeltaX[], float ballDeltaY[])
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pad->MoveHorizontal(padDeltaX);
	
    if (pad->leftEdgeX > leftWall->rightEdgeX || pad->rightEdgeX < rightWall->leftEdgeX)
    {
		pad->MoveHorizontal(-padDeltaX);
    }

    for (int i = 0; i < ballCount; i++)
    {
		balls[i]->MoveHorizontal(ballDeltaX[i]);
		balls[i]->MoveVertical(ballDeltaY[i]);

		if (balls[i]->leftEdgeX > leftWall->rightEdgeX || balls[i]->rightEdgeX < rightWall->leftEdgeX)
		{
			ballVelocityX[i] = -ballVelocityX[i];
			balls[i]->MoveHorizontal(-ballDeltaX[i]);
		}
		if (balls[i]->upperEdgeY > upperWall->bottomEdgeY)
		{
			ballVelocityY[i] = -ballVelocityY[i];
			balls[i]->MoveVertical(-ballDeltaY[i]);
		}
		if (balls[i]->rightEdgeX < pad->leftEdgeX && balls[i]->leftEdgeX > pad->rightEdgeX && 
			balls[i]->bottomEdgeY < pad->upperEdgeY && balls[i]->upperEdgeY > pad->upperEdgeY)
		{
			float ballMiddle = (balls[i]->leftEdgeX + balls[i]->rightEdgeX) / 2;
			float factor = ((ballMiddle - pad->rightEdgeX) / (pad->leftEdgeX - pad->rightEdgeX)) * 2 - 1;
			factor = roundf(factor * 100) / 100;

			ballVelocityY[i] = -ballVelocityY[i];
			ballVelocityX[i] = ballVelocityY[i] * factor;

			balls[i]->MoveVertical(-ballDeltaY[i]);
		}

		for (int j = 0; j < levelColumns * levelRows; j++)
		{
			if (!levelBlocks[j]->destroyed)
			{
				if ((balls[i]->bottomEdgeY < levelBlocks[j]->upperEdgeY && balls[i]->upperEdgeY > levelBlocks[j]->upperEdgeY) ||
					(balls[i]->upperEdgeY > levelBlocks[j]->bottomEdgeY && balls[i]->bottomEdgeY < levelBlocks[j]->bottomEdgeY))
				{
					if (balls[i]->leftEdgeX < levelBlocks[j]->leftEdgeX && balls[i]->rightEdgeX > levelBlocks[j]->rightEdgeX)
					{
						ballVelocityY[i] = -ballVelocityY[i];
						balls[i]->MoveVertical(-ballDeltaY[i]);
						levelBlocks[j]->destroyed = true;
						break;
					}
					if (balls[i]->rightEdgeX < levelBlocks[j]->leftEdgeX && balls[i]->leftEdgeX > levelBlocks[j]->rightEdgeX)
					{
						if (ballVelocityX[i] < 2)
						{
							ballVelocityY[i] = -ballVelocityY[i];
							balls[i]->MoveVertical(-ballDeltaY[i]);							
						}
						else
						{
							ballVelocityX[i] = -ballVelocityX[i];
							balls[i]->MoveHorizontal(-ballDeltaX[i]);
						}
						levelBlocks[j]->destroyed = true;
						break;
		    		}
				}
	    	}
		}		
    }

    for (int i = 0; i < ballCount; i++)
    {
		balls[i]->DrawObject();
    }

	pad->DrawObject();
	upperWall->DrawObject();
	leftWall->DrawObject();
	rightWall->DrawObject();
	ground->DrawObject();    
	
    for (int i = 0; i < levelColumns * levelRows; i++)
    {
		if (!levelBlocks[i]->destroyed)
		{
			levelBlocks[i]->DrawObject();
		}
    }

	glfwSwapBuffers(window);
}
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL-Arkanoid", nullptr, nullptr);
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

    float ballDeltaX[ballCount];
    float ballDeltaY[ballCount]; 
    float padDeltaX = 0.0f;
	for(int i=0; i<ballCount; i++)
	{
		ballDeltaX[i] = 0.0f;
		ballDeltaY[i] = 0.0f;
	}

    while (!glfwWindowShouldClose(window))
    {
		padDeltaX = glfwGetTime() * padVelocityX;

		for (int i = 0; i < ballCount; i++)
		{
			ballDeltaX[i] = glfwGetTime() * ballVelocityX[i];
			ballDeltaY[i] = glfwGetTime() * ballVelocityY[i];
		}

		glfwSetTime(0);

		drawScene(window, padDeltaX, ballDeltaX, ballDeltaY);

		glfwPollEvents();
    }

    freeOpenGLProgram();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}