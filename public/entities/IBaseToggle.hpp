#pragma once

#include <entities/IBaseAnimating.hpp>

namespace Metamod::Entities
{
    class IBaseToggle : public virtual IBaseAnimating
    {
        public:
            ~IBaseToggle() override = default;
    };
}