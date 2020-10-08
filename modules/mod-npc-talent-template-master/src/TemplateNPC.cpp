/* =============================================================
TO DO:
• Merge human sql template with alliance template
• As Barbz suggested: Rename to character_template the module 
    and all related files (to be less confusing and less generic)
• As Barbz suggested: Scaling system for twink servers
================================================================ */

#include "Pet.h"
#include "TemplateNPC.h"

static const std::unordered_set<std::string> knownSpecs = {
    "Discipline", "Holy",        "Shadow",      "Protection", "Retribution", "Fury",         "Arms",         "Arcane",   "Fire",          "Frost",  "Affliction", "Demonology", "Destruction",
    "Elemental",  "Enhancement", "Restoration", "Ballance",   "Feral",       "Marksmanship", "Beastmastery", "Survival", "Assassination", "Combat", "Subtlety",   "Blood",      "Unholy",
};

void sTemplateNPC::LearnPlateMailSpells(Player* player)
{
    switch (player->getClass())
    {
        case CLASS_WARRIOR:
        case CLASS_PALADIN:
        case CLASS_DEATH_KNIGHT:
            player->learnSpell(PLATE_MAIL);
            break;
        case CLASS_SHAMAN:
        case CLASS_HUNTER:
            player->learnSpell(MAIL);
            break;
        default:
            break;
    }
}

void sTemplateNPC::ApplyBonus(Player* player, Item* item, EnchantmentSlot slot, uint32 bonusEntry)
{
    if (!item)
        return;

    if (!bonusEntry || bonusEntry == 0)
        return;

    player->ApplyEnchantment(item, slot, false);
    item->SetEnchantment(slot, bonusEntry, 0, 0);
    player->ApplyEnchantment(item, slot, true);
}

void sTemplateNPC::LearnTemplateTalents(Player* player)
{
    for (TalentContainer::const_iterator itr = m_TalentContainer.begin(); itr != m_TalentContainer.end(); ++itr)
    {
        if ((*itr)->playerClass == GetClassString(player).c_str() && (*itr)->playerSpec == sTalentsSpec)
        {
            player->learnSpell((*itr)->talentId);
            player->addTalent((*itr)->talentId, player->GetActiveSpecMask(), true);
        }
    }

    player->SetFreeTalentPoints(0);
    player->SendTalentsInfoData(false);
}

void sTemplateNPC::EquipTemplateGear(Player* player)
{ 
    if (player->GetTeamId() == TEAM_ALLIANCE)
    {
        for (AllianceGearContainer::const_iterator itr = m_AllianceGearContainer.begin(); itr != m_AllianceGearContainer.end(); ++itr)
        {
            if ((*itr)->playerClass == GetClassString(player).c_str() && (*itr)->playerSpec == sTalentsSpec)
            {
                player->EquipNewItem((*itr)->pos, (*itr)->itemEntry, true); // Equip the item and apply enchants and gems
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), PERM_ENCHANTMENT_SLOT, (*itr)->enchant);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT, (*itr)->socket1);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT_2, (*itr)->socket2);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT_3, (*itr)->socket3);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), BONUS_ENCHANTMENT_SLOT, (*itr)->bonusEnchant);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), PRISMATIC_ENCHANTMENT_SLOT, (*itr)->prismaticEnchant);
            }
        }
    }
    else if (player->GetTeamId() == TEAM_HORDE)
    {
        for (HordeGearContainer::const_iterator itr = m_HordeGearContainer.begin(); itr != m_HordeGearContainer.end(); ++itr)
        {
            if ((*itr)->playerClass == GetClassString(player).c_str() && (*itr)->playerSpec == sTalentsSpec)
            {
                player->EquipNewItem((*itr)->pos, (*itr)->itemEntry, true); // Equip the item and apply enchants and gems
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), PERM_ENCHANTMENT_SLOT, (*itr)->enchant);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT, (*itr)->socket1);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT_2, (*itr)->socket2);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT_3, (*itr)->socket3);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), BONUS_ENCHANTMENT_SLOT, (*itr)->bonusEnchant);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), PRISMATIC_ENCHANTMENT_SLOT, (*itr)->prismaticEnchant);
            }
        }
    }
}

void sTemplateNPC::LoadTalentsContainer()
{
    for (TalentContainer::const_iterator itr = m_TalentContainer.begin(); itr != m_TalentContainer.end(); ++itr)
        delete *itr;

    m_TalentContainer.clear();

    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    QueryResult result = CharacterDatabase.Query("SELECT playerClass, playerSpec, talentId FROM template_npc_talents;");

    if (!result)
    {
        sLog->outString(">>TEMPLATE NPC: Loaded 0 talent templates. DB table `template_npc_talents` is empty!");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        TalentTemplate* pTalent = new TalentTemplate;

        pTalent->playerClass = fields[0].GetString();
        pTalent->playerSpec = fields[1].GetString();
        pTalent->talentId = fields[2].GetUInt32();

        m_TalentContainer.push_back(pTalent);
        ++count;
    } while (result->NextRow());
    sLog->outString(">>TEMPLATE NPC: Loaded %u talent templates in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void sTemplateNPC::LoadAllianceGearContainer()
{
    for (AllianceGearContainer::const_iterator itr = m_AllianceGearContainer.begin(); itr != m_AllianceGearContainer.end(); ++itr)
        delete *itr;

    m_AllianceGearContainer.clear();

    QueryResult result = CharacterDatabase.Query("SELECT playerClass, playerSpec, pos, itemEntry, enchant, socket1, socket2, socket3, bonusEnchant, prismaticEnchant FROM template_npc_alliance;");

    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    if (!result)
    {
        sLog->outString(">>TEMPLATE NPC: Loaded 0 'gear templates. DB table `template_npc_alliance` is empty!");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        AllianceGearTemplate* pItem = new AllianceGearTemplate;

        pItem->playerClass = fields[0].GetString();
        pItem->playerSpec = fields[1].GetString();
        pItem->pos = fields[2].GetUInt8();
        pItem->itemEntry = fields[3].GetUInt32();
        pItem->enchant = fields[4].GetUInt32();
        pItem->socket1 = fields[5].GetUInt32();
        pItem->socket2 = fields[6].GetUInt32();
        pItem->socket3 = fields[7].GetUInt32();
        pItem->bonusEnchant = fields[8].GetUInt32();
        pItem->prismaticEnchant = fields[9].GetUInt32();

        m_AllianceGearContainer.push_back(pItem);
        ++count;
    } while (result->NextRow());
    sLog->outString(">>TEMPLATE NPC: Loaded %u gear templates for Alliances in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void sTemplateNPC::LoadHordeGearContainer()
{
    for (HordeGearContainer::const_iterator itr = m_HordeGearContainer.begin(); itr != m_HordeGearContainer.end(); ++itr)
        delete *itr;

    m_HordeGearContainer.clear();

    QueryResult result = CharacterDatabase.Query("SELECT playerClass, playerSpec, pos, itemEntry, enchant, socket1, socket2, socket3, bonusEnchant, prismaticEnchant FROM template_npc_horde;");

    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    if (!result)
    {
        sLog->outString(">>TEMPLATE NPC: Loaded 0 'gear templates. DB table `template_npc_horde` is empty!");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        HordeGearTemplate* pItem = new HordeGearTemplate;

        pItem->playerClass = fields[0].GetString();
        pItem->playerSpec = fields[1].GetString();
        pItem->pos = fields[2].GetUInt8();
        pItem->itemEntry = fields[3].GetUInt32();
        pItem->enchant = fields[4].GetUInt32();
        pItem->socket1 = fields[5].GetUInt32();
        pItem->socket2 = fields[6].GetUInt32();
        pItem->socket3 = fields[7].GetUInt32();
        pItem->bonusEnchant = fields[8].GetUInt32();
        pItem->prismaticEnchant = fields[9].GetUInt32();

        m_HordeGearContainer.push_back(pItem);
        ++count;
    } while (result->NextRow());
    sLog->outString(">>TEMPLATE NPC: Loaded %u gear templates for Hordes in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void sTemplateNPC::CreatePet(Player* player)
{
    if (player->getClass() == CLASS_HUNTER)
    {
        //If we have a pet already skip this
        if (player->GetPet())
            return;

        Pet* pet = player->CreateTamedPetFrom(PET_WOLF, 13481);

        if (!pet)
            return;

        pet->SetPower(POWER_HAPPINESS, 1048000);

        pet->AddUInt64Value(UNIT_FIELD_CREATEDBY, player->GetGUID());
        pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->getFaction());
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());

        pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel() - 1);
        pet->GetMap()->AddToMap(pet->ToCreature());

        pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());

        pet->InitTalentForLevel();
        if (!pet->InitStatsForLevel(player->getLevel()))
        {
            pet->UpdateAllStats();
        }

        player->SetMinion(pet, true);

        pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);
        player->PetSpellInitialize();
        pet->InitLevelupSpellsForLevel();
        pet->SetFreeTalentPoints(pet->GetMaxTalentPointsForLevel(player->getLevel()));

        pet->SavePetToDB(PET_SAVE_AS_CURRENT, 0);
    }
}

void sTemplateNPC::AddWrathReputations(Player* player)
{
    if (player->GetReputation(EBON_BLADE_FACTION) < 3000)
    {
        player->AddItem(ARGENT_CRUSADE_TABARD, 1);
        player->AddItem(WYMREST_ACCORD_TABARD, 1);
        player->AddItem(EBON_BLADE_TABARD, 1);
        player->AddItem(KIRIN_TOR_TABARD, 1);

        player->SetReputation(EBON_BLADE_FACTION, urand(3000, 6000));
        player->SetReputation(KIRIN_TOR_FACTION, urand(3000, 6000));
        player->SetReputation(WYRMREST_ACCORD_FACTION, urand(3000, 6000));
        player->SetReputation(ARGENT_CRUSADE_FACTION, urand(3000, 6000));
    }
}

std::string sTemplateNPC::GetClassString(Player* player)
{
    switch (player->getClass())
    {
        case CLASS_PRIEST:       return "Priest";      break;
        case CLASS_PALADIN:      return "Paladin";     break;
        case CLASS_WARRIOR:      return "Warrior";     break;
        case CLASS_MAGE:         return "Mage";        break;
        case CLASS_WARLOCK:      return "Warlock";     break;
        case CLASS_SHAMAN:       return "Shaman";      break;
        case CLASS_DRUID:        return "Druid";       break;
        case CLASS_HUNTER:       return "Hunter";      break;
        case CLASS_ROGUE:        return "Rogue";       break;
        case CLASS_DEATH_KNIGHT: return "DeathKnight"; break;
        default:
            break;
        }
    return "Unknown"; // Fix warning, this should never happen
}

bool sTemplateNPC::OverwriteTemplate(Player* player, std::string& playerSpecStr)
{
    // Delete old talent and glyph templates before extracting new ones
    CharacterDatabase.PExecute("DELETE FROM template_npc_talents WHERE playerClass = '%s' AND playerSpec = '%s';", GetClassString(player).c_str(), playerSpecStr.c_str());

    if (player->GetTeamId() == TEAM_ALLIANCE)
    {
        CharacterDatabase.PExecute("DELETE FROM template_npc_alliance WHERE playerClass = '%s' AND playerSpec = '%s';", GetClassString(player).c_str(), playerSpecStr.c_str());
        player->GetSession()->SendAreaTriggerMessage("Template successfuly created!");
        return false;
    }
    else if (player->GetTeamId() == TEAM_HORDE)
    {                                                                                                        // ????????????? sTemplateNpcMgr here??
        CharacterDatabase.PExecute("DELETE FROM template_npc_horde WHERE playerClass = '%s' AND playerSpec = '%s';", GetClassString(player).c_str(), playerSpecStr.c_str());
        player->GetSession()->SendAreaTriggerMessage("Template successfuly created!");
        return false;
    }
    return true;
}

void sTemplateNPC::ExtractGearTemplateToDB(Player* player, std::string& playerSpecStr)
{
    CharacterDatabase.PExecute("DELETE FROM template_npc_alliance WHERE playerClass = '%s' AND playerSpec = '%s'", GetClassString(player), playerSpecStr);
    CharacterDatabase.PExecute("DELETE FROM template_npc_horde WHERE playerClass = '%s' AND playerSpec = '%s'", GetClassString(player), playerSpecStr);

    for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
    {
        Item* equippedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);

        if (equippedItem)
        {
            CharacterDatabase.PExecute("INSERT INTO template_npc_alliance (`playerClass`, `playerSpec`, `pos`, `itemEntry`, `enchant`, `socket1`, `socket2`, `socket3`, `bonusEnchant`, `prismaticEnchant`) VALUES ('%s', '%s', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u');"
                , GetClassString(player).c_str(), playerSpecStr.c_str(), equippedItem->GetSlot(), equippedItem->GetEntry(), equippedItem->GetEnchantmentId(PERM_ENCHANTMENT_SLOT),
                equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT), equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2), equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3),
                equippedItem->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT), equippedItem->GetEnchantmentId(PRISMATIC_ENCHANTMENT_SLOT));

            CharacterDatabase.PExecute("INSERT INTO template_npc_horde (`playerClass`, `playerSpec`, `pos`, `itemEntry`, `enchant`, `socket1`, `socket2`, `socket3`, `bonusEnchant`, `prismaticEnchant`) VALUES ('%s', '%s', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u');"
                , GetClassString(player).c_str(), playerSpecStr.c_str(), equippedItem->GetSlot(), equippedItem->GetEntry(), equippedItem->GetEnchantmentId(PERM_ENCHANTMENT_SLOT),
                equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT), equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2), equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3),
                equippedItem->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT), equippedItem->GetEnchantmentId(PRISMATIC_ENCHANTMENT_SLOT));
        }
    }
}

void sTemplateNPC::ExtractTalentTemplateToDB(Player* player, std::string& playerSpecStr)
{
    QueryResult result = CharacterDatabase.PQuery("SELECT spell FROM character_talent WHERE guid = '%u' "
        "AND specMask = '%u';", player->GetGUID(), player->GetActiveSpecMask());

    if (!result)
    {
        return;
    }
    else if (player->GetFreeTalentPoints() > 0)
    {
        player->GetSession()->SendAreaTriggerMessage("You have unspend talent points. Please spend all your talent points and re-extract the template.");
        return;
    }
    else
    {
        CharacterDatabase.PExecute("DELETE FROM template_npc_talents WHERE playerClass = '%s' AND playerSpec = '%s'", GetClassString(player), playerSpecStr);
        do
        {
            Field* fields = result->Fetch();
            uint32 spell = fields[0].GetUInt32();

            CharacterDatabase.PExecute("INSERT INTO template_npc_talents (playerClass, playerSpec, talentId) "
                "VALUES ('%s', '%s', '%u');", GetClassString(player).c_str(), playerSpecStr.c_str(), spell);
        } while (result->NextRow());
    }
}

bool sTemplateNPC::CanEquipTemplate(Player* player, std::string& playerSpecStr)
{
    if (player->GetTeamId() == TEAM_ALLIANCE)
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT playerClass, playerSpec FROM template_npc_alliance "
            "WHERE playerClass = '%s' AND playerSpec = '%s';", GetClassString(player).c_str(), playerSpecStr.c_str());

        if (!result)
            return false;
    }
    else if (player->GetTeamId() == TEAM_HORDE)
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT playerClass, playerSpec FROM template_npc_horde "
            "WHERE playerClass = '%s' AND playerSpec = '%s';", GetClassString(player).c_str(), playerSpecStr.c_str());

        if (!result)
            return false;
    }
    return true;
}

class TemplateNPC : public CreatureScript
{
public:
    TemplateNPC() : CreatureScript("TemplateNPC") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            switch (player->getClass())
            {
            case CLASS_PRIEST:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_holy_wordfortitude:30|t|r Discipline", GOSSIP_SENDER_MAIN, 0);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_holy_holybolt:30|t|r Holy", GOSSIP_SENDER_MAIN, 1);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_shadow_shadowwordpain:30|t|r Shadow", GOSSIP_SENDER_MAIN, 2);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            case CLASS_PALADIN:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_holy_holybolt:30|t|r Holy", GOSSIP_SENDER_MAIN, 3);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_holy_devotionaura:30|t|r Protection", GOSSIP_SENDER_MAIN, 4);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_holy_auraoflight:30|t|r Retribution", GOSSIP_SENDER_MAIN, 5);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            case CLASS_WARRIOR:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_warrior_innerrage:30|t|r Fury", GOSSIP_SENDER_MAIN, 6);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_rogue_eviscerate:30|t|r Arms", GOSSIP_SENDER_MAIN, 7);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_warrior_defensivestance:30|t|r Protection", GOSSIP_SENDER_MAIN, 8);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            case CLASS_MAGE:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_holy_magicalsentry:30|t|r Arcane", GOSSIP_SENDER_MAIN, 9);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_fire_flamebolt:30|t|r Fire", GOSSIP_SENDER_MAIN, 10);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_frost_frostbolt02:30|t|r Frost", GOSSIP_SENDER_MAIN, 11);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            case CLASS_WARLOCK:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_shadow_deathcoil:30|t|r Affliction", GOSSIP_SENDER_MAIN, 12);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_shadow_metamorphosis:30|t|r Demonology", GOSSIP_SENDER_MAIN, 13);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_shadow_rainoffire:30|t|r Destruction", GOSSIP_SENDER_MAIN, 14);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            case CLASS_SHAMAN:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_nature_lightning:30|t|r Elemental", GOSSIP_SENDER_MAIN, 15);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_nature_lightningshield:30|t|r Enhancement", GOSSIP_SENDER_MAIN, 16);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_nature_magicimmunity:30|t|r Restoration", GOSSIP_SENDER_MAIN, 17);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            case CLASS_DRUID:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_nature_starfall:30|t|r Balance", GOSSIP_SENDER_MAIN, 18);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_racial_bearform:30|t|r Feral", GOSSIP_SENDER_MAIN, 19);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_nature_healingtouch:30|t|r Restoration", GOSSIP_SENDER_MAIN, 20);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            case CLASS_HUNTER:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_marksmanship:30|t|r Marksmanship", GOSSIP_SENDER_MAIN, 21);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_hunter_beasttaming:30|t|r Beastmastery", GOSSIP_SENDER_MAIN, 22);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_Hunter_swiftstrike:30|t|r Survival", GOSSIP_SENDER_MAIN, 23);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            case CLASS_ROGUE:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_rogue_eviscerate:30|t|r Assasination", GOSSIP_SENDER_MAIN, 24);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_backstab:30|t|r Combat", GOSSIP_SENDER_MAIN, 25);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\ability_stealth:30|t|r Subtlety", GOSSIP_SENDER_MAIN, 26);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            case CLASS_DEATH_KNIGHT:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_deathknight_bloodpresence:30|t|r Blood", GOSSIP_SENDER_MAIN, 27);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_deathknight_frostpresence:30|t|r Frost", GOSSIP_SENDER_MAIN, 28);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|cff00ff00|TInterface\\icons\\spell_deathknight_unholypresence:30|t|r Unholy", GOSSIP_SENDER_MAIN, 29);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "----------------------------------------------", GOSSIP_SENDER_MAIN, 5000);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Trade_Engineering:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);*/
                break;
            }

            /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Spell_ChargeNegative:30|t|r Remove all glyphs", GOSSIP_SENDER_MAIN, 30);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Spell_ChargeNegative:30|t|r Reset Talents", GOSSIP_SENDER_MAIN, 31);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\Spell_ChargeNegative:30|t|r Destroy my equipped gear", GOSSIP_SENDER_MAIN, 32);*/
			player->SEND_GOSSIP_MENU(55002, creature->GetGUID());
            return true;
        }

        static void EquipFullTemplateGear(Player* player, std::string& playerSpecStr) // Merge
        {
            if (sTemplateNpcMgr->CanEquipTemplate(player, playerSpecStr) == false)
            {
                player->GetSession()->SendAreaTriggerMessage("There's no templates for %s specialization yet.", playerSpecStr.c_str());
                return;
            }

            // Don't let players to use Template feature while wearing some gear
            for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
            {
                if (Item* haveItemEquipped = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                {
                    if (haveItemEquipped)
                    {
                        player->GetSession()->SendAreaTriggerMessage("You need to remove all your equipped items in order to use this feature!");
						player->CLOSE_GOSSIP_MENU();
                        return;
                    }
                }
            }

            //// Don't let players to use Template feature after spending some talent points
            //if (player->GetFreeTalentPoints() < 71)
            //{
            //    player->GetSession()->SendAreaTriggerMessage("You have already spent some talent points. You need to reset your talents first!");
            //    player->CLOSE_GOSSIP_MENU();
            //    return;
            //}

            //sTemplateNpcMgr->LearnTemplateTalents(player);
            sTemplateNpcMgr->EquipTemplateGear(player);
            sTemplateNpcMgr->AddWrathReputations(player);
            sTemplateNpcMgr->LearnPlateMailSpells(player);
            sTemplateNpcMgr->CreatePet(player);

            LearnWeaponSkills(player);

            player->GetSession()->SendAreaTriggerMessage("Successfuly equipped %s %s template!", playerSpecStr.c_str(), sTemplateNpcMgr->GetClassString(player).c_str());

            if (player->getPowerType() == POWER_MANA)
                player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));

            player->SetHealth(player->GetMaxHealth());

            // Learn Riding/Flying
            if (player->HasSpell(SPELL_EXPERT_RIDING) ||
                player->HasSpell(SPELL_COLD_WEATHER_FLYING_LEARNT) ||
                player->HasSpell(SPELL_ARMORED_BROWN_BEAR) ||
                player->HasSpell(SPELL_EBON_GRPYON) ||
                player->HasSpell(SPELL_TAWNY_WINDSTRIDER)
                )
                return;

            player->learnSpell(SPELL_EXPERT_RIDING);
            player->CastSpell(player, SPELL_COLD_WEATHER_FLYING_LEARN);
            player->learnSpell(SPELL_ARMORED_BROWN_BEAR);
            player->GetTeamId() == TEAM_ALLIANCE ? player->learnSpell(SPELL_EBON_GRPYON) : player->learnSpell(SPELL_TAWNY_WINDSTRIDER);
            if (player->HasSkill(129))
                player->SetSkill(129, 375, 375, 450);

            player->SaveToDB(false, false);
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
        {
            player->PlayerTalkClass->ClearMenus();

            if (!player || !creature)
                return false;

            switch (uiAction)
            {
            case 0: // Use Discipline Priest Spec
                sTemplateNpcMgr->sTalentsSpec = "Discipline";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 1: // Use Holy Priest Spec
                sTemplateNpcMgr->sTalentsSpec = "Holy";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 2: // Use Shadow Priest Spec
                sTemplateNpcMgr->sTalentsSpec = "Shadow";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 3: // Use Holy Paladin Spec
                sTemplateNpcMgr->sTalentsSpec = "Holy";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 4: // Use Protection Paladin Spec
                sTemplateNpcMgr->sTalentsSpec = "Protection";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 5: // Use Retribution Paladin Spec
                sTemplateNpcMgr->sTalentsSpec = "Retribution";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 6: // Use Fury Warrior Spec
                sTemplateNpcMgr->sTalentsSpec = "Fury";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 7: // Use Arms Warrior Spec
                sTemplateNpcMgr->sTalentsSpec = "Arms";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 8: // Use Protection Warrior Spec
                sTemplateNpcMgr->sTalentsSpec = "Protection";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 9: // Use Arcane Mage Spec
                sTemplateNpcMgr->sTalentsSpec = "Arcane";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 10: // Use Fire Mage Spec
                sTemplateNpcMgr->sTalentsSpec = "Fire";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 11: // Use Frost Mage Spec
                sTemplateNpcMgr->sTalentsSpec = "Frost";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 12: // Use Affliction Warlock Spec
                sTemplateNpcMgr->sTalentsSpec = "Affliction";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 13: // Use Demonology Warlock Spec
                sTemplateNpcMgr->sTalentsSpec = "Demonology";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 14: // Use Destruction Warlock Spec
                sTemplateNpcMgr->sTalentsSpec = "Destruction";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 15: // Use Elemental Shaman Spec
                sTemplateNpcMgr->sTalentsSpec = "Elemental";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 16: // Use Enhancement Shaman Spec
                sTemplateNpcMgr->sTalentsSpec = "Enhancement";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 17: // Use Restoration Shaman Spec
                sTemplateNpcMgr->sTalentsSpec = "Restoration";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 18: // Use Ballance Druid Spec
                sTemplateNpcMgr->sTalentsSpec = "Ballance";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 19: // Use Feral Druid Spec
                sTemplateNpcMgr->sTalentsSpec = "Feral";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 20: // Use Restoration Druid Spec
                sTemplateNpcMgr->sTalentsSpec = "Restoration";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 21: // Use Marksmanship Hunter Spec
                sTemplateNpcMgr->sTalentsSpec = "Marksmanship";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 22: // Use Beastmastery Hunter Spec
                sTemplateNpcMgr->sTalentsSpec = "Beastmastery";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 23: // Use Survival Hunter Spec
                sTemplateNpcMgr->sTalentsSpec = "Survival";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 24: // Use Assassination Rogue Spec
                sTemplateNpcMgr->sTalentsSpec = "Assassination";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 25: // Use Combat Rogue Spec
                sTemplateNpcMgr->sTalentsSpec = "Combat";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 26: // Use Subtlety Rogue Spec
                sTemplateNpcMgr->sTalentsSpec = "Subtlety";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 27: // Use Blood DK Spec
                sTemplateNpcMgr->sTalentsSpec = "Blood";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 28: // Use Frost DK Spec
                sTemplateNpcMgr->sTalentsSpec = "Frost";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 29: // Use Unholy DK Spec
                sTemplateNpcMgr->sTalentsSpec = "Unholy";
                EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
                player->CLOSE_GOSSIP_MENU();
                break;

            case 30:
                player->resetTalents(true);
                player->SendTalentsInfoData(false);
                player->GetSession()->SendAreaTriggerMessage("Your talents have been reset.");
                player->CLOSE_GOSSIP_MENU();
                break;

            case 31:
                for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
                {
                    if (Item* haveItemEquipped = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                    {
                        if (haveItemEquipped)
                        {
                            player->DestroyItemCount(haveItemEquipped->GetEntry(), 1, true, true);

                            if (haveItemEquipped->IsInWorld())
                            {
                                haveItemEquipped->RemoveFromWorld();
                                haveItemEquipped->DestroyForPlayer(player);
                            }

                            haveItemEquipped->SetUInt64Value(ITEM_FIELD_CONTAINED, 0);
                            haveItemEquipped->SetSlot(NULL_SLOT);
                            haveItemEquipped->SetState(ITEM_REMOVED, player);
                        }
                    }
                }
                player->GetSession()->SendAreaTriggerMessage("Your equipped gear has been destroyed.");
                player->CLOSE_GOSSIP_MENU();
                break;

            case 5000:
                // return to OnGossipHello menu, otherwise it will freeze every menu
                OnGossipHello(player, creature);
                break;

            default: // Just in case
                player->GetSession()->SendAreaTriggerMessage("Something went wrong in the code. Please contact the administrator.");
                break;
            }
            player->UpdateSkillsForLevel();

            return true;
        }
};

class TemplateNPC_command : public CommandScript
{
public:
    TemplateNPC_command() : CommandScript("TemplateNPC_command") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> saveTable = {
            {"talents", SEC_ADMINISTRATOR, false, &HandleSaveTalents, "<spec>, example: `.templatenpc save talents Arms`"},
            {"gear", SEC_ADMINISTRATOR, false, &HandleSaveGear, "<spec>, example: `.templatenpc save gear Arms`"}
        };

        static std::vector<ChatCommand> TemplateNPCTable =
        {
            { "reload", SEC_ADMINISTRATOR, true, &HandleReloadTemplateNPCCommand, "" },
            { "save", SEC_ADMINISTRATOR, false, nullptr, "", saveTable},
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "templatenpc", SEC_ADMINISTRATOR, true, nullptr                         , "", TemplateNPCTable }
        };
        return commandTable;
    }


    static bool HandleReloadTemplateNPCCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outString("Reloading templates for Template NPC table...");
        sTemplateNpcMgr->LoadTalentsContainer();
        sTemplateNpcMgr->LoadAllianceGearContainer();
        sTemplateNpcMgr->LoadHordeGearContainer();
        handler->SendGlobalGMSysMessage("Template NPC templates reloaded.");
        return true;
    }

    static bool HandleSaveGear(ChatHandler* handler, const char* _args)
    {
        if (!*_args)
            return false;

        auto spec = std::string(_args);

        auto selectedSpec = knownSpecs.find(spec);
        if (selectedSpec == knownSpecs.end())
        {
            handler->SendGlobalGMSysMessage("Unknown spec passed");
            return false;
        }

        Player* p = handler->getSelectedPlayerOrSelf();
        if (p == nullptr)
            return false;

        sTemplateNpcMgr->ExtractGearTemplateToDB(p, spec);

        handler->SendGlobalGMSysMessage("Template extracted to DB. You might want to type \".template reload\".");

        return true;
    }

    static bool HandleSaveTalents(ChatHandler* handler, const char* _args)
    {
        if (!*_args)
            return false;

        auto spec = std::string(_args);

        auto selectedSpec = knownSpecs.find(spec);
        if (selectedSpec == knownSpecs.end())
        {
            handler->SendGlobalGMSysMessage("Unknown spec passed");
            return false;
        }

        Player* p = handler->getSelectedPlayerOrSelf();
        if (p == nullptr)
            return false;

        sTemplateNpcMgr->ExtractTalentTemplateToDB(p, spec);

        handler->SendGlobalGMSysMessage("Template extracted to DB. You might want to type \".template reload\".");

        return true;
    }
};

class TemplateNPC_World : public WorldScript
{
public:
    TemplateNPC_World() : WorldScript("TemplateNPC_World") { }

    void OnStartup() override
    {
        // Load templates for Template NPC #1
        sLog->outString("== TEMPLATE NPC ===========================================================================");
        sLog->outString("Loading Template Talents...");
        sTemplateNpcMgr->LoadTalentsContainer();

        // Load templates for Template NPC #4
        sLog->outString("Loading Template Gear for Alliances...");
        sTemplateNpcMgr->LoadAllianceGearContainer();

        // Load templates for Template NPC #5
        sLog->outString("Loading Template Gear for Hordes...");
        sTemplateNpcMgr->LoadHordeGearContainer();
        sLog->outString("== END TEMPLATE NPC ===========================================================================");
    }
};

void AddSC_TemplateNPC()
{
    new TemplateNPC();
    new TemplateNPC_command();
    new TemplateNPC_World();
}
