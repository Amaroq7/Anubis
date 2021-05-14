#pragma once

#include "HookChains.hpp"
#include <game/entities/IBasePlayer.hpp>
#include "BaseMonster.hpp"

#include <memory>
#include <game/EntitiesHooks.hpp>

class CBasePlayer;

namespace Metamod::Engine
{
    class Library;
}

namespace Metamod::Game::Entities::Valve
{
    class BasePlayer : public BaseMonster, public virtual IBasePlayer
    {
        public:
            static constexpr const char *CLASS_NAME = "player";

        public:
            BasePlayer(const Engine::Edict *edict, BasePlayerHooks *m_hooks);
            ~BasePlayer() override = default;

        public:
            int takeDamage(Engine::IEntVars *pevInflictor, Engine::IEntVars *pevAttacker, float flDamage, int bitsDamageType) override;
            void makeVIP() override;

        private:
            operator CBasePlayer *() const;

        private:
            BasePlayerHooks *m_hooks;
    };
}