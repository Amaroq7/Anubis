/*
 *  Copyright (C) 2020-2021 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <gamelib/IGameLib.hpp>
#include <EngineExports.hpp>
#include <Module.hpp>
#include "gamelib/entities/valve/BaseEntity.hpp"
#include "gamelib/entities/valve/BasePlayer.hpp"
#include "gamelib/entities/cstrike/BaseEntity.hpp"
#include "entities/cstrike/BasePlayer.hpp"
#include "Callbacks.hpp"
#include "Hooks.hpp"
#include "engine/Engine.hpp"
#include "engine/Edict.hpp"
#include "EntitiesHooks.hpp"

#include <unordered_map>

namespace Metamod::GameLib
{
    struct ModInfo
    {
        std::string_view name;      // name (the game dir)
        std::string_view libNameLinux;   // filename of shared lib
        std::string_view libNameWin32;   // filename of shared lib
        std::string_view desc;      // our long-name description
    };

    class GameLib final : public IGameLib
    {
    public:
        GameLib() = delete;
        GameLib(const std::unique_ptr<Engine::Engine> &engine,
            std::string_view gameDir
        );

        std::string_view getName() const override;
        std::string_view getDesc() const override;
        const fs::path &getGameDir() const override;
        const fs::path &getPathname() const override;
        Hooks *getHooks() const override;

        void pfnGameInit(FuncCallType callType = FuncCallType::Direct) override;
        bool pfnClientConnect(Engine::IEdict *pEntity, std::string_view pszName,
                                      std::string_view pszAddress, std::string &szRejectReason,
                                      FuncCallType callType) override;
        void pfnClientPutInServer(Engine::IEdict *pEntity, FuncCallType callType) override;
        void pfnClientCommand(Engine::IEdict *pEntity, FuncCallType callType) override;
        void pfnClientUserInfoChanged(Engine::IEdict *pEntity, char *infobuffer, FuncCallType callType) override;

        void pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax, FuncCallType callType) override;
        void pfnServerDeactivate(FuncCallType callType) override;
        void pfnStartFrame(FuncCallType callType) override;
        void pfnGameShutdown(FuncCallType callType) override;

        Entities::IBaseEntity *getBaseEntity(const Engine::IEdict *edict) override;
        Entities::IBasePlayer *getBasePlayer(const Engine::IEdict *edict) override;
        BasePlayerHooks *getCBasePlayerHooks() override;

        const DLL_FUNCTIONS *getDllFuncs() const;
        const NEW_DLL_FUNCTIONS *getNewDllFuncs() const;
        Module::SystemHandle getSystemHandle() const;
        void setMaxClients(std::uint32_t maxClients);
        std::uint32_t getPevOffset() const;
        std::intptr_t getOriginalVFunc(std::uint16_t func) const;

    private:
        void _loadGameDLL(const std::unique_ptr<Engine::Engine> &engine);
        void _replaceFuncs();

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Entities::IBaseEntity, T>>>
        T *_getEntity(const Engine::IEdict *edict)
        {
            auto edictImpl = dynamic_cast<const Engine::Edict *>(edict);
            if (!edictImpl->isValid() || !GET_PRIVATE(*edictImpl))
            {
                return nullptr;
            }

            const auto& [beginIt, endIt] = m_entities.equal_range(edictImpl->getIndex());
            if (beginIt != m_entities.end() && beginIt->second->serialNumber != edictImpl->getSerialNumber())
            {
                m_entities.erase(beginIt, endIt);
            }
            else
            {
                for (auto it = beginIt; it != endIt; ++it)
                {
                    if (auto entityBase = dynamic_cast<T *>(it->second->entity.get()); entityBase)
                    {
                        return entityBase;
                    }
                }
            }

            std::unique_ptr<T> entity;

            if constexpr (std::disjunction_v<
                std::is_same<Entities::Valve::BasePlayer, T>,
                std::is_same<Entities::CStrike::BasePlayer, T>>)
            {
                entity = std::make_unique<T>(const_cast<Engine::Edict *>(edictImpl), m_basePlayerhooks.get(), &m_originalVFuncs);
            }

            T *result = dynamic_cast<T *>(entity.get());
            m_entities.emplace(edictImpl->getIndex(), std::make_unique<EntityInfo>(std::move(entity), edictImpl->getSerialNumber()));

            return result;
        }

        template <typename t_hookregistry, typename t_origfunc, typename... t_args>
        auto _execGameDLLFunc(t_hookregistry *hookchain, t_origfunc origFunc, FuncCallType callType, t_args... args)
        {
            static_assert(std::is_invocable_v<t_origfunc, t_args...>, "Invalid function passed");
            if (callType == FuncCallType::Direct)
            {
                return std::invoke(origFunc, std::forward<t_args>(args)...);
            }

            return hookchain->callChain(origFunc, std::forward<t_args>(args)...);
        }

    private:
        struct EntityInfo
        {
            EntityInfo(std::unique_ptr<Entities::IBaseEntity> &&ent, std::uint32_t snumber) :
                entity(std::move(ent)), serialNumber(snumber)
            {}

            std::unique_ptr<Entities::IBaseEntity> entity;
            std::uint32_t serialNumber;
        };

    private:
        constexpr static inline std::size_t knownGamesNum = 5;
        constexpr static inline std::array<ModInfo, knownGamesNum> knownGames = {
            ModInfo{ "cstrike", "cs.so", "mp.dll", "Counter-Strike 1.6" },
            ModInfo{ "czero", "cs.so", "mp.dll", "Counter-Strike:Condition Zero" },
            ModInfo{ "dod", "dod.so", "dod.dll", "Day of Defeat" },
            ModInfo{ "tfc", "tfc.so", "tfc.dll", "Team Fortress Classic" },
            ModInfo{ "valve", "hl.so", "hl.dll", "Half-Life" }
        };

        static constexpr const char *engineEntryFuncName = "GiveFnptrsToDll";

        static constexpr inline std::int32_t entityAPIInterfaceVersion = INTERFACE_VERSION;
        static constexpr inline std::int32_t newDLLInterfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
        static constexpr inline const char *newDllApiFuncName = "GetNewDLLFunctions";
        static constexpr inline const char *entityApi2FuncName = "GetEntityAPI2";
        static constexpr inline const char *entityApiFuncName = "GetEntityAPI";

    private:
        std::unique_ptr<Hooks> m_hooks;
        std::unique_ptr<BasePlayerHooks> m_basePlayerhooks;
        std::size_t m_modPos = 0;
        fs::path m_gameDir;
        fs::path m_pathName;
        std::unique_ptr<Module> m_gameLibrary;
        std::unordered_multimap<std::uint32_t, std::unique_ptr<EntityInfo>> m_entities;
        std::uint32_t m_maxClients = 1;

        DLL_FUNCTIONS m_dllApiTable = {};
        NEW_DLL_FUNCTIONS m_newDllApiTable = {};
        std::unordered_map<std::uint16_t, std::intptr_t> m_originalVFuncs;
        std::uint32_t m_pevOffset;
    };
}
