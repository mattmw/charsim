/*
 * SkinnedMorphMesh.h
 *
 *      Author: MMW
 */
#ifndef SKINNEDMORPHMESH_H_
#define SKINNEDMORPHMESH_H_

#include "framework/SkinnedMesh.h"

class SkinnedMorphMesh : public SkinnedMesh
{
public:
    SkinnedMorphMesh(String meshId);

    virtual ~SkinnedMorphMesh();

    void setMorphVertices(const Vec3Array vertices);
    const Vec3Array getMorphVertices();
    void setMorphNormals(const Vec3Array normals);
    const Vec3Array getMorphNormals();

    /* Define offset for the renderer. */
    void setVertexOffset(const int vertexOffset);
    const uint getVertexOffset();

    void loadMorphVertexBuffers();
    void unloadMorphVertexBuffers() {};

    const VertexMorphBuffer& getMorphVertexBuffer() const;

    void printVertices();

private:
    int _vertexOffset;

    Vec3Array _morphVertices;
    Vec3Array _morphNormals;

    /* Device memory buffers. */
    VertexMorphBuffer _vertexMorphBuffer;
};

#endif /* SKINNEDMORPHMESH_H_ */
