#pragma once

#include "BaseEntity.hpp"
#include <entities/IBaseDelay.hpp>

namespace Metamod::Entities::CStrike
{
    class BaseDelay : public BaseEntity, public virtual IBaseDelay
    {
        public:
            explicit BaseDelay(const Engine::Edict *edict) : BaseEntity(edict) {};
            ~BaseDelay() override = default;
    };
}