#pragma once

#include <engine/IEntVars.hpp>
#include <extdll.h>

namespace Metamod::Engine
{
    class EntVars : public IEntVars
    {
        public:
            EntVars() = delete;
            EntVars(entvars_t *entvars);
            ~EntVars() override = default;

            virtual EntFlags getFlags() const override;
            virtual void setFlags(EntFlags flags) override;

            virtual std::uint32_t getIndex() const override;

            // EntVars
            operator entvars_t *() const;

        private:
            entvars_t *m_entVars = nullptr;
    };
}