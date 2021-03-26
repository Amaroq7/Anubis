#pragma once

#include <IHudTextParams.hpp>

namespace Metamod::Engine 
{
    class HudTextParams: public IHudTextParams
    {
        public:
            void setPosX(float x) override;
            void setPosY(float y) override;
            void setEffect(int effect) override;
            void setColor(std::byte r, std::byte g, std::byte b, std::byte a) override;
            void setColorEffect(std::byte r, std::byte g, std::byte b, std::byte a) override;
            void setProp(Prop property, float value) override;
            void setChannel(std::uint32_t channel) override;
    };
}