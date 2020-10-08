/*
 * Originally written by Xinef - Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "nexus.h"
#include "Player.h"
#include "SpellInfo.h"

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "nexus.h"
#include "ObjectAccessor.h"
#include "ScriptedCreature.h"
#include "TemporarySummon.h"

enum Spells
{
    SPELL_SPARK = 47751,
    H_SPELL_SPARK = 57062,
    SPELL_RIFT_SHIELD = 47748,
    SPELL_CHARGE_RIFT = 47747, // Works wrong (affect players, not rifts)
    SPELL_CREATE_RIFT = 47743, // Don't work, using WA
    SPELL_ARCANE_ATTRACTION = 57063, // No idea, when it's used
};

enum Adds
{
    NPC_CRAZED_MANA_WRAITH = 26746,
    NPC_CHAOTIC_RIFT = 26918
};

enum Yells
{
    SAY_AGGRO = 0,
    SAY_DEATH = 1,
    SAY_RIFT = 2,
    SAY_SHIELD = 3,
    SAY_RIFT_EMOTE = 4, // Needs to be added to script
    SAY_SHIELD_EMOTE = 5  // Needs to be added to script
};

enum RiftSpells
{
    SPELL_CHAOTIC_ENERGY_BURST = 47688,
    SPELL_CHARGED_CHAOTIC_ENERGY_BURST = 47737,
    SPELL_ARCANEFORM = 48019, // Chaotic Rift visual
};

Position const RiftLocation[6] =
{
    { 652.64f, -273.70f, -8.75f, 0.0f },
    { 634.45f, -265.94f, -8.44f, 0.0f },
    { 620.73f, -281.17f, -9.02f, 0.0f },
    { 626.10f, -304.67f, -9.44f, 0.0f },
    { 639.87f, -314.11f, -9.49f, 0.0f },
    { 651.72f, -297.44f, -9.37f, 0.0f }
};

enum Misc
{
    DATA_CHAOS_THEORY = 1
};

class boss_anomalus : public CreatureScript
{
public:
    boss_anomalus() : CreatureScript("boss_anomalus") { }

    struct boss_anomalusAI : public ScriptedAI
    {
        boss_anomalusAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
            instance = me->GetInstanceScript();
        }

        void Initialize()
        {
            Phase = 0;
            uiSparkTimer = 5000;
            uiChaoticRiftGUID = 0;
            chaosTheory = true;
        }

        InstanceScript* instance;

        uint8 Phase;
        uint32 uiSparkTimer;
        uint64 uiChaoticRiftGUID;
        bool chaosTheory;

        void Reset() override
        {
            Initialize();

            instance->SetBossState(DATA_ANOMALUS_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO);

            instance->SetBossState(DATA_ANOMALUS_EVENT, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(SAY_DEATH);

            instance->SetBossState(DATA_ANOMALUS_EVENT, DONE);
        }

        uint32 GetData(uint32 type) const override
        {
            if (type == DATA_CHAOS_THEORY)
                return chaosTheory ? 1 : 0;

            return 0;
        }

        void SummonedCreatureDies(Creature* summoned, Unit* /*who*/) override
        {
            if (summoned->GetEntry() == NPC_CHAOTIC_RIFT)
                chaosTheory = false;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (me->GetDistance(me->GetHomePosition()) > 60.0f)
            {
                // Not blizzlike, hack to avoid an exploit
                EnterEvadeMode();
                return;
            }

            if (me->HasAura(SPELL_RIFT_SHIELD))
            {
                if (Creature* Rift = ObjectAccessor::GetCreature(*me, uiChaoticRiftGUID))
                {
                    if (Rift && Rift->isDead())
                    {
                        me->RemoveAurasDueToSpell(SPELL_RIFT_SHIELD);
                        uiChaoticRiftGUID = 0;
                    }
                    return;
                }
            }
            else
                uiChaoticRiftGUID = 0;

            if ((Phase == 0) && HealthBelowPct(50))
            {
                Phase = 1;
                Talk(SAY_SHIELD);
                DoCast(me, SPELL_RIFT_SHIELD);
                if (Creature* Rift = me->SummonCreature(NPC_CHAOTIC_RIFT, RiftLocation[urand(0, 5)], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000))
                {
                    //DoCast(Rift, SPELL_CHARGE_RIFT);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        Rift->AI()->AttackStart(target);
                    uiChaoticRiftGUID = Rift->GetGUID();
                    Talk(SAY_RIFT);
                }
            }

            if (uiSparkTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_SPARK);
                uiSparkTimer = 5000;
            }
            else
                uiSparkTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_anomalusAI(creature);
    }
};

class npc_chaotic_rift : public CreatureScript
{
public:
    npc_chaotic_rift() : CreatureScript("npc_chaotic_rift") { }

    struct npc_chaotic_riftAI : public ScriptedAI
    {
        npc_chaotic_riftAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
            instance = me->GetInstanceScript();
            SetCombatMovement(false);
        }

        void Initialize()
        {
            uiChaoticEnergyBurstTimer = 1000;
            uiSummonCrazedManaWraithTimer = 5000;
        }

        InstanceScript* instance;

        uint32 uiChaoticEnergyBurstTimer;
        uint32 uiSummonCrazedManaWraithTimer;

        void Reset() override
        {
            Initialize();
            //me->SetDisplayId(me->GetCreatureTemplate()->Modelid2);
            DoCast(me, SPELL_ARCANEFORM, false);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            DoCast(me, SPELL_ARCANEFORM, false);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (uiChaoticEnergyBurstTimer <= diff)
            {
                Creature* Anomalus = ObjectAccessor::GetCreature(*me, instance->GetData(DATA_ANOMALUS_EVENT));
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    if (Anomalus && Anomalus->HasAura(SPELL_RIFT_SHIELD))
                        DoCast(target, SPELL_CHARGED_CHAOTIC_ENERGY_BURST);
                    else
                        DoCast(target, SPELL_CHAOTIC_ENERGY_BURST);
                }
                uiChaoticEnergyBurstTimer = 1000;
            }
            else
                uiChaoticEnergyBurstTimer -= diff;

            if (uiSummonCrazedManaWraithTimer <= diff)
            {
                if (Creature* Wraith = me->SummonCreature(NPC_CRAZED_MANA_WRAITH, me->GetPositionX() + 1, me->GetPositionY() + 1, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000))
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        Wraith->AI()->AttackStart(target);
                Creature* Anomalus = ObjectAccessor::GetCreature(*me, instance->GetData(DATA_ANOMALUS_EVENT));
                if (Anomalus && Anomalus->HasAura(SPELL_RIFT_SHIELD))
                    uiSummonCrazedManaWraithTimer = 5000;
                else
                    uiSummonCrazedManaWraithTimer = 10000;
            }
            else
                uiSummonCrazedManaWraithTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_chaotic_riftAI(creature);
    }
};

class achievement_chaos_theory : public AchievementCriteriaScript
{
public:
    achievement_chaos_theory() : AchievementCriteriaScript("achievement_chaos_theory")
    {
    }

    bool OnCheck(Player* /*player*/, Unit* target) override
    {
        if (!target)
            return false;

        if (Creature* Anomalus = target->ToCreature())
            if (Anomalus->AI()->GetData(DATA_CHAOS_THEORY))
                return true;

        return false;
    }
};

void AddSC_boss_anomalus()
{
    new boss_anomalus();
    new npc_chaotic_rift();
    new achievement_chaos_theory();
}
