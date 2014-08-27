/*
 * SceneGraph.cpp
 *
 *      Author: MMW
 */
#include "SceneGraph.h"

SceneGraph::SceneGraph(String nodeId) : GraphNode(nodeId), _nNodes{0}
{
}

SceneGraph::~SceneGraph()
{
    std::cout << "\nDestructing SceneGraph: \"" << _nodeId << "\"" << std::endl;
}

void SceneGraph::init()
{
    /* Initialise world transformation matrix (W) with the identity matrix. */
    _W = Mat4(1);
}

String SceneGraph::getId() const
{
    return _nodeId;
}

void SceneGraph::update(Mat4& W)
{
    updateAll(W);
}

void SceneGraph::updateAll(Mat4& W)
{
    if (!_nodes.empty())
    {
        for (uint i = 0; i < _nNodes; i++)
        {
            _nodes[i]->update(W);
        }
    }
}

void SceneGraph::render()
{
    if (!_nodes.empty())
    {
        for (uint i = 0; i < _nNodes; i++)
        {
            _nodes[i]->render();
        }
    }
}

void SceneGraph::add(GraphNode* node)
{
    /* Add node to array. */
    _nodes.push_back(node);
    /* Add index relation for find() functionality. */
    _nodeMap[node->getId()] = _nodes.size() - 1;
    _nNodes++;
}

GraphNode* SceneGraph::find(String nodeId)
{
    return _nodes[_nodeMap[nodeId]];
}
