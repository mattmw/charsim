/*
 * ControlNode.h
 *
 * Controller node base.
 * Such nodes deal with transformations and apply them to child controllers
 * and other nodes (lights, models, etc).
 * They can take keyboard and mouse input and trigger animations on the
 * model nodes they own - either based on player input or timings.
 *
 *      Author: MMW
 */
#ifndef CONTROLNODE_H_
#define CONTROLNODE_H_

#include "framework/SceneGraph.h"

class ControlNode : public SceneGraph
{
public:
    ControlNode(String nodeId);

    virtual ~ControlNode();

private:
    /* Affine transformations matrices. */
    Mat4 _translate;
    Mat4 _rotate;
    Mat4 _scale;
};

#endif /* CONTROLNODE_H_ */
