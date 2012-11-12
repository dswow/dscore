/*
 * Copyright (C) 2005 - 2011 MaNGOS <http://www.getmangos.org/>
 *
 * Copyright (C) 2008 - 2011 TrinityCore <http://www.trinitycore.org/>
 *
 * Copyright (C) 2011 - 2012 ArkCORE <http://www.arkania.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "blackwing_descent.h"

enum eSpell
{
    SPELL_ARCANE_ANNIHILATOR            = 79710,
    SPELL_CHEMICAL_BOMB                 = 80157,
    SPELL_POISON_SOAKED_SHELL           = 79835,
    SPELL_GOLEM_INACTIVE                = 78726,
    SPELL_ELECTRICAL_DISCHARGE          = 79879,
    SPELL_LIGHTNING_CONDUCTOR           = 79888,
    SPELL_UNSTABLE_SHIELD               = 79900,
    SPELL_SHUTTING_DOWN                 = 78746,
    SPELL_ACQUIRING_TARGET              = 79501,
    SPELL_BARRIER                       = 79582,
    SPELL_FLAMETHROWER                  = 79505,
    SPELL_INCINERATION_SECURITY_MEASURE = 79023,
    SPELL_POWER_GENERATOR               = 79624
};

uint32 order = urand(1,4);

class boss_toxitron : public CreatureScript
{
public:
    boss_toxitron() : CreatureScript("boss_toxitron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_toxitronAI (creature);
    }

    struct boss_toxitronAI : public ScriptedAI
    {
        boss_toxitronAI(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        
        //Rotation
        uint32 rotation;
        uint32 rotationTimer;
        bool rotationType;
        
        //Timer Registration
        uint32 uiChemicalBombTimer;
        uint32 uiPoisonSoakedShellTimer;   
        
        void Reset()
        {
            rotation = order;
            rotationType = false;
            rotationTimer = 45*rotation*IN_MILLISECONDS;
            
            if (rotation != 1)
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
            else
                rotationType = true;
            
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            
            uiChemicalBombTimer = 20*IN_MILLISECONDS; //Timer richtig?
        }

        void EnterCombat(Unit* /*pWho*/) {}

        void JustDied(Unit* /*Killer*/) {}

        void UpdateAI(const uint32 Diff)
        {
            if (!UpdateVictim())
                return;
            
            if (uiChemicalBombTimer <= Diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150, true))
                    me->CastSpell(target, SPELL_CHEMICAL_BOMB, true);

                uiChemicalBombTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS); //Timer richtig?
            } else uiChemicalBombTimer -= Diff;
            
            if (rotationTimer <= Diff)
            {
                if (rotationType)
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
                else
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
                
                rotationType = !rotationType;
                rotationTimer = 45*IN_MILLISECONDS;
            } else rotationTimer -= Diff;

            DoMeleeAttackIfReady();
        }
    };
};

class boss_magmatron : public CreatureScript
{
public:
    boss_magmatron() : CreatureScript("boss_magmatron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_magmatronAI (creature);
    }

    struct boss_magmatronAI : public ScriptedAI
    {
        boss_magmatronAI(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        
        //Rotation
        uint32 rotation;
        uint32 rotationTimer;
        bool rotationType;
        
        //Timer Registration
        uint32 uiAcquiringTargetTimer;
        uint32 uiBarrierTimer;
        uint32 uiFlamethrowerTimer;

        void Reset()
        {
            rotation = order+1 > 4 ? order-3 : order;
            rotationType = false;
            rotationTimer = 45*rotation*IN_MILLISECONDS;
            
            if (rotation != 1)
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
            else
                rotationType = true;
            
            me->SetReactState(REACT_PASSIVE);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void JustDied(Unit* /*Killer*/) { }

        void UpdateAI(const uint32 Diff)
        {
            if (!UpdateVictim())
                return;
            
            if (rotationTimer <= Diff)
            {
                if (rotationType)
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
                else
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
                
                rotationType = !rotationType;
                rotationTimer = 45*IN_MILLISECONDS;
            } else rotationTimer -= Diff;

            DoMeleeAttackIfReady();
        }
    };
};

class boss_arcanotron : public CreatureScript
{
public:
    boss_arcanotron() : CreatureScript("boss_arcanotron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_arcanotronAI (creature);
    }

    struct boss_arcanotronAI : public ScriptedAI
    {
        boss_arcanotronAI(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        
        //Rotation
        uint32 rotation;
        uint32 rotationTimer;
        bool rotationType;

        //Timer Registration
        uint32 uiArcaneAnnihilatorTimer;

        void Reset()
        {
            rotation = order+2 > 4 ? order-2 : order;
            rotationType = false;
            rotationTimer = 45*rotation*IN_MILLISECONDS;
            
            if (rotation != 1)
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
            else
                rotationType = true;

            if (pInstance)
                pInstance->SetData(DATA_OMNOTRON_DEFENSE_SYSTEM, NOT_STARTED);

            uiArcaneAnnihilatorTimer = 5*IN_MILLISECONDS;
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            if (pInstance)
                pInstance->SetData(DATA_OMNOTRON_DEFENSE_SYSTEM, IN_PROGRESS);
        }

        void JustReachedHome()
        {
            if (pInstance)
                pInstance->SetData(DATA_OMNOTRON_DEFENSE_SYSTEM, FAIL);
        }

        void JustDied(Unit* /*Killer*/)
        {
            if (pInstance)
                pInstance->SetData(DATA_OMNOTRON_DEFENSE_SYSTEM, DONE);
        }

        void UpdateAI(const uint32 Diff)
        {
            if (!UpdateVictim())
                return;

            if (uiArcaneAnnihilatorTimer <= Diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    me->CastSpell(target, SPELL_ARCANE_ANNIHILATOR, true);

                uiArcaneAnnihilatorTimer = urand(5*IN_MILLISECONDS, 7*IN_MILLISECONDS);
            } else uiArcaneAnnihilatorTimer -= Diff;
            
            if (rotationTimer <= Diff)
            {
                if (rotationType)
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
                else
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
                
                rotationType = !rotationType;
                rotationTimer = 45*IN_MILLISECONDS;
            } else rotationTimer -= Diff;

            DoMeleeAttackIfReady();
        }
    };
};

class boss_electron : public CreatureScript
{
public:
    boss_electron() : CreatureScript("boss_electron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_electronAI (creature);
    }

    struct boss_electronAI : public ScriptedAI
    {
        boss_electronAI(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        
        //Rotation
        uint32 rotation;
        uint32 rotationTimer;
        bool rotationType;
        
        //Timer Registration
        uint32 uiElectricalDischargeTimer;
        uint32 uiLightningConductorTimer;
        uint32 uiUnstableShieldTimer;

        void Reset()
        {
            rotation = order+3 > 4 ? order-1 : order;
            
            rotationType = false;
            rotationTimer = 45*rotation*IN_MILLISECONDS;
            
            if (rotation != 1)
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
            else
                rotationType = true;
            
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            
            uiElectricalDischargeTimer = 12*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void JustDied(Unit* /*Killer*/) {}

        void UpdateAI(const uint32 Diff)
        {
            if (!UpdateVictim())
                return;
            
            if (uiElectricalDischargeTimer <= Diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    me->CastSpell(target, SPELL_ELECTRICAL_DISCHARGE, true);

                uiElectricalDischargeTimer = urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS);
            } else uiElectricalDischargeTimer -= Diff;
            
            if (rotationTimer <= Diff)
            {
                if (rotationType)
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
                else
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
                
                rotationType = !rotationType;
                rotationTimer = 45*IN_MILLISECONDS;
            } else rotationTimer -= Diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_omnotron_defense_system()
{
    new boss_toxitron();
    new boss_magmatron();
    new boss_arcanotron();
    new boss_electron();
}
