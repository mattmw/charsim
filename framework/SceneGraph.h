/*
 * SceneGraph.h
 *
 *      Author: MMW
 */
#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

#include "framework/GraphNode.h"

class SceneGraph : public GraphNode
{
public:
    SceneGraph(String nodeId);

    virtual ~SceneGraph();

    virtual void init();

    virtual String getId() const;

    /* Recursive methods to perform through owned SceneGraph elements. */
    virtual void update(Mat4& W);
    virtual void updateAll(Mat4& W);
    virtual void render();
    virtual void shutdown() {};

    virtual void add(GraphNode* node);
    virtual void remove(GraphNode* node) {};
    virtual GraphNode* find(String nodeId);

private:
    /*
     * Array of nodes for quick iteration through their
     * update() and render() calls.
     */
    std::vector<GraphNode*> _nodes;
    uint _nNodes;
    /*
     * Hashtable to associate nodes with their Ids for fast find()
     * functionality.
     */
    std::map<String, uint> _nodeMap;
};

#endif /* SCENEGRAPH_H_ */
