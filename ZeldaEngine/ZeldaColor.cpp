#include "pch.h"
#include "ZeldaColor.h"

#include <cmath>

namespace ZeldaEngine
{
	const ZeldaColor ZeldaColor::Black = ZeldaColor(0.0f, 0.0f, 0.0f, 1.0f);
	const ZeldaColor ZeldaColor::White = ZeldaColor(1.0f, 1.0f, 1.0f, 1.0f);
	const ZeldaColor ZeldaColor::Red = ZeldaColor(1.0f, 0.0f, 0.0f, 1.0f);
	const ZeldaColor ZeldaColor::Green = ZeldaColor(0.0f, 1.0f, 0.0f, 1.0f);
	const ZeldaColor ZeldaColor::Blue = ZeldaColor(0.0f, 0.0f, 1.0f, 1.0f);
	const ZeldaColor ZeldaColor::Orange = ZeldaColor(1.f, 0.4f, 0.15f, 1.0f);

	ZeldaColor::ZeldaColor(float r, float g, float b, float a) : r(r), g(g), b(b), alpha(a)
	{

	}

	ZeldaColor::ZeldaColor(const ZeldaColor& color) : r(color.r), g(color.g), b(color.b), alpha(color.alpha)
	{

	}

	bool ZeldaColor::operator==(const ZeldaColor& right)
	{
		return (r == right.r) && (g == right.g) && (b == right.b) && (alpha == right.alpha);
	}

	ZeldaColor& ZeldaColor::operator=(const ZeldaColor& right)
	{
		this->r = right.r;
		this->g = right.g;
		this->b = right.b;
		this->alpha = right.alpha;

		return *this;
	}

	void ZeldaColor::PreSerialize(json& jsonData) const
	{
		jsonData["ZeldaColor"]["r"] = r;
		jsonData["ZeldaColor"]["g"] = g;
		jsonData["ZeldaColor"]["b"] = b;
		jsonData["ZeldaColor"]["alpha"] = alpha;
	}

	void ZeldaColor::PreDeserialize(const json& jsonData)
	{
		r = jsonData["ZeldaColor"]["r"];
		g = jsonData["ZeldaColor"]["g"];
		b = jsonData["ZeldaColor"]["b"];
		alpha = jsonData["ZeldaColor"]["alpha"];
	}

	void ZeldaColor::PostSerialize(json& jsonData) const
	{

	}

	void ZeldaColor::PostDeserialize(const json& jsonData)
	{

	}
}