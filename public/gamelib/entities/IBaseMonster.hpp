#pragma once

#include <gamelib/entities/IBaseToggle.hpp>

namespace Metamod::GameLib::Entities
{
    class IBaseMonster : public virtual IBaseToggle
    {
        public:
            ~IBaseMonster() override = default;
    };
}