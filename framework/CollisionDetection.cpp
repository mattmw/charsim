/*
 * CollisionDetection.cpp
 *
 *  Created on: 11 May 2014
 *      Author: MMW
 */
#include "CollisionDetection.h"

bool CollisionDetection::prelimLineSphereTest(const Vec3& p1,
        const Vec3& p2,
        const Vec3& p3,
        const float& r)
{
    /* Get line parameter where point would be closest to sphere centre. */
    float t = ((p3.x-p1.x)*(p2.x-p1.x)+(p3.y-p1.y)*(p2.y-p1.y)+(p3.z-p1.z)*(p2.z-p1.z))/((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y)+(p2.z-p1.z)*(p2.z-p1.z));

    t = abs(t);

    if (t >= 0.0f && t <= 1.0f)
    {
        /* Get point on line closest to sphere centre. */
        Vec3 p = p1 + t*(p2 - p1);

        /* Is the length greater than the sphere radius? */
        float l = glm::length(p3 - p);
        if (l <= r)
        {
            /* Point resides inside or on the sphere, and thus intersects or touches. */
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


Vec3 CollisionDetection::lineSphereTest(const Vec3& p1,
        const Vec3& p2,
        const Vec3& p3,
        const float& r)
{
    float a = pow((p2.x-p1.x), 2) + pow((p2.y-p1.y), 2) + pow((p2.z-p1.z), 2);
    float b = 2*((p2.x-p1.x)*(p1.x-p3.x) + (p2.y-p1.y)*(p1.y-p3.y) + (p2.z-p1.z)*(p1.z-p3.z));
    float c = pow(p3.x, 2) + pow(p3.y, 2) + pow(p3.z, 2) + pow(p1.x, 2)+ pow(p1.y,2) + pow(p1.z,2) - 2*(p3.x*p1.x + p3.y*p1.y + p3.z*p1.z) - pow(r, 2);

    /* Find parameters along line. */
    float root1 = quadraticRoot1(a, b, c);
    float root2 = quadraticRoot2(a, b, c);

    float t;

    /* Validate roots. */
    if (root1 >= 0.0 && root1 <= 1.0)
    {
        if (root2 >= 0.0 && root2 <= 1.0)
        {
            if (root1 > root2)
            {
                t = root1;
            }
            else
            {
                t = root2;
            }
        }
        else
        {
            t = root1;
        }
    }
    else
    {
        if (root2 >= 0.0 && root2 <= 1.0)
        {
            t = root2;
        }
        else
        {
            t = root2;
        }
    }

    /* Put parameter into line equation to find point. */
    Vec3 p = p1 + t*(p2 - p1);

    return p;
}

float CollisionDetection::quadraticRoot1(const float& a, const float& b, const float& c)
{
    return (-b - sqrt(pow(b, 2) - 4*a*c)) / (2 * a);
}

float CollisionDetection::quadraticRoot2(const float& a, const float& b, const float& c)
{
    return (-b + sqrt(pow(b, 2) - 4*a*c)) / (2 * a);
}
