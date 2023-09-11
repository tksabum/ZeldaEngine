#include "pch.h"
#include "ZeldaEngine.h"

#include "EngineSetting.h"

#include <iostream>
#include <fstream>

namespace ZeldaEngine
{
	_In_ int cmdShow;

	void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gamename, _In_ int nCmdShow, unsigned int width, unsigned int height, bool screenresizeable, bool notitlebar, bool minimizable, bool maximizable)
	{
		// 엔진의 컴포넌트 등록
		ComponentFactory::RegisterComponent<Transform>(L"Transform");
		ComponentFactory::RegisterComponent<Camera>(L"Camera");
		ComponentFactory::RegisterComponent<Rigidbody>(L"Rigidbody");

		ComponentFactory::RegisterComponent<CubeCollider>(L"CubeCollider");
		ComponentFactory::RegisterComponent<SphereCollider>(L"SphereCollider");
		ComponentFactory::RegisterComponent<CylinderCollider>(L"CylinderCollider");
		ComponentFactory::RegisterComponent<CapsuleCollider>(L"CapsuleCollider");
		ComponentFactory::RegisterComponent<Collider2D>(L"Collider2D");
		
		ComponentFactory::RegisterComponent<DirectionalLight>(L"DirectionalLight");

		ComponentFactory::RegisterComponent<MeshRenderer>(L"MeshRenderer");

		ComponentFactory::RegisterComponent<TestComponent>(L"TestComponent");
		ComponentFactory::RegisterComponent<TestComponent2>(L"TestComponent2");

		ComponentFactory::RegisterComponent<ZeldaUI>(L"ZeldaUI");
		ComponentFactory::RegisterComponent<Image>(L"Image");
		ComponentFactory::RegisterComponent<Button>(L"Button");

		ComponentFactory::RegisterComponent<SkinnedMeshRenderer>(L"SkinnedMeshRenderer");
		ComponentFactory::RegisterComponent<Animator>(L"Animator");

		ComponentFactory::RegisterComponent<SpriteRenderer>(L"SpriteRenderer");
		ComponentFactory::RegisterComponent<SpriteAnimator>(L"SpriteAnimator");
		ComponentFactory::RegisterComponent<ParticleRenderer>(L"ParticleRenderer");



		CoreSystem::GetInstance().Initialize(hInstance, gamename, width, height, screenresizeable, notitlebar, minimizable, maximizable);

		cmdShow = nCmdShow;
	}

	void Finalize()
	{
		CoreSystem::GetInstance().Finalize();
	}

	void Run()
	{
		CoreSystem::GetInstance().Run(cmdShow);
		CoreSystem::GetInstance().Finalize();
	}

	void SaveScene()
	{
		json sceneData = SceneManager::GetInstance().Serialize();

		CreateDirectory((EngineSetting::GetInstance().GetGameName() + L"//SceneData").c_str(), NULL);

		// JSON 파일로 출력
		std::ofstream outputFile(EngineSetting::GetInstance().GetGameName() + L"//SceneData//" + SceneManager::GetInstance().GetName() + L".scene");
		if (outputFile.is_open())
		{
			outputFile << sceneData.dump(4);  // 들여쓰기와 함께 JSON 출력
			outputFile.close();
			std::cout << "JSON 파일이 생성되었습니다." << std::endl;
		}
		else
		{
			std::cerr << "파일을 열 수 없습니다." << std::endl;
		}
	}

	void LoadScene(std::wstring fileName)
	{
		if (CoreSystem::GetInstance().IsRun())
		{
			CoreSystem::GetInstance().LoadScene(fileName);
		}
		else
		{
			SceneManager::GetInstance().DeleteAllGameObject();

			// JSON 파일에서 데이터 읽기
			std::ifstream inputFile(EngineSetting::GetInstance().GetGameName() + L"//SceneData//" + fileName + L".scene");
			if (inputFile.is_open())
			{
				json jsonData;
				inputFile >> jsonData;

				// JSON 역직렬화
				SceneManager::GetInstance().Deserialize(jsonData);

				inputFile.close();
			}
			else
			{
				std::cerr << "파일을 열 수 없습니다." << std::endl;
			}
		}
	}

	void SaveSetting()
	{
		json settingData = EngineSetting::GetInstance().Serialize();

		CreateDirectory(EngineSetting::GetInstance().GetGameName().c_str(), NULL);

		// JSON 파일로 출력
		std::ofstream outputFile(EngineSetting::GetInstance().GetGameName() + L"//" + EngineSetting::GetInstance().GetGameName() + L".zproj");
		if (outputFile.is_open())
		{
			outputFile << settingData.dump(4);  // 들여쓰기와 함께 JSON 출력
			outputFile.close();
			std::cout << "JSON 파일이 생성되었습니다." << std::endl;
		}
		else
		{
			std::cerr << "파일을 열 수 없습니다." << std::endl;
		}
	}

	void LoadSetting(std::wstring fileName)
	{
		// JSON 파일에서 데이터 읽기
		std::ifstream inputFile(fileName + L"//" + fileName + L".zproj");
		if (inputFile.is_open())
		{
			json jsonData;
			inputFile >> jsonData;

			// JSON 역직렬화
			EngineSetting::GetInstance().Deserialize(jsonData);

			inputFile.close();
		}
		else
		{
			std::cerr << "파일을 열 수 없습니다." << std::endl;
		}
	}

	void SaveResourceData(std::wstring fileName)
	{
		json resourceData = ResourceManager::GetInstance().Serialize();

		CreateDirectory((EngineSetting::GetInstance().GetGameName() + L"//Resources").c_str(), NULL);

		// JSON 파일로 출력
		std::ofstream outputFile(EngineSetting::GetInstance().GetGameName() + L"//Resources//" + fileName + L".zrsc");
		if (outputFile.is_open())
		{
			outputFile << resourceData.dump(4);  // 들여쓰기와 함께 JSON 출력
			outputFile.close();
			std::cout << "JSON 파일이 생성되었습니다." << std::endl;
		}
		else
		{
			std::cerr << "파일을 열 수 없습니다." << std::endl;
		}
	}

	void LoadResourceData(std::wstring fileName)
	{
		// JSON 파일에서 데이터 읽기
		std::ifstream inputFile(EngineSetting::GetInstance().GetGameName() + L"//Resources//" + fileName + L".zrsc");
		if (inputFile.is_open())
		{
			json jsonData;
			inputFile >> jsonData;

			// JSON 역직렬화
			ResourceManager::GetInstance().Deserialize(jsonData);

			inputFile.close();
		}
		else
		{
			std::cerr << "파일을 열 수 없습니다." << std::endl;
		}
	}

	GameObject* CreateGameObjectFromFBX(std::wstring fbxName)
	{
		const std::vector<FF::FFMeshData>& meshData = ResourceManager::GetInstance().GetFFMeshData(fbxName);

		GameObject* root = SceneManager::GetInstance().CreateGameObject(fbxName);

		for (size_t i = 0; i < meshData.size(); ++i)
		{
			GameObject* object = SceneManager::GetInstance().CreateGameObject(meshData[i].meshName);
			object->GetTransform()->SetLocalRotation(ZeldaMath::Vector3D(0,180,0));
			root->GetTransform()->AddChild(object->GetTransform());

			if (!meshData[i].hasAnimation)
			{
				MeshRenderer* renderer = object->AddComponent<MeshRenderer>();
				renderer->SetMesh(meshData[i].meshName);

				for (size_t j = 0; j < meshData[i].vecMaterialDesc.size(); ++j)
				{
					MaterialInfo info = {};
					info.SetForFFMaterialDesc(meshData[i].vecMaterialDesc[j]);
					renderer->PushMaterialInfo(info);
				}
			}
			else
			{
				SkinnedMeshRenderer* renderer = object->AddComponent<SkinnedMeshRenderer>();
				renderer->SetMesh(meshData[i].meshName);

				for (size_t j = 0; j < meshData[i].vecMaterialDesc.size(); ++j)
				{
					MaterialInfo info = {};
					info.SetForFFMaterialDesc(meshData[i].vecMaterialDesc[j]);
					renderer->PushMaterialInfo(info);
				}
			}
		}

		return root;
	}

	FF::FFAnimationClip& GetAnimationClip(std::wstring animationName)
	{
		return  ResourceManager::GetInstance().GetAnimationClip(animationName);
	}
}