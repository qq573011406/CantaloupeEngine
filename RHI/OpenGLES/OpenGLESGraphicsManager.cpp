#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "OpenGLESGraphicsManager.hpp"
#include "OpenGLESApplication.hpp"
using namespace Onion;

namespace Onion
{
	extern IApplication* g_pApp;
}

int Onion::OpenGLESGraphicsManager::Initialize()
{
	glViewport(0, 0, 960, 540);
	int error = glGetError();
	return error;
}

void Onion::OpenGLESGraphicsManager::Finalize()
{

}

void Onion::OpenGLESGraphicsManager::Tick()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	OpenGLESApplication* app = reinterpret_cast<OpenGLESApplication*>(g_pApp);
	app->SwapBuffer();
}

