/*
 * SkydomeNode.h
 *
 *      Author: MMW
 */
#ifndef SKYDOMENODE_H_
#define SKYDOMENODE_H_

#include "framework/SceneNode.h"
#include "framework/Renderer.h"
#include "framework/ResourceManager.h"
#include "framework/Mesh.h"
#include "materials/DiffuseMaterial.h"

class SkydomeNode : public SceneNode
{
public:
    SkydomeNode(String nodeId,
            ResourceManager* resMan,
            Renderer* renderer,
            GLFWwindow* window);

    virtual ~SkydomeNode();

    virtual void init();

    /* Recursive methods to perform through owned SceneGraph elements. */
    virtual void update(Mat4& W);
    virtual void render();
    virtual void shutdown() {};

private:
    std::shared_ptr<Mesh> _skydomeMesh;
    std::shared_ptr<DiffuseMaterial> _skydomeMaterial;

    ResourceManager* _resMan;
    Renderer* _renderer;
    GLFWwindow* _window;

};

#endif /* SKYDOMENODE_H_ */
