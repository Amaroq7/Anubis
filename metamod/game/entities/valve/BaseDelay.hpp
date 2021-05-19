#pragma once

#include "BaseEntity.hpp"
#include <game/entities/IBaseDelay.hpp>

namespace Metamod::Game::Entities::Valve
{
    class BaseDelay : public BaseEntity, public virtual IBaseDelay
    {
        public:
            explicit BaseDelay(Engine::IEdict *edict) : BaseEntity(edict) {};
            ~BaseDelay() override = default;
    };
}