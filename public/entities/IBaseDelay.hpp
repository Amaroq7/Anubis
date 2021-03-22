#pragma once

#include <entities/IBaseEntity.hpp>

namespace Metamod::Entities
{
    class IBaseDelay : public virtual IBaseEntity
    {
        public:
            ~IBaseDelay() override = default;
    };
}