/*
 * MorphMesh.h
 *
 *      Author: MMW
 */
#ifndef MORPHMESH_H_
#define MORPHMESH_H_

#include "framework/Mesh.h"

class MorphMesh : public Mesh
{
public:
    MorphMesh(String meshId);

    virtual ~MorphMesh();

    void setMorphVertices(const Vec3Array vertices);
    Vec3Array getMorphVertices() const;
    void setMorphNormals(const Vec3Array normals);
    Vec3Array getMorphNormals() const;

    /* Define offset for the renderer. */
    void setVertexOffset(const int vertexOffset);
    uint getVertexOffset() const;

    void loadMorphVertexBuffers();
    void unloadMorphVertexBuffers() {};

    const VertexMorphBuffer& getMorphVertexBuffer() const;

    /* DEBUG */
    void printVertices();

private:
    int _vertexOffset;

    Vec3Array _morphVertices;
    Vec3Array _morphNormals;

    /* Device memory buffers. */
    VertexMorphBuffer _vertexMorphBuffer;
};

#endif /* MORPHMESH_H_ */
