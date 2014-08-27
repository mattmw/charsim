/*
 * InverseKinematics.cpp
 *
 *      Author: MMW
 */
/*
#include "InverseKinematics.h"

InverseKinematics::InverseKinematics()
{
}

InverseKinematics::~InverseKinematics()
{
}

void DragonBustNode::genIKMatrices()
{
    Vec3 toTarget;
    Vec3 toTip;
    Vec3 boneAxis;
    Vec3 movement;
    Quat increment;
    float gradient;
    uint effector = 0;

    target = Vec3((10.0f*_input->x)-5.0f, -(10.0f*_input->y)+15.0f, 5.0f);

    if (true)
    {
    // Find most effective bone.
    uint n = _nBones;
    for (uint i = 0; i < n; i++)
    {
        // Find target.
        toTarget = target - _tip;

        // Find bone orientation.
        toTip = _tip - _globalIKChain[i]._pose._localPoseTranslation;

        // Find axis.
        boneAxis = glm::cross(toTarget, toTip);
        boneAxis = glm::normalize(boneAxis);

        movement = glm::cross(toTip, boneAxis);

        gradient = glm::dot(movement, toTarget);

        // Set active effector bone index.
        if (gradient > 1.0f)
        {
            effector = i;

            //increment = glm::angleAxis(50.0f*(*_tDelta), -bone_axis);
            increment = glm::angleAxis(10.0f*(*_tDelta), -boneAxis);

            Vec3 angleIncrement = glm::radians(glm::eulerAngles(increment));

            increment=Quat(angleIncrement);

            // Update bones down the chain of the effector bone.
            Vec3 origin = _globalIKChain[effector]._pose._localPoseTranslation;

            for (uint i = effector; i < n; i++)
            {
                // Update rotations.

                // Update positions (this should not change the position for the effector bone)
                Vec3 v = _globalIKChain[i]._pose._localPoseTranslation;
                v = origin + ((increment) * (v - origin));
                _globalIKChain[i]._pose._localPoseTranslation = v;
            }

            _tip = origin + ((increment) * (_tip - origin));

            Vec3 start = _basisIKChain[effector+1]._pose._localPoseTranslation;
            Vec3 dest = _globalIKChain[effector+1]._pose._localPoseTranslation;

           _globalIKChain[effector]._pose._localPoseRotation = Math::rotation(start, dest);
           _globalIKChain[0]._pose._localPoseRotation = Quat(1, 0, 0, 0);
           _globalIKChain[0]._pose._localPoseRotation =
                   Quat(Vec3(glm::radians(20.0f), 0.0f, 0.0f));
        }

    }

    // Convert global IK chain to local chain for animation matrix gen.

    _localIKChain[0]._pose._localPoseTranslation = _basisIKChain[0]._pose._localPoseTranslation;
    _localIKChain[0]._pose._localPoseRotation = _globalIKChain[0]._pose._localPoseRotation;

    for (uint i = 1; i < n; i++)
    {
        if (i < n)
        {
            _localIKChain[i]._pose._localPoseTranslation = _basisIKChain[i]._pose._localPoseTranslation;
            _localIKChain[i]._pose._localPoseRotation = _globalIKChain[i]._pose._localPoseRotation;
        }
    }
    }

    _renderer->setWorldTransformation(_W);

    // DEBUG: Render to-target line.
    Vec3Array points;
    for (uint i = 0; i < _nBones; i++)
    {
    points.push_back(_globalIKChain[i]._pose._localPoseTranslation);
    if (i < _nBones-1)
        points.push_back(_globalIKChain[i+1]._pose._localPoseTranslation);
    else
        points.push_back(_globalIKChain[i]._pose._localPoseTranslation);
    points.push_back(_globalIKChain[_nBones-1]._pose._localPoseTranslation);
    points.push_back(_tip);
    points.push_back(target);
    points.push_back(target);
    points.push_back(_globalIKChain[0]._pose._localPoseTranslation);
    points.push_back(_globalIKChain[0]._pose._localPoseTranslation+(boneAxis*5.0f));
    }
}
 */
