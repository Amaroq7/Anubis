#pragma once

#include <entities/IBasePlayer.hpp>
#include "BaseMonster.hpp"

class CBasePlayer;

namespace Metamod::Entities::CStrike
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