/*
 * DragonController.cpp
 *
 *      Author: MMW
 */
#include "DragonController.h"
#include <cstdlib>
#include <ctime>

DragonController::DragonController(String nodeId,
        GLFWwindow* window,
        float* tGame,
        float* tDelta,
        Input* input,
        Renderer* renderer)
        : ControlNode(nodeId), ITime(tGame, tDelta), IInput(input),
          _dragonBust{nullptr},
          _dragonFeatures{nullptr},
          _dragonLeftEye{nullptr},
          _dragonRightEye{nullptr},
          _tStart1{5.0f},
          _newSecond{true}
{
    _window = window;
    _renderer = renderer;
}

DragonController::~DragonController()
{
    std::cout << "Destructing DragonController: \"" << _nodeId << "\"" << std::endl;
}

void DragonController::init()
{
    /* Initial lookAt position - centre of screen. */
    _leftEyeLookAtPos = Vec3(0, 5, 6);
    _rightEyeLookAtPos = Vec3(0, 5, 6);

    /* Initialise random seed. */
    srand(time(NULL));
}

void DragonController::initPredefinedClips()
{
    /*
     * Add clips where the start times are known.
     * Clips determined to play based on on-the-fly update logic
     * go in update().
     */
    _dragonBust->playIdle1(1.0f, 3.0f, 100);
}

/* DEBUG: Model rotation. */
Vec3 eulerAngle1(0, 0, 0);
Vec3 eulerAngle2(0, 0, 0);
float rot1 = 0.0f;
float rot2 = 0.0f;
float vel = 0.0f;
float vel2 = 0.0f;
void DragonController::update(Mat4& W)
{
    /* DEBUG: point renderer. */
    Vec3Array points;

    _W = W;

    handleKeyboardMessage();

    /* Define on-the-fly clips. */
    if (*_tGame >= _tStart1 && *_tGame <= _tStart1 + 1.0f)
    {
        /* Check each second. */
        if (_newSecond)
        {
            /* Check chance of blinking. */
            /* 1-in-10 chance. */
            uint die = rand() % 10;
            if (die == 0)
            {
                _dragonBust->playBlink(*_tGame+0.1f, 0.5f, 1);
            }
            else if (die == 1)
            {
                _dragonBust->playBlink(*_tGame+0.1f, 0.25f, 2);
            }
            /* Check chance of snapping. */
            die = rand() % 10;
            if (die == 0)
            {
                _dragonBust->playSnap(*_tGame+0.1f, 1.0f, 1, 0.3f);
                _dragonFeatures->playSnap(*_tGame+0.1f, 1.0f, 1, 0.3f);
            }
            if (die == 1)
            {
                _dragonBust->playSnap(*_tGame+0.1f, 1.0f, 1, 0.6f);
                _dragonFeatures->playSnap(*_tGame+0.1f, 1.0f, 1, 0.6f);
            }
            _newSecond = false;
        }
    }
    else if (*_tGame > _tStart1 + 1.0f)
    {
        /* When current second has passed. */
        _tStart1 += 1.0f;
        _newSecond = true;
    }

    /* DEBUG: model rotation. */
    Quat quat1;
    Quat quat2;
    Quat rotateQ;

    if (eulerAngle1.z <= (360.0f - rot1))
        eulerAngle1.z += rot1;
    else
        eulerAngle1.z = rot1;

    if (eulerAngle2.y <= (360.0f - rot2))
        eulerAngle2.y += rot2;
    else
        eulerAngle2.y = rot2;

    rot1 = 0.0f * *(_tDelta);
    rot2 = 0.0f * *(_tDelta);

    quat1 = Quat(eulerAngle1);
    quat2 = Quat(eulerAngle2);

    rotateQ = glm::mix(quat1, quat2, 0.5f);
    rotateQ = quat1 * quat2;

    _rotation = glm::mat4_cast(rotateQ);

    _W *= _rotation;
    /* End of model rotation. */

    _renderer->setWorldTransformation(_W);

    /*
     * Update dragonBust and retrieve current head position and looking
     * direction.
     */
    _dragonBust->update(_W);

    Mat4 headPos = _dragonBust->getHeadPos();
    Vec3 eyeOrigin = _dragonBust->getEyeOrigin();
    Vec3 target = _dragonBust->getTarget();

    Vec3 targetLeftEye = target;
    Vec3 targetRightEye = target;

    /*
     * Make sure eyes only go cross-eyed when the cursor is in the centre.
     * The base pose should be wall-eyed.
     */
    if (!_input->isEntered)
    {
        targetLeftEye.x = 0.5f;
        targetRightEye.x = -0.5f;
    }

    /*
     * Quick hack in lieu of frustum clipping: limit target movement to stop
     * eyes from moving too far.
     */
    if (target.x < -0.5f)
    {
        targetLeftEye.x = -0.5f;
    }
    if (target.x > 0.5f)
    {
        targetRightEye.x = 0.5f;
    }

    /* Generate accelerating/decelerating target position. */
    Quat leftEyeRotQ = genEyeRotation(targetLeftEye, eyeOrigin, _leftEyeLookAtPos);
    Quat rightEyeRotQ = genEyeRotation(targetRightEye, eyeOrigin, _rightEyeLookAtPos);

    Mat4 leftEyeRotMat = glm::mat4_cast(leftEyeRotQ);
    Mat4 rightEyeRotMat = glm::mat4_cast(rightEyeRotQ);
    _dragonLeftEye->setEyeRotation(leftEyeRotMat);
    _dragonRightEye->setEyeRotation(rightEyeRotMat);

    /*
     * Update transformation with head position so that the dragon character's
     * features follow its position and rotation.
     */
    _W *= headPos;

    _dragonFeatures->update(_W);
    _dragonLeftEye->update(_W);
    _dragonRightEye->update(_W);

    /* DEBUG: render lookAt points. */
    points.clear();
    points.push_back(target);
    points.push_back(target);
    _renderer->renderDebug(points, 5.0f);
}

void DragonController::handleKeyboardMessage()
{
    /* DEBUG: Model rotation. */
    if (glfwGetKey(_window, GLFW_KEY_LEFT ) == GLFW_PRESS)
    {
        if (vel >= -2.0f) { vel -= 50.0f * *_tDelta; }
        rot1 = vel * *_tDelta;
    }
    else if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (vel <= 2.0f) { vel += 50.0f * *_tDelta; }
        rot1 = vel * *_tDelta;
    }
    else
    {
        float decel = ((vel < 0) ? -5.0f : 5.0f) * *_tDelta;
        if (std::fabs(decel) > std::fabs(vel)) { vel = 0.0f;} else { vel -= decel; }
        rot1 = vel * *_tDelta;
    }
    if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (vel2 >= -2.0f) { vel2 -= 50.0f * *_tDelta; }
        rot2 = vel2 * *_tDelta;
    }
    else if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (vel2 <= 2.0f) { vel2 += 50.0f * *_tDelta; }
        rot2 = vel2 * *_tDelta;
    }
    else
    {
        float decel = ((vel2 < 0) ? -5.0f : 5.0f) * *_tDelta;
        if (std::fabs(decel) > std::fabs(vel2)) { vel2 = 0.0f;} else { vel2 -= decel; }
        rot2 = vel2 * *_tDelta;
    }
    /* DEBUG: end of model rotation */
}

void DragonController::addDragonBust(DragonBustNode* dragonBust)
{
    _dragonBust = dragonBust;
}

void DragonController::addDragonFeatures(DragonFeaturesNode* dragonFeatures)
{
    _dragonFeatures = dragonFeatures;
}

void DragonController::addDragonLeftEye(DragonEyeNode* dragonLeftEye)
{
    _dragonLeftEye = dragonLeftEye;
}

void DragonController::addDragonRightEye(DragonEyeNode* dragonRightEye)
{
    _dragonRightEye = dragonRightEye;
}

Quat DragonController::genEyeRotation(Vec3 target, Vec3 eyeOrigin, Vec3& eyeLookAtPos)
{
    Vec3 lookAtTarget;

    /* Don't bother doing vector subtraction if the vectors are identical. */
    if (eyeLookAtPos != target)
    {
        /*
         * Find vector pointing from current eye lookAt position to
         * the current cursor target.
         */
        lookAtTarget = target - eyeLookAtPos;
    }
    else
    {
        lookAtTarget = target;
    }

    /* Increment lookAt position in direction of lookAtTarget each frame. */
    float d = glm::length(lookAtTarget);
    float fac = d*3;
    Vec3 lookAtTargetDir;
    Vec3 vel;
    float speed = 5.0f;
    if (_input->isEntered)
    {
        lookAtTargetDir = glm::normalize(lookAtTarget);
        vel = speed * fac * lookAtTargetDir;
        eyeLookAtPos += vel * *_tDelta;
    }
    else
    {
        /*
         * Prevent eyes from disappearing when cursor is outside window.
         * This especially occurs at low framerates and is due to the
         * lookAtTarget vector returning NaN values after its length (d) is
         * evaluated as zero more than once by the GLM library.
         */
        if (d != 0)
        {
            lookAtTargetDir = glm::normalize(lookAtTarget);
            vel = speed * fac * lookAtTargetDir;
            eyeLookAtPos += vel * *_tDelta;
        }
    }

    /* Calculate and return eye rotation quaternion. */
    Vec3 lookForward(0, 0, 1);
    Vec3 lookAtDir = eyeLookAtPos - eyeOrigin;

    return Math::rotation(lookForward, lookAtDir);
}
