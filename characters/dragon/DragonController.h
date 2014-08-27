/*
 * DragonController.h
 *
 * Owns all dragon character model nodes and controls their overall
 * transformation and triggers their animations based on timing and player
 * input.
 *
 *      Author: MMW
 */
#ifndef DRAGONCONTROLLER_H_
#define DRAGONCONTROLLER_H_

#include "framework/ControlNode.h"
#include "characters/dragon/DragonBustNode.h"
#include "characters/dragon/DragonEyeNode.h"
#include "characters/dragon/DragonFeaturesNode.h"
#include "framework/ITime.h"
#include "framework/IInput.h"
/* DEBUG: points renderer. */
#include "framework/Renderer.h"

class DragonController : public ControlNode, public ITime, public IInput
{
public:
    DragonController(String nodeId,
            GLFWwindow* window,
            float* tGame,
            float* tDelta,
            Input* input,
            Renderer* renderer);

    virtual ~DragonController();

    void init();

    void initPredefinedClips();

    void update(Mat4& W);

    void addDragonBust(DragonBustNode* dragonBust);
    void addDragonFeatures(DragonFeaturesNode* dragonFeatures);
    void addDragonLeftEye(DragonEyeNode* dragonLeftEye);
    void addDragonRightEye(DragonEyeNode* dragonRightEye);
private:
    void handleKeyboardMessage();

    /* Dragon character components. */
    std::vector<GraphNode*> _nodes;
    DragonBustNode* _dragonBust;
    DragonFeaturesNode* _dragonFeatures;
    DragonEyeNode* _dragonLeftEye;
    DragonEyeNode* _dragonRightEye;

    Quat genEyeRotation(Vec3 target, Vec3 eyeOrigin, Vec3& eyeLookAtPos);

    /* Initial lookAt position - centre of screen. */
    Vec3 _leftEyeLookAtPos;
    Vec3 _rightEyeLookAtPos;

    Mat4 _translate;
    Mat4 _rotation;
    Mat4 _scale;

    /* Timers. */
    float _tStart1;
    bool _newSecond;

    GLFWwindow* _window;
    Renderer* _renderer;
};

#endif /* DRAGONCONTROLLER_H_ */
