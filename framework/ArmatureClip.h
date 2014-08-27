/*
 * ArmatureClip.h
 *
 * Data for armature-based animation (such as limb and spine movement)
 * Each bone chain represents a state of the skeleton which is to be
 * interpolated with the next state by ClipRunner.
 *
 * The duration is defined by animated model file.
 *
 *      Author: MMW
 */
#ifndef ARMATURECLIP_H_
#define ARMATURECLIP_H_

#include "framework/Rigging.h"

struct ArmatureClip
{
    String _armatureClipId;
    float _duration;
    std::vector<Bone> _boneChain;
};

#endif /* ARMATURECLIP_H_ */
