#pragma once

#include "ZeldaEngineAPI.h"

#include <string>
#include <array>

#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include "ZeldaColor.h"

#include "Serializable.h"

#include "DirecX12AndTypedef.h"

namespace ZeldaEngine
{
	FF::FFMatrix Matrix4x4ToFFMatrix(const ZeldaMath::Matrix4x4& matrix4x4);
	FF::FFVec4 ZeldaColorToFFVec4(const ZeldaColor& color);

#pragma region 셰이더에서 사용할 enum

	enum class SHADER_TYPE :unsigned __int8
	{
		DEFERRED,
		DEFERRED_AND_SKIN,
		DEFFERRED_SHADOW,
		DEFFERRED_SKIN_SHADOW,
		FORWARD,
		LIGHTING,
		PARTICLE,
		COMPUTE,
		SHADOW,
		UI,
		EFFECT_ANIM,
		EFFECT
	};

	enum class RASTERIZER_TYPE
	{
		CULL_NONE,
		CULL_FRONT,
		CULL_BACK,
		WIREFRAME,
	};

	enum class DEPTH_STENCIL_TYPE
	{
		LESS,
		LESS_EQUAL,
		GREATER,
		GREATER_EQUAL,
		NO_DEPTH_TEST, // 깊이 테스트(x) + 깊이 기록(o)
		NO_DEPTH_TEST_NO_WRITE, // 깊이 테스트(x) + 깊이 기록(x)
		LESS_NO_WRITE, //깊이 테스트(o) + 깊이 기록(x)
	};

	enum class BLEND_TYPE :unsigned __int8
	{
		DEFAULT,
		ALPHA_BLEND,
		ONE_TO_ONE_BLEND,
		END,
	};

	enum class FFTopology
	{
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST = 1,
		D3D_PRIMITIVE_TOPOLOGY_LINELIST = 2,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
	};

	class ZELDAENGINE_API ShaderInfo : public Serializable
	{
	public:
		SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
		RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
		DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
		BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
		FFTopology topology = FFTopology::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		std::string vs = "VS_Main";
		std::string ps = "PS_Main";
		std::string gs = "";
		std::string cs = "";

		bool operator==(ShaderInfo& rhs)
		{
			if ((this->shaderType == rhs.shaderType)
				&& (this->rasterizerType == rhs.rasterizerType)
				&& (this->depthStencilType == rhs.depthStencilType)
				&& (this->blendType == rhs.blendType)
				&& (this->topology == rhs.topology)
				&& (this->vs == rhs.vs)
				&& (this->ps == rhs.ps))
			{
				return true;
			}

			return false;
		}

		FF::ShaderInfo ToFFShaderInfo() const;

		// Serializable을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};

	class ZELDAENGINE_API MaterialInfo : public Serializable
	{
	public:
		MaterialInfo();

		std::wstring materialName;
		std::wstring shader;
		std::array<std::wstring, 4> texture;
		std::array<int, 4> intParams;
		std::array<float, 4> floatParams;
		std::array<ZeldaMath::Vector2D, 4> vec2Params;
		std::array<ZeldaMath::Vector4D, 4> vec4Params;
		ZeldaColor color;

		bool operator==(MaterialInfo& rhs)
		{
			if ((this->materialName == rhs.materialName)
				&& (this->shader == rhs.shader)
				&& (equal(begin(this->texture), end(this->texture), begin(rhs.texture), end(rhs.texture)))
				&& (equal(begin(this->intParams), end(this->intParams), begin(rhs.intParams), end(rhs.intParams)))
				&& (equal(begin(this->floatParams), end(this->floatParams), begin(rhs.floatParams), end(rhs.floatParams)))
				&& (equal(begin(this->vec2Params), end(this->vec2Params), begin(rhs.vec2Params), end(rhs.vec2Params)))
				&& (equal(begin(this->vec4Params), end(this->vec4Params), begin(rhs.vec4Params), end(rhs.vec4Params)))
				&& (this->color == rhs.color))
			{
				return true;
			}

			return false;
		}

		FF::FFMATERIAL_DESC ToFFMaterialDesc() const;

		void SetForFFMaterialDesc(const FF::FFMATERIAL_DESC& desc);

		// Serializable을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
#pragma endregion


#pragma region 멀티 렌더 타겟용
	enum class RENDER_TARGET_TYPE : unsigned __int8
	{
		SWAP_CHAIN,
		G_BUFFER,
		LIGHTING,
		END,
	};

	enum
	{
		RENDER_TARGET_TYPE_COUNT = static_cast<unsigned __int8>(RENDER_TARGET_TYPE::END),
		G_BUFFER_MEMBER_COUNT = 4,
		LIGHTING_GROUP_MEMBER_COUNT = 2,
	};

	enum
	{
		POSITION,
		NORMAL,
		COLOR,
		DEPTH,
		DIFFUSELIGHT = 0,
		SPECULARLIGHT = 1,
	};
#pragma endregion


	enum class LIGHT_TYPE : unsigned __int8
	{
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
	};

	struct LightColor
	{
		ZeldaMath::Vector4D diffuse;
		ZeldaMath::Vector4D ambient;
		ZeldaMath::Vector4D specular;
	};

	struct LightInfo
	{
		LightColor color;
		ZeldaMath::Vector4D position;
		ZeldaMath::Vector4D direction;
		__int32 lightType;
		float range;
		float angle;
		__int32 padding;
	};

	struct LightParams
	{
		unsigned __int32 lightCount;
		ZeldaMath::Vector3D padding;
		LightInfo lights[50];

		FF::LightParams ToFFLightParams() const;
	};

#pragma region 파티클에서 쓸 정보
	struct ParticleInfo
	{
		ZeldaMath::Vector3D worldPos;
		float curTime;
		ZeldaMath::Vector3D worldDir;
		float lifeTime;
		__int32 alive;
		__int32 padding[3];
	};

	struct ComputeSharedInfo
	{
		__int32 addCount;
		__int32 padding[3];
	};
#pragma endregion

	enum class CONSTANT_BUFFER_TYPE : unsigned __int32
	{
		GLOBAL,
		TRANSFORM,
		MATERIAL,
		END
	};

	enum
	{
		CONSTANT_BUFFER_COUNT = static_cast<unsigned __int32>(CONSTANT_BUFFER_TYPE::END)
	};
}