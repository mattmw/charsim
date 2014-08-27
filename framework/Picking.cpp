/*
 * Picking.cpp
 *
 *  Created on: 26 Jun 2014
 *      Author: MMW
 */
#include "Picking.h"

Picking::Picking()
{
    /* TODO: Implement picking class. */

    /*
    float mx = _input->x;
    float my = _input->y;
    mx = 2*(mx-0.5);
    my = -(2*(my-0.5));
    Mat4 projMat = _renderer->getProjectionMatrix();
    mx = mx/projMat[0][0];
    my = my/projMat[1][1];
    Vec4 p_4 = Vec4(mx, my, 0.0f, 1.0f);
    Mat4 viewMat = _renderer->getViewMatrix();
    Mat4 viewMatInv = glm::inverse(viewMat);
    p_4 = viewMatInv * p_4;
    Vec3 cursor = Vec3(p_4);
     */
}

Picking::~Picking()
{

}

