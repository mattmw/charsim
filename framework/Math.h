/*
 * Math.h
 *
 * Helper class for mathematics relating to animating a 3D environment.
 *
 *      Author: MMW
 */
#ifndef MATH_H_
#define MATH_H_

#include "framework/core.h"

class Math
{
public:
    Math();

    ~Math();

    /*
     * Convert a Field of View angle into frustrum scale for perspective
     * matrix.
     */
    static float frustumScaleFOV(const float fovDeg);

    /*
     * Return quaternion needed to rotate one vertex, v1, to match another
     * vertex, v2, around a specified origin, o.
     */
    static Quat rotation(Vec3 v1, Vec3 v2);
    static Quat rotation(Vec3 v1, Vec3 v2, Vec3 origin);

    static float smoothstep(const float t);
    static float smoothstepRatio(const float t, const float r);
};

#endif /* MATH_H_ */
