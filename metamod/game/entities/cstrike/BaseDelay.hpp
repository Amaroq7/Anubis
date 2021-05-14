#pragma once

#include "BaseEntity.hpp"
#include <game/entities/IBaseDelay.hpp>

namespace Metamod::Game::Entities::CStrike
{
    class BaseDelay : public BaseEntity, public virtual IBaseDelay
    {
        public:
            explicit BaseDelay(const Engine::Edict *edict) : BaseEntity(edict) {};
            ~BaseDelay() override = default;
    };
}