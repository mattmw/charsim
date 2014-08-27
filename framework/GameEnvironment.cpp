/*
 * TestGame.cpp
 *
 *      Author: MMW
 */

#include <iostream>
#include "GameEnvironment.h"

GameEnvironment::GameEnvironment()
        : _window{nullptr},
          _width{0.0f},
          _height{0.0f},
          _aspectRatio{0.0f},
          _input{nullptr},
          _tGame{nullptr},
          _tDelta{nullptr},
          _renderer{nullptr}
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    _resMan = new ResourceManager();
    _scene = new SceneGraph("root");
}

GameEnvironment::~GameEnvironment()
{
    std::cout << "Destructing GameEnvironment" << std::endl;

    delete _resMan;
    delete _scene;
}

void GameEnvironment::setInput(Input* input)
{
    _input = input;
}

void GameEnvironment::init(float* tGame, float* tDelta, Input* input, GLFWwindow* window)
{
    _window = window;
    _tGame = tGame;
    _tDelta = tDelta;
    _input = input;
}

void GameEnvironment::update()
{
    /* Game-wide updates, such as keyboard handling goes here. */

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateAll();
}

void GameEnvironment::updateAll()
{
    Mat4 idMat = Mat4(1.0f);

    _scene->update(idMat);
}

SceneGraph* GameEnvironment::getSceneGraph() const
{
    return _scene;
}

void GameEnvironment::renderAll()
{
    _scene->render();
}

void GameEnvironment::render()
{
    renderAll();
}

void GameEnvironment::preRender()
{
}

void GameEnvironment::setWindowDimensions(float width, float height)
{
    _width = width;
    _height = height;
    _aspectRatio = width / height;
    _renderer->createPerspectiveTransformation(_aspectRatio);
}

void GameEnvironment::handleKeyboardMessage()
{
}

ResourceManager* GameEnvironment::getResourceManager() const
{
    return _resMan;
}

void GameEnvironment::setCurrentRenderer(Renderer* currentRenderer)
{
    _renderer = currentRenderer;
}
