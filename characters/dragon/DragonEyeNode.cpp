/*
 * DragonEyeNode.cpp
 *
 *      Author: MMW
 */
#include "DragonEyeNode.h"

DragonEyeNode::DragonEyeNode(String nodeId,
        ResourceManager* resMan,
        Renderer* renderer,
        GLFWwindow* window,
        Vec3 position)
        : SceneNode(nodeId)
{
    _resMan = resMan;
    _renderer = renderer;
    _window = window;
    _position = position;
}

DragonEyeNode::~DragonEyeNode()
{
    std::cout << "Destructing DragonEyeNode: \"" << _nodeId << "\"" << std::endl;
}

void DragonEyeNode::init()
{
    /* Set up mesh. */
    _resMan->loadMeshFromFile("dragon_eye",
            "./resources/meshes/dragon_eye01.js");
    _eyeMesh = std::dynamic_pointer_cast<Mesh>(
            _resMan->getResourceMesh("dragon_eye"));

    /* Set up material. */
    _resMan->createMaterial("EyeMaterial", std::make_shared<DiffuseMaterial>(
            "EyeMaterial",
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.vs"),
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.fs"),
            "./resources/textures/eye_tex.dds"));
    _eyeMaterial = std::dynamic_pointer_cast<DiffuseMaterial>(
            _resMan->getResourceMaterial("EyeMaterial"));

    _eyeMaterial->loadTextureBuffers();
}

void DragonEyeNode::update(Mat4& W)
{
    /* Update position. */
    Mat4 translate = glm::translate(Mat4(1), _position);

    _W = W * translate * _eyeRotation;
}

void DragonEyeNode::render()
{
    _renderer->setWorldTransformation(_W);

    _renderer->render(_eyeMesh->getIndexCount(),
            _eyeMaterial->getProgram(),
            _eyeMesh->getVertexBuffer(),
            _eyeMaterial->getDiffuseTextureBuffer());
}

void DragonEyeNode::setEyeRotation(Mat4 eyeRotation)
{
    _eyeRotation = eyeRotation;
}
