#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"
#include "ILight.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API Light : public Component, ILight
	{
	public:
		Light();
		virtual ~Light();

		// ILight��(��) ���� ��ӵ�
		virtual void Render(IGraphics* graphics) abstract;

		LightInfo GetLightInfo();
		void SetLightIndex(__int8 idx);
		
	protected:
		std::wstring meshName;

		LightInfo lightInfo;
		MaterialInfo materialInfo;

		virtual void UpdateLightInfo();

	public:
		// Component��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;
		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;
	};
}