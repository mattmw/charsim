/*
 * CollisionDetection.h
 *
 * Helper class for collision detection.
 *
 *      Author: MMW
 */
#ifndef COLLISIONDETECTION_H_
#define COLLISIONDETECTION_H_

#include "framework/core.h"

class CollisionDetection
{
public:
    /* Bezier curve - Armature intersection tests. */
    static bool prelimLineSphereTest(const Vec3& p1,
            const Vec3& p2,
            const Vec3& p3,
            const float& r);
    static glm::vec3 lineSphereTest(const Vec3& p1,
            const Vec3& p2,
            const Vec3& p3,
            const float& r);

private:
    /* Quadratic equation functions. */
    /* Negative root. */
    static float quadraticRoot1(const float& a, const float& b, const float& c);
    /* Positive root. */
    static float quadraticRoot2(const float& a, const float& b, const float& c);
    static float magnitude(const Vec3 p1, const Vec3 p2);
};

#endif /* COLLISIONDETECTION_H_ */
