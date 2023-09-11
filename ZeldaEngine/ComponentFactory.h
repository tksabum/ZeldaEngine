#pragma once

#include "ZeldaEngineAPI.h"

#include <string>
#include <unordered_map>
#include <functional>

namespace ZeldaEngine
{
    class Component;

    class ZELDAENGINE_API ComponentFactory {
    public:
        template<typename T>
        static void RegisterComponent(const std::wstring& className) {
            componentRegistry()[className] = []()
            {
                static_assert(std::is_base_of<Component, T>::value, "The class you are trying to create is not Component");

#ifdef _DEBUG
                Component::SetAddComponentFlag(true);
#endif // _DEBUG

                T* component = new T();

#ifdef _DEBUG
                Component::SetAddComponentFlag(false);
#endif // _DEBUG

                return component;
            };
        }

    private:
        typedef std::unordered_map<std::wstring, std::function<Component* ()>> ComponentRegistry;
        
        static Component* CreateComponentByName(const std::wstring& className) {
            auto it = componentRegistry().find(className);
            if (it != componentRegistry().end()) {
                return it->second();
            }
            return nullptr; // 등록되지 않은 스크립트 클래스일 경우 nullptr 반환
        }

        static ComponentRegistry& componentRegistry() {
            static ComponentRegistry registry;
            return registry;
        }

        friend class GameObject;
    };
}