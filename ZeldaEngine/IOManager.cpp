#include "pch.h"
#include "IOManager.h"

#include <cassert>

namespace ZeldaEngine
{
	void IOManager::Start()
	{
		assert(isRunning == false);
		isRunning = true;

		idCounter = 0;
		addressMap.clear();
		idMap.clear();
	}
	void IOManager::End()
	{
		assert(isRunning == true);
		isRunning = false;
	}

	void IOManager::SetNewAddress(int id, void* address)
	{
		assert(addressMap.count(id) == 0);
		addressMap[id] = address;
	}

	void* IOManager::GetAddress(int id)
	{
		if (id == -1) return nullptr;

		assert(addressMap.count(id) > 0);
		return addressMap[id];
	}

	int IOManager::GetNewID(void* address)
	{
		assert(idMap.count(address) == 0);
		idMap[address] = idCounter;
		idCounter++;

		return idMap[address];
	}

	int IOManager::GetID(void* address)
	{
		if (address == nullptr) return -1;

		assert(idMap.count(address) > 0);
		return idMap[address];
	}

	IOManager& IOManager::GetInstance()
	{
		static IOManager instance;
		return instance;
	}

	IOManager::IOManager() : isRunning(false), idCounter(0)
	{

	}

	IOManager::~IOManager()
	{

	}
}