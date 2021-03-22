#pragma once

#include <entities/IBaseToggle.hpp>
#include "BaseAnimating.hpp"

namespace Metamod::Entities::CStrike
{
    class BaseToggle : public BaseAnimating, public virtual IBaseToggle
    {
        public:
            explicit BaseToggle(const Engine::Edict *edict): BaseAnimating(edict) {};
            ~BaseToggle() override = default;
    };
}