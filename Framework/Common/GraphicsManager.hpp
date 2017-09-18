#pragma once
#include "IRuntimeModule.hpp"
namespace Onion
{
    class GraphicsManager : implements IRuntimeModule
    {
    public:
        virtual ~GraphicsManager(){}
        virtual int Initialize();
	    virtual void Finalize();
	    virtual void Tick();
    };
}