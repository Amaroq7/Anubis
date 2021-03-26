#pragma once

#include <IHookChains.hpp>
#include <gamelib/entities/IBasePlayer.hpp>
#include "BaseMonster.hpp"

class CBasePlayer;

namespace Metamod::GameLib::Entities::Valve
{
    class BasePlayer : public BaseMonster, public virtual IBasePlayer
    {
        public:
            explicit BasePlayer(const Engine::Edict *edict);
            void makeVIP() override;
            ~BasePlayer() override = default;

        private:
            operator CBasePlayer *() const;
    };
}