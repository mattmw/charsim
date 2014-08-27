/*
 * ClipRunner.cpp
 *
 *      Author: MMW
 */
#include "ClipRunner.h"

ClipRunner::ClipRunner(float* tGame, float* tDelta)
        : ITime(tGame, tDelta),
          _nClips{0},
          _morphs{nullptr},
          _armatureClips{nullptr},
          _nBones{0},
          _animPoses{nullptr}
{
}

ClipRunner::~ClipRunner()
{
    std::cout << "\nDestructing ClipRunner." << std::endl;
}

void ClipRunner::update()
{
    for (uint i = 0; i < _nClips; i++)
    {
        const Clip& clip = _clips[i];

        float tClip;
        /* Bounding timestamps of each animation step. */
        float lowerLim;
        float upperLim;
        /* Parameter for interpolation between steps. */
        float interval;
        /* Factor for clip duration. */
        float tWarp = clip.tWarp;
        /* Number of times to play clip in a row. */
        ushort nPlays = clip.nPlays;
        /* Start of clip in game time. */
        float tStart = clip.tStart;
        float tAnim = *_tGame - tStart;
        /* Maximum ratio for blending morph with (not used for armature clips). */
        float max = clip.max;

        if (clip.clipType == MORPHCLIP)
        {
            float clipDuration = 1.0f * tWarp;

            /* Initiate animation time. */
            if (*_tGame >= tStart && *_tGame <= tStart + (clipDuration * nPlays))
            {
                lowerLim = 0.0f;
                upperLim = tWarp;

                /* Initiate clip time. */
                tClip = fmod(tAnim, clipDuration);

                if (tClip > lowerLim && tClip < upperLim)
                {
                    /*
                     * Normalise range to find linear interpolation parameter.
                     * [clip_start_tDelta, anim_tDelta, clip_end_tDelta]
                     */
                    upperLim -= lowerLim;
                    interval = (tClip - lowerLim) / upperLim;
                    interval = Math::smoothstepRatio(interval, max);

                    (*_morphs)[clip.clipId]._keyFac = interval;
                }

                std::cout<<"elapsed: "<<*_tGame
                        <<" clipstart: "<<tStart
                        <<" duration: "<<clipDuration
                        << " cliptime: "<<tClip<<std::endl;
            }
        }
        else if (clip.clipType == MORPHCYCLECLIP)
        {
            float clipDuration = 1.0f * tWarp;

            /* Initiate animation time. */
            if (*_tGame >= tStart && *_tGame <= tStart + (clipDuration * nPlays))
            {
                lowerLim = 0.0f;
                upperLim = tWarp / 2.0f;

                /* Initiate clip time. */
                tClip = fmod(tAnim, clipDuration);

                if (tClip > lowerLim && tClip < upperLim)
                {
                    /*
                     * Normalise range to find linear interpolation parameter.
                     * [clip_start_tDelta, anim_tDelta, clip_end_tDelta]
                     */
                    upperLim -= lowerLim;
                    interval = (tClip - lowerLim) / upperLim;
                    interval = Math::smoothstepRatio(interval, max);

                    (*_morphs)[clip.clipId]._keyFac = interval;
                }

                lowerLim = (tWarp / 2.0f);
                upperLim = tWarp;

                if (tClip > lowerLim && tClip < upperLim)
                {
                    /*
                     * Normalise range to find linear interpolation parameter.
                     * [clip_start_tDelta, anim_tDelta, clip_end_tDelta]
                     */
                    upperLim -= lowerLim;
                    interval = (tClip - lowerLim) / upperLim;
                    interval = Math::smoothstepRatio(1-interval, max);

                    (*_morphs)[clip.clipId]._keyFac = interval;
                }

                std::cout<<"elapsed: "<<*_tGame
                        <<" clipstart: "<<tStart
                        <<" duration: "<<clipDuration
                        << " cliptime: "<<tClip<<std::endl;
            }
        }
        else if (clip.clipType == ARMATURECLIP)
        {
            float clipDuration = (*_armatureClips)[clip.clipId]._duration * tWarp;
            uint poseCount;

            /* Pose info to interpolate between. */
            Vec3 startPos;
            Vec3 endPos;
            Quat startRot;
            Quat endRot;

            Mat4 animMat;

            if (*_tGame >= tStart && *_tGame <= tStart + (clipDuration * nPlays))
            {
                for (uint i = 0; i < _nBones; i++)
                {
                    Bone& bone = (*_armatureClips)[clip.clipId]._boneChain[i];
                    poseCount = bone._poses.size();
                    clipDuration = bone._poses[poseCount-1]._timestamp;
                    clipDuration *= tWarp;
                    tClip = fmod(tAnim, clipDuration);

                    /* Find pose index to gather transformation data from. */
                    for (uint i = 0; i < poseCount-1; i++)
                    {
                        lowerLim = bone._poses[i]._timestamp * tWarp;
                        upperLim = bone._poses[i+1]._timestamp * tWarp;

                        if (tClip > lowerLim && tClip < upperLim)
                        {
                            startRot = bone._poses[i]._localPoseRotation;
                            startPos = bone._poses[i]._localPoseTranslation;
                            endRot = bone._poses[i+1]._localPoseRotation;
                            endPos = bone._poses[i+1]._localPoseTranslation;

                            /*
                             * Normalise range to find linear interpolation parameter.
                             * [clip_start_tDelta, anim_tDelta, clip_end_tDelta]
                             */
                            upperLim -= lowerLim;
                            interval = (tClip - lowerLim) / upperLim;
                            interval = Math::smoothstep(interval);

                            Quat rot = glm::mix(startRot, endRot, interval);

                            Mat4 rotMat = glm::mat4_cast(rot);
                            animMat = rotMat;
                        }
                    }
                    (*_animPoses)[i] = animMat;
                }
            }
        }
    }

    /* TODO: Delete finished clips based on iterators. */
}

void ClipRunner::setBoneCount(ushort nBones)
{
    _nBones = nBones;
}

void ClipRunner::addClip(Clip clip)
{
    _clips.push_back(clip);
    _nClips++;
}

void ClipRunner::addMorphClips(std::map<String, Morph>* morphs)
{
    _morphs = morphs;
}

void ClipRunner::addArmatureClips(std::map<String, ArmatureClip>* armatureClips)
{
    _armatureClips = armatureClips;
}

void ClipRunner::addAnimPoses(Mat4Array* animPoses)
{
    _animPoses = animPoses;
}
