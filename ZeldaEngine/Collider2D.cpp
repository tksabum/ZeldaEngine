#include "pch.h"
#include "Collider2D.h"

#include "PhysicsSystem2D.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	void Collider2D::PostInitialize_()
	{
		PhysicsSystem2D::GetInstance().AddCollider(this);
	}

	Collider2D::~Collider2D()
	{
		PhysicsSystem2D::GetInstance().RemoveCollider(this);
	}

	void Collider2D::SetShape(Shape shape)
	{
		switch (shape)
		{
			case ZeldaEngine::Collider2D::Box:
			{
				vertices = {
					Vector2D(0.5f, 0.5f),
					Vector2D(0.5f, -0.5f),
					Vector2D(-0.5f, -0.5f),
					Vector2D(-0.5f, 0.5f)
				};
				break;
			}
			default:
			{
				vertices = {
					Vector2D(0.5f, 0.5f),
					Vector2D(0.5f, -0.5f),
					Vector2D(-0.5f, -0.5f),
					Vector2D(-0.5f, 0.5f)
				};
				break;
			}
		}
	}

	void Collider2D::SetVertices(const std::vector<ZeldaMath::Vector2D>& v)
	{
		vertices = v;
	}

	const std::vector<ZeldaMath::Vector2D>& Collider2D::GetVertices()
	{
		return vertices;
	}

	void Collider2D::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(Collider2D);
		PRESERIALIZE_VECTOR_SERVALUE(vertices);
	}

	void Collider2D::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(Collider2D);
		PREDESERIALIZE_VECTOR_SERVALUE(vertices);
	}

	void Collider2D::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
		POSTSERIALIZE_VECTOR_SERVALUE(vertices);
	}

	void Collider2D::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
		POSTDESERIALIZE_VECTOR_SERVALUE(vertices);
	}
}