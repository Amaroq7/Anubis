#pragma once

#include <gamelib/entities/IBasePlayer.hpp>
#include "BaseMonster.hpp"

class CBasePlayer;

namespace Metamod::GameLib
{
    class BasePlayerHooks;
}

namespace Metamod::GameLib::Entities::CStrike
{
    class BasePlayer : public BaseMonster, public virtual IBasePlayer
    {
        public:
            explicit BasePlayer(const Engine::Edict *edict, const BasePlayerHooks *hooks);
            void makeVIP() override;
            ~BasePlayer() override = default;

        private:
            operator CBasePlayer *() const;

        private:
            const BasePlayerHooks *m_hooks;
    };
}