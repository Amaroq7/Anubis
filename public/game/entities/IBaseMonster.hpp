#pragma once

#include <game/entities/IBaseToggle.hpp>

namespace Metamod::Game::Entities
{
    class IBaseMonster : public virtual IBaseToggle
    {
        public:
            ~IBaseMonster() override = default;
    };
}