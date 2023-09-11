#pragma once

#include "IGraphics.h"

namespace ZeldaEngine
{
	class ILight
	{
	public:
		virtual void Render(IGraphics* graphics) abstract;
	};
}