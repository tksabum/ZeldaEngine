#pragma once

#include "ZeldaEngineAPI.h"

#include <map>

namespace ZeldaEngine
{
	class ZELDAENGINE_API IOManager
	{
	public:
		// 하나의 파일의 입출력 시작
		void Start();

		// 하나의 파일의 입출력 종료
		void End();

#pragma region Input

		//  등록되지 않은 id의 주소를 등록한다.
		void SetNewAddress(int id, void* address);

		// 등록된 id의 주소를 얻는다.
		void* GetAddress(int id);

#pragma endregion


#pragma region Output

		// 등록되지 않은 주소의 ID를 얻는다.
		int GetNewID(void* address);

		// 등록된 주소의 ID를 얻는다.
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