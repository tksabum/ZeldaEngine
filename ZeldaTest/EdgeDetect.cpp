#include "EdgeDetect.h"

void EdgeDetect::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(EdgeDetect);
}

void EdgeDetect::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(EdgeDetect);
}

void EdgeDetect::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(renderer);
}

void EdgeDetect::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(renderer);
}

void EdgeDetect::OnCollisionStay2D()
{
	if (useCollision)
	{
		OnEdge();
	}
}

void EdgeDetect::OnCollisionExit2D()
{
	OffEdge();
}

void EdgeDetect::Awake()
{
	useCollision = true;
}

void EdgeDetect::SetMeshRenderer(ZeldaEngine::SkinnedMeshRenderer* renderer)
{
	this->renderer = renderer;
}

void EdgeDetect::OnEdge()
{
	renderer->GetMaterialInfo(0).floatParams[3] = 1;
}

void EdgeDetect::OffEdge()
{
	renderer->GetMaterialInfo(0).floatParams[3] = 0;
}

void EdgeDetect::SetUseCollision(bool value)
{
	useCollision = value;
}

