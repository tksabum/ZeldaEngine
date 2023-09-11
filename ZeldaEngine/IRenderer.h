#pragma once

#include "IGraphics.h"

namespace ZeldaEngine
{
	class IRenderer
	{
	public:
		virtual void Render(IGraphics* graphics) abstract;
	};
}