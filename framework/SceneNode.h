/*
 * SceneNode.h
 *
 *      Author: MMW
 */
#ifndef SCENENODE_H_
#define SCENENODE_H_

#include "framework/GraphNode.h"

class SceneNode : public GraphNode
{
public:
    SceneNode(String nodeId);

    virtual ~SceneNode();

    virtual void init() {};

    virtual String getId() const;

    /* Recursive methods to perform through owned SceneGraph elements. */
    virtual void update(Mat4& W) {};
    virtual void updateAll(Mat4& W) {};
    virtual void render() {};
    virtual void shutdown() {};

    virtual void add(GraphNode* node) {};
    virtual void remove(GraphNode* node) {};
};

#endif /* SCENENODE_H_ */
