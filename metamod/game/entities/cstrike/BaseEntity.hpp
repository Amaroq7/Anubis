#pragma once

#include <game/entities/IBaseEntity.hpp>
#include <engine/IEdict.hpp>

#include <string_view>

class CBaseEntity;

namespace Metamod::Game::Entities::CStrike
{
    class BaseEntity : public virtual IBaseEntity
    {
        public:
            explicit BaseEntity(Engine::IEdict *edict);
            ~BaseEntity() override = default;

            Engine::IEdict *edict() const override;
            void remove() override;
            bool isAlive() const override;
            std::string_view getTeam() const override;
            int takeHealth(float flHealth, int bitsDamageType) const override;
            bool takeDamage(Engine::IEntVars *pevInflictor, Engine::IEntVars *pevAttacker, float flDamage, std::int32_t bitsDamageType) override;

        protected:
            Engine::IEdict *m_edict;

        private:
            operator CBaseEntity *() const;
    };
}