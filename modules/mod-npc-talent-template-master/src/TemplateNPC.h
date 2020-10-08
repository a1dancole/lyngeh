#ifndef TALENT_FUNCTIONS_H
#define TALENT_FUNCTIONS_H

#include "Define.h"
#include "Player.h"
#include "Item.h"
#include "DBCStores.h"
#include "Log.h"
#include "DatabaseEnv.h"
#include "WorldSession.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GossipDef.h"
#include "Creature.h"
#include "ObjectMgr.h"

#define SPELL_ARMORED_BROWN_BEAR 60116
#define SPELL_EXPERT_RIDING 34090
#define SPELL_COLD_WEATHER_FLYING_LEARNT 44197
#define SPELL_COLD_WEATHER_FLYING_LEARN 54198
#define SPELL_EBON_GRPYON 32239
#define SPELL_TAWNY_WINDSTRIDER 32243
#define PET_WOLF 3825 

enum templateSpells
{
    PLATE_MAIL = 750,
    MAIL = 8737
};

enum WeaponProficiencies
{
    BLOCK = 107,
    BOWS = 264,
    CROSSBOWS = 5011,
    DAGGERS = 1180,
    FIST_WEAPONS = 15590,
    GUNS = 266,
    ONE_H_AXES = 196,
    ONE_H_MACES = 198,
    ONE_H_SWORDS = 201,
    POLEARMS = 200,
    SHOOT = 5019,
    STAVES = 227,
    TWO_H_AXES = 197,
    TWO_H_MACES = 199,
    TWO_H_SWORDS = 202,
    WANDS = 5009,
    THROW_WAR = 2567
};

enum ReputationTabards
{
    ARGENT_CRUSADE_TABARD = 43154,
    WYMREST_ACCORD_TABARD = 43156,
    EBON_BLADE_TABARD = 43155,
    KIRIN_TOR_TABARD = 43157
};

enum Reputations
{
    KIRIN_TOR_FACTION = 1090,
    WYRMREST_ACCORD_FACTION = 1091,
    EBON_BLADE_FACTION = 1098,
    ARGENT_CRUSADE_FACTION = 1106
};

static void LearnWeaponSkills(Player* player)
{
    WeaponProficiencies wepSkills[] = {
        BLOCK, BOWS, CROSSBOWS, DAGGERS, FIST_WEAPONS, GUNS, ONE_H_AXES, ONE_H_MACES,
        ONE_H_SWORDS, POLEARMS, SHOOT, STAVES, TWO_H_AXES, TWO_H_MACES, TWO_H_SWORDS, WANDS, THROW_WAR
    };

    uint32 size = 17;

    for (uint32 i = 0; i < size; ++i)
        if (player->HasSpell(wepSkills[i]))
            continue;

    switch (player->getClass())
    {
    case CLASS_WARRIOR:
        player->learnSpell(THROW_WAR);
        player->learnSpell(TWO_H_SWORDS);
        player->learnSpell(TWO_H_MACES);
        player->learnSpell(TWO_H_AXES);
        player->learnSpell(STAVES);
        player->learnSpell(POLEARMS);
        player->learnSpell(ONE_H_SWORDS);
        player->learnSpell(ONE_H_MACES);
        player->learnSpell(ONE_H_AXES);
        player->learnSpell(GUNS);
        player->learnSpell(FIST_WEAPONS);
        player->learnSpell(DAGGERS);
        player->learnSpell(CROSSBOWS);
        player->learnSpell(BOWS);
        player->learnSpell(BLOCK);
        break;
    case CLASS_PRIEST:
        player->learnSpell(WANDS);
        player->learnSpell(STAVES);
        player->learnSpell(SHOOT);
        player->learnSpell(ONE_H_MACES);
        player->learnSpell(DAGGERS);
        break;
    case CLASS_PALADIN:
        player->learnSpell(TWO_H_SWORDS);
        player->learnSpell(TWO_H_MACES);
        player->learnSpell(TWO_H_AXES);
        player->learnSpell(POLEARMS);
        player->learnSpell(ONE_H_SWORDS);
        player->learnSpell(ONE_H_MACES);
        player->learnSpell(ONE_H_AXES);
        player->learnSpell(BLOCK);
        break;
    case CLASS_ROGUE:
        player->learnSpell(ONE_H_SWORDS);
        player->learnSpell(ONE_H_MACES);
        player->learnSpell(ONE_H_AXES);
        player->learnSpell(GUNS);
        player->learnSpell(FIST_WEAPONS);
        player->learnSpell(DAGGERS);
        player->learnSpell(CROSSBOWS);
        player->learnSpell(BOWS);
        break;
    case CLASS_DEATH_KNIGHT:
        player->learnSpell(TWO_H_SWORDS);
        player->learnSpell(TWO_H_MACES);
        player->learnSpell(TWO_H_AXES);
        player->learnSpell(POLEARMS);
        player->learnSpell(ONE_H_SWORDS);
        player->learnSpell(ONE_H_MACES);
        player->learnSpell(ONE_H_AXES);
        break;
    case CLASS_MAGE:
        player->learnSpell(WANDS);
        player->learnSpell(STAVES);
        player->learnSpell(SHOOT);
        player->learnSpell(ONE_H_SWORDS);
        player->learnSpell(DAGGERS);
        break;
    case CLASS_SHAMAN:
        player->learnSpell(TWO_H_MACES);
        player->learnSpell(TWO_H_AXES);
        player->learnSpell(STAVES);
        player->learnSpell(ONE_H_MACES);
        player->learnSpell(ONE_H_AXES);
        player->learnSpell(FIST_WEAPONS);
        player->learnSpell(DAGGERS);
        player->learnSpell(BLOCK);
        break;
    case CLASS_HUNTER:
        player->learnSpell(THROW_WAR);
        player->learnSpell(TWO_H_SWORDS);
        player->learnSpell(TWO_H_AXES);
        player->learnSpell(STAVES);
        player->learnSpell(POLEARMS);
        player->learnSpell(ONE_H_SWORDS);
        player->learnSpell(ONE_H_AXES);
        player->learnSpell(GUNS);
        player->learnSpell(FIST_WEAPONS);
        player->learnSpell(DAGGERS);
        player->learnSpell(CROSSBOWS);
        player->learnSpell(BOWS);
        break;
    case CLASS_DRUID:
        player->learnSpell(TWO_H_MACES);
        player->learnSpell(STAVES);
        player->learnSpell(POLEARMS);
        player->learnSpell(ONE_H_MACES);
        player->learnSpell(FIST_WEAPONS);
        player->learnSpell(DAGGERS);
        break;
    case CLASS_WARLOCK:
        player->learnSpell(WANDS);
        player->learnSpell(STAVES);
        player->learnSpell(SHOOT);
        player->learnSpell(ONE_H_SWORDS);
        player->learnSpell(DAGGERS);
        break;
    default:
        break;
    }
    player->UpdateSkillsToMaxSkillsForLevel();

}

struct TalentTemplate
{
    std::string    playerClass;
    std::string    playerSpec;
    uint32         talentId;
};

struct GlyphTemplate
{
    std::string    playerClass;
    std::string    playerSpec;
    uint8          slot;
    uint32         glyph;
};

struct HumanGearTemplate
{
    std::string    playerClass;
    std::string    playerSpec;
    uint8          pos;
    uint32         itemEntry;
    uint32         enchant;
    uint32         socket1;
    uint32         socket2;
    uint32         socket3;
    uint32         bonusEnchant;
    uint32         prismaticEnchant;
};

struct AllianceGearTemplate
{
    std::string    playerClass;
    std::string    playerSpec;
    uint8          pos;
    uint32         itemEntry;
    uint32         enchant;
    uint32         socket1;
    uint32         socket2;
    uint32         socket3;
    uint32         bonusEnchant;
    uint32         prismaticEnchant;
};

struct HordeGearTemplate
{
    std::string    playerClass;
    std::string    playerSpec;
    uint8          pos;
    uint32         itemEntry;
    uint32         enchant;
    uint32         socket1;
    uint32         socket2;
    uint32         socket3;
    uint32         bonusEnchant;
    uint32         prismaticEnchant;
};

typedef std::vector<AllianceGearTemplate*> AllianceGearContainer;
typedef std::vector<HordeGearTemplate*> HordeGearContainer;

typedef std::vector<TalentTemplate*> TalentContainer;
typedef std::vector<GlyphTemplate*> GlyphContainer;

class sTemplateNPC
{
public:
    static sTemplateNPC* instance()
    {
        static sTemplateNPC* instance = new sTemplateNPC();
        return instance;
    }
    void LoadTalentsContainer();
    void LoadAllianceGearContainer();
    void LoadHordeGearContainer();

    void ApplyBonus(Player* player, Item* item, EnchantmentSlot slot, uint32 bonusEntry);

    bool OverwriteTemplate(Player* /*player*/, std::string& /*playerSpecStr*/);
    void ExtractGearTemplateToDB(Player* /*player*/, std::string& /*playerSpecStr*/);
    void ExtractTalentTemplateToDB(Player* /*player*/, std::string& /*playerSpecStr*/);
    bool CanEquipTemplate(Player* /*player*/, std::string& /*playerSpecStr*/);

    std::string GetClassString(Player* /*player*/);
    std::string sTalentsSpec;

    void LearnTemplateTalents(Player* /*player*/);
    void EquipTemplateGear(Player* /*player*/);
    void CreatePet(Player* /*player*/);

    void LearnPlateMailSpells(Player* /*player*/);
    void AddWrathReputations(Player* /*player*/);

    TalentContainer m_TalentContainer;

    AllianceGearContainer m_AllianceGearContainer;
    HordeGearContainer m_HordeGearContainer;
};
#define sTemplateNpcMgr sTemplateNPC::instance()
#endif
