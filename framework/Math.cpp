/*
 * Math.cpp
 *
 *  Created on: 7 Jun 2014
 *      Author: MMW
 */
#include "Math.h"

Math::Math()
{
}

Math::~Math()
{
}

Quat Math::rotation(Vec3 v1, Vec3 v2)
{
    /* Get magnitude of v1 and v2 combined for quicker normalisation. */
    float magv1v2 = sqrt(glm::length2(v1) * glm::length2(v2));
    Vec3 axis = glm::cross(v1, v2);
    Quat q = Quat(magv1v2 + glm::dot(v1, v2),
            axis.x,
            axis.y,
            axis.z);

    /* DEBUG: naive approach */
    // v1 = glm::normalize(v1);
    // v2 = glm::normalize(v2);
    // float angle_cos = glm::dot(v1, v2);
    // glm::vec3 axis = glm::normalize(glm::cross(v1, v2));
    // float angle = acos(angle_cos);
    // return glm::angleAxis(angle, axis);

    return glm::normalize(q);
}

Quat Math::rotation(Vec3 v1, Vec3 v2, Vec3 origin)
{
    /* Set origin. */
    v1 = v1 - origin;
    v2 = v2 - origin;

    /* Get magnitude of v1 and v2 combined for quicker normalisation. */
    float magv1v2 = sqrt(glm::length2(v1) * glm::length2(v2));
    Vec3 axis = glm::cross(v1, v2);
    Quat q = Quat(magv1v2 + glm::dot(v1, v2),
            axis.x,
            axis.y,
            axis.z);

    return glm::normalize(q);
}

float Math::smoothstep(const float t)
{
    /* Assumes t is clamped within range [0, 1]. */
    return t * t * (3 - 2 * t);
}

float Math::frustumScaleFOV(const float fovDeg)
{
    float fovRad = glm::radians(fovDeg);

    return 1.0f / tan(fovRad / 2.0f);
}

float Math::smoothstepRatio(const float t, const float r)
{
    if (r > 0)
    {
        return r*(t * t * (3 - 2 * t));
    }
    else
    {
        return 0.0f;
    }
}
