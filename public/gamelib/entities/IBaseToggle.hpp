#pragma once

#include <gamelib/entities/IBaseAnimating.hpp>

namespace Metamod::GameLib::Entities
{
    class IBaseToggle : public virtual IBaseAnimating
    {
        public:
            ~IBaseToggle() override = default;
    };
}