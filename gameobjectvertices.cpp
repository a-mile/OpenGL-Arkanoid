#include "gameobjectvertices.h"

GameObjectVertices::GameObjectVertices(char* objFileName)
{
    loadOBJ(objFileName, vertices, texcoords, normals);

    bufVertices = makeBuffer(&vertices[0], vertices.size(), sizeof(float) * 3);		
    bufNormals = makeBuffer(&normals[0], normals.size(), sizeof(float) * 3);  
	bufTexCoords = makeBuffer(&texcoords[0], texcoords.size(), sizeof(float) * 2);

    glGenVertexArrays(1,&vao);
}