#pragma once

#include "HookChains.hpp"
#include <gamelib/entities/IBasePlayer.hpp>
#include "BaseMonster.hpp"

#include <memory>
#include <gamelib/EntitiesHooks.hpp>

class CBasePlayer;

namespace Metamod::GameLib::Entities::Valve
{
    class BasePlayer : public BaseMonster, public virtual IBasePlayer
    {
        public:
            BasePlayer(const Engine::Edict *edict, BasePlayerHooks *m_hooks, const std::unordered_map<std::string, std::intptr_t> &originalVFuncs);

            int takeDamage(Engine::IEntVars *pevInflictor, Engine::IEntVars *pevAttacker, float flDamage, int bitsDamageType) override;
            void makeVIP() override;
            ~BasePlayer() override = default;

        private:
            operator CBasePlayer *() const;

        private:
            BasePlayerHooks *m_hooks;
            const std::unordered_map<std::string, std::intptr_t> &m_originalVFuncs;
    };
}