#pragma once

#include <gamelib/entities/IBaseDelay.hpp>

namespace Metamod::GameLib::Entities
{
    class IBaseAnimating : public virtual IBaseDelay
    {
        public:
            ~IBaseAnimating() override = default;
    };
}