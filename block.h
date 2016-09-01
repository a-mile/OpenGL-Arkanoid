#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

class Block {
    private:
        glm::vec4 color;
        glm::mat4 model;
        int strength;
        float leftEdgeX;
        float rightEdgeX;
        float upperEdgeY;
        float bottomEdgeY;        
    public:
        Block(int strength = 3, glm::mat4 model = glm::mat4(1.0f));
        glm::mat4 getModel();
        glm::vec4 getColor();
        bool canDraw();
        void hit();
        float getLeftEdgeX();
        float getRightEdgeX();
        float getUpperEdgeY();
        float getBottomEdgeY();
        void calculateEdges();
};

#endif