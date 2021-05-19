#pragma once

#include <game/entities/IBaseAnimating.hpp>
#include "BaseDelay.hpp"

namespace Metamod::Game::Entities::CStrike
{
    class BaseAnimating : public BaseDelay, public virtual IBaseAnimating
    {
        public:
            explicit BaseAnimating(Engine::IEdict *edict): BaseDelay(edict) {};
            ~BaseAnimating() override = default;
    };
}