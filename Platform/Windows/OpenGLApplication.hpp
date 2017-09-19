#pragma once
#include "WindowsApplication.hpp"

namespace Onion
{
    class OpenGLApplication : public WindowsApplication
    {

    public:
		OpenGLApplication(GfxConfiguration& config)
			: WindowsApplication(config) {};
        virtual int Initialize();
        virtual void Finalize();
        virtual void Tick();
    private:
        HGLRC m_RenderContext;
    };
}