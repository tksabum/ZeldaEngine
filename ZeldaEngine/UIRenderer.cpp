#include "pch.h"
#include "UIRenderer.h"

#include "GraphicsSystem.h"

namespace ZeldaEngine
{
	UIRenderer::UIRenderer()
		: layer(0)
	{
		
	}

	void UIRenderer::PostInitialize_()
	{
		GraphicsSystem::GetInstance().AddUIRenderer(this);
	}

	UIRenderer::~UIRenderer()
	{
		GraphicsSystem::GetInstance().RemoveUIRenderer(this);
	}

	void UIRenderer::SetLayer(int value)
	{
		GraphicsSystem::GetInstance().ChangedUILayer();

		if (-999 <= value && value <= 999)
		{
			layer = value;
		}
	}

	int UIRenderer::GetLayer()
	{
		return layer;
	}
}