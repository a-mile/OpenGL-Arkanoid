#ifndef BLOCK_H
#define BLOCK_H

#include "gameobject.h"

class Block : public GameObject {
    public:
        Block(GameObjectVertices* vertices, GLuint texture0, GLuint texture1,
            glm::vec3 position, glm::vec3 scale, glm::mat4 view, glm::mat4 perspective, ShaderProgram *shaderProgram);

        bool destroyed;
};

#endif