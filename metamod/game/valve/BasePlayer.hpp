#pragma once

#include "HookChains.hpp"
#include <game/IBasePlayer.hpp>
#include "BaseMonster.hpp"

#include <memory>

class CBasePlayer;

namespace Metamod::Engine
{
    class Library;
}

namespace Metamod::Game::Valve
{
    class BasePlayer : public BaseMonster, public virtual IBasePlayer
    {
        public:
            static constexpr const char *CLASS_NAME = "player";

        public:
            explicit BasePlayer(Engine::IEdict *edict);
            ~BasePlayer() override = default;

        public:
            bool takeDamage(Engine::IEntVars *pevInflictor, Engine::IEntVars *pevAttacker, float flDamage, std::int32_t bitsDamageType) override;
            void makeVIP() override;
            void giveShield(bool deploy) override;
            void removeShield() override;
            void dropShield(bool deploy) override;
            [[nodiscard]] bool hasShield() const override;

        public:
            explicit operator CBasePlayer *() const;
    };
}