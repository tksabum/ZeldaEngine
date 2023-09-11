#include "pch.h"
#include "Renderer.h"

#include "GraphicsSystem.h"

namespace ZeldaEngine
{
	Renderer::Renderer()
	{
		GraphicsSystem::GetInstance().AddRenderer(this);
	}

	Renderer::~Renderer()
	{
		GraphicsSystem::GetInstance().RemoveRenderer(this);
	}
}