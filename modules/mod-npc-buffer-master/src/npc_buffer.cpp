/*

# Buffer NPC

_This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))_

### Description
------------------------------------------------------------------------------------------------------------------
This is a one-click buffing NPC that will buff the player with a specific set of spells. The NPC can also buff
everyone the same or by player level. He speaks a configurable random phrase after every use and can also attract
the player using configurable emote options.

- Creates a Buff NPC with emotes
- Buffs the player with no dialogue interaction
- Buffs all the same or by player level
- Speaks configurable random whispers to the player after every use
- Attracts the player using configurable emotes
- Config:
    - Buff by level
    - Spell ID(s) for buffs
    - Enable/Disable cure resurrection sickness
    - Emote Options

### Data
------------------------------------------------------------------------------------------------------------------
- Type: NPC (ID: 601016)
- Script: buff_npc
- Config: Yes
- SQL: Yes

### Version
------------------------------------------------------------------------------------------------------------------
- v2019.04.17 - Fix Cure Resurrection Sickness, works now! Courtesy of Poszer and Milestorme
- v2019.04.15 - Ported to AzerothCore by gtao725 (https://github.com/gtao725/)
- v2019.02.13 - Added phrases/emotes, config options, updated AI
- v2017.08.06 - Removed dialogue options (Just buffs player on click)
- v2017.08.05

### CREDITS
------------------------------------------------------------------------------------------------------------------
![Styx](https://stygianthebest.github.io/assets/img/avatar/avatar-128.jpg "Styx")
![StygianCore](https://stygianthebest.github.io/assets/img/projects/stygiancore/StygianCore.png "StygianCore")

##### This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))

#### Additional Credits

- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [OwnedCore](http://ownedcore.com/)
- [ModCraft.io](http://modcraft.io/)
- [MMO Society](https://www.mmo-society.com/)
- [AoWoW](https://wotlk.evowow.com/)
- [More credits are cited in the sources](https://github.com/StygianTheBest)

### LICENSE
------------------------------------------------------------------------------------------------------------------
This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).

*/

#include "Config.h"
#include "ScriptMgr.h"

bool BFAnnounceModule;
bool BuffByLevel;
bool BuffCureRes;
uint32 BuffNumPhrases;
uint32 BuffNumWhispers;
uint32 BuffMessageTimer;
uint32 BuffEmoteSpell;
uint32 BuffEmoteCommand;

class BufferConfig : public WorldScript
{
public:
    BufferConfig() : WorldScript("BufferConfig_conf") { }

    void OnBeforeConfigLoad(bool reload) override
    {

    }
};

class BufferAnnounce : public PlayerScript
{

public:

    BufferAnnounce() : PlayerScript("BufferAnnounce") {}
};

class buff_npc : public CreatureScript
{
public:

    buff_npc() : CreatureScript("buff_npc") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        AddGossipItemFor(player, GOSSIP_ICON_CHAT_11, "Repair", GOSSIP_SENDER_MAIN, 1);

        SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        ClearGossipMenuFor(player);
        switch (sender)
        {
        case GOSSIP_SENDER_MAIN:
            SendActionMenu(player, creature, action);
            break;
        }
        return true;
    }

    void SendActionMenu(Player* player, Creature* creature, uint32 action)
    {
        switch (action)
        {
        case 1:
            uint32 cost = player->DurabilityRepairAll(true, 0, false);
            creature->MonsterWhisper(fmt::format("Repaired all items for {}g {}s {}c", cost%100, cost%100%100, cost%100%100%100).c_str(), player, false);
            break;
        }
    }

    // Passive Emotes
    struct NPC_PassiveAI : public ScriptedAI
    {
        NPC_PassiveAI(Creature * creature) : ScriptedAI(creature) { }

        uint32 MessageTimer;

        // Called once when client is loaded
        void Reset()
        {
            MessageTimer = urand(BuffMessageTimer, 300000); // 1-5 minutes
        }

        // Called at World update tick
        void UpdateAI(const uint32 diff)
        {
            Map::PlayerList const& PlList = me->GetMap()->GetPlayers();
            if (PlList.isEmpty())
                return;

            for (const MapReference& i : PlList)
            {
                if (Player* player = i.GetSource())
                {
                    if (player->IsGameMaster())
                        continue;

                    if (player->IsAlive() && me->GetDistance(player) < 10.0f)
                    {
                        if(player->HasAura(15007))
                            player->RemoveAura(15007); // Res sickness

                        if (!player->HasAura(48162) || !player->HasAura(25898) || !player->HasAura(48470) || !player->HasAura(48074) || !player->HasAura(48170) || !player->HasAura(43002))
                        {
                                player->AddAura(48162, player); //Prayer of Fortitude
                                player->AddAura(25898, player); //Greater Blessing of Kings
                                player->AddAura(48470, player); //Gift of the Wild
                                player->AddAura(48074, player); //Prayer of Spirit(48074)
                                player->AddAura(48170, player); //Prayer of Shadow Protection(48170)
                                player->AddAura(43002, player); //Arcane Brilliance

                                me->CastSpell(player, 31726, true);
                                player->CastSpell(player, 31726);
                        }
                    }
                }
            }
        }      
    };

    // CREATURE AI
    CreatureAI * GetAI(Creature * creature) const
    {
        return new NPC_PassiveAI(creature);
    }
};

void AddNPCBufferScripts()
{
    new BufferConfig();
    new BufferAnnounce();
    new buff_npc();
}
