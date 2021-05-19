#pragma once

#include "HookChains.hpp"
#include <game/entities/IBasePlayer.hpp>
#include "BaseMonster.hpp"

#include <memory>
#include "EntitiesHooks.hpp"

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
            BasePlayer(Engine::IEdict *edict);
            ~BasePlayer() override = default;

        public:
            bool takeDamage(Engine::IEntVars *pevInflictor, Engine::IEntVars *pevAttacker, float flDamage, std::int32_t bitsDamageType, FuncCallType callType) override;
            void makeVIP() override;
            //void killed() final;

        private:
            operator CBasePlayer *() const;
    };
}