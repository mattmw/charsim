/*
 * Renderer.cpp
 *
 *      Author: MMW
 */

#include "Renderer.h"

Renderer::Renderer(ResourceManager* resman)
        : _resMan{resman},
          _W{Mat4(1)},
          _V{Mat4(1)},
          _P{Mat4(1)},
          _WVP{Mat4(1)},
          _frustrumScale{0.0f},
          _zNear{0.0f},
          _zFar{0.0f},
          _FOV{0.0f},
          _morphOffset{0}
{
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
    /* Set up debug material. */
    _resMan->createMaterial("DebugMaterial1", std::make_shared<BaseMaterial>(
            "DebugMaterial1",
            ResourceManager::loadShaderFromFile("./shaders/FragPosition120.vs"),
            ResourceManager::loadShaderFromFile("./shaders/FragPosition120.fs")));
    _debugMaterial = std::dynamic_pointer_cast<BaseMaterial>(
            _resMan->getResourceMaterial("DebugMaterial1"));

    /* Create view transformation matrix */
    Vec3 cameraPosition = Vec3(0, 5.0, 12.0);
    Vec3 lookAt = Vec3(0, 5.0, 0);
    Vec3 orientation = Vec3(0, 1, 0);
    _V = glm::lookAt(cameraPosition, lookAt, orientation);
}

void Renderer::createPerspectiveTransformation(float aspectRatio)
{
    /* Field of View angle in degrees */
    _FOV = 36.0f;
    _frustrumScale = Math::frustumScaleFOV(_FOV);
    _zNear = 0.1f;
    _zFar = 100.0f;
    _P[0][0] = _frustrumScale / aspectRatio;
    _P[1][1] = _frustrumScale;
    _P[2][2] = (_zFar + _zNear) / (_zNear - _zFar);
    _P[3][2] = (2 * _zFar * _zNear) / (_zNear - _zFar);
    _P[2][3] = -1.0f;
}

void Renderer::setWorldTransformation(Mat4& W)
{
    _W = W;
}

void Renderer::setPoseTransformations(Mat4Array& globalPoseTransformations)
{
    _globalPoseTransformations = globalPoseTransformations;
}

Mat4 Renderer::getProjectionMatrix() const
{
    return _P;
}

Mat4 Renderer::getViewMatrix() const
{
    return _V;
}

void Renderer::renderAxes()
{
    Vec3Array points;
    points.push_back(Vec3(0, 0, 0));
    points.push_back(Vec3(5, 0, 0));
    setWorldTransformation(_W);
    renderDebug(points, 5.0f);

    points.clear();
    points.push_back(Vec3(0, 0, 0));
    points.push_back(Vec3(0, 5, 0));
    setWorldTransformation(_W);
    renderDebug(points, 5.0f);

    points.clear();
    points.push_back(Vec3(0, 0, 0));
    points.push_back(Vec3(0, 0, 5));
    setWorldTransformation(_W);
    renderDebug(points, 5.0f);
}

void Renderer::renderDebug(Vec3Array points, float pointSize)
{
    glPointSize(pointSize);

    _WVP = _P * _V * _W;

    glUseProgram(_debugMaterial->getProgram());

    GLuint _linesBuffer;
    glGenBuffers(1, &_linesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _linesBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            points.size() * sizeof(Vec3),
            &points[0],
            GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _linesBuffer);
    glVertexAttribPointer(
            0,          /* Attribute */
            3,          /* Size */
            GL_FLOAT,   /* Type */
            GL_FALSE,   /* Is normalised? */
            0,          /* Stride */
            0           /* Array buffer offset */
    );

    GLuint matrixId = glGetUniformLocation(_debugMaterial->getProgram(), "WVP");
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &_WVP[0][0]);

    glDisableVertexAttribArray(VERTEX_LOC);
    glDisableVertexAttribArray(NORMAL_LOC);
    glDisableVertexAttribArray(UV_LOC);
    glDisableVertexAttribArray(BONE_ID_LOC);
    glDisableVertexAttribArray(BONE_WEIGHT_LOC);
    glDisableVertexAttribArray(MORPH0_POS_LOC);
    glDisableVertexAttribArray(MORPH0_NORM_LOC);
    glDisableVertexAttribArray(MORPH1_POS_LOC);
    glDisableVertexAttribArray(MORPH1_NORM_LOC);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::render(uint indexCount,
        const GLuint& program,
        const VertexBuffer& vertexBuffer,
        const GLuint textureBuffer)
{
    _WVP = _P * _V * _W;

    glUseProgram(program);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer._vertexBuffer);
    glEnableVertexAttribArray(VERTEX_LOC);
    glVertexAttribPointer(VERTEX_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer._normalBuffer);
    glEnableVertexAttribArray(NORMAL_LOC);
    glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1i(glGetUniformLocation(program, "myTextureSampler"),
            0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer._UVBuffer);
    glEnableVertexAttribArray(UV_LOC);
    glVertexAttribPointer(UV_LOC, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /* Assign uniforms. */
    glUniformMatrix4fv(glGetUniformLocation(program, "WVP"),
            1,
            GL_FALSE,
            &_WVP[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(program, "W"),
            1,
            GL_FALSE,
            &_W[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(program, "V"),
            1,
            GL_FALSE,
            &_V[0][0]);

    /* Draw using this buffer set of verts (implicit association) */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer._indexBuffer);

    /* Wireframe mode. TODO: Make this a setting as part of Renderer. */
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPointSize(5.0f);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(VERTEX_LOC);
    glDisableVertexAttribArray(NORMAL_LOC);
    glDisableVertexAttribArray(UV_LOC);
    glDisableVertexAttribArray(BONE_ID_LOC);
    glDisableVertexAttribArray(BONE_WEIGHT_LOC);
    glDisableVertexAttribArray(MORPH0_POS_LOC);
    glDisableVertexAttribArray(MORPH0_NORM_LOC);
    glDisableVertexAttribArray(MORPH1_POS_LOC);
    glDisableVertexAttribArray(MORPH1_NORM_LOC);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* Morphing renderer. */
void Renderer::render(uint indexCount,
        const GLuint& program,
        const VertexBuffer& vertexBuffer,
        const VertexMorphBuffer& vertexMorphBuffer,
        const int vertexOffset,
        const float keyFactors[],
        const uint keyIds[],
        const GLuint textureBuffer)
{
    _WVP = _P * _V * _W;

    glUseProgram(program);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint vertexLoc = glGetAttribLocation(program, "vertexPosition");
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer._vertexBuffer);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint normalLoc = glGetAttribLocation(program, "vertexNormal");
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer._normalBuffer);
    glEnableVertexAttribArray(normalLoc);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1i(glGetUniformLocation(program, "myTextureSampler"),
            0);

    GLuint UVLoc = glGetAttribLocation(program, "vertexUV");
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer._UVBuffer);
    glEnableVertexAttribArray(UVLoc);
    glVertexAttribPointer(UVLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /* Bind morph targets based on offsets and vertex counts. */
    /* Blend target 01 */
    _morphOffset = vertexOffset*keyIds[0];
    GLuint morphPosition0Loc = glGetAttribLocation(program, "morphPosition0");
    glBindBuffer(GL_ARRAY_BUFFER, vertexMorphBuffer._vertexBuffer);
    glEnableVertexAttribArray(morphPosition0Loc);
    glVertexAttribPointer(morphPosition0Loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)_morphOffset);
    GLuint morphNormal0Loc = glGetAttribLocation(program, "morphNormal0");
    glBindBuffer(GL_ARRAY_BUFFER, vertexMorphBuffer._normalBuffer);
    glEnableVertexAttribArray(morphNormal0Loc);
    glVertexAttribPointer(morphNormal0Loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)_morphOffset);
    /* Blend target 02 */
    _morphOffset = vertexOffset*keyIds[1];
    GLuint morphPosition1 = glGetAttribLocation(program, "morphPosition1Loc");
    glBindBuffer(GL_ARRAY_BUFFER, vertexMorphBuffer._vertexBuffer);
    glEnableVertexAttribArray(morphPosition1);
    glVertexAttribPointer(morphPosition1, 3, GL_FLOAT, GL_FALSE, 0, (void*)_morphOffset);
    GLuint morphNormal1Loc = glGetAttribLocation(program, "morphNormal1Loc");
    glBindBuffer(GL_ARRAY_BUFFER, vertexMorphBuffer._normalBuffer);
    glEnableVertexAttribArray(morphNormal1Loc);
    glVertexAttribPointer(morphNormal1Loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)_morphOffset);

    /* Assign uniforms. */
    glUniformMatrix4fv(glGetUniformLocation(program, "WVP"),
            1,
            GL_FALSE,
            &_WVP[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(program, "W"),
            1,
            GL_FALSE,
            &_W[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(program, "V"),
            1,
            GL_FALSE,
            &_V[0][0]);

    glUniform1fv(glGetUniformLocation(program, "morphWeight0"),
            1,
            &keyFactors[0]);
    glUniform1fv(glGetUniformLocation(program, "morphWeight1"),
            1,
            &keyFactors[1]);

    /* Draw using this buffer set of verts (implicit association) */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer._indexBuffer);

    /* Wireframe mode. TODO: Make this a setting as part of Renderer. */
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPointSize(5.0f);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(VERTEX_LOC);
    glDisableVertexAttribArray(NORMAL_LOC);
    glDisableVertexAttribArray(UV_LOC);
    glDisableVertexAttribArray(BONE_ID_LOC);
    glDisableVertexAttribArray(BONE_WEIGHT_LOC);
    glDisableVertexAttribArray(MORPH0_POS_LOC);
    glDisableVertexAttribArray(MORPH0_NORM_LOC);
    glDisableVertexAttribArray(MORPH1_POS_LOC);
    glDisableVertexAttribArray(MORPH1_NORM_LOC);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* Morphing and skinning renderer. */
void Renderer::render(uint indexCount,
        const GLuint& program,
        const VertexBuffer& vertexBuffer,
        const VertexBoneBuffer& vertexBoneBuffer,
        const VertexMorphBuffer& vertexMorphBuffer,
        const int vertexOffset,
        const float keyFactors[],
        const uint keyIds[],
        const GLuint textureBuffer)
{
    _WVP = _P * _V * _W;

    glUseProgram(program);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer._vertexBuffer);
    glEnableVertexAttribArray(VERTEX_LOC);
    glVertexAttribPointer(VERTEX_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer._normalBuffer);
    glEnableVertexAttribArray(NORMAL_LOC);
    glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1i(glGetUniformLocation(program, "myTextureSampler"),
            0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer._UVBuffer);
    glEnableVertexAttribArray(UV_LOC);
    glVertexAttribPointer(UV_LOC, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /* Attempt to bind a skin index. */
    glBindBuffer(GL_ARRAY_BUFFER, vertexBoneBuffer._skinIndexBuffer);
    glEnableVertexAttribArray(BONE_ID_LOC);
    glVertexAttribIPointer(BONE_ID_LOC, 4, GL_INT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBoneBuffer._skinWeightBuffer);
    glEnableVertexAttribArray(BONE_WEIGHT_LOC);
    glVertexAttribPointer(BONE_WEIGHT_LOC, 4, GL_FLOAT, GL_FALSE, 0, 0);

    /* Bind morph targets based on offsets and vertex counts. */
    /* Blend target 01 */
    _morphOffset = vertexOffset*keyIds[0];
    glBindBuffer(GL_ARRAY_BUFFER, vertexMorphBuffer._vertexBuffer);
    glEnableVertexAttribArray(MORPH0_POS_LOC);
    glVertexAttribPointer(MORPH0_POS_LOC, 3, GL_FLOAT, GL_FALSE, 0, (void*)_morphOffset);
    glBindBuffer(GL_ARRAY_BUFFER, vertexMorphBuffer._normalBuffer);
    glEnableVertexAttribArray(MORPH0_NORM_LOC);
    glVertexAttribPointer(MORPH0_NORM_LOC, 3, GL_FLOAT, GL_FALSE, 0, (void*)_morphOffset);
    /* Blend target 02 */
    _morphOffset = vertexOffset*keyIds[1];
    glBindBuffer(GL_ARRAY_BUFFER, vertexMorphBuffer._vertexBuffer);
    glEnableVertexAttribArray(MORPH1_POS_LOC);
    glVertexAttribPointer(MORPH1_POS_LOC, 3, GL_FLOAT, GL_FALSE, 0, (void*)_morphOffset);
    glBindBuffer(GL_ARRAY_BUFFER, vertexMorphBuffer._normalBuffer);
    glEnableVertexAttribArray(MORPH1_NORM_LOC);
    glVertexAttribPointer(MORPH1_NORM_LOC, 3, GL_FLOAT, GL_FALSE, 0, (void*)_morphOffset);

    /* Assign uniforms. */
    glUniformMatrix4fv(glGetUniformLocation(program, "WVP"),
            1,
            GL_FALSE,
            &_WVP[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(program, "W"),
            1,
            GL_FALSE,
            &_W[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(program, "V"),
            1,
            GL_FALSE,
            &_V[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(program, "boneTransformations"),
            TRANSFORMATIONS,
            GL_FALSE,
            &_globalPoseTransformations[0][0][0]);

    glUniform1fv(glGetUniformLocation(program, "morphWeight0"),
            1,
            &keyFactors[0]);
    glUniform1fv(glGetUniformLocation(program, "morphWeight1"),
            1,
            &keyFactors[1]);

    /* Draw using this buffer set of verts (implicit association) */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer._indexBuffer);

    /* Wireframe mode. TODO: Make this a setting as part of Renderer. */
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPointSize(5.0f);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(VERTEX_LOC);
    glDisableVertexAttribArray(NORMAL_LOC);
    glDisableVertexAttribArray(UV_LOC);
    glDisableVertexAttribArray(BONE_ID_LOC);
    glDisableVertexAttribArray(BONE_WEIGHT_LOC);
    glDisableVertexAttribArray(MORPH0_POS_LOC);
    glDisableVertexAttribArray(MORPH0_NORM_LOC);
    glDisableVertexAttribArray(MORPH1_POS_LOC);
    glDisableVertexAttribArray(MORPH1_NORM_LOC);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
