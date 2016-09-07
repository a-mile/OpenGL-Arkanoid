#include "block.h"


Block::Block(int strength, glm::mat4 model, std::vector<glm::vec3> vertices)
{
    Block::model = model;
    Block::strength  = strength;
	Block::vertices = vertices;   	
}

void Block::calculateEdges()
{

	Block::leftEdgeX = (Block::model*glm::vec4(Block::vertices[0].x, Block::vertices[0].y, Block::vertices[0].z, 1.0f)).x;	
	Block::rightEdgeX = (Block::model*glm::vec4(Block::vertices[0].x, Block::vertices[0].y, Block::vertices[0].z, 1.0f)).x;
	Block::upperEdgeY = (Block::model*glm::vec4(Block::vertices[0].x, Block::vertices[0].y, Block::vertices[0].z, 1.0f)).y;
	Block::bottomEdgeY = (Block::model*glm::vec4(Block::vertices[0].x, Block::vertices[0].y, Block::vertices[0].z, 1.0f)).y;

	for(int i=0; i<Block::vertices.size(); i++)
	{		
		glm::vec4 vertex = glm::vec4(Block::vertices[i].x, Block::vertices[i].y, Block::vertices[i].z, 1.0f);
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

 		
