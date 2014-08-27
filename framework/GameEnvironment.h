/*
 * GameEnvironment.h
 *
 *      Author: MMW
 */

#ifndef GAMEENVIRONMENT_H_
#define GAMEENVIRONMENT_H_

#include "framework/core.h"
#include "framework/Input.h"
#include "framework/ResourceManager.h"
#include "framework/SceneGraph.h"
#include "framework/Renderer.h"
#include "framework/Buffers.h"

class GameEnvironment
{
public:
    GameEnvironment();

    virtual ~GameEnvironment();

    /*
     * Scene and rendering.
     */
    /* Setup scene. */
    virtual void init(float* tGame, float* tDelta, Input* input, GLFWwindow* window);
    virtual void shutdown() {};
    /* General scene updates. */
    virtual void update();
    virtual void render();
    virtual void setWindowDimensions(float width, float height);
    /* Set mouse and keyboard interface. */
    virtual void setInput(Input* input);

    virtual void getProjectionMatrix() {};

protected:
    SceneGraph* getSceneGraph() const;
    ResourceManager* getResourceManager() const;
    void setCurrentRenderer(Renderer* currentRenderer);
    /* Recurse through scenegraph. */
    virtual void updateAll();
    /* Call prerender, postrender and render scenegraph. */
    virtual void renderAll();

private:
    GLFWwindow* _window;

    virtual void handleKeyboardMessage();

    /*
     * Scene and rendering.
     */
    /* Called before scenegraph is rendered. */
    virtual void preRender();
    /* Called after scenegraph is rendered. */
    virtual void postRender() {};

    /* Vertex Attribute Object. */
    GLuint _vao;

    float _width, _height, _aspectRatio;

    /* Pointer tp input interface */
    Input* _input;

    /* Elapsed game time (time elapsed since first frame). */
    float* _tGame;
    /* Frame time delta (time taken to draw the previous frame). */
    float* _tDelta;

    SceneGraph* _scene;
    Renderer* _renderer;
    ResourceManager* _resMan;
};

#endif /* GAMEENVIRONMENT_H_ */
