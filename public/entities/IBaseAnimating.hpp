#pragma once

#include <entities/IBaseDelay.hpp>

namespace Metamod::Entities
{
    class IBaseAnimating : public virtual IBaseDelay
    {
        public:
            ~IBaseAnimating() override = default;
    };
}