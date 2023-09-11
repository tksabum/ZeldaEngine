#include "SoundBar.h"

#include "SoundHandle.h"
#include "ScrollUI.h"

void SoundBar::Awake()
{
	if (scroll == nullptr)
	{
		scroll = GetGameObject()->GetComponent<ScrollUI>();
	}
}

void SoundBar::Start()
{
	lastMousePos = Input::GetInstance().GetMousePosition();
	currentMousePos = Input::GetInstance().GetMousePosition();

	float vol = SoundManager::GetInstance().GetBGMVolume();

	if (handle)
	{
		handle->GetGameObject()->SetActive(true);
		handle->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D((vol * 2 - 1.0f) *(UISize.x / 2 - padding), 0, 0));
	}
}

void SoundBar::Update()
{
	if (handle && scroll)
	{
		if (handle->IsMouseLeftDown())
		{
			scrolling = true;
			lastMousePos = Input::GetInstance().GetMousePosition();
		}

		if (scrolling == true && (Input::GetInstance().ReleasedKey(VK_LBUTTON, true) || (GetGameObject()->GetRootActive() == false)))
		{
			scrolling = false;
		}

		if (scrolling)
		{
			currentMousePos = Input::GetInstance().GetMousePosition();
			handlePos = handle->GetGameObject()->GetTransform()->GetLocalPosition();

			float deltaX = currentMousePos.x - lastMousePos.x;

			handle->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(handlePos.x + deltaX, 0, 0));

			if (handlePos.x + deltaX > UISize.x / 2 - padding)
			{
				handle->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(UISize.x / 2 - padding, 0, 0));
			}
			else if (handlePos.x + deltaX < - UISize.x / 2 + padding)
			{
				handle->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(-(UISize.x / 2 - padding), 0, 0));
			}

			float vol = (handle->GetGameObject()->GetTransform()->GetLocalPosition().x / (UISize.x / 2 - padding) + 1.0f) / 2;
			
			SoundManager::GetInstance().SetVolume(vol);
			SoundManager::GetInstance().SetBGM(vol);
		}

		lastMousePos = currentMousePos;
	}
}

void SoundBar::SetHandle(const SoundHandle& handle)
{
	this->handle = const_cast<SoundHandle*>(&handle);
}

void SoundBar::SetUISize(const Vector2D& size)
{
	this->UISize = size;
}

void SoundBar::SetPadding(float padding)
{
	this->padding = padding;
}

void SoundBar::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(SoundBar);

	PRESERIALIZE_SERVALUE(UISize);
	PRESERIALIZE_VALUE(padding);
}

void SoundBar::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(SoundBar);

	PREDESERIALIZE_SERVALUE(UISize);
	PREDESERIALIZE_VALUE(padding);
}

void SoundBar::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(handle);
	POSTSERIALIZE_PTR(scroll);
}

void SoundBar::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(handle);
	POSTDESERIALIZE_PTR(scroll);
}
