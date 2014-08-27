/*
 * DragonSim.cpp
 *
 *      Author: MMW
 */
#include "DragonSim.h"

DragonSim::DragonSim()
        : _defaultRenderer{nullptr},
          _dragonBust{nullptr},
          _dragonFeatures{nullptr},
          _dragonLeftEye{nullptr},
          _dragonRightEye{nullptr},
          _skydome{nullptr},
          _dragonController{nullptr},
          _input{nullptr},
          _tGame{nullptr},
          _tDelta{nullptr},
          _resMan{nullptr},
          _scene{nullptr},
          _window{nullptr}
{
}

DragonSim::~DragonSim()
{
    delete _defaultRenderer;
    delete _dragonBust;
    delete _dragonFeatures;
    delete _dragonLeftEye;
    delete _dragonRightEye;
    delete _skydome;
    delete _dragonController;
}

void DragonSim::init(float* tGame, float* tDelta, Input* input, GLFWwindow* window)
{
    _window = window;
    _tGame = tGame;
    _tDelta = tDelta;
    _input = input;

    _resMan = getResourceManager();
    _scene = getSceneGraph();

    /* Set active renderer. */
    _defaultRenderer = new Renderer(_resMan);
    _defaultRenderer->init();

    setCurrentRenderer(_defaultRenderer);

    /* Set up the game scene. */
    _dragonBust = new DragonBustNode("DragonBust",
            _resMan,
            _defaultRenderer,
            _window,
            _tGame,
            _tDelta,
            _input);
    _dragonFeatures = new DragonFeaturesNode("DragonFeatures",
            _resMan,
            _defaultRenderer,
            _window,
            _tGame,
            _tDelta);
    _dragonLeftEye = new DragonEyeNode("DragonLeftEye",
            _resMan,
            _defaultRenderer,
            _window,
            Vec3(0.375f, 4.70f, 5.735f));
    _dragonRightEye = new DragonEyeNode("DragonRightEye",
            _resMan,
            _defaultRenderer,
            _window,
            Vec3(-0.375f, 4.70f, 5.735f));
    _skydome = new SkydomeNode("Skydome",
            _resMan,
            _defaultRenderer,
            _window);
    _dragonBust->init();
    _dragonFeatures->init();
    _dragonLeftEye->init();
    _dragonRightEye->init();
    _skydome->init();

    _dragonController = new DragonController("DragonController",
            _window,
            _tGame,
            _tDelta,
            _input,
            _defaultRenderer);
    _dragonController->init();

    _dragonController->add(_dragonBust);
    _dragonController->add(_dragonFeatures);
    _dragonController->add(_dragonLeftEye);
    _dragonController->add(_dragonRightEye);
    _dragonController->addDragonBust(_dragonBust);
    _dragonController->addDragonFeatures(_dragonFeatures);
    _dragonController->addDragonLeftEye(_dragonLeftEye);
    _dragonController->addDragonRightEye(_dragonRightEye);

    _dragonController->initPredefinedClips();

    _scene->add(_dragonController);
    _scene->add(_skydome);
}

void DragonSim::update()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateAll();
}
