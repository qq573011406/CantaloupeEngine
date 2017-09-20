#pragma once
#include "GraphicsManager.hpp"

namespace Onion
{
	class OpenGLESGraphicsManager : public GraphicsManager
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();
	};
}