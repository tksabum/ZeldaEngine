#pragma once

#include "ZeldaEngineAPI.h"

#include <string>

namespace ZeldaEngine
{
	class ZELDAENGINE_API ISprite
	{
	public:
		virtual void SetTexture(const std::wstring& textureName) abstract;
		virtual void SetAlpha(float alpha) abstract;
	};
}