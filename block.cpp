#include "block.h"

int vertexCount = 36;

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

Block::Block(int strength, glm::mat4 model)
{
    Block::model = model;
    Block::strength  = strength;   	
}

void Block::calculateEdges()
{

	Block::leftEdgeX = (Block::model*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
		Block::rightEdgeX = (Block::model*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).x;
		Block::upperEdgeY = (Block::model*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).y;
		Block::bottomEdgeY = (Block::model*glm::vec4(vertices[0], vertices[1], vertices[2], vertices[3])).y;

	for(int i=4; i<vertexCount*4 - 3; i+=4)
	{
		

		glm::vec4 vertex = glm::vec4(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]);
		glm::vec4 blockPosition = Block::model*vertex;					
		if(blockPosition.x > Block::leftEdgeX)
			Block::leftEdgeX = blockPosition.x;
		if(blockPosition.x < Block::rightEdgeX)
			Block::rightEdgeX = blockPosition.x;						
		if(blockPosition.y > Block::upperEdgeY)
			Block::upperEdgeY = blockPosition.y;
		if(blockPosition.y < Block::bottomEdgeY)
			Block::bottomEdgeY = blockPosition.y;		
	}		
}

glm::mat4 Block::getModel()
{
    return Block::model;
}

glm::vec4 Block::getColor()
{
    switch (strength)
    {
        case 1:
            return glm::vec4(1,0,0,1);
            break;
        case 2:
            return glm::vec4(0,1,0,1);
            break;       
        default:
            return glm::vec4(0,0,1,1);
            break;
    }
}

void Block::hit()
{
    strength --;
}

bool Block::canDraw()
{
    return strength > 0;
}

float Block::getLeftEdgeX()
{
 return Block::leftEdgeX;
}
        float Block::getRightEdgeX()
        {
            return Block::rightEdgeX;
        }
        float Block::getUpperEdgeY()
        {
            return Block::upperEdgeY;
        }
        float Block::getBottomEdgeY()
        {
            return Block::bottomEdgeY;
        }

 		
