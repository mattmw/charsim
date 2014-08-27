/*
 * Buffers.h
 *
 * OpenGL Buffer Object data types.
 * To be defined by Meshes and supplied to the Renderer by model nodes.
 *
 *      Author: MMW
 */
#ifndef BUFFERS_H_
#define BUFFERS_H_

/* Vertex array attribute locations for shader layout locations. */
/* (Not used for glsl version 120 shaders.) */
#define VERTEX_LOC		0
#define NORMAL_LOC		1
#define UV_LOC			2
#define BONE_ID_LOC		3
#define BONE_WEIGHT_LOC	4
#define MORPH0_POS_LOC  5
#define MORPH0_NORM_LOC 6
#define MORPH1_POS_LOC  7
#define MORPH1_NORM_LOC 8

struct VertexBuffer
{
    GLuint _vertexBuffer;
    GLuint _normalBuffer;
    GLuint _UVBuffer;
    GLuint _indexBuffer;
};

struct VertexBoneBuffer
{
    GLuint _skinIndexBuffer;
    GLuint _skinWeightBuffer;
    GLuint _boneBuffer;
};

struct VertexMorphBuffer
{
    GLuint _vertexBuffer;
    GLuint _normalBuffer;
};

#endif /* BUFFERS_H_ */
