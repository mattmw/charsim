/*
 * MorphClip.h
 *
 * Data for vertex morphing-based animation (such as facial expressions).
 * _keyFac is the ratio of interpolation between the base mesh and one of its
 * morph meshes. It is factored with each morph vertex in the vertex shader.
 *
 * _keyId links the morph factor with a morph mesh. Each morph mesh is given
 * a unique Id so that the Renderer can determine which morph meshes should
 * be used to blend with the base mesh.
 * There can be any number of morph meshes defined, but only two can be sent
 * to the Renderer for any particular draw. This can be easily upgraded
 * to five morphs, but no more because it would exceed the common limit of 16
 * vec4 shader locations.
 *
 * The duration is assumed to be 1 second, so it can be uniformly altered
 * by the warp factor in the clip's attributes (Clip).
 *
 *      Author: MMW
 */
#ifndef MORPHCLIP_H_
#define MORPHCLIP_H_

struct Morph
{
    float _keyFac;
    uint _keyId;
};

#endif /* MORPHCLIP_H_ */
