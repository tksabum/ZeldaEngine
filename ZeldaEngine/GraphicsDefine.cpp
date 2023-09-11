#include "pch.h"
#include "GraphicsDefine.h"

namespace ZeldaEngine
{
	FF::FFMatrix Matrix4x4ToFFMatrix(const ZeldaMath::Matrix4x4& matrix4x4)
	{
		FF::FFMatrix ffmatrix;
		ffmatrix._11 = matrix4x4.m1_1;	ffmatrix._12 = matrix4x4.m2_1;	ffmatrix._13 = matrix4x4.m3_1;	ffmatrix._14 = matrix4x4.m4_1;
		ffmatrix._21 = matrix4x4.m1_2;	ffmatrix._22 = matrix4x4.m2_2;	ffmatrix._23 = matrix4x4.m3_2;	ffmatrix._24 = matrix4x4.m4_2;
		ffmatrix._31 = matrix4x4.m1_3;	ffmatrix._32 = matrix4x4.m2_3;	ffmatrix._33 = matrix4x4.m3_3;	ffmatrix._34 = matrix4x4.m4_3;
		ffmatrix._41 = matrix4x4.m1_4;	ffmatrix._42 = matrix4x4.m2_4;	ffmatrix._43 = matrix4x4.m3_4;	ffmatrix._44 = matrix4x4.m4_4;

		return ffmatrix;
	}

	FF::FFVec4 ZeldaColorToFFVec4(const ZeldaColor& color)
	{
		FF::FFVec4 vec4;
		vec4.x = color.r;
		vec4.y = color.g;
		vec4.z = color.b;
		vec4.w = color.alpha;

		return vec4;
	}

	FF::ShaderInfo ShaderInfo::ToFFShaderInfo() const
	{
		FF::ShaderInfo ffshader;
		ffshader.shaderType = static_cast<FF::SHADER_TYPE>(shaderType);
		ffshader.rasterizerType = static_cast<FF::RASTERIZER_TYPE>(rasterizerType);
		ffshader.depthStencilType = static_cast<FF::DEPTH_STENCIL_TYPE>(depthStencilType);
		ffshader.blendType = static_cast<FF::BLEND_TYPE>(blendType);
		ffshader.topology = static_cast<FF::FFTopology>(topology);
		ffshader.vs = vs;
		ffshader.ps = ps;
		ffshader.gs = gs;
		ffshader.cs = cs;

		return ffshader;
	}

	void ShaderInfo::PreSerialize(json& jsonData) const
	{
		jsonData["ShaderInfo"]["shaderType"] = static_cast<int>(shaderType);
		jsonData["ShaderInfo"]["rasterizerType"] = static_cast<int>(rasterizerType);
		jsonData["ShaderInfo"]["depthStencilType"] = static_cast<int>(depthStencilType);
		jsonData["ShaderInfo"]["blendType"] = static_cast<int>(blendType);
		jsonData["ShaderInfo"]["topology"] = static_cast<int>(topology);

		jsonData["ShaderInfo"]["vs"] = vs;
		jsonData["ShaderInfo"]["ps"] = ps;
		jsonData["ShaderInfo"]["gs"] = gs;
		jsonData["ShaderInfo"]["cs"] = cs;
	}

	void ShaderInfo::PreDeserialize(const json& jsonData)
	{
		shaderType = static_cast<SHADER_TYPE>(jsonData["ShaderInfo"]["shaderType"]);
		rasterizerType = static_cast<RASTERIZER_TYPE>(jsonData["ShaderInfo"]["rasterizerType"]);
		depthStencilType = static_cast<DEPTH_STENCIL_TYPE>(jsonData["ShaderInfo"]["depthStencilType"]);
		blendType = static_cast<BLEND_TYPE>(jsonData["ShaderInfo"]["blendType"]);
		topology = static_cast<FFTopology>(jsonData["ShaderInfo"]["topology"]);

		vs = jsonData["ShaderInfo"]["vs"];
		ps = jsonData["ShaderInfo"]["ps"];
		gs = jsonData["ShaderInfo"]["gs"];
		cs = jsonData["ShaderInfo"]["cs"];
	}

	void ShaderInfo::PostSerialize(json& jsonData) const
	{

	}

	void ShaderInfo::PostDeserialize(const json& jsonData)
	{

	}



	MaterialInfo::MaterialInfo() :
		materialName(L""),
		shader(L""),
		texture({ L"", L"", L"", L"" }),
		intParams({ 0,0,0,0 }),
		floatParams({ 0.0f, 0.0f, 0.0f, 0.0f }),
		vec2Params({ ZeldaMath::Vector2D::zero, ZeldaMath::Vector2D::zero, ZeldaMath::Vector2D::zero, ZeldaMath::Vector2D::zero }),
		vec4Params({ ZeldaMath::Vector4D::zero, ZeldaMath::Vector4D::zero, ZeldaMath::Vector4D::zero, ZeldaMath::Vector4D::zero }),
		color(ZeldaColor::White)
	{
		std::wstring materialName;
		std::wstring shader;
		std::array<std::wstring, 4> texture;
		std::array<int, 4> intParams;
		std::array<float, 4> floatParams;
		std::array<ZeldaMath::Vector2D, 4> vec2Params;
		std::array<ZeldaMath::Vector4D, 4> vec4Params;
		ZeldaColor color;
	}

	FF::FFMATERIAL_DESC MaterialInfo::ToFFMaterialDesc() const
	{
		FF::FFMATERIAL_DESC ffmatrixdesc;

		ffmatrixdesc.materialName = materialName;
		ffmatrixdesc.shader = shader;

		ffmatrixdesc.texture[0] = texture[0];
		ffmatrixdesc.texture[1] = texture[1];
		ffmatrixdesc.texture[2] = texture[2];
		ffmatrixdesc.texture[3] = texture[3];

		ffmatrixdesc.intParams[0] = intParams[0];
		ffmatrixdesc.intParams[1] = intParams[1];
		ffmatrixdesc.intParams[2] = intParams[2];
		ffmatrixdesc.intParams[3] = intParams[3];

		ffmatrixdesc.floatParams[0] = floatParams[0];
		ffmatrixdesc.floatParams[1] = floatParams[1];
		ffmatrixdesc.floatParams[2] = floatParams[2];
		ffmatrixdesc.floatParams[3] = floatParams[3];

		ffmatrixdesc.vec2Params[0].x = vec2Params[0].x;		ffmatrixdesc.vec2Params[0].y = vec2Params[0].y;
		ffmatrixdesc.vec2Params[1].x = vec2Params[1].x;		ffmatrixdesc.vec2Params[1].y = vec2Params[1].y;
		ffmatrixdesc.vec2Params[2].x = vec2Params[2].x;		ffmatrixdesc.vec2Params[2].y = vec2Params[2].y;
		ffmatrixdesc.vec2Params[3].x = vec2Params[3].x;		ffmatrixdesc.vec2Params[3].y = vec2Params[3].y;

		ffmatrixdesc.vec4Params[0].x = vec4Params[0].x;	ffmatrixdesc.vec4Params[0].y = vec4Params[0].y;	ffmatrixdesc.vec4Params[0].z = vec4Params[0].z;	ffmatrixdesc.vec4Params[0].w = vec4Params[0].w;
		ffmatrixdesc.vec4Params[1].x = vec4Params[1].x;	ffmatrixdesc.vec4Params[1].y = vec4Params[1].y;	ffmatrixdesc.vec4Params[1].z = vec4Params[1].z;	ffmatrixdesc.vec4Params[1].w = vec4Params[1].w;
		ffmatrixdesc.vec4Params[2].x = vec4Params[2].x;	ffmatrixdesc.vec4Params[2].y = vec4Params[2].y;	ffmatrixdesc.vec4Params[2].z = vec4Params[2].z;	ffmatrixdesc.vec4Params[2].w = vec4Params[2].w;
		ffmatrixdesc.vec4Params[3].x = vec4Params[3].x;	ffmatrixdesc.vec4Params[3].y = vec4Params[3].y;	ffmatrixdesc.vec4Params[3].z = vec4Params[3].z;	ffmatrixdesc.vec4Params[3].w = vec4Params[3].w;

		ffmatrixdesc.color = ZeldaColorToFFVec4(color);

		return ffmatrixdesc;
	}

	void MaterialInfo::SetForFFMaterialDesc(const FF::FFMATERIAL_DESC& desc)
	{

		this->materialName = desc.materialName;
		this->shader = desc.shader;

		this->texture[0] = desc.texture[0];
		this->texture[1] = desc.texture[1];
		this->texture[2] = desc.texture[2];
		this->texture[3] = desc.texture[3];

		this->intParams[0] = desc.intParams[0];
		this->intParams[1] = desc.intParams[1];
		this->intParams[2] = desc.intParams[2];
		this->intParams[3] = desc.intParams[3];

		this->floatParams[0] = desc.floatParams[0];
		this->floatParams[1] = desc.floatParams[1];
		this->floatParams[2] = desc.floatParams[2];
		this->floatParams[3] = desc.floatParams[3];

		this->vec2Params[0].x = desc.vec2Params[0].x;		this->vec2Params[0].y = desc.vec2Params[0].y;
		this->vec2Params[1].x = desc.vec2Params[1].x;		this->vec2Params[1].y = desc.vec2Params[1].y;
		this->vec2Params[2].x = desc.vec2Params[2].x;		this->vec2Params[2].y = desc.vec2Params[2].y;
		this->vec2Params[3].x = desc.vec2Params[3].x;		this->vec2Params[3].y = desc.vec2Params[3].y;

		this->vec4Params[0].x = desc.vec4Params[0].x;	this->vec4Params[0].y = desc.vec4Params[0].y;	this->vec4Params[0].z = desc.vec4Params[0].z;	this->vec4Params[0].w = desc.vec4Params[0].w;
		this->vec4Params[1].x = desc.vec4Params[1].x;	this->vec4Params[1].y = desc.vec4Params[1].y;	this->vec4Params[1].z = desc.vec4Params[1].z;	this->vec4Params[1].w = desc.vec4Params[1].w;
		this->vec4Params[2].x = desc.vec4Params[2].x;	this->vec4Params[2].y = desc.vec4Params[2].y;	this->vec4Params[2].z = desc.vec4Params[2].z;	this->vec4Params[2].w = desc.vec4Params[2].w;
		this->vec4Params[3].x = desc.vec4Params[3].x;	this->vec4Params[3].y = desc.vec4Params[3].y;	this->vec4Params[3].z = desc.vec4Params[3].z;	this->vec4Params[3].w = desc.vec4Params[3].w;

		this->color.r = desc.color.x;
		this->color.g = desc.color.y;
		this->color.b = desc.color.z;
		this->color.alpha = desc.color.w;
	}

	void MaterialInfo::PreSerialize(json& jsonData) const
	{
		jsonData["MaterialInfo"]["materialName"] = wstring_to_string(materialName);
		jsonData["MaterialInfo"]["shaderFilePath"] = wstring_to_string(shader);

		jsonData["MaterialInfo"]["texture0"] = wstring_to_string(texture[0]);
		jsonData["MaterialInfo"]["texture1"] = wstring_to_string(texture[1]);
		jsonData["MaterialInfo"]["texture2"] = wstring_to_string(texture[2]);
		jsonData["MaterialInfo"]["texture3"] = wstring_to_string(texture[3]);

		jsonData["MaterialInfo"]["intParams0"] = intParams[0];
		jsonData["MaterialInfo"]["intParams1"] = intParams[1];
		jsonData["MaterialInfo"]["intParams2"] = intParams[2];
		jsonData["MaterialInfo"]["intParams3"] = intParams[3];

		jsonData["MaterialInfo"]["floatParams0"] = floatParams[0];
		jsonData["MaterialInfo"]["floatParams1"] = floatParams[1];
		jsonData["MaterialInfo"]["floatParams2"] = floatParams[2];
		jsonData["MaterialInfo"]["floatParams3"] = floatParams[3];

		vec2Params[0].PreSerialize(jsonData["MaterialInfo"]["vec2Params0"]);
		vec2Params[1].PreSerialize(jsonData["MaterialInfo"]["vec2Params1"]);
		vec2Params[2].PreSerialize(jsonData["MaterialInfo"]["vec2Params2"]);
		vec2Params[3].PreSerialize(jsonData["MaterialInfo"]["vec2Params3"]);

		vec4Params[0].PreSerialize(jsonData["MaterialInfo"]["vec4Params0"]);
		vec4Params[1].PreSerialize(jsonData["MaterialInfo"]["vec4Params1"]);
		vec4Params[2].PreSerialize(jsonData["MaterialInfo"]["vec4Params2"]);
		vec4Params[3].PreSerialize(jsonData["MaterialInfo"]["vec4Params3"]);

		color.PreSerialize(jsonData["MaterialInfo"]["color"]);
	}

	void MaterialInfo::PreDeserialize(const json& jsonData)
	{
		materialName = string_to_wstring(jsonData["MaterialInfo"]["materialName"]);
		shader = string_to_wstring(jsonData["MaterialInfo"]["shaderFilePath"]);

		texture[0] = string_to_wstring(jsonData["MaterialInfo"]["texture0"]);
		texture[1] = string_to_wstring(jsonData["MaterialInfo"]["texture1"]);
		texture[2] = string_to_wstring(jsonData["MaterialInfo"]["texture2"]);
		texture[3] = string_to_wstring(jsonData["MaterialInfo"]["texture3"]);

		intParams[0] = jsonData["MaterialInfo"]["intParams0"];
		intParams[1] = jsonData["MaterialInfo"]["intParams1"];
		intParams[2] = jsonData["MaterialInfo"]["intParams2"];
		intParams[3] = jsonData["MaterialInfo"]["intParams3"];

		floatParams[0] = jsonData["MaterialInfo"]["floatParams0"];
		floatParams[1] = jsonData["MaterialInfo"]["floatParams1"];
		floatParams[2] = jsonData["MaterialInfo"]["floatParams2"];
		floatParams[3] = jsonData["MaterialInfo"]["floatParams3"];

		vec2Params[0].PreDeserialize(jsonData["MaterialInfo"]["vec2Params0"]);
		vec2Params[1].PreDeserialize(jsonData["MaterialInfo"]["vec2Params1"]);
		vec2Params[2].PreDeserialize(jsonData["MaterialInfo"]["vec2Params2"]);
		vec2Params[3].PreDeserialize(jsonData["MaterialInfo"]["vec2Params3"]);

		vec4Params[0].PreDeserialize(jsonData["MaterialInfo"]["vec4Params0"]);
		vec4Params[1].PreDeserialize(jsonData["MaterialInfo"]["vec4Params1"]);
		vec4Params[2].PreDeserialize(jsonData["MaterialInfo"]["vec4Params2"]);
		vec4Params[3].PreDeserialize(jsonData["MaterialInfo"]["vec4Params3"]);

		color.PreDeserialize(jsonData["MaterialInfo"]["color"]);
	}

	void MaterialInfo::PostSerialize(json& jsonData) const
	{
		vec2Params[0].PostSerialize(jsonData["MaterialInfo"]["vec2Params0"]);
		vec2Params[1].PostSerialize(jsonData["MaterialInfo"]["vec2Params1"]);
		vec2Params[2].PostSerialize(jsonData["MaterialInfo"]["vec2Params2"]);
		vec2Params[3].PostSerialize(jsonData["MaterialInfo"]["vec2Params3"]);

		vec4Params[0].PostSerialize(jsonData["MaterialInfo"]["vec4Params0"]);
		vec4Params[1].PostSerialize(jsonData["MaterialInfo"]["vec4Params1"]);
		vec4Params[2].PostSerialize(jsonData["MaterialInfo"]["vec4Params2"]);
		vec4Params[3].PostSerialize(jsonData["MaterialInfo"]["vec4Params3"]);

		color.PostSerialize(jsonData["MaterialInfo"]["color"]);
	}

	void MaterialInfo::PostDeserialize(const json& jsonData)
	{
		vec2Params[0].PostDeserialize(jsonData["MaterialInfo"]["vec2Params0"]);
		vec2Params[1].PostDeserialize(jsonData["MaterialInfo"]["vec2Params1"]);
		vec2Params[2].PostDeserialize(jsonData["MaterialInfo"]["vec2Params2"]);
		vec2Params[3].PostDeserialize(jsonData["MaterialInfo"]["vec2Params3"]);

		vec4Params[0].PostDeserialize(jsonData["MaterialInfo"]["vec4Params0"]);
		vec4Params[1].PostDeserialize(jsonData["MaterialInfo"]["vec4Params1"]);
		vec4Params[2].PostDeserialize(jsonData["MaterialInfo"]["vec4Params2"]);
		vec4Params[3].PostDeserialize(jsonData["MaterialInfo"]["vec4Params3"]);

		color.PostDeserialize(jsonData["MaterialInfo"]["color"]);
	}

	FF::LightParams LightParams::ToFFLightParams() const
	{
		FF::LightParams lightParams;
		lightParams.lightCount = this->lightCount;
		FF::FFVec3 padding = {this->padding.x,this->padding.y,this->padding.z };
		lightParams.padding = padding;
		for (int i = 0; i < 50; ++i)
		{
			// LightColor
			FF::FFVec4 ambient = { this->lights[i].color.ambient.x, this->lights[i].color.ambient.y, this->lights[i].color.ambient.z,this->lights[i].color.ambient.w };
			lightParams.lights[i].color.ambient = ambient;
			FF::FFVec4 diffuse = { this->lights[i].color.diffuse.x, this->lights[i].color.diffuse.y, this->lights[i].color.diffuse.z,this->lights[i].color.diffuse.w };
			lightParams.lights[i].color.diffuse = diffuse;
			FF::FFVec4 specular = { this->lights[i].color.specular.x, this->lights[i].color.specular.y, this->lights[i].color.specular.z,this->lights[i].color.specular.w };
			lightParams.lights[i].color.specular = specular;

			// Position
			FF::FFVec4 position = { this->lights[i].position.x, this->lights[i].position.y, this->lights[i].position.z, this->lights[i].position.w };
			lightParams.lights[i].position = position;

			// Direction
			FF::FFVec4 direction = { this->lights[i].direction.x, this->lights[i].direction.y, this->lights[i].direction.z, this->lights[i].direction.w };
			lightParams.lights[i].direction = direction;

			// LightType
			lightParams.lights[i].lightType = this->lights[i].lightType;

			// Range
			lightParams.lights[i].range = this->lights[i].range;

			// Angle
			lightParams.lights[i].angle = this->lights[i].angle;

			// Padding
			lightParams.lights[i].padding = this->lights[i].padding;
		}

		return lightParams;
	}

}