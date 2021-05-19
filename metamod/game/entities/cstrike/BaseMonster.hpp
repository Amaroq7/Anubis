#pragma once

#include <game/entities/IBaseMonster.hpp>
#include "BaseToggle.hpp"

namespace Metamod::Game::Entities::CStrike
{
    class BaseMonster: public BaseToggle, public virtual IBaseMonster
    {
        public:
            explicit BaseMonster(Engine::IEdict *edict): BaseToggle(edict) {};
            ~BaseMonster() override = default;
    };
}