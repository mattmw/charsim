/*
 * DragonFeaturesNode.h
 *
 *      Author: MMW
 */
#ifndef DRAGONFEATURESNODE_H_
#define DRAGONFEATURESNODE_H_

#include "framework/SceneNode.h"
#include "framework/ITime.h"
#include "framework/Renderer.h"
#include "framework/MorphClip.h"
#include "framework/Clip.h"
#include "framework/ClipRunner.h"
#include "framework/ResourceManager.h"
#include "framework/CollisionDetection.h"
#include "framework/Math.h"
#include "framework/MorphMesh.h"
#include "materials/DiffuseMaterial.h"

class DragonFeaturesNode : public SceneNode, public ITime
{
public:
    DragonFeaturesNode(String nodeId,
            ResourceManager* resMan,
            Renderer* renderer,
            GLFWwindow* window,
            float* tGame,
            float* tDelta);

    ~DragonFeaturesNode();

    virtual void init();

    /* Recursive methods to perform through owned SceneGraph elements. */
    virtual void update(Mat4& W);
    virtual void render();
    virtual void shutdown() {};

    void playSnap(float tStart, float tWarp, ushort nPlays, float max);

private:
    std::shared_ptr<Mesh> _upperGumsMesh;
    std::shared_ptr<DiffuseMaterial> _upperGumsMaterial;
    std::shared_ptr<MorphMesh> _lowerGumsMesh;
    std::shared_ptr<DiffuseMaterial> _lowerGumsMaterial;
    std::shared_ptr<Mesh> _upperTeethMesh;
    std::shared_ptr<DiffuseMaterial> _upperTeethMaterial;
    std::shared_ptr<MorphMesh> _lowerTeethMesh;
    std::shared_ptr<DiffuseMaterial> _lowerTeethMaterial;
    std::shared_ptr<MorphMesh> _tongueMesh;
    std::shared_ptr<DiffuseMaterial> _tongueMaterial;

    ResourceManager* _resMan;
    Renderer* _renderer;
    ClipRunner* _clipRunner;
    /* For input control. Should use Input abstraction, really. */
    GLFWwindow* _window;

    Mat4 _translate;
    Vec3 _position;

    /* Morph targets. */
    std::map<String, Morph> _morphs;

    /* Active key attributes (limited to the shader's limit). */
    float _activeKeyFactors[MORPHLIMIT];
    uint _activeKeyIds[MORPHLIMIT];
};

#endif /* DRAGONFEATURESNODE_H_ */
