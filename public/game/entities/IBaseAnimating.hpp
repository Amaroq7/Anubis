#pragma once

#include <game/entities/IBaseDelay.hpp>

namespace Metamod::Game::Entities
{
    class IBaseAnimating : public virtual IBaseDelay
    {
        public:
            ~IBaseAnimating() override = default;
    };
}