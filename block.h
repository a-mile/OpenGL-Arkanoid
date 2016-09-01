#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

class Block {
    private:
        glm::vec4 color;
        glm::mat4 model;
        int strength;
    public:
        Block(int strength = 3, glm::mat4 model = glm::mat4(1.0f));
        glm::mat4 getModel();
        glm::vec4 getColor();
        bool canDraw();
        void hit();
};

#endif