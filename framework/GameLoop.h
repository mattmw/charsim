/*
 * GameLoop.h
 *
 *      Author: MMW
 */

#ifndef GAMELOOP_H_
#define GAMELOOP_H_

#include <iostream>
#include <string>
#include <math.h>

#include "framework/core.h"
#include "framework/Input.h"
#include "framework/GameEnvironment.h"

class GameLoop
{
public:
    GameLoop(GLFWwindow* window, GameEnvironment* game);

    virtual ~GameLoop();

    /* Main() entry point into engine. */
    void run();

    static void setViewportSize(int width, int height);

protected:
    void closeWindow();

private:
    /* GLFW window & openGL context handle. */
    GLFWwindow* _window;

    /* Game environment pointer. */
    GameEnvironment* _game;

    /* GLFW callback functions. */
    static void keyCallback(GLFWwindow* window,
            int key, int scancode, int action, int mods);
    static void cursorEnterCallback(GLFWwindow* window, int isEntered);
    static void cursorPosCallback(GLFWwindow* window, double x, double y);
    static void windowSizeCallback(GLFWwindow* window, int width, int height);

    float _aspectRatio;
    int _width;
    int _height;

    /* Main GLFW loop. */
    void loop();

    /* Pointer to input interface. */
    Input* _input;

    /* Elapsed game time (time elapsed since first game loop). */
    float* _tGame;
    /* Frame time delta (time taken to draw the previous frame). */
    float* _tDelta;
};

#endif /* GAMELOOP_H_ */
