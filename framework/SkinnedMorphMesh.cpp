/*
 * SkinnedMorphMesh.cpp
 *
 *      Author: MMW
 */
#include "SkinnedMorphMesh.h"

SkinnedMorphMesh::SkinnedMorphMesh(String meshId)
        : SkinnedMesh(meshId),
          _vertexOffset{0}
{
}

SkinnedMorphMesh::~SkinnedMorphMesh()
{
}

void SkinnedMorphMesh::setMorphVertices(const Vec3Array vertices)
{
    _morphVertices = vertices;
}

const Vec3Array SkinnedMorphMesh::getMorphVertices()
{
    return _morphVertices;
}

void SkinnedMorphMesh::setMorphNormals(const Vec3Array normals)
{
    _morphNormals = normals;
}

const Vec3Array SkinnedMorphMesh::getMorphNormals()
{
    return _morphNormals;
}

void SkinnedMorphMesh::setVertexOffset(const int vertexOffset)
{
    _vertexOffset = vertexOffset;
}

const uint SkinnedMorphMesh::getVertexOffset()
{
    return _vertexOffset;
}

void SkinnedMorphMesh::loadMorphVertexBuffers()
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

const VertexMorphBuffer& SkinnedMorphMesh::getMorphVertexBuffer() const
{
    return _vertexMorphBuffer;
}

void SkinnedMorphMesh::printVertices()
{
    for (uint i = 0; i < _morphVertices.size(); i++)
    {
        cout<<glm::to_string(_morphVertices[i])<<endl;
    }
}
