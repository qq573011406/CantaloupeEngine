#include <stdio.h>
#include "glad/glad.h"
#include "OpenGLApplication.hpp"
#include "OpenGLGraphicsManager.hpp"

using namespace Onion;

namespace Onion
{
	extern IApplication* g_pApp;
}

extern struct gladGLversionStruct GLVersion;

int Onion::OpenGLGraphicsManager::Initialize()
{
    int result;

    result = gladLoadGL();
    if (!result) {
        printf("OpenGL load failed!\n");
        result = -1;
    } else {
        result = 0;
        printf("OpenGL Version %d.%d loaded\n", GLVersion.major, GLVersion.minor);

        if (GLAD_GL_VERSION_3_0) {
            // Set the depth buffer to be entirely cleared to 1.0 values.
            glClearDepth(1.0f);

            // Enable depth testing.
            glEnable(GL_DEPTH_TEST);

            // Set the polygon winding to front facing for the left handed system.
            glFrontFace(GL_CW);

            // Enable back face culling.
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
    }

    return result;
}

void Onion::OpenGLGraphicsManager::Finalize()
{
}

void Onion::OpenGLGraphicsManager::Tick()
{
	OpenGLApplication* app = reinterpret_cast<OpenGLApplication*>(g_pApp);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	app->SwapBuffer();
}