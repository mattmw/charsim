/*
 * MorphMesh.cpp
 *
 *      Author: MMW
 */
#include "MorphMesh.h"

MorphMesh::MorphMesh(String meshId)
        : Mesh(meshId),
          _vertexOffset{0}
{
}

MorphMesh::~MorphMesh()
{
}

void MorphMesh::setMorphVertices(const Vec3Array vertices)
{
    _morphVertices = vertices;
}

Vec3Array MorphMesh::getMorphVertices() const
{
    return _morphVertices;
}

void MorphMesh::setMorphNormals(const Vec3Array normals)
{
    _morphNormals = normals;
}

Vec3Array MorphMesh::getMorphNormals() const
{
    return _morphNormals;
}

void MorphMesh::setVertexOffset(const int vertexOffset)
{
    _vertexOffset = vertexOffset;
}

uint MorphMesh::getVertexOffset() const
{
    return _vertexOffset;
}

void MorphMesh::loadMorphVertexBuffers()
{
    glGenBuffers(1, &_vertexMorphBuffer._vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexMorphBuffer._vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            _morphVertices.size() * sizeof(Vec3),
            &_morphVertices[0],
            GL_STATIC_DRAW);

    glGenBuffers(1, &_vertexMorphBuffer._normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexMorphBuffer._normalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            _morphNormals.size() * sizeof(Vec3),
            &_morphNormals[0],
            GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const VertexMorphBuffer& MorphMesh::getMorphVertexBuffer() const
{
    return _vertexMorphBuffer;
}

void MorphMesh::printVertices()
{
    for (uint i = 0; i < _morphVertices.size(); i++)
    {
        cout<<glm::to_string(_morphVertices[i])<<endl;
    }
}
