#pragma once

#include <gamelib/entities/IBaseMonster.hpp>
#include "BaseToggle.hpp"

namespace Metamod::GameLib::Entities::Valve
{
    class BaseMonster: public BaseToggle, public virtual IBaseMonster
    {
        public:
            explicit BaseMonster(const Engine::Edict *edict): BaseToggle(edict) {};
            ~BaseMonster() override = default;
    };
}