/*
 * GraphNode.h
 *
 *      Author: MMW
 */
#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include "framework/core.h"

class GraphNode
{
public:
    GraphNode(String nodeId)
            : _nodeId{nodeId},
              _W{Mat4(1.0f)} {};
    virtual ~GraphNode() {};

    virtual void init() = 0;

    virtual String getId() const = 0;

    /* Recursive methods to perform through owned SceneGraph elements. */
    virtual void update(Mat4& W) = 0;
    virtual void updateAll(Mat4& W) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    /* SceneGraph functionality (not required by SceneNodes) */
    virtual void add(GraphNode* node) = 0;
    virtual void remove(GraphNode* node) = 0;

protected:
    String _nodeId;
    /* World transformation matrix. */
    Mat4 _W;

};

#endif /* GRAPHNODE_H_ */
