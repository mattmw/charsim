/*
 * Mesh.h
 *
 * Data type for storing mesh vertex data.
 * Member functions support loading vertex data to graphics device and
 * binding it to OpenGL Vertex Buffer Objects.
 *
 *      Author: MMW
 */
#ifndef MESH_H_
#define MESH_H_

#include "framework/core.h"
#include "framework/Buffers.h"

class Mesh
{
public:
    Mesh(String meshId);

    virtual ~Mesh();

    void setJSONVertices(const Vec3Array JSONVertices);
    const Vec3Array getJSONVertices() const;

    void setVertices(const Vec3Array vertices);
    Vec3Array getVertices() const;
    void setNormals(const Vec3Array normals);
    Vec3Array getNormals() const;
    void setUVs(const Vec2Array UVs);
    Vec2Array getUVs() const;
    void setIndices(const IndexArray indices);
    IndexArray getIndices() const;

    const VertexBuffer& getVertexBuffer() const;
    uint getIndexCount() const;

    const uint getVertexCount() const;

    /* Methods for loading and unloading vertices from device memory. */
    void loadVertexBuffers();
    void unloadVertexBuffers() {};

    void setIndexRecord(const std::vector<bool> indexRecord);
    const std::vector<bool> getIndexRecord() const;

    void setPosRecords(const IndexArray vertexPosRecord, const IndexArray normalPosRecord);
    const IndexArray getVertexPosRecord() const;
    const IndexArray getNormalPosRecord() const;

    /* DEBUG */
    void printVertices() const;
    void printUVs() const;
    void generateNormalLines();
    Vec3Array getNormalLines() const;
    GLuint getNormalLinesBuffer() const;
    uint getNormalLinesCount() const;

private:
    std::string _meshId;

    /* Host memory mesh data. */
    Vec3Array _JSONVertices;
    Vec3Array _vertices;
    Vec3Array _normals;
    Vec2Array _UVs;
    IndexArray _indices;
    /* Device memory buffers. */
    VertexBuffer _vertexBuffer;
    /* Vertex counts. */
    uint _vertexCount;
    uint _normalCount;
    uint _UVCount;
    uint _indexCount;

    std::vector<bool> _indexRecord;
    IndexArray _vertexPosRecord, _normalPosRecord;

    /* DEBUG */
    Vec3Array _normalLines;
    GLuint _normalLinesBuffer;
    uint _normalLinesCount;
};

#endif /* MESH_H_ */
