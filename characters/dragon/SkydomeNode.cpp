/*
 * SkydomeNode.cpp
 *
 *      Author: MMW
 */
#include "SkydomeNode.h"

SkydomeNode::SkydomeNode(String nodeId,
        ResourceManager* resMan,
        Renderer* renderer,
        GLFWwindow* window)
        : SceneNode(nodeId)
{
    _resMan = resMan;
    _renderer = renderer;
    _window = window;
}

SkydomeNode::~SkydomeNode()
{
}

void SkydomeNode::init()
{
    /* Set up skydome. */
    _resMan->loadMeshFromFile("skydome01",
            "./resources/meshes/skydome01.js");
    _skydomeMesh = std::dynamic_pointer_cast<Mesh>(
            _resMan->getResourceMesh("skydome01"));

    /* Set up material. */
    _resMan->createMaterial("SkydomeMaterial", std::make_shared<DiffuseMaterial>(
            "SkydomeMaterial",
            ResourceManager::loadShaderFromFile("./shaders/BasicShader120.vs"),
            ResourceManager::loadShaderFromFile("./shaders/SkydomeShader120.fs"),
            "./resources/textures/skydome01.dds"));
    _skydomeMaterial = std::dynamic_pointer_cast<DiffuseMaterial>(
            _resMan->getResourceMaterial("SkydomeMaterial"));

    _skydomeMaterial->loadTextureBuffers();
}

void SkydomeNode::update(Mat4& W)
{
    /* Update position. */
    Vec3 position = Vec3(0.0f, -35.0f, 0.0f);
    Mat4 translate = glm::translate(Mat4(1), position);

    /* Update rotation. */
    Quat quat1 = Quat(Vec3(0, glm::radians(35.0f), 0));
    Mat4 rotate = glm::mat4_cast(quat1);

    _W = W * translate * rotate;
}

void SkydomeNode::render()
{
    _renderer->setWorldTransformation(_W);

    /* Render skydome. */
    _renderer->render(_skydomeMesh->getIndexCount(),
            _skydomeMaterial->getProgram(),
            _skydomeMesh->getVertexBuffer(),
            _skydomeMaterial->getDiffuseTextureBuffer());
}
