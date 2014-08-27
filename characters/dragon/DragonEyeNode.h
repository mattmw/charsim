/*
 * DragonEyeNode.h
 *
 *      Author: MMW
 */
#ifndef DRAGONEYENODE_H_
#define DRAGONEYENODE_H_

#include "framework/SceneNode.h"
#include "framework/Renderer.h"
#include "framework/ResourceManager.h"
#include "framework/CollisionDetection.h"
#include "framework/Math.h"
#include "framework/Mesh.h"
#include "materials/DiffuseMaterial.h"

class DragonEyeNode : public SceneNode
{
public:
    DragonEyeNode(String nodeId,
            ResourceManager* resMan,
            Renderer* renderer,
            GLFWwindow* window,
            Vec3 position);

    virtual ~DragonEyeNode();

    virtual void init();

    /* Recursive methods to perform through owned SceneGraph elements. */
    virtual void update(Mat4& W);
    virtual void render();
    virtual void shutdown() {};

    void setEyeRotation(Mat4 eyeRotation);

private:
    std::shared_ptr<Mesh> _eyeMesh;
    std::shared_ptr<DiffuseMaterial> _eyeMaterial;

    ResourceManager* _resMan;
    Renderer* _renderer;
    GLFWwindow* _window;

    Vec3 _position;
    Mat4 _eyeRotation;
};

#endif /* DRAGONEYENODE_H_ */
