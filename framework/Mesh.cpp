/*
 * Mesh.cpp
 *
 *      Author: MMW
 */
#include "Mesh.h"

Mesh::Mesh(String meshId)
        : _meshId{meshId},
          _vertexCount{0},
          _normalCount{0},
          _UVCount{0},
          _indexCount{0},
          _normalLinesBuffer{0},
          _normalLinesCount{0}
{
}

Mesh::~Mesh()
{
#if DEBUG==true
    if (_meshId == "")
        printf("Destructing a temporary mesh.\n");
    else
        printf("Destructing a mesh of ID: %s\n", _meshId.c_str());
#endif
}

void Mesh::setVertices(Vec3Array vertices)
{
    _vertices = vertices;
    _vertexCount = vertices.size();
}

Vec3Array Mesh::getVertices() const
{
    return _vertices;
}

void Mesh::setIndices(IndexArray indices)
{
    _indices = indices;
    _indexCount = indices.size();
}

IndexArray Mesh::getIndices() const
{
    return _indices;
}

void Mesh::setNormals(Vec3Array normals)
{
    _normals = normals;
    _normalCount = normals.size();
}

Vec3Array Mesh::getNormals() const
{
    return _normals;
}

void Mesh::setUVs(const Vec2Array UVs)
{
    _UVs = UVs;
}

Vec2Array Mesh::getUVs() const
{
    return _UVs;
}

void Mesh::printVertices() const
{
    std::for_each(_vertices.begin(), _vertices.end(), [] (Vec3 vertex)
    {
        printf("%f, %f, %f\n", vertex.x, vertex.y, vertex.z);
    });
    printf("Number of vertices: %d\n", (int)_vertices.size());
}

void Mesh::printUVs() const
{
    for (uint i = 0; i < _vertices.size(); i++)
    {
        printf("UV: %f, %f\n", _UVs[i].x, _UVs[i].y);
    }
    printf("Number of UVs: %d\n", (int)_UVs.size());
    cout<<endl;
}

void Mesh::loadVertexBuffers()
{
    /* Create and bind vertex buffer. */
    glGenBuffers(1, &_vertexBuffer._vertexBuffer);
    /* Bind so that context switches to vertex buffer. */
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer._vertexBuffer);
    /* Load vertices into device memory. */
    glBufferData(GL_ARRAY_BUFFER,
            _vertices.size() * sizeof(Vec3),
            &_vertices[0],
            GL_STATIC_DRAW);

    /* Create and bind normal buffer. */
    glGenBuffers(1, &_vertexBuffer._normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer._normalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            _normals.size() * sizeof(Vec3),
            &_normals[0],
            GL_STATIC_DRAW);

    /* Create and bind UV buffer. */
    glGenBuffers(1, &_vertexBuffer._UVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer._UVBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            _UVs.size() * sizeof(Vec2),
            &_UVs[0],
            GL_STATIC_DRAW);

    /* Create and bind index buffer. */
    glGenBuffers(1, &_vertexBuffer._indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexBuffer._indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            _indices.size() * sizeof(Index),
            &_indices[0],
            GL_STATIC_DRAW);

    /* Release binding. */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const VertexBuffer& Mesh::getVertexBuffer() const
{
    return _vertexBuffer;
}

uint Mesh::getIndexCount() const
{
    return _indexCount;
}

const uint Mesh::getVertexCount() const
{
    return _vertexCount;
}

void Mesh::generateNormalLines()
{
    Vec3 point1;
    Vec3 point2;

    for (uint i = 0; i < _vertices.size(); i++)
    {
        point1 = _vertices[i];
        point2 = _vertices[i] + ( _normals[i] * 0.3f );

        _normalLines.push_back(point1);
        _normalLines.push_back(point2);
    }

    _normalLinesCount = _normalLines.size();
}

Vec3Array Mesh::getNormalLines() const
{
    return _normalLines;
}

void Mesh::setJSONVertices(const Vec3Array JSONVertices)
{
    _JSONVertices = JSONVertices;
}

const Vec3Array Mesh::getJSONVertices() const
{
    return _JSONVertices;
}

GLuint Mesh::getNormalLinesBuffer() const
{
    return _normalLinesBuffer;
}

uint Mesh::getNormalLinesCount() const
{
    return _normalLinesCount;
}

void Mesh::setIndexRecord(const std::vector<bool> indexRecord)
{
    _indexRecord = indexRecord;
}

const std::vector<bool> Mesh::getIndexRecord() const
{
    return _indexRecord;
}

void Mesh::setPosRecords(const IndexArray vertexPosRecord,
        const IndexArray normalPosRecord)
{
    _vertexPosRecord = vertexPosRecord;
    _normalPosRecord = normalPosRecord;
}

const IndexArray Mesh::getVertexPosRecord() const
{
    return _vertexPosRecord;
}

const IndexArray Mesh::getNormalPosRecord() const
{
    return _normalPosRecord;
}
