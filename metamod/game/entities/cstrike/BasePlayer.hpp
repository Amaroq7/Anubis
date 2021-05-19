#pragma once

#include <game/entities/IBasePlayer.hpp>
#include "BaseMonster.hpp"

class CBasePlayer;

namespace Metamod::Game::CStrike
{
    class BasePlayerHooks;
}

namespace Metamod::Game::Entities::CStrike
{
    class BasePlayer : public BaseMonster, public virtual IBasePlayer
    {
    public:
        static constexpr const char *CLASS_NAME = "player";

    public:
        explicit BasePlayer(Engine::IEdict *edict);
        ~BasePlayer() override = default;

    public:
        bool takeDamage(Engine::IEntVars *pevInflictor, Engine::IEntVars *pevAttacker, float flDamage, std::int32_t bitsDamageType, FuncCallType funcCallType) override;
        void makeVIP() override;
        //void killed() override;

    private:
        operator CBasePlayer *() const;
    };
}