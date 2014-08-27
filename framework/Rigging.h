/*
 * Rigging.h
 *
 *      Author: MMW
 */
#ifndef RIGGING_H_
#define RIGGING_H_

struct SkinIndices
{
    uint _boneIndex1;
    uint _boneIndex2;
    uint _boneIndex3;
    uint _boneIndex4;
};

struct SkinWeights
{
    float _boneWeight1;
    float _boneWeight2;
    float _boneWeight3;
    float _boneWeight4;
};

/* Pose keyframes to be interpolated between for animation. */
struct BonePose
{
    float _timestamp;
    Quat _localPoseRotation;
    Vec3 _localPoseTranslation;
};

struct Bone
{
    String _name;
    int _parentIndex;
    /* First pose is binding pose. */
    std::vector<BonePose> _poses;
};

struct IKBone
{
    int _parentIndex;
    /* Mutable pose for IK solver. */
    BonePose _pose;
    float _length;
};

#endif /* RIGGING_H_ */
