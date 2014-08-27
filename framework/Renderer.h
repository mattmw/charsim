/*
 * Renderer.h
 *
 * Called by nodes in scenegraph each frame which select the buffer objects
 * to render with.
 *
 *      Author: MMW
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "framework/core.h"
#include "framework/Buffers.h"
#include "framework/Math.h"
#include "framework/ResourceManager.h"
#include "materials/BaseMaterial.h"

class Renderer
{
public:
    Renderer(ResourceManager* resMan);

    virtual ~Renderer();

    void init();

    void setWorldTransformation(Mat4& W);

    /* Create view transformation matrix. */
    void createPerspectiveTransformation(float aspectRatio);

    /* For a mobile camera - To be set by a CameraNode. */
    void setCameraTransformation(Mat4& W) {};

    /*
     * Skinning.
     */
    /* Set array of matrices to transform each bone-weighted vertex group with. */
    void setPoseTransformations(Mat4Array& globalPoseTransformations);

    Mat4 getProjectionMatrix() const;
    Mat4 getViewMatrix() const;

    /* Model renderer. */
    void render(uint indexCount,
            const GLuint& program,
            const VertexBuffer& vertexBuffer,
            const GLuint textureBuffer);

    /* Morphing model renderer. */
    void render(uint indexCount,
            const GLuint& program,
            const VertexBuffer& vertexBuffer,
            const VertexMorphBuffer& vertexMorphBuffer,
            const int vertexOffset,
            const float keyFactors[],
            const uint keyIds[],
            const GLuint textureBuffer);

    /* Skinned and morphing model renderer. */
    void render(uint indexCount,
            const GLuint& program,
            const VertexBuffer& vertexBuffer,
            const VertexBoneBuffer& vertexBoneBuffer,
            const VertexMorphBuffer& vertexMorphBuffer,
            const int vertexOffset,
            const float keyFactors[],
            const uint keyIds[],
            const GLuint textureBuffer);

    /* Debug renderer for normals and lines. */
    void renderDebug(Vec3Array points, float pointSize);
    void renderAxes();

    void shutdown() {};

private:
    ResourceManager* _resMan;
    std::shared_ptr<BaseMaterial> _debugMaterial;

    /*
     * Matrix transformations.
     */
    /* Model-to-World transformation matrix. */
    Mat4 _W;
    /* View transformation matrix. */
    Mat4 _V;
    /* Projection transformation matrix. */
    Mat4 _P;
    /* World-View-Projection matrix. */
    Mat4 _WVP;

    /* Viewing frustum information. */
    float _frustrumScale;
    float _zNear;
    float _zFar;
    float _FOV;

    /* Skinning. */
    Mat4Array _globalPoseTransformations;

    /* Morph values. */
    int _morphOffset;
};

#endif /* RENDERER_H_ */
