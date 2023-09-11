#include "sampleComponent.h"

void SampleComponent::Awake()
{
	moveSpeed = 10.0f;
}

void SampleComponent::Update()
{
	// 씬 매니저에서 메인 카메라를 얻어오거나 메인카메라를 다른 카메라로 바꿀 수 있음
	Camera* maincamera = SceneManager::GetInstance().GetMainCamera();
	GameObject* cameraobject = maincamera->GetGameObject();

	// TimeController에서 DeltaTime을 얻어오거나 TimeScale을 변경하는 등 시간관련된 것들을 할 수 있음
	float deltaTime = TimeController::GetInstance().GetDeltaTime();

	// Input에서 키입력, 마우스입력에 관련된 것들을 할 수 있음
	if (Input::GetInstance().PressedKey(VK_UP))
	{
		Vector3D pos = cameraobject->GetTransform()->GetLocalPosition();
		cameraobject->GetTransform()->SetLocalPosition(pos + Vector3D::up * deltaTime * moveSpeed);
	}
}

void SampleComponent::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(SampleComponent); // 반드시 해줘야되는 것

	PRESERIALIZE_SERVALUE(vector);

	// value = new SAMPLE();
	// PRESERIALIZE_SERPTR(value);

	//PRESERIALIZE_VALUE(moveSpeed);
}

void SampleComponent::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(SampleComponent);  // 반드시 해줘야되는 것

	PREDESERIALIZE_SERVALUE(vector);

	//PREDESERIALIZE_VALUE(moveSpeed);
}

void SampleComponent::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();  // 반드시 해줘야되는 것

	// 다른 곳에서 생성한 객체의 주소만 가져오는 경우
	POSTSERIALIZE_PTR(cameraptr);

	POSTSERIALIZE_SERVALUE(vector);
}

void SampleComponent::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();  // 반드시 해줘야되는 것

	// 다른 곳에서 생성한 객체의 주소만 가져오는 경우
	POSTDESERIALIZE_PTR(cameraptr);

	POSTDESERIALIZE_SERVALUE(vector);
}
