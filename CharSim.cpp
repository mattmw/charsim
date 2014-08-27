#include "framework/core.h"
#include "framework/GameLoop.h"
#include "framework/GameEnvironment.h"
#include "characters/dragon/DragonSim.h"
#include "framework/Input.h"

void errorCallback(int error, const char* desc);

int main(int argc, char** argv)
{
    /* Handle for openGL context. */
    GLFWwindow* window;

    glfwSetErrorCallback(errorCallback);

    try
    {
        if (!glfwInit())
        {
            throw InitException("GLFW Initialisation failed");
        }

        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 8);

        // glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
        // glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);

        // glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(1280, 720, "CharSim001", NULL, NULL);

        if (!window)
        {
            glfwTerminate();
            throw InitException("GLFW Window creation failed");
        }

        glfwMakeContextCurrent(window);

        /*
         * Initialise GLEW.
         */
        /* Must be true for core */
        glewExperimental = true;
        GLenum errGlew = glewInit();
        if (GLEW_OK != errGlew)
        {
            std::ostringstream ss;
            ss << "GLEW init error: " << glewGetErrorString(errGlew);
            throw InitException(ss.str());
        }
        fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        std::terminate();
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    int n;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
    cout<<"Max attrs: "<<n<<endl;

    /* Set up game environment. */
    DragonSim* dragonSim;
    GameLoop* gameLoop;

    /* Catch all errors and gracefully shut down game. */
    try
    {
        dragonSim = new DragonSim();
        gameLoop = new GameLoop(window, dragonSim);

        gameLoop->run();

		delete dragonSim;
		delete gameLoop;

    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void errorCallback(int error, const char* desc)
{
    fputs(desc, stderr);
}
