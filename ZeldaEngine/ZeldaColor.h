#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

namespace ZeldaEngine
{
    class ZELDAENGINE_API ZeldaColor : public Serializable
    {
    public:
        float r;
        float g;
        float b;
        float alpha;

        ZeldaColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
        ZeldaColor(const ZeldaColor& color);

        bool operator==(const ZeldaColor& right);
        ZeldaColor& operator=(const ZeldaColor & right);

        const static ZeldaColor Black;
        const static ZeldaColor White;
        const static ZeldaColor Red;
        const static ZeldaColor Green;
        const static ZeldaColor Blue;
        const static ZeldaColor Orange;

        // Serializable을(를) 통해 상속됨
        virtual void PreSerialize(json& jsonData) const override;
        virtual void PreDeserialize(const json& jsonData) override;
        virtual void PostSerialize(json& jsonData) const override;
        virtual void PostDeserialize(const json& jsonData) override;
    };
}