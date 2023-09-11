#include "sampleComponent.h"

void SampleComponent::Awake()
{
	moveSpeed = 10.0f;
}

void SampleComponent::Update()
{
	// �� �Ŵ������� ���� ī�޶� �����ų� ����ī�޶� �ٸ� ī�޶�� �ٲ� �� ����
	Camera* maincamera = SceneManager::GetInstance().GetMainCamera();
	GameObject* cameraobject = maincamera->GetGameObject();

	// TimeController���� DeltaTime�� �����ų� TimeScale�� �����ϴ� �� �ð����õ� �͵��� �� �� ����
	float deltaTime = TimeController::GetInstance().GetDeltaTime();

	// Input���� Ű�Է�, ���콺�Է¿� ���õ� �͵��� �� �� ����
	if (Input::GetInstance().PressedKey(VK_UP))
	{
		Vector3D pos = cameraobject->GetTransform()->GetLocalPosition();
		cameraobject->GetTransform()->SetLocalPosition(pos + Vector3D::up * deltaTime * moveSpeed);
	}
}

void SampleComponent::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(SampleComponent); // �ݵ�� ����ߵǴ� ��

	PRESERIALIZE_SERVALUE(vector);

	// value = new SAMPLE();
	// PRESERIALIZE_SERPTR(value);

	//PRESERIALIZE_VALUE(moveSpeed);
}

void SampleComponent::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(SampleComponent);  // �ݵ�� ����ߵǴ� ��

	PREDESERIALIZE_SERVALUE(vector);

	//PREDESERIALIZE_VALUE(moveSpeed);
}

void SampleComponent::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();  // �ݵ�� ����ߵǴ� ��

	// �ٸ� ������ ������ ��ü�� �ּҸ� �������� ���
	POSTSERIALIZE_PTR(cameraptr);

	POSTSERIALIZE_SERVALUE(vector);
}

void SampleComponent::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();  // �ݵ�� ����ߵǴ� ��

	// �ٸ� ������ ������ ��ü�� �ּҸ� �������� ���
	POSTDESERIALIZE_PTR(cameraptr);

	POSTDESERIALIZE_SERVALUE(vector);
}
