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
#include "lodepng.h"
#include "objloader.h"

const GLuint WIDTH = 1024, HEIGHT = 768;

ShaderProgram *shaderProgram;

GLuint bufCubeVertices; 
GLuint bufCubeNormals;
GLuint bufCubeTexCoords; 
GLuint bufSphereVertices; 
GLuint bufSphereNormals;
GLuint bufSphereTexCoords;

GLuint vao;
GLuint floor0;
GLuint floor1;
GLuint wall0;
GLuint wall1;
GLuint pad0;
GLuint pad1;
GLuint block0;
GLuint block1;

float padVelocityX = 0;
float ballVelocityX[] = {0,0,0};
float ballVelocityY[] = {40,40,40};

std::vector<glm::vec3> sphereVertices;
std::vector<glm::vec2> sphereUvs;
std::vector<glm::vec3> sphereNormals;

std::vector<glm::vec3> cubeVertices;
std::vector<glm::vec2> cubeUvs;
std::vector<glm::vec3> cubeNormals;

glm::mat4 leftWallModel = glm::mat4(1.0f);
glm::mat4 rightWallModel = glm::mat4(1.0f);
glm::mat4 upperWallModel = glm::mat4(1.0f);
glm::mat4 floorModel = glm::mat4(1.0f);
glm::mat4 padModel = glm::mat4(1.0f);
glm::mat4 ballModel[] = {glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f)};
glm::mat4 P = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.2f, 200.0f); 		
glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, -69.0f, -64.0f),
		glm::vec3(0.0f, 21.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
float leftWallX;
float rightWallX;	
float upperWallY;
float padY;

float a = 0;
float b = 56;
float c = -9;
float d = 0;
float e = -26;
float f = -9;

const int levelColumns = 9;
const int levelRows = 9;
Block* levelBlocks[levelColumns * levelRows];

GLuint readTexture(char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
									   //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	

	return tex;
}
GLuint makeBuffer(void *data, int vertexCount, int vertexsize) {
	GLuint handle;
	
	glGenBuffers(1,&handle);
	glBindBuffer(GL_ARRAY_BUFFER,handle); 
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexsize, data, GL_STATIC_DRAW);
	
	return handle;
}
void assignVBOtoAttribute(ShaderProgram *shaderProgram,char* attributeName, GLuint bufVBO, int vertexsize) {
	GLuint location=shaderProgram->getAttribLocation(attributeName); 
	glBindBuffer(GL_ARRAY_BUFFER,bufVBO);   
	glEnableVertexAttribArray(location); 
	glVertexAttribPointer(location,vertexsize,GL_FLOAT, GL_FALSE, 0, NULL); 
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
		if(key == GLFW_KEY_Q)
			a++;
		if(key == GLFW_KEY_A)
			a--;	
		if(key == GLFW_KEY_W)
			b++;	
		if(key == GLFW_KEY_S)
			b--;
		if(key == GLFW_KEY_E)
			c++;
		if(key == GLFW_KEY_D)
			c--;	
		if(key == GLFW_KEY_R)
			d++;	
		if(key == GLFW_KEY_F)
			d--;
		if(key == GLFW_KEY_T)
			e++;
		if(key == GLFW_KEY_G)
			e--;
		if(key == GLFW_KEY_Y)
			f++;	
		if(key == GLFW_KEY_H)
			f--;
		std::cout<<a<<" "<<b<<" "<<c<<" "<<d<<" "<<e<<" "<<f<<std::endl;								
    }
    if(action == GLFW_RELEASE)
    {
        padVelocityX = 0;
    }
} 
void freeOpenGLProgram() {
	delete shaderProgram; 
	
	glDeleteVertexArrays(1,&vao); 
	glDeleteBuffers(1,&bufCubeVertices); 	
    glDeleteBuffers(1,&bufCubeNormals);	
	glDeleteBuffers(1, &bufCubeTexCoords);			
}
void generatelevelBlocks()
{
	for(int j=0; j< levelRows; j++)
	{
		for(int i=0; i< levelColumns; i++)
		{
			glm::mat4 blockModel = glm::mat4(1.0f);
			blockModel = glm::scale(blockModel, glm::vec3(5.1f,1.0f,1.0f));
			blockModel = glm::translate(blockModel, glm::vec3(8.4f,(float)(68.0f-3.0f*j),0.0f));
			blockModel = glm::translate(blockModel, glm::vec3((float)(-i*2.1f),0.0f,0.0f));	
			int strength = 1;
			if(i == 0 || i == 1 || i == levelColumns -1 || i == levelColumns -2)
				strength = 0;			
			Block* levelBlock = new Block(strength, blockModel);
			levelBlock->calculateEdges();
			levelBlocks[levelColumns*j + i] = levelBlock;
		}
	}
}
void initOpenGLProgram(GLFWwindow* window)
{
	glClearColor(0, 0, 1, 1); 	
	glEnable(GL_DEPTH_TEST); 
	glfwSetKeyCallback(window, key_callback);

	shaderProgram=new ShaderProgram("vshader.txt",NULL,"fshader.txt");

	loadOBJ("cube.obj", cubeVertices, cubeUvs, cubeNormals);
	loadOBJ("sphere.obj", sphereVertices, sphereUvs, sphereNormals);
    
	bufCubeVertices=makeBuffer(&cubeVertices[0], cubeVertices.size(), sizeof(float)*3);		
    bufCubeNormals=makeBuffer(&cubeNormals[0], cubeNormals.size(), sizeof(float)*3);  
	bufCubeTexCoords = makeBuffer(&cubeUvs[0], cubeUvs.size(), sizeof(float) * 2);
	
	bufSphereVertices=makeBuffer(&sphereVertices[0], sphereVertices.size(), sizeof(float)*3);		
    bufSphereNormals=makeBuffer(&sphereNormals[0], sphereNormals.size(), sizeof(float)*3);  
	bufSphereTexCoords = makeBuffer(&sphereUvs[0], sphereUvs.size(), sizeof(float) * 2);

	glGenVertexArrays(1,&vao); 

	floor0 = readTexture("plaster.png");
	floor1 = readTexture("plaster_spec.png");
	wall0 = readTexture("tiles.png");
	wall1 = readTexture("tiles_spec.png");	
	pad0 = readTexture("boards.png");
	pad1 = readTexture("boards_spec.png");	
	block0 = readTexture("cardboard.png");
	block1 = readTexture("cardboard_spec.png");	

	upperWallModel = glm::translate(upperWallModel, glm::vec3(0.0f,70.0f,0.0f));
	upperWallModel = glm::scale(upperWallModel, glm::vec3(50.0f,1.0f,1.0f));	
	leftWallModel = glm::translate(leftWallModel, glm::vec3(49.0f,20.0f,0.0f));
    leftWallModel = glm::scale(leftWallModel, glm::vec3(1.0f,50.0f,1.0f)); 	
	rightWallModel = glm::translate(rightWallModel, glm::vec3(-49.0f,20.0f,0.0f));
    rightWallModel = glm::scale(rightWallModel, glm::vec3(1.0f,50.0f,1.0f));
	floorModel = glm::translate(floorModel, glm::vec3(0.0f,20.0f,1.0f)); 	
	floorModel = glm::scale(floorModel, glm::vec3(50.0f,50.0f,1.0f));	
	padModel = glm::scale(padModel, glm::vec3(6.0f,1.0f,1.0f));
	padModel = glm::translate(padModel, glm::vec3(0.0f,-29.0f,0.0f));
	ballModel[0] = glm::translate(ballModel[0], glm::vec3(0.0f,-27.0f,0.0f));
	ballModel[1] = glm::translate(ballModel[0], glm::vec3(3.0f,-27.0f,0.0f));
	ballModel[2] = glm::translate(ballModel[0], glm::vec3(-3.0f,-27.0f,0.0f));	

	generatelevelBlocks();

	leftWallX = (leftWallModel*glm::vec4(cubeVertices[0].x, cubeVertices[0].y, cubeVertices[0].z, 1.0f)).x;
	rightWallX = (rightWallModel*glm::vec4(cubeVertices[0].x, cubeVertices[0].y, cubeVertices[0].z, 1.0f)).x;
	upperWallY = (upperWallModel*glm::vec4(cubeVertices[0].x, cubeVertices[0].y, cubeVertices[0].z, 1.0f)).y;
	padY = (padModel*glm::vec4(cubeVertices[0].x, cubeVertices[0].y, cubeVertices[0].z, 1.0f)).y;

	for(int i=0; i<cubeVertices.size(); i++)
	{
		glm::vec4 vertex = glm::vec4(cubeVertices[i].x, cubeVertices[i].y, cubeVertices[i].z, 1.0f);
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
void drawCube(glm::mat4 mP, glm::mat4 mV, glm::mat4 mM, glm::vec4 color, GLuint tex0, GLuint tex1) {
	glBindVertexArray(vao);

	assignVBOtoAttribute(shaderProgram,"position",bufCubeVertices,3); 	
    assignVBOtoAttribute(shaderProgram,"normal",bufCubeNormals,3);
	assignVBOtoAttribute(shaderProgram, "texCoord0", bufCubeTexCoords, 2);

	glBindVertexArray(0);

	shaderProgram->use();
		
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(mM));	
	glUniform4fv(shaderProgram->getUniformLocation("color"),1, glm::value_ptr(color));
	glUniform4f(shaderProgram->getUniformLocation("lightPos0"), a,b,c,1);
	glUniform4f(shaderProgram->getUniformLocation("lightPos1"), d,e,f,1);	
	glUniform1i(shaderProgram->getUniformLocation("textureMap0"), 0);
	glUniform1i(shaderProgram->getUniformLocation("textureMap1"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);			
	
	glBindVertexArray(vao);
	
	glDrawArrays(GL_TRIANGLES,0,cubeVertices.size());
		
	glBindVertexArray(0);
}
void drawSphere(glm::mat4 mP, glm::mat4 mV, glm::mat4 mM, glm::vec4 color, GLuint tex0, GLuint tex1) {
	glBindVertexArray(vao);

	assignVBOtoAttribute(shaderProgram,"position",bufSphereVertices,3); 	
    assignVBOtoAttribute(shaderProgram,"normal",bufSphereNormals,3);
	assignVBOtoAttribute(shaderProgram, "texCoord0", bufSphereTexCoords, 2);

	glBindVertexArray(0);
		
	shaderProgram->use();
		
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(mM));	
	glUniform4fv(shaderProgram->getUniformLocation("color"),1, glm::value_ptr(color));
	glUniform4f(shaderProgram->getUniformLocation("lightPos0"), a,b,c,1);
	glUniform4f(shaderProgram->getUniformLocation("lightPos1"), d,e,f,1);	
	glUniform1i(shaderProgram->getUniformLocation("textureMap0"), 0);
	glUniform1i(shaderProgram->getUniformLocation("textureMap1"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);			
	
	glBindVertexArray(vao);
	
	glDrawArrays(GL_TRIANGLES,0,sphereVertices.size());
		
	glBindVertexArray(0);
}
void drawScene(GLFWwindow* window, float padDeltaX, float ballDeltaX[], float ballDeltaY[]) {	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			
	padModel = glm::translate(padModel, glm::vec3(padDeltaX,0.0f,0.0f));
	for(int i=0;i<3;i++)
	{
		ballModel[i] = glm::translate(ballModel[i], glm::vec3(ballDeltaX[i],ballDeltaY[i],0.0f));		
	}	

	float padLeftEdgeX = (padModel*glm::vec4(cubeVertices[0].x, cubeVertices[0].y, cubeVertices[0].z, 1.0f)).x;
	float padRightEdgeX = (padModel*glm::vec4(cubeVertices[0].x, cubeVertices[0].y, cubeVertices[0].z, 1.0f)).x;
	for(int i=0; i<cubeVertices.size(); i++)
	{
		glm::vec4 vertex = glm::vec4(cubeVertices[i].x, cubeVertices[i].y, cubeVertices[i].z, 1.0f);
		glm::vec4 padPosition = padModel*vertex;
		if(padPosition.x > padLeftEdgeX)
			padLeftEdgeX = padPosition.x;
		if(padPosition.x < padRightEdgeX)
			padRightEdgeX = padPosition.x;
	}

	float ballLeftEdgeX[3];
	float ballRightEdgeX[3];
	float ballUpperEdgeY[3];
	float ballBottomEdgeY[3];

	for(int j=0; j<3; j++)
	{
		ballLeftEdgeX[j] = (ballModel[j]*glm::vec4(sphereVertices[0].x, sphereVertices[0].y, sphereVertices[0].z, 1.0f)).x;
		ballRightEdgeX[j] = (ballModel[j]*glm::vec4(sphereVertices[0].x, sphereVertices[0].y, sphereVertices[0].z, 1.0f)).x;
		ballUpperEdgeY[j] = (ballModel[j]*glm::vec4(sphereVertices[0].x, sphereVertices[0].y, sphereVertices[0].z, 1.0f)).x;
		ballBottomEdgeY[j] = (ballModel[j]*glm::vec4(sphereVertices[0].x, sphereVertices[0].y, sphereVertices[0].z, 1.0f)).x;

		for(int i=0; i<sphereVertices.size(); i++)
		{
			glm::vec4 vertex = glm::vec4(cubeVertices[i].x, cubeVertices[i].y, cubeVertices[i].z, 1.0f);		
			glm::vec4 ballPosition = ballModel[j]*vertex;
							
			if(ballPosition.x > ballLeftEdgeX[j])
				ballLeftEdgeX[j] = ballPosition.x;
			if(ballPosition.x < ballRightEdgeX[j])
				ballRightEdgeX[j] = ballPosition.x;
			if(ballPosition.y > ballUpperEdgeY[j])
				ballUpperEdgeY[j] = ballPosition.y;
			if(ballPosition.y < ballBottomEdgeY[j])
				ballBottomEdgeY[j] = ballPosition.y;
		}	
	}	
	/* Zderzenia paletki ze ścianami bocznymi */
	if(padLeftEdgeX > leftWallX || padRightEdgeX < rightWallX)
	{		
		padModel = glm::translate(padModel, glm::vec3(-padDeltaX,0.0f,0.0f));		
	}	

	for(int j=0;j<3;j++)
	{	
		/* Zderzenia piłeczki ze ścianami */
		if(ballLeftEdgeX[j] > leftWallX || ballRightEdgeX[j] < rightWallX)
		{
			ballModel[j] = glm::translate(ballModel[j], glm::vec3(-ballDeltaX[j],0.0f,0.0f));	
			ballVelocityX[j] = -ballVelocityX[j];	
		}
		if(ballUpperEdgeY[j] > upperWallY)
		{
			ballModel[j] = glm::translate(ballModel[j], glm::vec3(0.0f,-ballDeltaY[j],0.0f));
			ballVelocityY[j] = -ballVelocityY[j];
		}
		/* Zderzenia piłeczki z pletką */
		if(ballBottomEdgeY[j] < padY && ballUpperEdgeY[j] > padY)
		{
			if(ballRightEdgeX[j] < padLeftEdgeX && ballLeftEdgeX[j] > padRightEdgeX)
			{
				float ballMiddle = (ballLeftEdgeX[j] + ballRightEdgeX[j])/2;
				float factor = ((ballMiddle - padRightEdgeX) / (padLeftEdgeX - padRightEdgeX))*2 - 1;
				factor = roundf(factor * 100) / 100;			
										
				ballVelocityY[j] = -ballVelocityY[j];		
				ballVelocityX[j] = ballVelocityY[j] * factor;	

				ballModel[j] = glm::translate(ballModel[j], glm::vec3(0.0f,-ballDeltaY[j],0.0f));
			}
		}	
		/* Zderzenia piłeczki z przeszkodami */
		for(int i=0; i<levelColumns * levelRows; i++)
		{
			if(levelBlocks[i]->canDraw())
			{
				if(ballBottomEdgeY[j] < levelBlocks[i]->getUpperEdgeY() && ballUpperEdgeY[j] > levelBlocks[i]->getBottomEdgeY())
				{	
					if(ballRightEdgeX[j] < levelBlocks[i]->getLeftEdgeX() && ballLeftEdgeX[j] > levelBlocks[i]->getRightEdgeX())
					{					
						float delta = levelBlocks[i]->getLeftEdgeX() - ballRightEdgeX[j];
						if(ballLeftEdgeX[j] - levelBlocks[i]->getRightEdgeX() < delta)
							delta = ballLeftEdgeX[j] - levelBlocks[i]->getRightEdgeX();					

						if(delta > 1)
						{
							ballModel[j] = glm::translate(ballModel[j], glm::vec3(0.0f,-ballDeltaY[j],0.0f));
							ballVelocityY[j] = -ballVelocityY[j];
						}
						else
						{
							ballModel[j] = glm::translate(ballModel[j], glm::vec3(-ballDeltaX[j],0.0f,0.0f));
							ballVelocityX[j] = -ballVelocityX[j];
						}
						
						levelBlocks[i]->hit();
						break;
					}
				}			
			}
		}	
	}

	for(int i=0;i<3;i++)
	{
		drawSphere(P,V,ballModel[i], glm::vec4(1.0f,0.0f,1.0f,1.0f),wall0, wall1);	
	}	
	drawCube(P,V, padModel, glm::vec4(1.0f,0.0f,0.0f,1.0f),pad0, pad1);
    drawCube(P,V,upperWallModel, glm::vec4(1.0f,1.0f,0.0f,1.0f),wall0, wall1);	 
    drawCube(P,V,leftWallModel, glm::vec4(1.0f,1.0f,0.0f,1.0f),wall0, wall1);	
    drawCube(P,V,rightWallModel, glm::vec4(1.0f,1.0f,0.0f,1.0f),wall0, wall1);
	drawCube(P,V,floorModel, glm::vec4(0.0f,1.0f,1.0f,1.0f),floor0, floor1);

	for(int i=0; i<levelColumns * levelRows; i++)
	{
		if(levelBlocks[i]->canDraw())
		{			
			drawCube(P,V,levelBlocks[i]->getModel(), levelBlocks[i]->getColor(), block0, block1);			
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
	float ballDeltaX[] = {0,0,0};     
	float ballDeltaY[] = {0,0,0}; 	    		

    while(!glfwWindowShouldClose(window))
    {        			
		padDeltaX = glfwGetTime() * padVelocityX;
		for(int i=0;i<3;i++)
		{	
			ballDeltaX[i] = glfwGetTime() * ballVelocityX[i];		
			ballDeltaY[i] = glfwGetTime() * ballVelocityY[i];	
		}	

        glfwSetTime(0);

        drawScene(window,padDeltaX,ballDeltaX, ballDeltaY);		

        glfwPollEvents();
    }

    freeOpenGLProgram();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}