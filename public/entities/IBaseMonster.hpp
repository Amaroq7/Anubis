#pragma once

#include <entities/IBaseToggle.hpp>

namespace Metamod::Entities
{
    class IBaseMonster : public virtual IBaseToggle
    {
        public:
            ~IBaseMonster() override = default;
    };
}