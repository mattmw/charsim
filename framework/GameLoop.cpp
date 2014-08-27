/*
 * GameEngine.cpp
 *
 *      Author: MMW
 */

#include "framework/GameLoop.h"
#include "Input.h"

/* Reference to self for GLFW message handling. */
GameLoop* pGameLoop = nullptr;
/* Reference to game environment. */
GameEnvironment* pGame = nullptr;
/* Reference to inputs for C-based static GLFW input callback function. */
Input* pInputs = nullptr;

GameLoop::GameLoop(GLFWwindow* window, GameEnvironment* game)
        : _window{window},
          _game{game},
          _aspectRatio{0.0f},
          _width{0},
          _height{0},
          _tGame{nullptr},
          _tDelta{nullptr}
{
    pGameLoop = this;
    pGame = game;
    _input = new Input();
    pInputs = _input;

    glfwSetKeyCallback(_window, keyCallback);
    glfwSetCursorEnterCallback(_window, cursorEnterCallback);
    glfwSetCursorPosCallback(_window, cursorPosCallback);
    glfwSetWindowSizeCallback(_window, windowSizeCallback);
}

GameLoop::~GameLoop()
{
    std::cout << "Destructing GameLoop" << std::endl;

    delete _input;
    delete _tDelta;
    delete _tGame;
}

void GameLoop::run()
{
    /* Set initial window size and viewport size. */
    glfwGetFramebufferSize(_window, &_width, &_height);
    _aspectRatio = _width / (float)_height;

    glViewport(0, 0, _width, _height);

    /* Set up timer. */
    _tGame = new float(0.0f);
    _tDelta = new float(0.0f);

    /* Initialise game environment and load time interval. */
    _game->init(_tGame, _tDelta, _input, _window);

    _game->setWindowDimensions(_width, _height);

    /* Run messaging loop. */
    loop();
}

void GameLoop::loop()
{
    float frame = 1;
    float previousTime = 0;
    float newTime2 = 0;
    int FPS = 120;
    /* Time taken since start of app till first game loop. */
    float lapse = glfwGetTime();

    double lastTime = 0.0;
    /* Elapsed running time of application. */
    double timer = 0.0;

    double frameTime = 1.0 / FPS;

    while (!glfwWindowShouldClose(_window))
    {
        timer = glfwGetTime();

        if (timer - lastTime >= frameTime)
        {
            /* Get elapsed game running time. */
            *_tGame = (float)timer - lapse;
            /* Get time delta/interval. */
            *_tDelta = (float)timer - lastTime;
            _game->update();
            _game->render();
            lastTime = timer;

            /* DEBUG: FPS timer. */
            if (frame < FPS)
            {
                frame++;
            }
            else
            {
                newTime2 = *_tGame - previousTime;

                previousTime += newTime2;
                frame = 1;
                float actualFPS = FPS / newTime2;
                printf("Game time: %.0f, FPS: %.0f\n", *_tGame, actualFPS);
            }

            glfwSwapBuffers(_window);
            glfwPollEvents();
        }
    }
}

void GameLoop::closeWindow()
{
    glfwSetWindowShouldClose(_window, GL_TRUE);
}

void GameLoop::setViewportSize(int width, int height)
{
    pGameLoop->_width = width;
    pGameLoop->_height = height;
    glViewport(0, 0, width, height);
}

void GameLoop::windowSizeCallback(GLFWwindow* window, int width, int height)
{
    GameLoop::setViewportSize(width, height);
    pGame->setWindowDimensions(width, height);
}

void GameLoop::cursorEnterCallback(GLFWwindow* window, int isEntered)
{
    if (isEntered == GL_TRUE)
    {
        pInputs->isEntered = true;
    }
    else if (isEntered == GL_FALSE)
    {
        pInputs->isEntered = false;
    }
}

void GameLoop::cursorPosCallback(GLFWwindow* window, double x, double y)
{
    /*
     * Update Input handler with projection-space coordinates
     * using screen width and height and cursor x-position and y-position.
     * It is up to the SceneGraph node to invoke the projection matrix
     * and compute the world-space values.
     */
    pInputs->x = (float)x / (1.0f / ((float)pGameLoop->_width/(float)pGameLoop->_height));
    pInputs->y = (float)y / 1.0f;

    pInputs->x = (float)x / (float)pGameLoop->_width;
    pInputs->y = (float)y / (float)pGameLoop->_height;
}

void GameLoop::keyCallback(GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
