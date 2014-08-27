/*
 * characters/dragon/DragonSim.h
 *
 * Game environment for the dragon character simulation.
 *
 *      Author: MMW
 */
#ifndef DRAGONSIM_H_
#define DRAGONSIM_H_

#include "framework/GameEnvironment.h"
#include "characters/dragon/DragonFeaturesNode.h"
#include "characters/dragon/DragonBustNode.h"
#include "characters/dragon/DragonController.h"
#include "characters/dragon/DragonEyeNode.h"
#include "characters/dragon/SkydomeNode.h"

class DragonSim : public GameEnvironment
{
public:
    DragonSim();

    virtual ~DragonSim();

    /* Set up scene and choose renderer. */
    void init(float* tGame, float* tDelta, Input* input, GLFWwindow* window);

private:
    /*
     * update():
     * General game-wide updates, such as general keyboard input handling.
     * One use would be to acquire the current position of the
     * camera node and update the position of a skydome so that it is always
     * aligned equally around the camera.
     */
    void update();

    /* Active renderer. */
    Renderer* _defaultRenderer;

    /* Scene elements. */
    DragonBustNode* _dragonBust;
    DragonFeaturesNode* _dragonFeatures;
    DragonEyeNode* _dragonLeftEye;
    DragonEyeNode* _dragonRightEye;
    SkydomeNode* _skydome;
    DragonController* _dragonController;

    /*
     *  Pointers for fast access to game environment state and helper classes.
     */
    /* Input */
    Input* _input;
    /* Elapsed game time (time elapsed since first game loop). */
    float* _tGame;
    /* Frame time delta (time taken to draw the previous frame). */
    float* _tDelta;
    ResourceManager* _resMan;
    SceneGraph* _scene;
    GLFWwindow* _window;
};

#endif /* DRAGONSIM_H_ */
