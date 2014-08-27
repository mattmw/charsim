/*
 * SkinnedMesh.h
 *
 *      Author: MMW
 */
#ifndef SKINNEDMESH_H_
#define SKINNEDMESH_H_

#include "framework/Mesh.h"
#include "framework/Rigging.h"

class SkinnedMesh : public Mesh
{
public:
    SkinnedMesh(String meshId);

    virtual ~SkinnedMesh();

    void setBoneChain(std::vector<Bone> bones);
    const std::vector<Bone>& getBoneChain() const;
    std::vector<Bone>* getBoneChainPtr();
    void setSkinIndices(std::vector<SkinIndices> skinIndices);
    const std::vector<SkinIndices> getSkinIndices() const;
    void setSkinWeights(std::vector<SkinWeights> skinWeights);
    const std::vector<SkinWeights> getSkinWeights() const;

    const BonePose& getArmatureBindPose() const;

    const VertexBoneBuffer& getVertexBoneBuffer() const;

    const uint& getBoneCount() const;
    const uint& getPoseCount() const;

    void loadSkinnedVertexBuffers();
    void unloadSkinnedVertexBuffers() {};

private:
    std::vector<SkinWeights>  _skinWeights;
    std::vector<SkinIndices> _skinIndices;
    std::vector<Bone> _boneChain;

    uint _boneCount;
    uint _poseCount;

    /* Device memory buffers. */
    VertexBoneBuffer _vertexBoneBuffer;

};

#endif /* SKINNEDMESH_H_ */
