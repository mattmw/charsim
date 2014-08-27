/*
 * DragonFeaturesNode.cpp
 *
 *      Author: MMW
 */
#include "DragonFeaturesNode.h"

DragonFeaturesNode::DragonFeaturesNode(String nodeId,
        ResourceManager* resMan,
        Renderer* renderer,
        GLFWwindow* window,
        float* tGame,
        float* tDelta)
        : SceneNode(nodeId), ITime(tGame, tDelta)
{
    _resMan = resMan;
    _renderer = renderer;
    _clipRunner = new ClipRunner(tGame, tDelta);
    _window = window;
}

DragonFeaturesNode::~DragonFeaturesNode()
{
    delete _clipRunner;
}

void DragonFeaturesNode::init()
{
    /*
     * All keys have to be filled, even though DragonFeatures only has one
     * morph target.
     */
    _activeKeyFactors[0] = 0.0f;
    _activeKeyIds[0] = 0;
    _activeKeyFactors[1] = 0.0f;
    _activeKeyIds[1] = 1;

    std::vector<String> morphPaths;

    /* Specify morph factors and identities. */
    _morphs["MouthOpen"]._keyFac = 0.0f;
    _morphs["MouthOpen"]._keyId = 0;

    /* Set up lower teeth mesh. */

    /* Specify morph targets. */
    morphPaths.clear();
    morphPaths.push_back("./resources/meshes/dragon_lowerteeth01_openmouth.js");

    _resMan->loadMorphMeshFromFile("dragon_lowerteeth01",
            "./resources/meshes/dragon_lowerteeth01.js",
            morphPaths);
    _lowerTeethMesh = std::dynamic_pointer_cast<MorphMesh>(
            _resMan->getResourceMesh("dragon_lowerteeth01"));

    /* Set up material. */
    _resMan->createMaterial("LowerTeethMaterial", std::make_shared<DiffuseMaterial>(
            "LowerTeethMaterial",
            ResourceManager::loadShaderFromFile("./shaders/BasicShader_morphing120.vs"),
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.fs"),
            "./resources/textures/teethlower_tex.dds"));
    _lowerTeethMaterial = std::dynamic_pointer_cast<DiffuseMaterial>(
            _resMan->getResourceMaterial("LowerTeethMaterial"));

    _lowerTeethMaterial->loadTextureBuffers();

    /* Set up upper teeth mesh. */
    _resMan->loadMeshFromFile("dragon_upperteeth01",
            "./resources/meshes/dragon_upperteeth01.js");
    _upperTeethMesh = std::dynamic_pointer_cast<Mesh>(
            _resMan->getResourceMesh("dragon_upperteeth01"));

    /* Set up material. */
    _resMan->createMaterial("UpperTeethMaterial", std::make_shared<DiffuseMaterial>(
            "UpperTeethMaterial",
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.vs"),
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.fs"),
            "./resources/textures/teethupper_tex.dds"));
    _upperTeethMaterial = std::dynamic_pointer_cast<DiffuseMaterial>(
            _resMan->getResourceMaterial("UpperTeethMaterial"));

    _upperTeethMaterial->loadTextureBuffers();

    /* Set up upper gums mesh. */
    _resMan->loadMeshFromFile("dragon_uppergums01",
            "./resources/meshes/dragon_uppergums01.js");
    _upperGumsMesh = std::dynamic_pointer_cast<Mesh>(
            _resMan->getResourceMesh("dragon_uppergums01"));

    /* Set up material. */
    _resMan->createMaterial("UpperGumsMaterial", std::make_shared<DiffuseMaterial>(
            "UpperGumsMaterial",
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.vs"),
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.fs"),
            "./resources/textures/gumsupper_tex.dds"));
    _upperGumsMaterial = std::dynamic_pointer_cast<DiffuseMaterial>(
            _resMan->getResourceMaterial("UpperGumsMaterial"));

    _upperGumsMaterial->loadTextureBuffers();

    /* Set up lower gums mesh. */

    /* Specify morph targets. */
    morphPaths.clear();
    morphPaths.push_back("./resources/meshes/dragon_lowergums01_openmouth.js");

    _resMan->loadMorphMeshFromFile("dragon_lowergums01",
            "./resources/meshes/dragon_lowergums01.js",
            morphPaths);
    _lowerGumsMesh = std::dynamic_pointer_cast<MorphMesh>(
            _resMan->getResourceMesh("dragon_lowergums01"));

    /* Set up material. */
    _resMan->createMaterial("LowerGumsMaterial", std::make_shared<DiffuseMaterial>(
            "LowerGumsMaterial",
            ResourceManager::loadShaderFromFile("./shaders/BasicShader_morphing120.vs"),
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.fs"),
            "./resources/textures/gumslower_tex.dds"));
    _lowerGumsMaterial = std::dynamic_pointer_cast<DiffuseMaterial>(
            _resMan->getResourceMaterial("LowerGumsMaterial"));

    _lowerGumsMaterial->loadTextureBuffers();

    /* Set up tongue mesh. */
    std::vector<String> morphPathsTongue;
    morphPathsTongue.push_back("./resources/meshes/dragon_tongue01_openmouth.js");

    _resMan->loadMorphMeshFromFile("dragon_tongue01",
            "./resources/meshes/dragon_tongue01.js",
            morphPathsTongue);
    _tongueMesh = std::dynamic_pointer_cast<MorphMesh>(
            _resMan->getResourceMesh("dragon_tongue01"));

    /* Set up material. */
    _resMan->createMaterial("TongueMaterial", std::make_shared<DiffuseMaterial>(
            "TongueMaterial",
            ResourceManager::loadShaderFromFile("./shaders/BasicShader_morphing120.vs"),
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.fs"),
            "./resources/textures/tongue_tex.dds"));
    _tongueMaterial = std::dynamic_pointer_cast<DiffuseMaterial>(
            _resMan->getResourceMaterial("TongueMaterial"));

    _tongueMaterial->loadTextureBuffers();

    /* Add clip collections to ClipRunner. */
    _clipRunner->addMorphClips(&_morphs);
}

void DragonFeaturesNode::update(Mat4& W)
{
    _W = W;

    _clipRunner->update();
}

void DragonFeaturesNode::render()
{
    /* Set active facial morph attributes. */
    /* TODO: This can be turned into one function. */
    _activeKeyFactors[0] = _morphs["MouthOpen"]._keyFac;
    _activeKeyIds[0] = _morphs["MouthOpen"]._keyId;

    _renderer->setWorldTransformation(_W);

    /* Render upper gums. */
    _renderer->render(_upperGumsMesh->getIndexCount(),
            _upperGumsMaterial->getProgram(),
            _upperGumsMesh->getVertexBuffer(),
            _upperGumsMaterial->getDiffuseTextureBuffer());

    /* Render upper teeth. */
    _renderer->render(_upperTeethMesh->getIndexCount(),
            _upperTeethMaterial->getProgram(),
            _upperTeethMesh->getVertexBuffer(),
            _upperTeethMaterial->getDiffuseTextureBuffer());

    /* Render lower teeth. */
    _renderer->render(_lowerTeethMesh->getIndexCount(),
            _lowerTeethMaterial->getProgram(),
            _lowerTeethMesh->getVertexBuffer(),
            _lowerTeethMesh->getMorphVertexBuffer(),
            _lowerTeethMesh->getVertexOffset(),
            _activeKeyFactors,
            _activeKeyIds,
            _lowerTeethMaterial->getDiffuseTextureBuffer());

    /* Render lower gums */
    _renderer->render(_lowerGumsMesh->getIndexCount(),
            _lowerGumsMaterial->getProgram(),
            _lowerGumsMesh->getVertexBuffer(),
            _lowerGumsMesh->getMorphVertexBuffer(),
            _lowerGumsMesh->getVertexOffset(),
            _activeKeyFactors,
            _activeKeyIds,
            _lowerGumsMaterial->getDiffuseTextureBuffer());

    /* Render tongue. */
    _renderer->render(_tongueMesh->getIndexCount(),
            _tongueMaterial->getProgram(),
            _tongueMesh->getVertexBuffer(),
            _tongueMesh->getMorphVertexBuffer(),
            _tongueMesh->getVertexOffset(),
            _activeKeyFactors,
            _activeKeyIds,
            _tongueMaterial->getDiffuseTextureBuffer());
}

void DragonFeaturesNode::playSnap(float tStart, float tWarp, ushort nPlays, float max)
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
