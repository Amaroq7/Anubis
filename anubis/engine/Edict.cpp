/*
 *  Copyright (C) 2020-2021 Anubis Development Team
 *
 *  This file is part of Anubis.
 *
 *  Anubis is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Anubis is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Anubis.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Edict.hpp"
#include <engine/ILibrary.hpp>

#include <extdll.h>

namespace Anubis::Engine
{
    Edict::Edict(edict_t *edict, nstd::observer_ptr<ILibrary> engine) : m_edict(edict), m_engine(engine) {}

    std::uint32_t Edict::getIndex() const
    {
        return m_engine->getIndexOfEdict(m_edict);
    }

    std::uint32_t Edict::getSerialNumber() const
    {
        return static_cast<std::uint32_t>(m_edict->serialnumber);
    }

    bool Edict::isFree() const
    {
        return m_edict->free;
    }

    void *Edict::getPrivateData() const
    {
        return m_edict->pvPrivateData;
    }

    IEdict::Flag Edict::getFlags() const
    {
        return static_cast<Flag>(m_edict->v.flags);
    }

    void Edict::setFlags(IEdict::Flag flags)
    {
        m_edict->v.flags = static_cast<int>(flags);
    }

    Edict::operator entvars_t *() const
    {
        return &m_edict->v;
    }

    Edict::operator edict_t *() const
    {
        return m_edict;
    }

    std::array<float, 3> Edict::getVecProperty(IEdict::VecProperty property) const
    {
        auto getVec = [this, property]()
        {
            switch (property)
            {
                case VecProperty::Origin:
                    return m_edict->v.origin;
                case VecProperty::OldOrigin:
                    return m_edict->v.oldorigin;
                case VecProperty::Velocity:
                    return m_edict->v.velocity;
                case VecProperty::BaseVelocity:
                    return m_edict->v.basevelocity;
                case VecProperty::ClBaseVelocity:
                    return m_edict->v.clbasevelocity;
                case VecProperty::MoveDir:
                    return m_edict->v.movedir;
                case VecProperty::Angles:
                    return m_edict->v.angles;
                case VecProperty::AVelocity:
                    return m_edict->v.avelocity;
                case VecProperty::PunchAngle:
                    return m_edict->v.punchangle;
                case VecProperty::ViewingAngle:
                    return m_edict->v.v_angle;
                case VecProperty::EndPos:
                    return m_edict->v.endpos;
                case VecProperty::StartPos:
                    return m_edict->v.startpos;
                case VecProperty::AbsMin:
                    return m_edict->v.absmin;
                case VecProperty::AbsMax:
                    return m_edict->v.absmax;
                case VecProperty::Mins:
                    return m_edict->v.mins;
                case VecProperty::Maxs:
                    return m_edict->v.maxs;
                case VecProperty::Size:
                    return m_edict->v.size;
                case VecProperty::RenderColor:
                    return m_edict->v.rendercolor;
                case VecProperty::ViewingOffset:
                    return m_edict->v.view_ofs;
                case VecProperty::User1:
                    return m_edict->v.vuser1;
                case VecProperty::User2:
                    return m_edict->v.vuser2;
                case VecProperty::User3:
                    return m_edict->v.vuser3;
                case VecProperty::User4:
                    return m_edict->v.vuser4;
                default:
                    return Vector {0.0f, 0.0f, 0.0f};
            }
        };
        vec3_t vec = getVec();

        return {vec[0], vec[1], vec[2]};
    }

    StringOffset Edict::getStrProperty(StrProperty property) const
    {
        auto getStringT = [this, property]()
        {
            switch (property)
            {
                case StrProperty::ClassName:
                    return static_cast<std::uint32_t>(m_edict->v.classname);
                case StrProperty::GlobalName:
                    return static_cast<std::uint32_t>(m_edict->v.globalname);
                case StrProperty::Model:
                    return static_cast<std::uint32_t>(m_edict->v.model);
                case StrProperty::ViewModel:
                    return static_cast<std::uint32_t>(m_edict->v.viewmodel);
                case StrProperty::WeaponModel:
                    return static_cast<std::uint32_t>(m_edict->v.weaponmodel);
                case StrProperty::Target:
                    return static_cast<std::uint32_t>(m_edict->v.target);
                case StrProperty::TargetName:
                    return static_cast<std::uint32_t>(m_edict->v.targetname);
                case StrProperty::NetName:
                    return static_cast<std::uint32_t>(m_edict->v.netname);
                case StrProperty::Message:
                    return static_cast<std::uint32_t>(m_edict->v.message);
                case StrProperty::Noise:
                    return static_cast<std::uint32_t>(m_edict->v.noise);
                case StrProperty::Noise1:
                    return static_cast<std::uint32_t>(m_edict->v.noise1);
                case StrProperty::Noise2:
                    return static_cast<std::uint32_t>(m_edict->v.noise2);
                case StrProperty::Noise3:
                    return static_cast<std::uint32_t>(m_edict->v.noise3);
                default:
                    return 0u;
            }
        };

        return StringOffset(static_cast<StringOffset::BaseType>(getStringT()));
    }

    float Edict::getFlProperty(FlProperty property) const
    {
        switch (property)
        {
            case FlProperty::ImpactTime:
                return m_edict->v.impacttime;
            case FlProperty::StartTime:
                return m_edict->v.starttime;
            case FlProperty::IdealPitch:
                return m_edict->v.idealpitch;
            case FlProperty::IdealYaw:
                return m_edict->v.ideal_yaw;
            case FlProperty::SpeedPitch:
                return m_edict->v.pitch_speed;
            case FlProperty::SpeedYaw:
                return m_edict->v.yaw_speed;
            case FlProperty::LTime:
                return m_edict->v.ltime;
            case FlProperty::NextThink:
                return m_edict->v.nextthink;
            case FlProperty::Gravity:
                return m_edict->v.gravity;
            case FlProperty::Friction:
                return m_edict->v.friction;
            case FlProperty::Frame:
                return m_edict->v.frame;
            case FlProperty::AnimTime:
                return m_edict->v.animtime;
            case FlProperty::FrameRate:
                return m_edict->v.framerate;
            case FlProperty::Scale:
                return m_edict->v.scale;
            case FlProperty::RenderAmount:
                return m_edict->v.renderamt;
            case FlProperty::Health:
                return m_edict->v.health;
            case FlProperty::Frags:
                return m_edict->v.frags;
            case FlProperty::TakeDamage:
                return m_edict->v.takedamage;
            case FlProperty::MaxHealth:
                return m_edict->v.max_health;
            case FlProperty::TeleportTime:
                return m_edict->v.teleport_time;
            case FlProperty::ArmorType:
                return m_edict->v.armortype;
            case FlProperty::ArmorValue:
                return m_edict->v.armorvalue;
            case FlProperty::DmgTake:
                return m_edict->v.dmg_take;
            case FlProperty::Dmg:
                return m_edict->v.dmg;
            case FlProperty::DmgTime:
                return m_edict->v.dmgtime;
            case FlProperty::DmgSave:
                return m_edict->v.dmg_save;
            case FlProperty::Speed:
                return m_edict->v.speed;
            case FlProperty::AirFinished:
                return m_edict->v.air_finished;
            case FlProperty::PainFinished:
                return m_edict->v.pain_finished;
            case FlProperty::RadSuitFinished:
                return m_edict->v.radsuit_finished;
            case FlProperty::MaxSpeed:
                return m_edict->v.maxspeed;
            case FlProperty::Fov:
                return m_edict->v.fov;
            case FlProperty::FallVelocity:
                return m_edict->v.flFallVelocity;
            case FlProperty::User1:
                return m_edict->v.fuser1;
            case FlProperty::User2:
                return m_edict->v.fuser2;
            case FlProperty::User3:
                return m_edict->v.fuser3;
            case FlProperty::User4:
                return m_edict->v.fuser4;
            default:
                return 0.0f;
        }
    }

    std::int32_t Edict::getIntProperty(IntProperty property) const
    {
        switch (property)
        {
            case IntProperty::Skin:
                return m_edict->v.skin;
            case IntProperty::Body:
                return m_edict->v.body;
            case IntProperty::Sequence:
                return m_edict->v.sequence;
            case IntProperty::GaitSequence:
                return m_edict->v.gaitsequence;
            case IntProperty::Weapons:
                return m_edict->v.weapons;
            case IntProperty::Team:
                return m_edict->v.team;
            case IntProperty::WaterLevel:
                return m_edict->v.waterlevel;
            case IntProperty::WaterType:
                return m_edict->v.watertype;
            case IntProperty::PlayerClass:
                return m_edict->v.playerclass;
            case IntProperty::WeaponAnim:
                return m_edict->v.weaponanim;
            case IntProperty::PushMSec:
                return m_edict->v.pushmsec;
            case IntProperty::InDuck:
                return m_edict->v.bInDuck;
            case IntProperty::TimeStepSound:
                return m_edict->v.flTimeStepSound;
            case IntProperty::SwimTime:
                return m_edict->v.flSwimTime;
            case IntProperty::DuckTime:
                return m_edict->v.flDuckTime;
            case IntProperty::StepLeft:
                return m_edict->v.iStepLeft;
            case IntProperty::GameState:
                return m_edict->v.gamestate;
            case IntProperty::GroupInfo:
                return m_edict->v.groupinfo;
            case IntProperty::User1:
                return m_edict->v.iuser1;
            case IntProperty::User2:
                return m_edict->v.iuser2;
            case IntProperty::User3:
                return m_edict->v.iuser3;
            case IntProperty::User4:
                return m_edict->v.iuser4;
            default:
                return 0;
        }
    }

    std::int16_t Edict::getShortProperty(ShortProperty property) const
    {
        switch (property)
        {
            case ShortProperty::Button:
                return static_cast<std::int16_t>(m_edict->v.button);
            case ShortProperty::OldButtons:
                return static_cast<std::int16_t>(m_edict->v.oldbuttons);
            default:
                return 0;
        }
    }

    std::uint16_t Edict::getUShortProperty(UShortProperty property) const
    {
        switch (property)
        {
            case UShortProperty::ColorMap:
                return static_cast<std::uint16_t>(m_edict->v.colormap);
            default:
                return 0;
        }
    }

    std::byte Edict::getByteProperty(ByteProperty property) const
    {
        switch (property)
        {
            case ByteProperty::Impulse:
                return std::byte {static_cast<unsigned char>(m_edict->v.impulse)};
            default:
                return std::byte {};
        }
    }

    nstd::observer_ptr<IEdict> Edict::getEdictProperty(EdictProperty property) const
    {
        switch (property)
        {
            case EdictProperty::Chain:
                return m_engine->getEdict(m_edict->v.chain);
            case EdictProperty::DmgInflictor:
                return m_engine->getEdict(m_edict->v.dmg_inflictor);
            case EdictProperty::Enemy:
                return m_engine->getEdict(m_edict->v.enemy);
            case EdictProperty::Aiment:
                return m_engine->getEdict(m_edict->v.aiment);
            case EdictProperty::Owner:
                return m_engine->getEdict(m_edict->v.owner);
            case EdictProperty::GroundEntity:
                return m_engine->getEdict(m_edict->v.groundentity);
            case EdictProperty::User1:
                return m_engine->getEdict(m_edict->v.euser1);
            case EdictProperty::User2:
                return m_engine->getEdict(m_edict->v.euser2);
            case EdictProperty::User3:
                return m_engine->getEdict(m_edict->v.euser3);
            case EdictProperty::User4:
                return m_engine->getEdict(m_edict->v.euser4);
            default:
                return {};
        }
    }

    FixAngle Edict::getFixAngle() const
    {
        return static_cast<FixAngle>(m_edict->v.fixangle);
    }

    PrecacheId Edict::getModelIndex() const
    {
        return PrecacheId {static_cast<PrecacheId::BaseType>(m_edict->v.modelindex)};
    }

    IEdict::MoveType Edict::getMoveType() const
    {
        return static_cast<MoveType>(m_edict->v.movetype);
    }

    IEdict::SolidType Edict::getSolidType() const
    {
        return static_cast<SolidType>(m_edict->v.solid);
    }

    IEdict::Effects Edict::getEffects() const
    {
        return static_cast<Effects>(m_edict->v.effects);
    }

    std::array<std::byte, 4> Edict::getController() const
    {
        return {std::byte {m_edict->v.controller[0]}, std::byte {m_edict->v.controller[1]},
                std::byte {m_edict->v.controller[2]}, std::byte {m_edict->v.controller[3]}};
    }

    std::array<std::byte, 2> Edict::getBlending() const
    {
        return {std::byte {m_edict->v.blending[0]}, std::byte {m_edict->v.blending[1]}};
    }

    IEdict::RenderMode Edict::getRenderMode() const
    {
        return static_cast<RenderMode>(m_edict->v.rendermode);
    }

    IEdict::DeadFlag Edict::getDeadFlag() const
    {
        return static_cast<DeadFlag>(m_edict->v.deadflag);
    }

    IEdict::SpawnFlag Edict::getSpawnFlag() const
    {
        return static_cast<SpawnFlag>(m_edict->v.spawnflags);
    }

    void Edict::setVecProperty(VecProperty property, std::array<float, 3> value)
    {
        auto getVec = [this, property, value]()
        {
            switch (property)
            {
                case VecProperty::Origin:
                    m_edict->v.origin = {value[0], value[1], value[2]};
                    break;
                case VecProperty::OldOrigin:
                    m_edict->v.oldorigin = {value[0], value[1], value[2]};
                    break;
                case VecProperty::Velocity:
                    m_edict->v.velocity = {value[0], value[1], value[2]};
                    break;
                case VecProperty::BaseVelocity:
                    m_edict->v.basevelocity = {value[0], value[1], value[2]};
                    break;
                case VecProperty::ClBaseVelocity:
                    m_edict->v.clbasevelocity = {value[0], value[1], value[2]};
                    break;
                case VecProperty::MoveDir:
                    m_edict->v.movedir = {value[0], value[1], value[2]};
                    break;
                case VecProperty::Angles:
                    m_edict->v.angles = {value[0], value[1], value[2]};
                    break;
                case VecProperty::AVelocity:
                    m_edict->v.avelocity = {value[0], value[1], value[2]};
                    break;
                case VecProperty::PunchAngle:
                    m_edict->v.punchangle = {value[0], value[1], value[2]};
                    break;
                case VecProperty::ViewingAngle:
                    m_edict->v.v_angle = {value[0], value[1], value[2]};
                    break;
                case VecProperty::EndPos:
                    m_edict->v.endpos = {value[0], value[1], value[2]};
                    break;
                case VecProperty::StartPos:
                    m_edict->v.startpos = {value[0], value[1], value[2]};
                    break;
                case VecProperty::AbsMin:
                    m_edict->v.absmin = {value[0], value[1], value[2]};
                    break;
                case VecProperty::AbsMax:
                    m_edict->v.absmax = {value[0], value[1], value[2]};
                    break;
                case VecProperty::Mins:
                    m_edict->v.mins = {value[0], value[1], value[2]};
                    break;
                case VecProperty::Maxs:
                    m_edict->v.maxs = {value[0], value[1], value[2]};
                    break;
                case VecProperty::Size:
                    m_edict->v.size = {value[0], value[1], value[2]};
                    break;
                case VecProperty::RenderColor:
                    m_edict->v.rendercolor = {value[0], value[1], value[2]};
                    break;
                case VecProperty::ViewingOffset:
                    m_edict->v.view_ofs = {value[0], value[1], value[2]};
                    break;
                case VecProperty::User1:
                    m_edict->v.vuser1 = {value[0], value[1], value[2]};
                    break;
                case VecProperty::User2:
                    m_edict->v.vuser2 = {value[0], value[1], value[2]};
                    break;
                case VecProperty::User3:
                    m_edict->v.vuser3 = {value[0], value[1], value[2]};
                    break;
                case VecProperty::User4:
                    m_edict->v.vuser4 = {value[0], value[1], value[2]};
                    break;
            }
        };
    }

    void Edict::setStrProperty(StrProperty property, StringOffset value)
    {
        switch (property)
        {
            case StrProperty::ClassName:
                m_edict->v.classname = static_cast<string_t>(value.value);
                break;
            case StrProperty::GlobalName:
                m_edict->v.globalname = static_cast<string_t>(value.value);
                break;
            case StrProperty::Model:
                m_edict->v.model = static_cast<string_t>(value.value);
                break;
            case StrProperty::ViewModel:
                m_edict->v.viewmodel = static_cast<int>(value.value);
                break;
            case StrProperty::WeaponModel:
                m_edict->v.weaponmodel = static_cast<int>(value.value);
                break;
            case StrProperty::Target:
                m_edict->v.target = static_cast<string_t>(value.value);
                break;
            case StrProperty::TargetName:
                m_edict->v.targetname = static_cast<string_t>(value.value);
                break;
            case StrProperty::NetName:
                m_edict->v.netname = static_cast<string_t>(value.value);
                break;
            case StrProperty::Message:
                m_edict->v.message = static_cast<string_t>(value.value);
                break;
            case StrProperty::Noise:
                m_edict->v.noise = static_cast<string_t>(value.value);
                break;
            case StrProperty::Noise1:
                m_edict->v.noise1 = static_cast<string_t>(value.value);
                break;
            case StrProperty::Noise2:
                m_edict->v.noise2 = static_cast<string_t>(value.value);
                break;
            case StrProperty::Noise3:
                m_edict->v.noise3 = static_cast<string_t>(value.value);
                break;
        }
    }
    void Edict::setFlProperty(FlProperty property, float value)
    {
        switch (property)
        {
            case FlProperty::ImpactTime:
                m_edict->v.impacttime = value;
                break;
            case FlProperty::StartTime:
                m_edict->v.starttime = value;
                break;
            case FlProperty::IdealPitch:
                m_edict->v.idealpitch = value;
                break;
            case FlProperty::IdealYaw:
                m_edict->v.ideal_yaw = value;
                break;
            case FlProperty::SpeedPitch:
                m_edict->v.pitch_speed = value;
                break;
            case FlProperty::SpeedYaw:
                m_edict->v.yaw_speed = value;
                break;
            case FlProperty::LTime:
                m_edict->v.ltime = value;
                break;
            case FlProperty::NextThink:
                m_edict->v.nextthink = value;
                break;
            case FlProperty::Gravity:
                m_edict->v.gravity = value;
                break;
            case FlProperty::Friction:
                m_edict->v.friction = value;
                break;
            case FlProperty::Frame:
                m_edict->v.frame = value;
                break;
            case FlProperty::AnimTime:
                m_edict->v.animtime = value;
                break;
            case FlProperty::FrameRate:
                m_edict->v.framerate = value;
                break;
            case FlProperty::Scale:
                m_edict->v.scale = value;
                break;
            case FlProperty::RenderAmount:
                m_edict->v.renderamt = value;
                break;
            case FlProperty::Health:
                m_edict->v.health = value;
                break;
            case FlProperty::Frags:
                m_edict->v.frags = value;
                break;
            case FlProperty::TakeDamage:
                m_edict->v.takedamage = value;
                break;
            case FlProperty::MaxHealth:
                m_edict->v.max_health = value;
                break;
            case FlProperty::TeleportTime:
                m_edict->v.teleport_time = value;
                break;
            case FlProperty::ArmorType:
                m_edict->v.armortype = value;
                break;
            case FlProperty::ArmorValue:
                m_edict->v.armorvalue = value;
                break;
            case FlProperty::DmgTake:
                m_edict->v.dmg_take = value;
                break;
            case FlProperty::Dmg:
                m_edict->v.dmg = value;
                break;
            case FlProperty::DmgTime:
                m_edict->v.dmgtime = value;
                break;
            case FlProperty::DmgSave:
                m_edict->v.dmg_save = value;
                break;
            case FlProperty::Speed:
                m_edict->v.speed = value;
                break;
            case FlProperty::AirFinished:
                m_edict->v.air_finished = value;
                break;
            case FlProperty::PainFinished:
                m_edict->v.pain_finished = value;
                break;
            case FlProperty::RadSuitFinished:
                m_edict->v.radsuit_finished = value;
                break;
            case FlProperty::MaxSpeed:
                m_edict->v.maxspeed = value;
                break;
            case FlProperty::Fov:
                m_edict->v.fov = value;
                break;
            case FlProperty::FallVelocity:
                m_edict->v.flFallVelocity = value;
                break;
            case FlProperty::User1:
                m_edict->v.fuser1 = value;
                break;
            case FlProperty::User2:
                m_edict->v.fuser2 = value;
                break;
            case FlProperty::User3:
                m_edict->v.fuser3 = value;
                break;
            case FlProperty::User4:
                m_edict->v.fuser4 = value;
                break;
        }
    }
    void Edict::setIntProperty(IntProperty property, std::int32_t value)
    {
        switch (property)
        {
            case IntProperty::Skin:
                m_edict->v.skin = value;
                break;
            case IntProperty::Body:
                m_edict->v.body = value;
                break;
            case IntProperty::Sequence:
                m_edict->v.sequence = value;
                break;
            case IntProperty::GaitSequence:
                m_edict->v.gaitsequence = value;
                break;
            case IntProperty::Weapons:
                m_edict->v.weapons = value;
                break;
            case IntProperty::Team:
                m_edict->v.team = value;
                break;
            case IntProperty::WaterLevel:
                m_edict->v.waterlevel = value;
                break;
            case IntProperty::WaterType:
                m_edict->v.watertype = value;
                break;
            case IntProperty::PlayerClass:
                m_edict->v.playerclass = value;
                break;
            case IntProperty::WeaponAnim:
                m_edict->v.weaponanim = value;
                break;
            case IntProperty::PushMSec:
                m_edict->v.pushmsec = value;
                break;
            case IntProperty::InDuck:
                m_edict->v.bInDuck = value;
                break;
            case IntProperty::TimeStepSound:
                m_edict->v.flTimeStepSound = value;
                break;
            case IntProperty::SwimTime:
                m_edict->v.flSwimTime = value;
                break;
            case IntProperty::DuckTime:
                m_edict->v.flDuckTime = value;
                break;
            case IntProperty::StepLeft:
                m_edict->v.iStepLeft = value;
                break;
            case IntProperty::GameState:
                m_edict->v.gamestate = value;
                break;
            case IntProperty::GroupInfo:
                m_edict->v.groupinfo = value;
                break;
            case IntProperty::User1:
                m_edict->v.iuser1 = value;
                break;
            case IntProperty::User2:
                m_edict->v.iuser2 = value;
                break;
            case IntProperty::User3:
                m_edict->v.iuser3 = value;
                break;
            case IntProperty::User4:
                m_edict->v.iuser4 = value;
                break;
        }
    }

    void Edict::setShortProperty(ShortProperty property, std::int16_t value)
    {
        switch (property)
        {
            case ShortProperty::Button:
                m_edict->v.button = value;
                break;
            case ShortProperty::OldButtons:
                m_edict->v.oldbuttons = value;
                break;
        }
    }

    void Edict::setUShortProperty(UShortProperty property, std::uint16_t value)
    {
        switch (property)
        {
            case UShortProperty::ColorMap:
                m_edict->v.colormap = static_cast<int>(value);
        }
    }

    void Edict::setByteProperty(ByteProperty property, std::byte value)
    {
        switch (property)
        {
            case ByteProperty::Impulse:
                m_edict->v.impulse = std::to_integer<int>(value);
                break;
        }
    }

    void Edict::setEdictProperty(EdictProperty property, nstd::observer_ptr<IEdict> value)
    {
        switch (property)
        {
            case EdictProperty::Chain:
                m_edict->v.chain = static_cast<edict_t *>(*value);
            case EdictProperty::DmgInflictor:
                m_edict->v.dmg_inflictor = static_cast<edict_t *>(*value);
            case EdictProperty::Enemy:
                m_edict->v.enemy = static_cast<edict_t *>(*value);
            case EdictProperty::Aiment:
                m_edict->v.aiment = static_cast<edict_t *>(*value);
            case EdictProperty::Owner:
                m_edict->v.owner = static_cast<edict_t *>(*value);
            case EdictProperty::GroundEntity:
                m_edict->v.groundentity = static_cast<edict_t *>(*value);
            case EdictProperty::User1:
                m_edict->v.euser1 = static_cast<edict_t *>(*value);
            case EdictProperty::User2:
                m_edict->v.euser2 = static_cast<edict_t *>(*value);
            case EdictProperty::User3:
                m_edict->v.euser3 = static_cast<edict_t *>(*value);
            case EdictProperty::User4:
                m_edict->v.euser4 = static_cast<edict_t *>(*value);
        }
    }

    void Edict::setFixAngle(FixAngle fixAngle)
    {
        m_edict->v.fixangle = static_cast<int>(fixAngle);
    }

    void Edict::setModelIndex(PrecacheId modelIndex)
    {
        m_edict->v.modelindex = static_cast<int>(modelIndex.value);
    }

    void Edict::setMoveType(MoveType moveType)
    {
        m_edict->v.movetype = static_cast<int>(moveType);
    }

    void Edict::setSolidType(SolidType solidType)
    {
        m_edict->v.solid = static_cast<int>(solidType);
    }

    void Edict::setEffects(Effects effects)
    {
        m_edict->v.effects = static_cast<int>(effects);
    }

    void Edict::setController(std::array<std::byte, 4> controller)
    {
        std::size_t i = 0;
        for (auto value : controller)
        {
            m_edict->v.controller[i++] = std::to_integer<byte>(value);
        }
    }

    void Edict::setBlending(std::array<std::byte, 2> blending)
    {
        std::size_t i = 0;
        for (auto value : blending)
        {
            m_edict->v.blending[i++] = std::to_integer<byte>(value);
        }
    }

    void Edict::setRenderMode(RenderMode renderMode)
    {
        m_edict->v.rendermode = static_cast<int>(renderMode);
    }

    void Edict::setDeadFlag(DeadFlag deadFlag)
    {
        m_edict->v.deadflag = static_cast<int>(deadFlag);
    }

    void Edict::setSpawnFlag(SpawnFlag spawnFlag)
    {
        m_edict->v.spawnflags = static_cast<int>(spawnFlag);
    }

} // namespace Anubis::Engine
