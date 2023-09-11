#include "pch.h"
#include "EngineSetting.h"

using namespace std;

namespace ZeldaEngine
{
	const int EngineSetting::LAYER_MAX_SIZE = 20;
	const bool EngineSetting::SHOWDEBUGINFO_DEFAULT = true;
	const bool EngineSetting::DEBUGTOGGLEKEYALLOWED_DEFAULT = true;
	const float EngineSetting::WINDOWSIZEX_DEFAULT = 1920.0f;
	const float EngineSetting::WINDOWSIZEY_DEFAULT = 1080.0f;

	int EngineSetting::GetLayerID(wstring layername)
	{
		if (layername == L"")
		{
			return -1;
		}

		for (int i = 0; i < layer.size(); i++)
		{
			if (layer[i] == layername)
			{
				return i;
			}
		}

		return -1;
	}

	void EngineSetting::SetLayerName(int layerid, wstring layername)
	{
		if (0 <= layerid && layerid < layer.size())
		{
			layer[layerid] = layername;
		}
	}

	wstring EngineSetting::GetLayerName(int layerid)
	{
		if (0 <= layerid && layerid < layer.size())
		{
			return layer[layerid];
		}

		return L"";
	}

	void EngineSetting::SetCollisionSetting(bool collision, int layerid1, int layerid2)
	{
		if (layerid1 < 0 || collisionSetting.size() <= layerid1 || layerid2 < 0 || collisionSetting.size() <= layerid2)
		{
			return;
		}

		if (layer[layerid1] == L"")
		{
			return;
		}

		if (layer[layerid2] == L"")
		{
			return;
		}

		collisionSetting[layerid1][layerid2] = collision;
		collisionSetting[layerid2][layerid1] = collision;
	}

	bool EngineSetting::GetCollisionSetting(int layerid1, int layerid2)
	{
		if (layerid1 < 0 || collisionSetting.size() <= layerid1 || layerid2 < 0 || collisionSetting.size() <= layerid2)
		{
			return false;
		}

		if (layer[layerid1] == L"")
		{
			return false;
		}

		if (layer[layerid2] == L"")
		{
			return false;
		}

		return collisionSetting[layerid1][layerid2];
	}

	void EngineSetting::SetDebugToggleKeyAllowed(bool value)
	{
		debugToggleKeyAllowed = value;
	}

	bool EngineSetting::GetDebugToggleKeyAllowed()
	{
		return debugToggleKeyAllowed;
	}

	void EngineSetting::SetShowDebugInfo(bool value)
	{
		showDebugInfo = value;
	}

	bool EngineSetting::GetShowDebugInfo()
	{
		return showDebugInfo;
	}

	void EngineSetting::SetWindowSize(float sizex, float sizey)
	{
		windowSizeX = sizex;
		windowSizeY = sizey;
	}

	float EngineSetting::GetWindowSizeX()
	{
		return windowSizeX;
	}

	float EngineSetting::GetWindowSizeY()
	{
		return windowSizeY;
	}

	void EngineSetting::SetGameName(std::wstring gamename)
	{
		gameName = gamename;
	}

	std::wstring EngineSetting::GetGameName()
	{
		return gameName;
	}

	void EngineSetting::SetStartScene(std::wstring sceneName)
	{
		startSceneName = sceneName;
	}

	std::wstring EngineSetting::GetStartScene()
	{
		return startSceneName;
	}

	void EngineSetting::SetResourceFile(std::wstring resourceName)
	{
		resourceFileName = resourceName;
	}

	std::wstring EngineSetting::GetResourceFile()
	{
		return resourceFileName;
	}

	void EngineSetting::SetPhysicsOverlapMode(PhysicsOverlapMode mode)
	{
		physicsOverlapMode = mode;
	}

	EngineSetting::PhysicsOverlapMode EngineSetting::GetPhysicsOverlapMode()
	{
		return physicsOverlapMode;
	}

	void EngineSetting::SetDefaultGravity(ZeldaMath::Vector3D gravity)
	{
		defaultGravity = gravity;
	}

	ZeldaMath::Vector3D EngineSetting::GetDefaultGravity()
	{
		return defaultGravity;
	}

	EngineSetting& EngineSetting::GetInstance()
	{
		static EngineSetting instance;
		return instance;
	}

	EngineSetting::EngineSetting() :
		graphicsEngine(EngineSetting::GraphicsEngine::FF_GRAPHICS),
		showDebugInfo(SHOWDEBUGINFO_DEFAULT),
		debugToggleKeyAllowed(DEBUGTOGGLEKEYALLOWED_DEFAULT),
		layer(vector<wstring>(LAYER_MAX_SIZE, L"")),
		collisionSetting(vector<vector<bool>>(LAYER_MAX_SIZE, vector<bool>(LAYER_MAX_SIZE, false))),
		windowSizeX(WINDOWSIZEX_DEFAULT),
		windowSizeY(WINDOWSIZEY_DEFAULT),
		startSceneName(L""),
		resourceFileName(L""),
		defaultGravity(ZeldaMath::Vector3D(0.0f, -9.8f, 0.0f))
	{
		layer[0] = L"Default";
	}

	EngineSetting::~EngineSetting()
	{

	}

	void EngineSetting::PreSerialize(json& jsonData) const
	{
		jsonData["EngineSetting"]["GraphicsEngine"] = static_cast<int>(graphicsEngine);
		jsonData["EngineSetting"]["showDebugInfo"] = showDebugInfo;
		jsonData["EngineSetting"]["debugToggleKeyAllowed"] = debugToggleKeyAllowed;
		jsonData["EngineSetting"]["windowSizeX"] = windowSizeX;
		jsonData["EngineSetting"]["windowSizeY"] = windowSizeY;
		jsonData["EngineSetting"]["gameName"] = wstring_to_string(gameName);
		jsonData["EngineSetting"]["startSceneName"] = wstring_to_string(startSceneName);
		jsonData["EngineSetting"]["resourceFileName"] = wstring_to_string(resourceFileName);

		jsonData["EngineSetting"]["physicsOverlapMode"] = static_cast<int>(physicsOverlapMode);

		defaultGravity.PreSerialize(jsonData["EngineSetting"]["defaultGravity"]);

		jsonData["EngineSetting"]["layer"]["size"] = layer.size();
		for (size_t i = 0; i < layer.size(); i++)
		{
			jsonData["EngineSetting"]["layer"][to_string(i)] = wstring_to_string(layer[i]);
		}

		jsonData["EngineSetting"]["collisionSetting"]["size"] = collisionSetting.size();
		for (size_t i = 0; i < collisionSetting.size(); i++)
		{
			jsonData["EngineSetting"]["collisionSetting"][to_string(i)]["size"] = collisionSetting[i].size();
			for (size_t j = 0; j < collisionSetting[i].size(); j++)
			{
				jsonData["EngineSetting"]["collisionSetting"][to_string(i)][to_string(j)] = collisionSetting[i][j];
			}
		}
	}

	void EngineSetting::PreDeserialize(const json& jsonData)
	{
		graphicsEngine = static_cast<GraphicsEngine>(jsonData["EngineSetting"]["GraphicsEngine"]);
		showDebugInfo = jsonData["EngineSetting"]["showDebugInfo"];
		debugToggleKeyAllowed = jsonData["EngineSetting"]["debugToggleKeyAllowed"];
		windowSizeX = jsonData["EngineSetting"]["windowSizeX"];
		windowSizeY = jsonData["EngineSetting"]["windowSizeY"];
		gameName = string_to_wstring(jsonData["EngineSetting"]["gameName"]);
		startSceneName = string_to_wstring(jsonData["EngineSetting"]["startSceneName"]);
		resourceFileName = string_to_wstring(jsonData["EngineSetting"]["resourceFileName"]);

		physicsOverlapMode = static_cast<PhysicsOverlapMode>(jsonData["EngineSetting"]["physicsOverlapMode"]);

		defaultGravity.PreDeserialize(jsonData["EngineSetting"]["defaultGravity"]);

		size_t layersize = jsonData["EngineSetting"]["layer"]["size"];
		layer.clear();
		for (size_t i = 0; i < layersize; i++)
		{
			layer.push_back(string_to_wstring(jsonData["EngineSetting"]["layer"][to_string(i)]));
		}

		// collisionSetting ÃÊ±âÈ­
		for (size_t i = 0; i < collisionSetting.size(); i++)
		{
			collisionSetting[i].clear();
		}
		collisionSetting.clear();

		size_t collisionSettingSizeI = jsonData["EngineSetting"]["collisionSetting"]["size"];
		collisionSetting.resize(collisionSettingSizeI);
		for (size_t i = 0; i < collisionSettingSizeI; i++)
		{
			size_t collisionSettingSizeJ = jsonData["EngineSetting"]["collisionSetting"][to_string(i)]["size"];
			collisionSetting[i].resize(collisionSettingSizeJ);
			for (size_t j = 0; j < collisionSettingSizeJ; j++)
			{
				collisionSetting[i][j] = jsonData["EngineSetting"]["collisionSetting"][to_string(i)][to_string(j)];
			}
		}
	}

	void EngineSetting::PostSerialize(json& jsonData) const
	{
		defaultGravity.PostSerialize(jsonData["EngineSetting"]["defaultGravity"]);
	}

	void EngineSetting::PostDeserialize(const json& jsonData)
	{
		defaultGravity.PostDeserialize(jsonData["EngineSetting"]["defaultGravity"]);
	}
}