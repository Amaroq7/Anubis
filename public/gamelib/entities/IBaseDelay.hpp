#pragma once

#include <gamelib/entities/IBaseEntity.hpp>

namespace Metamod::GameLib::Entities
{
    class IBaseDelay : public virtual IBaseEntity
    {
        public:
            ~IBaseDelay() override = default;
    };
}