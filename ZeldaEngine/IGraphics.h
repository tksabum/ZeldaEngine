#pragma once

#include <string>
#include <vector>
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "ZeldaColor.h"

#include "GraphicsDefine.h"

namespace ZeldaEngine
{
	class IGraphics
	{
	public:
		virtual void Initialize(long long p_hWnd, float p_width, float p_height, bool p_isWindowed) abstract;
		virtual void Finalize() abstract;
		virtual void BeginDraw(float p_deltaTime) abstract;
		virtual void EndDraw() abstract;
		virtual void Resize(unsigned int width, unsigned int height) abstract;

		virtual void DrawMesh(const ZeldaMath::Matrix4x4& p_WTM, const std::wstring& p_mesh, const MaterialInfo& p_material, std::wstring& p_materialName, int p_materialIndex) abstract;
		//virtual void DrawSkinnedMesh(const unsigned __int64 p_id, const std::wstring& p_currentAnimationName ,const ZeldaMath::Matrix4x4& p_WTM, const std::wstring& p_mesh, const MaterialInfo& p_material, std::wstring& p_materialName) abstract;
		virtual void DrawParticle(const ZeldaMath::Matrix4x4& p_WTM, float p_deltaTime, unsigned __int32 p_maxParticle, float p_createInterval, float p_accTime, float p_minLifeTime, float p_maxLifeTime, float p_minSpeed, float p_maxSpeed, float p_startScale, float p_endScale) abstract;

		virtual void DrawDeferredDebug() abstract;
		virtual void SetCameraInfo(unsigned __int32 p_cameraIndex, float p_naer, float p_far, float p_fov, float p_scale, float p_width, float p_height, const ZeldaMath::Matrix4x4& p_WTM) abstract;
		virtual void SetLightData(void* p_buffer, unsigned __int32 p_size) abstract;

		virtual void CreateMesh(const std::wstring& p_path) abstract;
		virtual void CreateShader(const std::wstring& p_path, const ShaderInfo& p_info) abstract;
		virtual void CreateTexture(const std::wstring& p_path) abstract;
		virtual void CreateMaterial(const std::wstring& p_path, const MaterialInfo& p_desc) abstract;
		virtual void LoadFBX(const std::wstring& p_path, const std::wstring& p_shaderPath) abstract;
		virtual std::vector<FF::FFMeshData>& GetMeshDataFromFBX(const std::wstring& p_path) abstract;

		virtual void PushAnimationData(unsigned __int64 p_id, __int32 p_frame, __int32 p_nextFrame, float p_frameRatio) abstract;

		virtual void DrawSkinnedMesh(unsigned __int64 p_id, __int32 p_frame, __int32 p_nextFrame, float p_frameRatio, const std::wstring& p_currentAnimationName, const ZeldaMath::Matrix4x4& p_WTM, const std::wstring& p_mesh, const MaterialInfo& p_material, std::wstring& p_materialName,int p_materialIndex) abstract;

		// 에디터에 제공할 GPU_DESCRIPTOR_HANDLE값
		virtual unsigned __int64 GetGPUHandle() abstract;

		virtual void DrawTest() abstract;
	};
}