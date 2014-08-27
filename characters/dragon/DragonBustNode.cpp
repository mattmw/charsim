/*
 * DragonBustNode.cpp
 *
 *      Author: MMW
 */
#include "DragonBustNode.h"

DragonBustNode::DragonBustNode(String nodeId,
        ResourceManager* resMan,
        Renderer* renderer,
        GLFWwindow* window,
        float* tGame,
        float* tDelta,
        Input* input)
        : SceneNode(nodeId), ITime(tGame, tDelta), IInput(input),
         _resMan{resMan},
         _renderer{renderer},
         _window{window},
         _nBones{0},
         _pBoneChain{nullptr},
         _lookAtPosG1{Vec3(0, 10.0, -1.0)},
         _lookAtPosG2{Vec3(0, 5, 6)},
         _firstRun{true}
{
    _clipRunner = new ClipRunner(tGame, tDelta);
}

DragonBustNode::~DragonBustNode()
{
    delete _clipRunner;
}

void DragonBustNode::init()
{
    _activeKeyFactors[0] = 0.0f;
    _activeKeyIds[0] = 0;
    _activeKeyFactors[1] = 0.0f;
    _activeKeyIds[1] = 1;

    /* Specify morph targets. */
    std::vector<String> morphPaths;
    morphPaths.push_back("./resources/meshes/dragon_rig04_openmouth.js");
    _morphs["MouthOpen"]._keyFac = 0.0f;
    _morphs["MouthOpen"]._keyId = 0;
    morphPaths.push_back("./resources/meshes/dragon_rig04_blink.js");
    _morphs["Blink"]._keyFac = 0.0f;
    _morphs["Blink"]._keyId = 1;

    _resMan->loadSkinnedMorphMeshFromFile("dragon_rig04",
            "./resources/meshes/dragon_rig04.js",
            morphPaths);
    _mesh = std::dynamic_pointer_cast<SkinnedMorphMesh>(
            _resMan->getResourceMesh("dragon_rig04"));

    _nBones = _mesh->getBoneCount();

    /* Set up chains for IK solving. */
    _localIKChain.resize(_nBones);
    _globalIKChain.resize(_nBones);
    _bezierIKChain.resize(_nBones);

    /* Load animations for mesh. */
    ArmatureClip armatureClip;
    armatureClip._armatureClipId = "idle1";
    armatureClip._boneChain = _resMan->loadArmatureClip("dragon_rig04",
            "./resources/meshes/dragon_rig_anim_idle02.dae");
    Bone& bone = armatureClip._boneChain[0];
    uint poseCount = bone._poses.size();
    armatureClip._duration = bone._poses[poseCount-1]._timestamp;

    _armatureClip["idle1"] = armatureClip;

    _inverseBindPoses.resize(_nBones);
    _bindPoses.resize(_nBones);
    _globalPoses.resize(_nBones);
    _animPoses.resize(_nBones);
    _localPoses.resize(_nBones);
    _IKPoses.resize(_nBones);

    _pBoneChain = _mesh->getBoneChainPtr();

    /* Set up material. */
    _resMan->createMaterial("ArmatureMaterial1", std::make_shared<DiffuseMaterial>(
            "ArmatureMaterial1",
            ResourceManager::loadShaderFromFile("./shaders/BasicShader_morphing_skinning120.vs"),
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.fs"),
            "./resources/textures/bust_final_smoothed.dds"));
    _material = std::dynamic_pointer_cast<DiffuseMaterial>(
            _resMan->getResourceMaterial("ArmatureMaterial1"));

    _material->loadTextureBuffers();

    genBindMatrices();
    genInverseBindMatrices();
    initIKChain();

    /* Add clip collections to ClipRunner. */
    _clipRunner->addAnimPoses(&_animPoses);
    _clipRunner->addArmatureClips(&_armatureClip);
    _clipRunner->setBoneCount(_nBones);
    _clipRunner->addMorphClips(&_morphs);
}

void DragonBustNode::update(Mat4& W)
{
    _W = W;

    /* Facial morphing. */
    float tClip;
    /* Bounding timestamps of each animation step. */
    float lowerLim;
    float upperLim;
    /* Parameter for linear interpolation between steps. */
    float interval;

    float tWarp = 1.0f;

    uint repeat = 1;
    float clipDuration = 1.0f * tWarp;
    float tStart = 2.0f;
    float tAnim = *_tGame - tStart;
    float max = 1.0f;

    /* Initiate animation time. */
    if (*_tGame >= tStart && *_tGame <= tStart + (clipDuration*repeat))
    {
        lowerLim = 0.0f;
        upperLim = tWarp;

        /* Initiate clip time. */
        tClip = fmod(tAnim, clipDuration);

        cout<<"el: "<<*_tGame<<" tr: "<<tStart<<" cl: "<<clipDuration<< " an: "<<tClip<<endl;

        if (tClip > lowerLim && tClip < upperLim)
        {
            /*
             * Normalise range to find linear interpolation parameter.
             * [clip_start_tDelta, anim_tDelta, clip_end_tDelta]
             */
            upperLim -= lowerLim;
            interval = (tClip - lowerLim) / upperLim;
            interval = Math::smoothstepRatio(1-interval, max);

            _morphs["Blink"]._keyFac = interval;
        }
        _firstRun = false;
    }
    else
    {
        if (_firstRun)
        {
            _morphs["Blink"]._keyFac = 1.0f;
        }
        else
        {
            _morphs["Blink"]._keyFac = 0.0f;
            _morphs["MouthOpen"]._keyFac = 0.0f;
        }
    }

    _clipRunner->update();

    genBezierIKMatrices();
    genGlobalMatrices();
}

void DragonBustNode::render()
{
    _renderer->setWorldTransformation(_W);

    _renderer->setPoseTransformations(_globalPoses);

    /* Set active facial morph attributes. */
    /* TODO: This can be turned into one function. */
    _activeKeyFactors[0] = _morphs["MouthOpen"]._keyFac;
    _activeKeyIds[0] = _morphs["MouthOpen"]._keyId;
    _activeKeyFactors[1] = _morphs["Blink"]._keyFac;
    _activeKeyIds[1] = _morphs["Blink"]._keyId;

    _renderer->render(_mesh->getIndexCount(),
            _material->getProgram(),
            _mesh->getVertexBuffer(),
            _mesh->getVertexBoneBuffer(),
            _mesh->getMorphVertexBuffer(),
            _mesh->getVertexOffset(),
            _activeKeyFactors,
            _activeKeyIds,
            _material->getDiffuseTextureBuffer());
}

void DragonBustNode::genInverseBindMatrices()
{
    for (uint i = 0; i < _nBones; i++)
    {
        _inverseBindPoses[i] = glm::inverse(_bindPoses[i]);
    }
}

void DragonBustNode::genBindMatrices()
{
    QuatArray globalBindRotations;
    globalBindRotations.resize(_nBones);
    Vec3Array globalBindTranslations;
    globalBindTranslations.resize(_nBones);

    uint n = _nBones;

    /* Load local armature. */
    for (uint i = 0; i < _nBones; i++)
    {
        Bone& bone = _armatureClip["idle1"]._boneChain[i];
        Quat rot = bone._poses[0]._localPoseRotation;
        Vec3 pos = bone._poses[0]._localPoseTranslation;

        Mat4 rotMat = glm::mat4_cast(rot);
        Mat4 posMat = glm::translate(Mat4(1.0f), pos);
        Mat4 bindMat = rotMat * posMat;
        _bindPoses[i] = bindMat;
    }

    /* Globalise armature, shift armature and convert back to local. */
    for (uint i = 0; i < _nBones; i++)
    {
        Mat4 globalMat = accumulateBoneSpaceTransformations(_pBoneChain,
                (*_pBoneChain)[i]._parentIndex) * _bindPoses[i];

        /* Extract LocRot from global matrix. */
        Quat rot = glm::toQuat(globalMat);
        Vec3 pos;
        pos.x = globalMat[3].x;
        pos.y = globalMat[3].y;
        pos.z = globalMat[3].z;

        globalBindTranslations[i] = pos;
        globalBindRotations[i] = rot;
    }

    /* Convert global transformations to local transformations for matrix gen. */
    _armatureClip["idle1"]._boneChain[0]._poses[0]._localPoseTranslation =
            globalBindTranslations[0] - globalBindTranslations[0];

    for (uint i = 1; i < n; i++)
    {
        if (i < n)
        {
            _armatureClip["idle1"]._boneChain[n-i]._poses[0]._localPoseTranslation =
                    globalBindTranslations[n-i] - globalBindTranslations[(n-i)-1];
        }
    }

    /* Generate local bind matrices */
    for (uint i = 0; i < n; i++)
    {
        Bone& bone = _armatureClip["idle1"]._boneChain[i];
        Quat rot = bone._poses[0]._localPoseRotation;
        Vec3 pos = bone._poses[0]._localPoseTranslation;

        Mat4 rotMat = glm::mat4_cast(rot);
        Mat4 posMat = glm::translate(Mat4(1.0f), pos);
        Mat4 bindMat = rotMat * posMat;
        _bindPoses[i] = bindMat;
    }
}

void DragonBustNode::genGlobalMatrices()
{
    /* Blend animation and IK matrices. */
    for (uint i = 0; i < _nBones; i++)
    {
        _localPoses[i] = _animPoses[i] * _IKPoses[i];
    }

    /*
     * Traverse bone hierarchy for each bone and generate a global pose
     * matrix for each bone.
     */
    for (uint i = 0; i < _nBones; i++)
    {
        /* Accurate, but the first cube doesn't seem to be modified much. */
        Mat4 newMat = accumulateAnimBoneSpaceTransformations(_pBoneChain,
                (*_pBoneChain)[i]._parentIndex) * _inverseBindPoses[i] * _localPoses[i];

        _globalPoses[i] = newMat;
    }

    /* Update head position. */
    _headPos = _globalPoses[_nBones-2];
}

void DragonBustNode::initIKChain()
{
    for (uint i = 0; i < _nBones; i++)
    {
        _localIKChain[i]._parentIndex = (*_pBoneChain)[i]._parentIndex;
        _globalIKChain[i]._parentIndex = (*_pBoneChain)[i]._parentIndex;
    }

    for (uint i = 0; i < _nBones; i++)
    {
        Mat4 globalMat = accumulateBoneSpaceTransformations(_pBoneChain,
                (*_pBoneChain)[i]._parentIndex) * _bindPoses[i];

        /* Extract LocRot from global matrix. */
        Quat rot = glm::toQuat(globalMat);
        Vec3 pos;
        pos.x = globalMat[3].x;
        pos.y = globalMat[3].y;
        pos.z = globalMat[3].z;

        _globalIKChain[i]._pose._localPoseRotation = rot;
        _globalIKChain[i]._pose._localPoseTranslation = pos;
    }

    /* Generate bone "lengths". */
    for (uint i = 0; i < _nBones-1; i++)
    {
        _globalIKChain[i]._length =
                glm::length(_globalIKChain[i+1]._pose._localPoseTranslation
                        - _globalIKChain[i]._pose._localPoseTranslation);
    }

    _basisIKChain = _globalIKChain;
}

void DragonBustNode::genBezierIKMatrices()
{
    /* Copy translation values to bezier chain for bezier-based IK solving. */
    for (uint i = 0; i < _nBones; i++)
    {
        _bezierIKChain[i]._length = _globalIKChain[i]._length;
        _bezierIKChain[i]._pose._localPoseTranslation = _globalIKChain[i]._pose._localPoseTranslation;
    }

    /* Generate segments along bezier curve. */
    /* DEBUG: armature rendering. */
    Vec3Array points;
    _renderer->setWorldTransformation(_W);

    Vec3Array g(3);
    g[0] = Vec3(0, 0, 0);
    g[1] = Vec3(0, 10.0, -1.0);
    g[2] = Vec3(0, 5, 6);

    /* Tuning for cursor-following. */
    if (_input->isEntered)
    {
        g[1].x = -(_input->x-0.5f)*2.5f;
        g[2].x = 10.0f*(_input->x-0.5f);
        g[2].y = -(5.0f*(_input->y-1.75));
    }
    else
    {
        g[2].y = 6.25f;
    }

    _target = g[2];

    /* Apply acceleration to points. */
    float fac;
    float d;
    Vec3 lookAtTargetDir;
    Vec3 vel;
    Vec3 lookAtTarget;

    lookAtTarget = g[1] - _lookAtPosG1;
    if (lookAtTarget != Vec3(0,0,0))
    {
        d = glm::length(lookAtTarget);
        fac = (d*2);
        lookAtTargetDir = glm::normalize(lookAtTarget);
        vel = 2.0f * fac * lookAtTargetDir;
        _lookAtPosG1 += vel * *_tDelta;
        g[1] = _lookAtPosG1;
    }

    lookAtTarget = g[2] - _lookAtPosG2;
    d = glm::length(lookAtTarget);
    fac = (d*2);
    lookAtTargetDir = glm::normalize(lookAtTarget);
    vel = 2.0f * fac * lookAtTargetDir;
    _lookAtPosG2 += vel * *_tDelta;
    g[2] = _lookAtPosG2;

    points.push_back(g[0]);
    points.push_back(g[1]);
    points.push_back(g[1]);
    points.push_back(g[2]);
    _renderer->renderDebug(points, 5.0f);

    Vec3Array bezPoints;

    genBezierSubdivide(g, 3, bezPoints);
    uint segmentCount = 8;

    _renderer->renderDebug(bezPoints, 5.0f);

    /* Compact list of points for intersection testing. */
    Vec3Array segmentPoints;

    for (uint i = 0; i < bezPoints.size(); i+=2)
    {
        segmentPoints.push_back(bezPoints[i]);
    }
    segmentPoints.push_back(bezPoints[bezPoints.size()-1]);

    /* Match armature _ik_global_chain[i] to curve. */
    Vec3 p1; /* Start delimiter of line segment. */
    Vec3 p2; /* End delimiter of line segment. */
    Vec3 p3; /* Location of sphere centre. */
    Vec3 p;  /* Location of intersection. */
    Vec3 t;  /* Translation vector for realignment of child joints. */
    float r; /* Sphere radius. */
    int segmentIndex = -1;

    /* For each bone. */
    for (uint i = 0; i < _nBones-1; i++)
    {
        /*
         * Get radius of sphere defined by all possible rotations of the
         * current bone "vector".
         */
        r = _bezierIKChain[i]._length;
        /* Get location of sphere's centre. */
        p3 = _bezierIKChain[i]._pose._localPoseTranslation;

        /* For each curve segment. */
        for (int j = 0; j < segmentCount; j++)
        {
            /* Get start and end delimiters of curve segment. */
            p1 = segmentPoints[j];
            p2 = segmentPoints[j+1];

            if (CollisionDetection::prelimLineSphereTest(p1, p2, p3, r))
            {
                if (j > segmentIndex)
                {
                    segmentIndex = j;
                }
            }
        }

        /*
         * Get intersection point of farthest matched curve segment and
         * realign armature.
         */
        if (segmentIndex >= 0)
        {
            /* Get intersection point. */
            p1 = segmentPoints[segmentIndex];
            p2 = segmentPoints[segmentIndex+1];
            p = CollisionDetection::lineSphereTest(p1, p2, p3, r);

            points.clear();
            points.push_back(p);
            points.push_back(p);

            /*
             * Move joint's children to new point.
             * The first child joint can be set to the point directly.
             * The sequential joints can be moved by adding the point to their existing position.
             */
            t = p - _bezierIKChain[i+1]._pose._localPoseTranslation;
            Vec3 start = _bezierIKChain[i+1]._pose._localPoseTranslation;
            for (uint j = i+1; j < _nBones; j++)
            {
                _bezierIKChain[j]._pose._localPoseTranslation += t;
            }
            /*
             * Find relative rotation between old child joint and new child
             * joint where origin is the parent (current) joint.
             */
            Vec3 dest = p;
            _bezierIKChain[i]._pose._localPoseRotation = Math::rotation(start, dest);

            _bezierIKChain[0]._pose._localPoseRotation = Quat(1, 0, 0, 0);
            _bezierIKChain[0]._pose._localPoseRotation =
                    Quat(Vec3(glm::radians(20.0f), 0.0f, 0.0f));
        }
        segmentIndex = -1;
    }

    _eyeOrigin = _bezierIKChain[_nBones-2]._pose._localPoseTranslation;

    /* Reset bezier chain translation. */
    for (uint i = 0; i < _nBones; i++)
    {
        _bezierIKChain[i]._length = _globalIKChain[i]._length;
        _bezierIKChain[i]._pose._localPoseTranslation = _globalIKChain[i]._pose._localPoseTranslation;
    }

    /* Convert global armature to local armature for animation matrix generation. */
    _localIKChain[0]._pose._localPoseTranslation = _bezierIKChain[0]._pose._localPoseTranslation - _bezierIKChain[1]._pose._localPoseTranslation;
    _localIKChain[0]._pose._localPoseRotation = _bezierIKChain[0]._pose._localPoseRotation;
    uint n = _nBones;
    for (uint i = 1; i < n; i++)
    {
        if (i < n)
        {
            _localIKChain[n-i]._pose._localPoseTranslation = _bezierIKChain[n-i]._pose._localPoseTranslation - _bezierIKChain[(n-i)-1]._pose._localPoseTranslation;
            _localIKChain[i]._pose._localPoseRotation = _bezierIKChain[i]._pose._localPoseRotation;
        }
    }

    for (uint i = 0; i < _nBones; i++)
    {
        Quat rot = (_localIKChain[i]._pose._localPoseRotation);
        Vec3 pos = _localIKChain[i]._pose._localPoseTranslation;
        Mat4 rotMat = glm::mat4_cast(rot);
        Mat4 posMat = glm::translate(glm::mat4(1.0f), pos);

        Mat4 animMat = rotMat*posMat;

        _IKPoses[i] = animMat;
    }

    /* DEBUG: Draw bones. */
    _renderer->setWorldTransformation(_W);

    /* DEBUG: Render to-target line. */
    points.clear();
    for (uint i = 0; i < _nBones; i++)
    {
        points.push_back(_bezierIKChain[i]._pose._localPoseTranslation);
        if (i < _nBones-1)
            points.push_back(_bezierIKChain[i+1]._pose._localPoseTranslation);
        else
            points.push_back(_bezierIKChain[i]._pose._localPoseTranslation);
    }
}

Mat4 DragonBustNode::getHeadPos()
{
    return _headPos;
}

Vec3 DragonBustNode::getEyeOrigin()
{
    return _eyeOrigin;
}

Vec3 DragonBustNode::getTarget()
{
    return _target;
}

void DragonBustNode::playBlink(float tStart, float tWarp, ushort nPlays)
{
    Clip clip;

    clip.clipType = MORPHCYCLECLIP;
    clip.clipId = "Blink";
    clip.nPlays = nPlays;
    clip.tStart = tStart;
    clip.tWarp = tWarp;

    _clipRunner->addClip(clip);
}

void DragonBustNode::playIdle1(float tStart, float tWarp, ushort nPlays)
{
    Clip clip;

    clip.clipType = ARMATURECLIP;
    clip.clipId = "idle1";
    clip.nPlays = nPlays;
    clip.tStart = tStart;
    clip.tWarp = tWarp;

    _clipRunner->addClip(clip);
}

void DragonBustNode::playSnap(float tStart, float tWarp, ushort nPlays, float max)
{
    Clip clip;

    clip.clipType = MORPHCYCLECLIP;
    clip.clipId = "MouthOpen";
    clip.nPlays = nPlays;
    clip.tStart = tStart;
    clip.tWarp = tWarp;
    clip.max = max;

    _clipRunner->addClip(clip);
}

void DragonBustNode::genBezierSubdivide(Vec3Array g, uint depth, Vec3Array& points)
{
    /*
     * Based on code from "Curved Surfaces Using Bézier Patches"
     * http://www.gamasutra.com/view/feature/131755/curved_surfaces_using_bzier_.php?page=6
     * See: Bibliography.
     */
    if (depth == 0)
    {
        points.push_back(g[0]);
        points.push_back(g[2]);

        return;
    }

    Vec3Array gl(3);
    Vec3Array gr(3);

    gl[0] = g[0];
    gl[1] = (g[0] + g[1]) * 0.5f;
    gr[1] = (g[1] + g[2]) * 0.5f;
    gl[2] = gr[0] = (gl[1] + gr[1]) * 0.5f;
    gr[2] = g[2];

    genBezierSubdivide(gl, --depth, points);
    genBezierSubdivide(gr, depth, points);
}

Mat4 DragonBustNode::accumulateAnimBoneSpaceTransformations(
        const std::vector<Bone>* bones,
        const int& parent)
{
    if (parent != -1)
    {
        return accumulateAnimBoneSpaceTransformations(
                bones, (*bones)[parent]._parentIndex)
                * _inverseBindPoses[parent] * _localPoses[parent];
    }
    else
    {
        return _localPoses[0];
    }
}

Mat4 DragonBustNode::accumulateBoneSpaceTransformations(
        const std::vector<Bone>* bones, const int& parent)
{
    if (parent != -1)
    {
        return accumulateBoneSpaceTransformations(
                bones, (*bones)[parent]._parentIndex) * _bindPoses[parent];
    }
    else
    {
        return _bindPoses[0];

    }
}
