#include "pch.h"
#include "FFGraphics.h"

#include "ResourceManager.h"
#include "PhysicsSystem.h"



#include "TimeController.h"
#include "SceneManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include <list>

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	FFGraphics::FFGraphics() : graphics(nullptr)
	{

	}

	FFGraphics::~FFGraphics()
	{

	}

	void FFGraphics::Initialize(long long p_hWnd, float p_width, float p_height, bool p_isWindowed)
	{
		graphics = CreateFFRenderer();
		graphics->Init(p_hWnd, p_width, p_height, p_isWindowed);
	}

	void FFGraphics::Finalize()
	{
		DeleteFFRenderer(&graphics);
	}

	void ZeldaEngine::FFGraphics::BeginDraw(float p_deltaTime)
	{
		graphics->RenderBegin(p_deltaTime);
	}

	void ZeldaEngine::FFGraphics::EndDraw()
	{
		graphics->RenderEnd();
	}

	void ZeldaEngine::FFGraphics::Resize(unsigned int width, unsigned int height)
	{
		graphics->ResizeWindow(width, height);
	}

	void FFGraphics::DrawMesh(const ZeldaMath::Matrix4x4& p_WTM, const std::wstring& p_mesh, const MaterialInfo& p_material, std::wstring& p_materialName, int p_materialIndex)
	{
		graphics->DrawMesh(Matrix4x4ToFFMatrix(p_WTM), p_mesh, p_material.ToFFMaterialDesc(), p_materialName,p_materialIndex);
	}

	//void FFGraphics::DrawSkinnedMesh(const unsigned __int64 p_id, const std::wstring& p_currentAnimationName ,const ZeldaMath::Matrix4x4& p_WTM, const std::wstring& p_mesh, const MaterialInfo& p_material, std::wstring& p_materialName)
	//{
	//	graphics->DrawSkinnedMesh(p_id, p_currentAnimationName, Matrix4x4ToFFMatrix(p_WTM), p_mesh, p_material.ToFFMaterialDesc(), p_materialName);
	//}

	void FFGraphics::DrawParticle(const ZeldaMath::Matrix4x4& p_WTM, float p_deltaTime, unsigned __int32 p_maxParticle, float p_createInterval, float p_accTime, float p_minLifeTime, float p_maxLifeTime, float p_minSpeed, float p_maxSpeed, float p_startScale, float p_endScale)
	{
		graphics->DrawParticle(Matrix4x4ToFFMatrix(p_WTM), p_deltaTime, p_maxParticle, p_createInterval, p_accTime, p_minLifeTime, p_maxLifeTime, p_minSpeed, p_maxSpeed, p_startScale, p_endScale);
	}

	void FFGraphics::DrawDeferredDebug()
	{
		graphics->DrawDeferredDebug();
	}

	void FFGraphics::SetCameraInfo(unsigned __int32 p_cameraIndex, float p_near, float p_far, float p_fov, float p_scale, float p_width, float p_height, const ZeldaMath::Matrix4x4& p_WTM)
	{
		if (p_width == 0.0f) p_width = 1.0f;
		if (p_height == 0.0f) p_height = 1.0f;

		// FFGraphics는 CamaraIndex를 0만을 사용한다.
		graphics->SetCameraInfo(0, p_near, p_far, p_fov, p_scale, p_width, p_height, Matrix4x4ToFFMatrix(p_WTM));
	}

	void FFGraphics::SetLightData(void* p_buffer, unsigned __int32 p_size)
	{
		graphics->SetLightData(p_buffer, p_size);
	}

	void FFGraphics::CreateMesh(const std::wstring& p_path)
	{
		graphics->CreateMesh(p_path);
	}

	void FFGraphics::CreateShader(const std::wstring& p_path, const ShaderInfo& p_info)
	{
		graphics->CreateShader(p_path, p_info.ToFFShaderInfo());
	}

	void FFGraphics::CreateTexture(const std::wstring& p_path)
	{
		graphics->CreateTexture(p_path);
	}

	void FFGraphics::CreateMaterial(const std::wstring& p_path, const MaterialInfo& p_desc)
	{
		graphics->CreateMaterial(p_path, p_desc.ToFFMaterialDesc());
	}

	void FFGraphics::LoadFBX(const std::wstring& p_path, const std::wstring& p_shaderPath)
	{
		graphics->LoadFBX(p_path, p_shaderPath);
	}

	std::vector<FF::FFMeshData>& FFGraphics::GetMeshDataFromFBX(const std::wstring& p_path)
	{
		return graphics->GetMeshDataFromFBX(p_path);
	}

	void FFGraphics::PushAnimationData(unsigned __int64 p_id, __int32 p_frame, __int32 p_nextFrame, float p_frameRatio)
	{
		graphics->PushAnimationData(p_id, p_frame, p_nextFrame, p_frameRatio);
	}

	void FFGraphics::DrawSkinnedMesh(unsigned __int64 p_id, __int32 p_frame, __int32 p_nextFrame, float p_frameRatio, const std::wstring& p_currentAnimationName, const ZeldaMath::Matrix4x4& p_WTM, const std::wstring& p_mesh, const MaterialInfo& p_material, std::wstring& p_materialName, int p_materialIndex)
	{
		graphics->DrawSkinnedMesh(p_id, p_frame, p_nextFrame, p_frameRatio,p_currentAnimationName,
			Matrix4x4ToFFMatrix(p_WTM),
			p_mesh,
			p_material.ToFFMaterialDesc(),
			p_materialName,
			p_materialIndex);
	}

	unsigned __int64 FFGraphics::GetGPUHandle()
	{
		return graphics->GetGPUHandle();
	}

	void FFGraphics::DrawTest()
	{

	}
}