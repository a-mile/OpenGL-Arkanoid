#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

class Block {
    private:
        std::vector<glm::vec3> vertices;
        glm::vec4 color;
        glm::mat4 model;
        int strength;
        float leftEdgeX;
        float rightEdgeX;
        float upperEdgeY;
        float bottomEdgeY;        
    public:
        Block(int strength, glm::mat4 model, std::vector<glm::vec3> vertices);
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