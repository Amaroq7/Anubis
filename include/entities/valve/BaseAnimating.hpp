#pragma once

#include <entities/IBaseAnimating.hpp>
#include "BaseDelay.hpp"

namespace Metamod::Entities::Valve
{
    class BaseAnimating: public BaseDelay, public virtual IBaseAnimating
    {
        public:
            explicit BaseAnimating(const Engine::Edict *edict): BaseDelay(edict) {};
            ~BaseAnimating() override = default;
    };
}