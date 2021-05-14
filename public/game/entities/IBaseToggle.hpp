#pragma once

#include <game/entities/IBaseAnimating.hpp>

namespace Metamod::Game::Entities
{
    class IBaseToggle : public virtual IBaseAnimating
    {
        public:
            ~IBaseToggle() override = default;
    };
}