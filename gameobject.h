#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "tools.h"
#include "shaderprogram.h"
#include "gameobjectvertices.h"

class GameObject
{
    protected:
        glm::vec3 position;
        glm::vec3 scale;
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 perspective;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;        
        ShaderProgram *shaderProgram;
        GLuint bufVertices; 
        GLuint bufNormals;
        GLuint bufTexCoords;
        GLuint texture0;
        GLuint texture1;
        GLuint vao;

        void CalculateEdges();
        void CalculateModel();
    public:
        float leftEdgeX;
        float rightEdgeX;
        float upperEdgeY;
        float bottomEdgeY;
        bool show;

        GameObject(GameObjectVertices* gameObjectVertices, GLuint texture0, GLuint texture1,
         glm::vec3 position, glm::vec3 scale, glm::mat4 view, glm::mat4 perspective, ShaderProgram *shaderProgram);
        void DrawObject();
        void MoveHorizontal(float delta);
        void MoveVertical(float delta);
};

#endif