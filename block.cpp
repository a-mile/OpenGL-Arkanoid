#include "block.h"

Block::Block(GameObjectVertices* vertices, GLuint texture0, GLuint texture1,
            glm::vec3 position, glm::vec3 scale, glm::mat4 view, glm::mat4 perspective, ShaderProgram *shaderProgram) : 
        GameObject(vertices, texture0, texture1,
            position, scale, view, perspective, shaderProgram)
        {
            destroyed = false;
        };