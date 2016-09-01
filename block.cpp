#include "block.h"

Block::Block(int strength, glm::mat4 model)
{
    Block::model = model;
    Block::strength  = strength;
    

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