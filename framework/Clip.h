/*
 * framework/Clip.h
 *
 * Clip attributes are defined by model and controller nodes in order to
 * tell ClipRunner what clip to play and how to play it.
 *
 *      Author: MMW
 */
#ifndef CLIP_H_
#define CLIP_H_

/* Clip types */
#define MORPHCLIP 0
#define ARMATURECLIP 1
#define MORPHCYCLECLIP 2

struct Clip
{
    String clipId;
    /* Start of animation in game time. */
    float tStart;
    /*
     * Factor to multiply clip duration with.
     * e.g.: 0.5 - double speed; 2.0 - half speed, etc.
     */
    float tWarp;
    /* Maximum ratio for blending morph with (not used for armature clips). */
    float max = 1.0f;
    /* Number of times to play clip in a row. */
    ushort nPlays;
    /* MORPHCLIP, ARMATURECLIP, MORPHCYCLECLIP. */
    ushort clipType;
};

#endif /* CLIP_H_ */
