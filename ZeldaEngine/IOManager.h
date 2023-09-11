#pragma once

#include "ZeldaEngineAPI.h"

#include <map>

namespace ZeldaEngine
{
	class ZELDAENGINE_API IOManager
	{
	public:
		// �ϳ��� ������ ����� ����
		void Start();

		// �ϳ��� ������ ����� ����
		void End();

#pragma region Input

		//  ��ϵ��� ���� id�� �ּҸ� ����Ѵ�.
		void SetNewAddress(int id, void* address);

		// ��ϵ� id�� �ּҸ� ��´�.
		void* GetAddress(int id);

#pragma endregion


#pragma region Output

		// ��ϵ��� ���� �ּ��� ID�� ��´�.
		int GetNewID(void* address);

		// ��ϵ� �ּ��� ID�� ��´�.
		int GetID(void* address);

#pragma endregion

	private:
		int idCounter;
		bool isRunning;
		std::map<int, void*> addressMap;
		std::map<void*, int> idMap;

		//singleton
	public:
		static IOManager& GetInstance();

	private:
		IOManager();
		~IOManager();
		IOManager(const IOManager& ref);
		IOManager& operator=(const IOManager& ref) = delete;
	};
}