/*
 * Originally written by Xinef - Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "naxxramas.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_GREET           = 1,
    SAY_SLAY            = 2,
    EMOTE_LOCUST        = 3
};

enum Spells
{
    SPELL_IMPALE_10                 = 28783,
    SPELL_IMPALE_25                 = 56090,
    SPELL_LOCUST_SWARM_10           = 28785,
    SPELL_LOCUST_SWARM_25           = 54021,
    SPELL_SUMMON_CORPSE_SCRABS_5    = 29105,
    SPELL_SUMMON_CORPSE_SCRABS_10   = 28864,
    SPELL_BERSERK                   = 26662,
};

enum Events
{
    EVENT_SPELL_IMPALE              = 1,
    EVENT_SPELL_LOCUST_SWARM        = 2,
    EVENT_SPELL_BERSERK             = 3,
    EVENT_SUMMON_CRYPT_FIEND        = 4,
};

enum Misc
{
    NPC_CORPSE_SCARAB               = 16698,
    NPC_CRYPT_GUARD                 = 16573,

    ACHIEV_TIMED_START_EVENT        = 9891,
};

const Position cryptGuardSummoningPositions[2] =
{
    {3336.169189f, -3476.1000342f, 287.069336f},
    {3205.690430f, -3476.024902, 287.061340f}
};

class boss_anubrekhan : public CreatureScript
{
public:
    boss_anubrekhan() : CreatureScript("boss_anubrekhan") { }

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new boss_anubrekhanAI (pCreature);
    }

    struct boss_anubrekhanAI : public BossAI
    {
        explicit boss_anubrekhanAI(Creature *c) : BossAI(c, BOSS_ANUB), summons(me)
        {
            pInstance = c->GetInstanceScript();
            sayGreet = false;
        }

        InstanceScript* pInstance;
        EventMap events;
        SummonList summons;
        bool sayGreet;


        void SummonCryptGuards()
        {
            if (Is25ManRaid())
            {
                me->SummonCreature(NPC_CRYPT_GUARD, 3299.087f, -3450.93f, 287.16f, M_PI, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000);
            }
        }

        void Reset() override
        {
            BossAI::Reset();
            events.Reset();
            summons.DespawnAll();
            SummonCryptGuards();

            if (pInstance)
            {
                if (GameObject* go = me->GetMap()->GetGameObject(pInstance->GetData64(DATA_ANUB_GATE)))
                    go->SetGoState(GO_STATE_ACTIVE);

                std::list<Creature*> scarabs;
                me->GetCreaturesWithEntryInRange(scarabs, 100.0f, 16698);

                std::list<Creature*>::const_iterator itr;
                for (itr = scarabs.begin(); itr != scarabs.end(); ++itr)
                    (*itr)->DespawnOrUnsummon();
            }
        }

        void JustSummoned(Creature* cr) override
        {
            if (me->IsInCombat())
                cr->SetInCombatWithZone();
            if (cr->GetEntry() == NPC_CORPSE_SCARAB)
            {
                cr->SetReactState(REACT_PASSIVE);
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    cr->AI()->AttackStart(target);
            }
            summons.Summon(cr);
        }

        void SummonedCreatureDespawn(Creature* cr) override
        {
            cr->CastSpell(cr, SPELL_SUMMON_CORPSE_SCRABS_5, true, nullptr, nullptr, cr->GetGUID());
            summons.Despawn(cr);
        }

        void JustDied(Unit*  killer) override
        {
            BossAI::JustDied(killer);
            summons.DespawnAll();
            if (pInstance)
            {
                pInstance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_TIMED_START_EVENT);
            }
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() != TYPEID_PLAYER)
                return;

            Talk(SAY_SLAY);

            if (pInstance)
                pInstance->SetData(DATA_IMMORTAL_FAIL, 0);
        }

        void EnterCombat(Unit * who) override
        {
            BossAI::EnterCombat(who);
            me->CallForHelp(30.0f); // catch helpers
            Talk(SAY_AGGRO);
            if (pInstance)
            {
                if (GameObject* go = me->GetMap()->GetGameObject(pInstance->GetData64(DATA_ANUB_GATE)))
                    go->SetGoState(GO_STATE_READY);
            }

            events.ScheduleEvent(EVENT_SPELL_IMPALE, 15000);
            events.ScheduleEvent(EVENT_SPELL_LOCUST_SWARM, 70000+urand(0,50000));
            events.ScheduleEvent(EVENT_SPELL_BERSERK, 600000);
            events.ScheduleEvent(EVENT_SUMMON_CRYPT_FIEND, 20000 + urand(0, 20000));
            
            if (!summons.HasEntry(NPC_CRYPT_GUARD))
                SummonCryptGuards();
        }

        void MoveInLineOfSight(Unit *who) override
        {
            if (!sayGreet && who->GetTypeId() == TYPEID_PLAYER)
            {
                Talk(SAY_GREET);
                sayGreet = true;
            }

            ScriptedAI::MoveInLineOfSight(who);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.GetEvent())
            {
                case EVENT_SPELL_IMPALE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        me->CastSpell(target, RAID_MODE(SPELL_IMPALE_10, SPELL_IMPALE_25), false);
                    events.RepeatEvent(20000);
                    break;
                case EVENT_SPELL_LOCUST_SWARM:
                {
                    Talk(EMOTE_LOCUST);
                    me->CastSpell(me, RAID_MODE(SPELL_LOCUST_SWARM_10, SPELL_LOCUST_SWARM_25), false);
                    me->SummonCreature(NPC_CRYPT_GUARD, cryptGuardSummoningPositions[urand(1,2)], TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                    events.RepeatEvent(90000);
                    events.DelayEvents(16000);
                    break;
                }
                case EVENT_SPELL_BERSERK:
                    me->CastSpell(me, SPELL_BERSERK, true);
                    events.PopEvent();
                    break;
                case EVENT_SUMMON_CRYPT_FIEND:
                    if (Is25ManRaid())
                    {
                        me->SummonCreature(NPC_CRYPT_GUARD, cryptGuardSummoningPositions[2], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                    }
                    me->SummonCreature(NPC_CRYPT_GUARD, cryptGuardSummoningPositions[1], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                    events.RepeatEvent(60000);
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };  
};

void AddSC_boss_anubrekhan()
{
    new boss_anubrekhan();
}

