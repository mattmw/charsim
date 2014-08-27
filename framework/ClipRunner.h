/*
 * ClipRunner.h
 *
 * A helper class created by each model node.
 * It receives clips from the node with timing data and updates the clip
 * states accordingly on each frame cycle.
 *
 *      Author: MMW
 */
#ifndef CLIPRUNNER_H_
#define CLIPRUNNER_H_

#include "framework/core.h"
#include "framework/ITime.h"
#include "framework/Math.h"
#include "framework/Clip.h"
#include "framework/MorphClip.h"
#include "framework/ArmatureClip.h"

class ClipRunner : public ITime
{
public:
    ClipRunner(float* tGame, float* tDelta);

    virtual ~ClipRunner();

    void update();

    /* Setters for animation clip data (provided by model nodes). */
    void addMorphClips(std::map<String, Morph>* morphs);
    void addArmatureClips(std::map<String, ArmatureClip>* armatureClips);
    void addAnimPoses(Mat4Array* animPoses);
    void setBoneCount(ushort nBones);

    void addClip(Clip clip);

private:
    std::vector<Clip> _clips;

    ushort _nClips;

    /* Animation collections set by model nodes each update-draw cycle. */
    /* Morph clip data. */
    std::map<String, Morph>* _morphs;
    /* Armature clip data. */
    std::map<String, ArmatureClip>* _armatureClips;
    ushort _nBones;
    Mat4Array* _animPoses;
};

#endif /* CLIPRUNNER_H_ */
