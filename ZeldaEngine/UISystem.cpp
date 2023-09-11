#include "pch.h"
#include "UISystem.h"

#include "GameObject.h"
#include "Transform.h"
#include "Input.h"
#include "GraphicsSystem.h"
#include "ZeldaUI.h"

#include "ZeldaMath.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	void UISystem::AddUI(ZeldaUI* zeldaUI)
	{
		uiList.push_back(zeldaUI);
	}
	void UISystem::RemoveUI(ZeldaUI* zeldaUI)
	{
		uiList.erase(remove(uiList.begin(), uiList.end(), zeldaUI), uiList.end());
	}

	void UISystem::Initialize(HWND hwnd)
	{
		hWnd = hwnd;
	}

	void UISystem::UIEvent()
	{
		// current�� last�� �̵�
		lastMouseOver = currentMouseOver;

		// uiList�� ����� ���
		if (currentMouseOver.size() != uiList.size())
		{
			lastMouseOver.resize(uiList.size(), false);
			currentMouseOver.resize(uiList.size(), false);
		}

		// â�� Ȱ��ȭ�� ��쿡�� �浹 ������ �Ѵ�.
		if (hWnd == GetFocus())
		{
			Vector2D leftUpPos = Input::GetInstance().GetMousePosition();
			Vector2D windowSize = GraphicsSystem::GetInstance().GetWindowSize();

			Vector3D pos = Vector3D(leftUpPos.x - (windowSize.x / 2.0f), (windowSize.y / 2.0f) - leftUpPos.y, 0.0f);

			for (size_t i = 0; i < uiList.size(); i++)
			{
				// �ϴ� true�� �صΰ� �浹 ���ϸ� false�� ����
				currentMouseOver[i] = true;

				// ��Ȱ��ȭ �Ǿ��ִ� ������Ʈ�� �浹���� ���� ������ ����Ѵ�.
				if (uiList[i]->GetGameObject()->GetRootActive() == false)
				{
					currentMouseOver[i] = false;
					continue;
				}

				Matrix4x4 matrix = uiList[i]->GetGameObject()->GetTransform()->GetWorldMatrix();

				Vector3D v[4] =
				{
					matrix * Vector3D(0.5f, 0.5f, 0.0f),
					matrix * Vector3D(0.5f, -0.5f, 0.0f),
					matrix * Vector3D(-0.5f, -0.5f, 0.0f),
					matrix * Vector3D(-0.5f, 0.5f, 0.0f),
				};

				Vector3D cross = Vector3D::zero;

				for (int n = 0; n < 4; n++)
				{
					int m = (n + 1) % 4;

					Vector3D v0 = v[n];
					Vector3D v1 = v[m];

					if (cross == Vector3D::zero)
					{
						cross = CrossProduct(v1 - v0, pos - v0);
					}
					else
					{
						float dot = DotProduct(cross, CrossProduct(v1 - v0, pos - v0));

						// �浹���� �ʴ� ���
						if (dot < 0.0f)
						{
							currentMouseOver[i] = false;
						}
					}
				}
			}
		}

		// OnMouseEnterEvent
		for (int i = 0; i < uiList.size(); i++)
		{
			if (lastMouseOver[i] == false && currentMouseOver[i] == true)
			{
				uiList[i]->GetGameObject()->OnMouseEnterEvent();
			}
		}

		// OnMouseExitEvent
		for (int i = 0; i < uiList.size(); i++)
		{
			if (lastMouseOver[i] == true && currentMouseOver[i] == false)
			{
				uiList[i]->GetGameObject()->OnMouseExitEvent();
			}
		}

		// OnMouseOverEvent
		for (int i = 0; i < uiList.size(); i++)
		{
			if (lastMouseOver[i] == true && currentMouseOver[i] == true)
			{
				uiList[i]->GetGameObject()->OnMouseOverEvent();
			}
		}


		// OnMouseLeftDownEvent
		if (Input::GetInstance().PressedKeyNow(VK_LBUTTON))
		{
			for (int i = 0; i < uiList.size(); i++)
			{
				if (currentMouseOver[i])
				{
					uiList[i]->GetGameObject()->OnMouseLeftDownEvent();
				}
			}
		}

		// OnMouseLeftUpEvent
		if (Input::GetInstance().ReleasedKeyNow(VK_LBUTTON))
		{
			for (int i = 0; i < uiList.size(); i++)
			{
				if (currentMouseOver[i])
				{
					uiList[i]->GetGameObject()->OnMouseLeftUpEvent();
				}
			}
		}

		// OnMouseRightDownEvent
		if (Input::GetInstance().PressedKeyNow(VK_RBUTTON))
		{
			for (int i = 0; i < uiList.size(); i++)
			{
				if (currentMouseOver[i])
				{
					uiList[i]->GetGameObject()->OnMouseRightDownEvent();
				}
			}
		}

		// OnMouseRightUpEvent
		if (Input::GetInstance().ReleasedKeyNow(VK_RBUTTON))
		{
			for (int i = 0; i < uiList.size(); i++)
			{
				if (currentMouseOver[i])
				{
					uiList[i]->GetGameObject()->OnMouseRightUpEvent();
				}
			}
		}
	}

	UISystem& UISystem::GetInstance()
	{
		static UISystem instance;
		return instance;
	}

	UISystem::UISystem() : hWnd(NULL)
	{

	}

	UISystem::~UISystem()
	{

	}
}