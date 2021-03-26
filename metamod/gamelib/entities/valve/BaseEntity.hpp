#pragma once

#include <gamelib/entities/IBaseEntity.hpp>
#include <engine/Edict.hpp>

#include <string_view>

class CBaseEntity;

namespace Metamod::GameLib::Entities::Valve
{
    class BaseEntity : public virtual IBaseEntity
    {
        public:
            explicit BaseEntity(const Engine::Edict *edict);
            ~BaseEntity() override = default;

            Engine::Edict *edict() const override;
            void remove() override;
            bool isAlive() const override;
            std::string_view getTeam() const override;
            int takeHealth(float flHealth, int bitsDamageType) const override;
            int takeDamage(Engine::IEntVars *pevInflictor, Engine::IEntVars *pevAttacker, float flDamage, int bitsDamageType) override;

            void setCallHooks(bool call);

        protected:
            std::intptr_t _getCurrentVTable();

        protected:
            Engine::Edict *m_edict;
            bool m_callHooks;

        private:
            operator CBaseEntity *() const;
    };
}