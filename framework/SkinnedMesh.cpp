/*
 * SkinnedMesh.cpp
 *
 *      Author: MMW
 */
#include "SkinnedMesh.h"

SkinnedMesh::SkinnedMesh(String meshId)
        : Mesh(meshId),
	      _boneCount{0},
          _poseCount{0}
{
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::setBoneChain(std::vector<Bone> bones)
{
    _boneChain = bones;
    _boneCount = bones.size();
    _poseCount = bones[0]._poses.size();
}

const std::vector<Bone>& SkinnedMesh::getBoneChain() const
{
    return _boneChain;
}

std::vector<Bone>* SkinnedMesh::getBoneChainPtr()
{
    return &_boneChain;
}

void SkinnedMesh::setSkinIndices(std::vector<SkinIndices> skinIndices)
{
    _skinIndices = skinIndices;
}

const std::vector<SkinIndices> SkinnedMesh::getSkinIndices() const
{
    return _skinIndices;
}

void SkinnedMesh::setSkinWeights(std::vector<SkinWeights> skinWeights)
{
    _skinWeights = skinWeights;
}

const std::vector<SkinWeights> SkinnedMesh::getSkinWeights() const
{
    return _skinWeights;
}

const BonePose& SkinnedMesh::getArmatureBindPose() const
{
    return _boneChain[0]._poses[0];
}

void SkinnedMesh::loadSkinnedVertexBuffers()
{
    /*
     * Create and bind bone hierarchy buffer.
     * Renderer must define VAO to read struct.
     */
    glGenBuffers(1, &_vertexBoneBuffer._boneBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBoneBuffer._boneBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            _boneChain.size() * sizeof(_boneChain[0]),
            &_boneChain[0],
            GL_STATIC_DRAW);

    glGenBuffers(1, &_vertexBoneBuffer._skinIndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBoneBuffer._skinIndexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            _skinIndices.size() * sizeof(_skinIndices[0]),
            &_skinIndices[0],
            GL_STATIC_DRAW);

    glGenBuffers(1, &_vertexBoneBuffer._skinWeightBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBoneBuffer._skinWeightBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            _skinWeights.size() * sizeof(_skinWeights[0]),
            &_skinWeights[0],
            GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const uint& SkinnedMesh::getBoneCount() const
{
    return _boneCount;
}

const uint& SkinnedMesh::getPoseCount() const
{
    return _poseCount;
}

const VertexBoneBuffer& SkinnedMesh::getVertexBoneBuffer() const
{
    return _vertexBoneBuffer;
}
