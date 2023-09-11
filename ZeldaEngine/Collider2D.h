#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"

#include "ZeldaMath.h"

#include <vector>

namespace ZeldaEngine
{
	class ZELDAENGINE_API Collider2D : public Component
	{
	public:
		enum Shape
		{
			Box
		};

		void PostInitialize_() override;
		virtual ~Collider2D();

		void SetShape(Shape shape);

		// v�� �ð���� �����ε� ������ ���
		void SetVertices(const std::vector<ZeldaMath::Vector2D>& v);
		const std::vector<ZeldaMath::Vector2D>& GetVertices();

	private:
		std::vector<ZeldaMath::Vector2D> vertices;

	private:
		// Component��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	};
}