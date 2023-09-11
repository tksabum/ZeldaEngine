#pragma once

#include "IGraphics.h"

#include "IFFGraphics.h"

namespace ZeldaEngine
{
	class FFGraphics : public IGraphics
	{
	public:
		FFGraphics();
		virtual ~FFGraphics();

		// IGraphics을(를) 통해 상속됨
		virtual void Initialize(long long p_hWnd, float p_width, float p_height, bool p_isWindowed) override;
		virtual void Finalize() override;
		virtual void BeginDraw(float p_deltaTime) override;
		virtual void EndDraw() override;
		virtual void Resize(unsigned int width, unsigned int height) override;

		virtual void DrawMesh(const ZeldaMath::Matrix4x4& p_WTM, const std::wstring& p_mesh, const MaterialInfo& p_material, std::wstring& p_materialName, int p_materialIndex) override;
		//virtual void DrawSkinnedMesh(const unsigned __int64 p_id, const std::wstring& p_currentAnimationName ,const ZeldaMath::Matrix4x4& p_WTM, const std::wstring& p_mesh, const MaterialInfo& p_material, std::wstring& p_materialName) override;
		virtual void DrawParticle(const ZeldaMath::Matrix4x4& p_WTM, float p_deltaTime, unsigned __int32 p_maxParticle, float p_createInterval, float p_accTime, float p_minLifeTime, float p_maxLifeTime, float p_minSpeed, float p_maxSpeed, float p_startScale, float p_endScale) override;

		virtual void DrawDeferredDebug() override;
		virtual void SetCameraInfo(unsigned __int32 p_cameraIndex, float p_near, float p_far, float p_fov, float p_scale, float p_width, float p_height, const ZeldaMath::Matrix4x4& p_WTM) override;
		virtual void SetLightData(void* p_buffer, unsigned __int32 p_size) override;

		virtual void CreateMesh(const std::wstring& p_path) override;
		virtual void CreateShader(const std::wstring& p_path, const ShaderInfo& p_info) override;
		virtual void CreateTexture(const std::wstring& p_path) override;
		virtual void CreateMaterial(const std::wstring& p_path, const MaterialInfo& p_desc) override;
		virtual void LoadFBX(const std::wstring& p_path, const std::wstring& p_shaderPath) override;
		virtual std::vector<FF::FFMeshData>& GetMeshDataFromFBX(const std::wstring& p_path) override;

		virtual void PushAnimationData(unsigned __int64 p_id, __int32 p_frame, __int32 p_nextFrame, float p_frameRatio) override;

		virtual void DrawSkinnedMesh(unsigned __int64 p_id, __int32 p_frame, __int32 p_nextFrame, float p_frameRatio, const std::wstring& p_currentAnimationName, const ZeldaMath::Matrix4x4& p_WTM, const std::wstring& p_mesh, const MaterialInfo& p_material, std::wstring& p_materialName, int p_materialIndex) override;

		// 에디터에 제공할 GPU_DESCRIPTOR_HANDLE값
		virtual unsigned __int64 GetGPUHandle() override;

		virtual void DrawTest() override;

	private:
		IFFGraphics* graphics;

	};
}