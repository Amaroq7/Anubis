#pragma once

#include <game/entities/IBaseToggle.hpp>
#include "BaseAnimating.hpp"

namespace Metamod::Game::Entities::Valve
{
    class BaseToggle: public BaseAnimating, public virtual IBaseToggle
    {
        public:
            explicit BaseToggle(Engine::IEdict *edict): BaseAnimating(edict) {};
            ~BaseToggle() override = default;
    };
}