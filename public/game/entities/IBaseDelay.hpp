#pragma once

#include <game/entities/IBaseEntity.hpp>

namespace Metamod::Game::Entities
{
    class IBaseDelay : public virtual IBaseEntity
    {
        public:
            ~IBaseDelay() override = default;
    };
}