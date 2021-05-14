#pragma once

#include <game/entities/IBaseMonster.hpp>
#include "BaseToggle.hpp"

namespace Metamod::Game::Entities::Valve
{
    class BaseMonster: public BaseToggle, public virtual IBaseMonster
    {
        public:
            explicit BaseMonster(const Engine::Edict *edict): BaseToggle(edict) {};
            ~BaseMonster() override = default;
    };
}