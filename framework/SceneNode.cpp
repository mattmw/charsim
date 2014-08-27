/*
 * SceneNode.cpp
 *
 *      Author: MMW
 */
#include "SceneNode.h"

SceneNode::SceneNode(String nodeId) : GraphNode(nodeId)
{
}

SceneNode::~SceneNode()
{
    std::cout << "Destructing SceneNode: \"" << _nodeId << "\"" << std::endl;
}

String SceneNode::getId() const
{
    return _nodeId;
}
