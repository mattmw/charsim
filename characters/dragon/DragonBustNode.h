/*
 * DragonBustNode.h
 *
 *      Author: MMW
 */
#ifndef DRAGONBUSTNODE_H_
#define DRAGONBUSTNODE_H_

#include "framework/SceneNode.h"
#include "framework/ITime.h"
#include "framework/IInput.h"
#include "framework/Renderer.h"
#include "framework/ClipRunner.h"
#include "framework/ResourceManager.h"
#include "framework/CollisionDetection.h"
#include "framework/Math.h"
#include "framework/Clip.h"
#include "framework/MorphClip.h"
#include "framework/Rigging.h"
#include "framework/ArmatureClip.h"
#include "framework/SkinnedMorphMesh.h"
#include "materials/DiffuseMaterial.h"

class DragonBustNode : public SceneNode, public ITime, public IInput
{
public:
    DragonBustNode(String nodeId,
            ResourceManager* resMan,
            Renderer* renderer,
            GLFWwindow* window,
            float* tGame,
            float* tDelta,
            Input* input);

    virtual ~DragonBustNode();

    virtual void init();

    /* Recursive methods to perform through owned SceneGraph elements. */
    virtual void update(Mat4& W);
    virtual void render();
    virtual void shutdown() {};

    Mat4 getHeadPos();
    Vec3 getEyeOrigin();
    Vec3 getTarget();

    /* Animations. */
    void playBlink(float tStart, float tWarp, ushort nPlays);
    void playSnap(float tStart, float tWarp, ushort nPlays, float max);
    void playIdle1(float tStart, float tWarp, ushort nPlays);

private:
    void genInverseBindMatrices();
    void genBindMatrices();
    void initIKChain();
    void genBezierIKMatrices();
    void genGlobalMatrices();
    /*
     * Recursive functions to convert an array of relative matrix
     * transformations into global matrix transformations.
     */
    glm::mat4 accumulateAnimBoneSpaceTransformations(
            const std::vector<Bone>* bones, const int& parent);
    glm::mat4 accumulateBoneSpaceTransformations(
            const std::vector<Bone>* bones, const int& parent);

    /* Return list of points on bezier curve generated from input control points. */
    void genBezierSubdivide(Vec3Array g,
            uint depth,
            Vec3Array& points);

    std::shared_ptr<SkinnedMorphMesh> _mesh;
    std::shared_ptr<DiffuseMaterial> _material;

    ResourceManager* _resMan;
    Renderer* _renderer;
    ClipRunner* _clipRunner;
    /* For input control. Should use Input abstraction, really. */
    GLFWwindow* _window;

    uint _nBones;

    /* Bone hierarchy pointer provided by mesh. */
    std::vector<Bone>* _pBoneChain;

    /* Repository of animations */
    std::map<String, ArmatureClip> _armatureClip;

    /* Armatures for inverse kinematics (IK). */
    std::vector<IKBone> _localIKChain;
    std::vector<IKBone> _globalIKChain;
    std::vector<IKBone> _bezierIKChain;
    std::vector<IKBone> _basisIKChain;

    /* Values for cursor-tracking. */
    /* Current head position. */
    Mat4 _headPos;
    Vec3 _eyeOrigin;
    /* Location of cursor. */
    Vec3 _target;
    /* Location of bezier curve control points. */
    Vec3 _lookAtPosG1;
    Vec3 _lookAtPosG2;

    /* Armature transformation matrices. */
    Mat4Array _inverseBindPoses;
    Mat4Array _bindPoses;
    Mat4Array _IKPoses;
    Mat4Array _animPoses;
    Mat4Array _localPoses;
    Mat4Array _globalPoses;

    /*
     * Morph targets.
     * For animation purposes, the ModelNode for the dragon character
     * contains a set of morph weight factors relating to the number of possible
     * morph targets (key shapes) that have been generated for the
     * character's expressions.
     */
    std::map<String, Morph> _morphs;

    /* Active key attributes (limited to the shader's limit). */
    float _activeKeyFactors[MORPHLIMIT];
    uint _activeKeyIds[MORPHLIMIT];

    bool _firstRun;
};

#endif /* DRAGONBUSTNODE_H_ */
