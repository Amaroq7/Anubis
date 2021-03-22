#pragma once

#include "BaseEntity.hpp"
#include <entities/IBaseDelay.hpp>

namespace Metamod::Entities::Valve
{
    class BaseDelay : public BaseEntity, public virtual IBaseDelay
    {
        public:
            explicit BaseDelay(const Engine::Edict *edict) : BaseEntity(edict) {};
            ~BaseDelay() override = default;
    };
}