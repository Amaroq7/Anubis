#pragma once

#include <game/entities/IBaseAnimating.hpp>
#include "BaseDelay.hpp"

namespace Metamod::Game::Entities::Valve
{
    class BaseAnimating: public BaseDelay, public virtual IBaseAnimating
    {
        public:
            explicit BaseAnimating(const Engine::Edict *edict): BaseDelay(edict) {};
            ~BaseAnimating() override = default;
    };
}