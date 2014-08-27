/*
 * ControlNode.cpp
 *
 *      Author: MMW
 */
#include "ControlNode.h"

ControlNode::ControlNode(String nodeId)
        : SceneGraph(nodeId)
{
}

ControlNode::~ControlNode()
{
    std::cout << "Destructing ControlNode: \"" << _nodeId << "\"" << std::endl;
}
