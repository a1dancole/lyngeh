/*
 * Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "ObjectMgr.h"
#include "AuctionHouseMgr.h"
#include "AuctionHouseBot.h"
#include "Config.h"
#include "Player.h"
#include "WorldSession.h"
#include <vector>

using namespace std;
vector<uint32> whiteTradeGoodsBin;

AuctionHouseBot::AuctionHouseBot()
{
    debug_Out = false;
    debug_Out_Filters = false;
    AHBSeller = true;
    AHBBuyer = false;

    _lastrun_a = time(NULL);
    _lastrun_h = time(NULL);
    _lastrun_n = time(NULL);

    AllianceConfig = AHBConfig(2);
    HordeConfig = AHBConfig(6);
    NeutralConfig = AHBConfig(7);
}

AuctionHouseBot::~AuctionHouseBot()
{
}

void AuctionHouseBot::addNewAuctions(Player *AHBplayer, AHBConfig *config)
{
    if (!AHBSeller)
    {
        if (debug_Out) sLog->outString("AHSeller: Disabled");
        return;
    }

    uint32 minItems = config->GetMinItems();
    uint32 maxItems = config->GetMaxItems();

    if (maxItems == 0)
    {
        //if (debug_Out) sLog->outString( "AHSeller: Auctions disabled");
        return;
    }

    AuctionHouseEntry const* ahEntry =  sAuctionMgr->GetAuctionHouseEntry(config->GetAHFID());
    if (!ahEntry)
    {
        return;
    }
    AuctionHouseObject* auctionHouse =  sAuctionMgr->GetAuctionsMap(config->GetAHFID());
    if (!auctionHouse)
    {
        return;
    }

    uint32 auctions = auctionHouse->Getcount();

    uint32 items = 0;

    if (auctions >= minItems)
    {
        //if (debug_Out) sLog->outError( "AHSeller: Auctions above minimum");
        return;
    }

    if (auctions >= maxItems)
    {
        //if (debug_Out) sLog->outError( "AHSeller: Auctions at or above maximum");
        return;
    }

    if ((maxItems - auctions) >= ItemsPerCycle)
        items = ItemsPerCycle;
    else
        items = (maxItems - auctions);

    if (debug_Out) sLog->outString("AHSeller: Adding %u Auctions", items);

    uint32 AuctioneerGUID = 0;

    switch (config->GetAHID())
    {
    case 2:
        AuctioneerGUID = 79707; //Human in stormwind.
        break;
    case 6:
        AuctioneerGUID = 4656; //orc in Orgrimmar
        break;
    case 7:
        AuctioneerGUID = 23442; //goblin in GZ
        break;
    default:
        if (debug_Out) sLog->outError( "AHSeller: GetAHID() - Default switch reached");
        AuctioneerGUID = 23442; //default to neutral 7
        break;
    }

    if (debug_Out) sLog->outError( "AHSeller: Current Auctineer GUID is %u", AuctioneerGUID);

/*    uint32 total = greyTGcount + whiteTGcount + greenTGcount + blueTGcount
        + purpleTGcount + orangeTGcount + yellowTGcount
        + whiteIcount + greenIcount + blueIcount + purpleIcount
        + orangeIcount + yellowIcount;
*/

    if (debug_Out) sLog->outError( "AHSeller: %u items", items);

    // only insert a few at a time, so as not to peg the processor
    for (uint32 cnt = 1; cnt <= items; cnt++)
    {
    if (debug_Out) sLog->outError( "AHSeller: %u count", cnt);
        uint32 itemID = 0;
        uint32 itemColor = 99;
        uint32 loopbreaker = 0;
        while (itemID == 0 && loopbreaker <= 50)
        {
            ++loopbreaker;
            uint32 choice = urand(0, 13);
            itemColor = choice;
            switch (choice)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                break;
            case 8:
                {
                    if ((whiteTradeGoodsBin.size() > 0))
                        itemID = whiteTradeGoodsBin[urand(0, whiteTradeGoodsBin.size() - 1)];
                    else continue;
                    break;
                }
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
                break;
            default:
                {
                    if (debug_Out) sLog->outError( "AHSeller: itemID Switch - Default Reached");
                    break;
                }
            }

            if (itemID == 0)
            {
                if (debug_Out) sLog->outError( "AHSeller: Item::CreateItem() - ItemID is 0");
                continue;
            }

            ItemTemplate const* prototype = sObjectMgr->GetItemTemplate(itemID);
            if (prototype == NULL)
            {
                if (debug_Out) sLog->outError( "AHSeller: Huh?!?! prototype == NULL");
                continue;
            }

            Item* item = Item::CreateItem(itemID, 1, AHBplayer);
            if (item == NULL)
            {
                if (debug_Out) sLog->outError( "AHSeller: Item::CreateItem() returned NULL");
                break;
            }
            item->AddToUpdateQueueOf(AHBplayer);

            uint32 randomPropertyId = Item::GenerateItemRandomPropertyId(itemID);
            if (randomPropertyId != 0)
                item->SetItemRandomProperties(randomPropertyId);

            uint64 buyoutPrice = 0;
            uint64 bidPrice = 0;
            uint32 stackCount = 1;

            if (SellMethod)
                buyoutPrice = prototype->BuyPrice;
            else
                buyoutPrice = prototype->SellPrice;

            if (prototype->Quality <= AHB_MAX_QUALITY)
            {
                if (config->GetMaxStack(prototype->Quality) > 1 && item->GetMaxStackCount() > 1)
                    stackCount = urand(1, minValue(item->GetMaxStackCount(), config->GetMaxStack(prototype->Quality)));
                else if (config->GetMaxStack(prototype->Quality) == 0 && item->GetMaxStackCount() > 1)
                    stackCount = urand(1, item->GetMaxStackCount());
                else
                    stackCount = 1;
                buyoutPrice = urand(1000, 80000);
                bidPrice = urand(500, 60000);

                if (bidPrice > buyoutPrice)
                    bidPrice = urand(0, buyoutPrice);
            }
            else
            {
                // quality is something it shouldn't be, let's get out of here
                if (debug_Out) sLog->outError( "AHBuyer: Quality %u not Supported", prototype->Quality);
                item->RemoveFromUpdateQueueOf(AHBplayer);
                continue;
            }

            uint32 etime = urand(1,3);
            switch(etime)
            {
            case 1:
                etime = 43200;
                break;
            case 2:
                etime = 86400;
                break;
            case 3:
                etime = 172800;
                break;
            default:
                etime = 86400;
                break;
            }
            item->SetCount(stackCount);

            uint32 dep =  sAuctionMgr->GetAuctionDeposit(ahEntry, etime, item, stackCount);

            SQLTransaction trans = CharacterDatabase.BeginTransaction();
            AuctionEntry* auctionEntry = new AuctionEntry();
            auctionEntry->Id = sObjectMgr->GenerateAuctionID();
            auctionEntry->auctioneer = AuctioneerGUID;
			auctionEntry->item_guidlow = item->GetGUIDLow();
            auctionEntry->item_template = item->GetEntry();
            auctionEntry->itemCount = item->GetCount();
            auctionEntry->owner = AHBplayer->GetGUIDLow();
            auctionEntry->startbid = bidPrice * stackCount;
            auctionEntry->buyout = buyoutPrice * stackCount;
            auctionEntry->bidder = 0;
            auctionEntry->bid = 0;
            auctionEntry->deposit = dep;
            auctionEntry->expire_time = (time_t) etime + time(NULL);
            auctionEntry->auctionHouseEntry = ahEntry;
            item->SaveToDB(trans);
            item->RemoveFromUpdateQueueOf(AHBplayer);
            sAuctionMgr->AddAItem(item);
            auctionHouse->AddAuction(auctionEntry);
            auctionEntry->SaveToDB(trans);
            CharacterDatabase.CommitTransaction(trans);
        }
    }
}
void AuctionHouseBot::addNewAuctionBuyerBotBid(Player *AHBplayer, AHBConfig *config, WorldSession *session)
{
    if (!AHBBuyer)
    {
        if (debug_Out) sLog->outError( "AHBuyer: Disabled");
        return;
    }

    QueryResult result = CharacterDatabase.PQuery("SELECT id FROM auctionhouse WHERE itemowner<>%u AND buyguid<>%u", AHBplayerGUID, AHBplayerGUID);

    if (!result)
        return;

    if (result->GetRowCount() == 0)
        return;

    // Fetches content of selected AH
    AuctionHouseObject* auctionHouse =  sAuctionMgr->GetAuctionsMap(config->GetAHFID());
    vector<uint32> possibleBids;

    do
    {
        uint32 tmpdata = result->Fetch()->GetUInt32();
        possibleBids.push_back(tmpdata);
    }while (result->NextRow());

    for (uint32 count = 1; count <= config->GetBidsPerInterval(); ++count)
    {
        // Do we have anything to bid? If not, stop here.
        if (possibleBids.empty())
        {
            //if (debug_Out) sLog->outError( "AHBuyer: I have no items to bid on.");
            count = config->GetBidsPerInterval();
            continue;
        }

        // Choose random auction from possible auctions
        uint32 vectorPos = urand(0, possibleBids.size() - 1);
        vector<uint32>::iterator iter = possibleBids.begin();
        advance(iter, vectorPos);

        // from auctionhousehandler.cpp, creates auction pointer & player pointer
        AuctionEntry* auction = auctionHouse->GetAuction(*iter);

        // Erase the auction from the vector to prevent bidding on item in next iteration.
        possibleBids.erase(iter);

        if (!auction)
            continue;

        // get exact item information
		Item *pItem = sAuctionMgr->GetAItem(auction->item_guidlow);
        if (!pItem)
        {
			if (debug_Out) sLog->outError( "AHBuyer: Item %u doesn't exist, perhaps bought already?", auction->item_guidlow);
            continue;
        }

        // get item prototype
        ItemTemplate const* prototype = sObjectMgr->GetItemTemplate(auction->item_template);

        // check which price we have to use, startbid or if it is bidded already
        uint32 currentprice;
        if (auction->bid)
            currentprice = auction->bid;
        else
            currentprice = auction->startbid;

        // Prepare portion from maximum bid
        double bidrate = static_cast<double>(urand(1, 100)) / 100;
        long double bidMax = 0;

        // check that bid has acceptable value and take bid based on vendorprice, stacksize and quality
        if (BuyMethod)
        {
            if (prototype->Quality <= AHB_MAX_QUALITY)
            {
                if (currentprice < prototype->SellPrice * pItem->GetCount() * config->GetBuyerPrice(prototype->Quality))
                    bidMax = prototype->SellPrice * pItem->GetCount() * config->GetBuyerPrice(prototype->Quality);
            }
            else
            {
                // quality is something it shouldn't be, let's get out of here
                if (debug_Out) sLog->outError( "AHBuyer: Quality %u not Supported", prototype->Quality);
                    continue;
            }
        }
        else
        {
            if (prototype->Quality <= AHB_MAX_QUALITY)
            {
                if (currentprice < prototype->BuyPrice * pItem->GetCount() * config->GetBuyerPrice(prototype->Quality))
                    bidMax = prototype->BuyPrice * pItem->GetCount() * config->GetBuyerPrice(prototype->Quality);
            }
            else
            {
                // quality is something it shouldn't be, let's get out of here
                if (debug_Out) sLog->outError( "AHBuyer: Quality %u not Supported", prototype->Quality);
                    continue;
            }
        }        

        // check some special items, and do recalculating to their prices
        switch (prototype->Class)
        {
            // ammo
        case 6:
            bidMax = 0;
            break;
        default:
            break;
        }

        if (bidMax == 0)
        {
            // quality check failed to get bidmax, let's get out of here
            continue;
        }

        // Calculate our bid
        long double bidvalue = currentprice + ((bidMax - currentprice) * bidrate);
        // Convert to uint32
        uint32 bidprice = static_cast<uint32>(bidvalue);

        // Check our bid is high enough to be valid. If not, correct it to minimum.
        if ((currentprice + auction->GetAuctionOutBid()) > bidprice)
            bidprice = currentprice + auction->GetAuctionOutBid();

        if (debug_Out)
        {
            sLog->outString("-------------------------------------------------");
            sLog->outString("AHBuyer: Info for Auction #%u:", auction->Id);
            sLog->outString("AHBuyer: AuctionHouse: %u", auction->GetHouseId());
            sLog->outString("AHBuyer: Auctioneer: %u", auction->auctioneer);
            sLog->outString("AHBuyer: Owner: %u", auction->owner);
            sLog->outString("AHBuyer: Bidder: %u", auction->bidder);
            sLog->outString("AHBuyer: Starting Bid: %u", auction->startbid);
            sLog->outString("AHBuyer: Current Bid: %u", currentprice);
            sLog->outString("AHBuyer: Buyout: %u", auction->buyout);
            sLog->outString("AHBuyer: Deposit: %u", auction->deposit);
            sLog->outString("AHBuyer: Expire Time: %u", uint32(auction->expire_time));
            sLog->outString("AHBuyer: Bid Rate: %f", bidrate);
            sLog->outString("AHBuyer: Bid Max: %Lf", bidMax);
            sLog->outString("AHBuyer: Bid Value: %Lf", bidvalue);
            sLog->outString("AHBuyer: Bid Price: %u", bidprice);
            sLog->outString("AHBuyer: Item GUID: %u", auction->item_guidlow);
            sLog->outString("AHBuyer: Item Template: %u", auction->item_template);
            sLog->outString("AHBuyer: Item Info:");
            sLog->outString("AHBuyer: Item ID: %u", prototype->ItemId);
            sLog->outString("AHBuyer: Buy Price: %u", prototype->BuyPrice);
            sLog->outString("AHBuyer: Sell Price: %u", prototype->SellPrice);
            sLog->outString("AHBuyer: Bonding: %u", prototype->Bonding);
            sLog->outString("AHBuyer: Quality: %u", prototype->Quality);
            sLog->outString("AHBuyer: Item Level: %u", prototype->ItemLevel);
            sLog->outString("AHBuyer: Ammo Type: %u", prototype->AmmoType);
            sLog->outString("-------------------------------------------------");
        }

        // Check whether we do normal bid, or buyout
        if ((bidprice < auction->buyout) || (auction->buyout == 0))
        {

            if (auction->bidder > 0)
            {
                if (auction->bidder == AHBplayer->GetGUIDLow())
                {
                    //pl->ModifyMoney(-int32(price - auction->bid));
                }
                else
                {
                    // mail to last bidder and return money
                    SQLTransaction trans = CharacterDatabase.BeginTransaction();
                    sAuctionMgr->SendAuctionOutbiddedMail(auction , bidprice, session->GetPlayer(), trans);
                    CharacterDatabase.CommitTransaction(trans);
                    //pl->ModifyMoney(-int32(price));
                }
           }

            auction->bidder = AHBplayer->GetGUIDLow();
            auction->bid = bidprice;

            // Saving auction into database
            CharacterDatabase.PExecute("UPDATE auctionhouse SET buyguid = '%u',lastbid = '%u' WHERE id = '%u'", auction->bidder, auction->bid, auction->Id);
        }
        else
        {
            SQLTransaction trans = CharacterDatabase.BeginTransaction();
            //buyout
            if ((auction->bidder) && (AHBplayer->GetGUIDLow() != auction->bidder))
            {
                sAuctionMgr->SendAuctionOutbiddedMail(auction, auction->buyout, session->GetPlayer(), trans);
            }
            auction->bidder = AHBplayer->GetGUIDLow();
            auction->bid = auction->buyout;

            // Send mails to buyer & seller
            //sAuctionMgr->SendAuctionSalePendingMail(auction, trans);
            sAuctionMgr->SendAuctionSuccessfulMail(auction, trans);
            sAuctionMgr->SendAuctionWonMail(auction, trans);
            auction->DeleteFromDB( trans);

			sAuctionMgr->RemoveAItem(auction->item_guidlow);
            auctionHouse->RemoveAuction(auction);
            CharacterDatabase.CommitTransaction(trans);
        }
    }
}

void AuctionHouseBot::Update()
{
    time_t _newrun = time(NULL);
    if ((!AHBSeller) && (!AHBBuyer))
        return;

	WorldSession _session(AHBplayerAccount, NULL, SEC_PLAYER, sWorld->getIntConfig(CONFIG_EXPANSION), 0, LOCALE_zhCN,0,false,false,0);
    Player _AHBplayer(&_session);
    _AHBplayer.Initialize(AHBplayerGUID);
    sObjectAccessor->AddObject(&_AHBplayer);

    // Add New Bids
    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION))
    {
        addNewAuctions(&_AHBplayer, &AllianceConfig);
        if (((_newrun - _lastrun_a) >= (AllianceConfig.GetBiddingInterval() * MINUTE)) && (AllianceConfig.GetBidsPerInterval() > 0))
        {
            //if (debug_Out) sLog->outError( "AHBuyer: %u seconds have passed since last bid", (_newrun - _lastrun_a));
            //if (debug_Out) sLog->outError( "AHBuyer: Bidding on Alliance Auctions");
            addNewAuctionBuyerBotBid(&_AHBplayer, &AllianceConfig, &_session);
            _lastrun_a = _newrun;
        }

        addNewAuctions(&_AHBplayer, &HordeConfig);
        if (((_newrun - _lastrun_h) >= (HordeConfig.GetBiddingInterval() * MINUTE)) && (HordeConfig.GetBidsPerInterval() > 0))
        {
            //if (debug_Out) sLog->outError( "AHBuyer: %u seconds have passed since last bid", (_newrun - _lastrun_h));
            //if (debug_Out) sLog->outError( "AHBuyer: Bidding on Horde Auctions");
            addNewAuctionBuyerBotBid(&_AHBplayer, &HordeConfig, &_session);
            _lastrun_h = _newrun;
        }
    }

    addNewAuctions(&_AHBplayer, &NeutralConfig);
    if (((_newrun - _lastrun_n) >= (NeutralConfig.GetBiddingInterval() * MINUTE)) && (NeutralConfig.GetBidsPerInterval() > 0))
    {
        //if (debug_Out) sLog->outError( "AHBuyer: %u seconds have passed since last bid", (_newrun - _lastrun_n));
        //if (debug_Out) sLog->outError( "AHBuyer: Bidding on Neutral Auctions");
        addNewAuctionBuyerBotBid(&_AHBplayer, &NeutralConfig, &_session);
        _lastrun_n = _newrun;
    }
    sObjectAccessor->RemoveObject(&_AHBplayer);
}

void AuctionHouseBot::Initialize()
{
    DisableItemStore.clear();
    QueryResult result = WorldDatabase.PQuery("SELECT item FROM mod_auctionhousebot_disabled_items");

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            DisableItemStore.insert(fields[0].GetUInt32());
        } while (result->NextRow());
    }

    //End Filters
    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION))
    {
        LoadValues(&AllianceConfig);
        LoadValues(&HordeConfig);
    }
    LoadValues(&NeutralConfig);

    //
    // check if the AHBot account/GUID in the config actually exists
    //

    if ((AHBplayerAccount != 0) || (AHBplayerGUID != 0))
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT 1 FROM characters WHERE account = %u AND guid = %u", AHBplayerAccount, AHBplayerGUID);
        if (!result)
       {
           sLog->outError( "AuctionHouseBot: The account/GUID-information set for your AHBot is incorrect (account: %u guid: %u)", AHBplayerAccount, AHBplayerGUID);
           return;
        }
    }

    if (AHBSeller)
    {
        QueryResult results = QueryResult(NULL);

        ItemTemplateContainer const* its = sObjectMgr->GetItemTemplateStore();
        for (ItemTemplateContainer::const_iterator itr = its->begin(); itr != its->end(); ++itr)
        {
            switch (itr->second.ItemId)
            {
            case 36912: //Saronite Ore
            case 36909: //Cobalt Ore
            case 37700: 
            case 37701:
            case 37702:
            case 37703:
            case 37704:
            case 37705:
            case 36905:
            case 36907:
            case 36901:
            case 36903: //Addre's Tonue
            case 33568:
            case 33470:
            case 41806:
            case 41809:
            case 41813:
            case 43012:
            case 34736:
            case 43010:
            case 41810:
            case 41801:
            case 43009:
            case 43011:
            case 36904:
            case 36906:
                whiteTradeGoodsBin.push_back(itr->second.ItemId);
                break;
            }
        }

        if ((whiteTradeGoodsBin.size() == 0))
        {
            sLog->outError( "AuctionHouseBot: No items");
            AHBSeller = 0;
        }

        sLog->outString("AuctionHouseBot:");
        sLog->outString("%u disabled items", uint32(DisableItemStore.size()));
        sLog->outString("loaded %u white trade goods", uint32(whiteTradeGoodsBin.size()));
    }
    sLog->outString("AuctionHouseBot and AuctionHouseBuyer have been loaded.");
}

void AuctionHouseBot::InitializeConfiguration()
{
    debug_Out = sConfigMgr->GetBoolDefault("AuctionHouseBot.DEBUG", false);
    debug_Out_Filters = sConfigMgr->GetBoolDefault("AuctionHouseBot.DEBUG_FILTERS", false);

    //AHBSeller = sConfigMgr->GetBoolDefault("AuctionHouseBot.EnableSeller", false);
    AHBBuyer = sConfigMgr->GetBoolDefault("AuctionHouseBot.EnableBuyer", false);
    SellMethod = sConfigMgr->GetBoolDefault("AuctionHouseBot.UseBuyPriceForSeller", false);
    BuyMethod = sConfigMgr->GetBoolDefault("AuctionHouseBot.UseBuyPriceForBuyer", false);

    AHBplayerAccount = sConfigMgr->GetIntDefault("AuctionHouseBot.Account", 0);
    AHBplayerGUID = sConfigMgr->GetIntDefault("AuctionHouseBot.GUID", 0);
    ItemsPerCycle = sConfigMgr->GetIntDefault("AuctionHouseBot.ItemsPerCycle", 200);
}

void AuctionHouseBot::IncrementItemCounts(AuctionEntry* ah)
{
    // from auctionhousehandler.cpp, creates auction pointer & player pointer

    // get exact item information
    Item *pItem =  sAuctionMgr->GetAItem(ah->item_guidlow);
    if (!pItem)
    {
		if (debug_Out) sLog->outError( "AHBot: Item %u doesn't exist, perhaps bought already?", ah->item_guidlow);
        return;
    }

    // get item prototype
    ItemTemplate const* prototype = sObjectMgr->GetItemTemplate(ah->item_template);

    AHBConfig *config;

    FactionTemplateEntry const* u_entry = sFactionTemplateStore.LookupEntry(ah->GetHouseFaction());
    if (!u_entry)
    {
        if (debug_Out) sLog->outError( "AHBot: %u returned as House Faction. Neutral", ah->GetHouseFaction());
        config = &NeutralConfig;
    }
    else if (u_entry->ourMask & FACTION_MASK_ALLIANCE)
    {
        if (debug_Out) sLog->outError( "AHBot: %u returned as House Faction. Alliance", ah->GetHouseFaction());
        config = &AllianceConfig;
    }
    else if (u_entry->ourMask & FACTION_MASK_HORDE)
    {
        if (debug_Out) sLog->outError( "AHBot: %u returned as House Faction. Horde", ah->GetHouseFaction());
        config = &HordeConfig;
    }
    else
    {
        if (debug_Out) sLog->outError( "AHBot: %u returned as House Faction. Neutral", ah->GetHouseFaction());
        config = &NeutralConfig;
    }

    config->IncItemCounts(prototype->Class, prototype->Quality);
}

void AuctionHouseBot::DecrementItemCounts(AuctionEntry* ah, uint32 itemEntry)
{
    // get item prototype
    ItemTemplate const* prototype = sObjectMgr->GetItemTemplate(itemEntry);

    AHBConfig *config;

    FactionTemplateEntry const* u_entry = sFactionTemplateStore.LookupEntry(ah->GetHouseFaction());
    if (!u_entry)
    {
        if (debug_Out) sLog->outError( "AHBot: %u returned as House Faction. Neutral", ah->GetHouseFaction());
        config = &NeutralConfig;
    }
    else if (u_entry->ourMask & FACTION_MASK_ALLIANCE)
    {
        if (debug_Out) sLog->outError( "AHBot: %u returned as House Faction. Alliance", ah->GetHouseFaction());
        config = &AllianceConfig;
    }
    else if (u_entry->ourMask & FACTION_MASK_HORDE)
    {
        if (debug_Out) sLog->outError( "AHBot: %u returned as House Faction. Horde", ah->GetHouseFaction());
        config = &HordeConfig;
    }
    else
    {
        if (debug_Out) sLog->outError( "AHBot: %u returned as House Faction. Neutral", ah->GetHouseFaction());
        config = &NeutralConfig;
    }

    config->DecItemCounts(prototype->Class, prototype->Quality);
}

void AuctionHouseBot::Commands(uint32 command, uint32 ahMapID, uint32 col, char* args)
{
    AHBConfig *config = NULL;
    switch (ahMapID)
    {
    case 2:
        config = &AllianceConfig;
        break;
    case 6:
        config = &HordeConfig;
        break;
    case 7:
        config = &NeutralConfig;
        break;
    }
    std::string color;
    switch (col)
    {
    case AHB_GREY:
        color = "grey";
        break;
    case AHB_WHITE:
        color = "white";
        break;
    case AHB_GREEN:
        color = "green";
        break;
    case AHB_BLUE:
        color = "blue";
        break;
    case AHB_PURPLE:
        color = "purple";
        break;
    case AHB_ORANGE:
        color = "orange";
        break;
    case AHB_YELLOW:
        color = "yellow";
        break;
    default:
        break;
    }
    switch (command)
    {
    case 0:     //ahexpire
        {
            AuctionHouseObject* auctionHouse =  sAuctionMgr->GetAuctionsMap(config->GetAHFID());

            AuctionHouseObject::AuctionEntryMap::iterator itr;
            itr = auctionHouse->GetAuctionsBegin();

            while (itr != auctionHouse->GetAuctionsEnd())
            {
                if (itr->second->owner == AHBplayerGUID)
                {
                    itr->second->expire_time = sWorld->GetGameTime();
                    uint32 id = itr->second->Id;
                    uint32 expire_time = itr->second->expire_time;
                    CharacterDatabase.PExecute("UPDATE auctionhouse SET time = '%u' WHERE id = '%u'", expire_time, id);
                }
                ++itr;
            }
        }
        break;
    case 1:     //min items
        {
            char * param1 = strtok(args, " ");
            uint32 minItems = (uint32) strtoul(param1, NULL, 0);
            WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET minitems = '%u' WHERE auctionhouse = '%u'", minItems, ahMapID);
            config->SetMinItems(minItems);
        }
        break;
    case 2:     //max items
        {
            char * param1 = strtok(args, " ");
            uint32 maxItems = (uint32) strtoul(param1, NULL, 0);
			WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET maxitems = '%u' WHERE auctionhouse = '%u'", maxItems, ahMapID);
            config->SetMaxItems(maxItems);
            config->CalculatePercents();
        }
        break;
    case 3:     //min time Deprecated (Place holder for future commands)
        break;
    case 4:     //max time Deprecated (Place holder for future commands)
        break;
    case 5:     //percentages
        {
            char * param1 = strtok(args, " ");
            char * param2 = strtok(NULL, " ");
            char * param3 = strtok(NULL, " ");
            char * param4 = strtok(NULL, " ");
            char * param5 = strtok(NULL, " ");
            char * param6 = strtok(NULL, " ");
            char * param7 = strtok(NULL, " ");
            char * param8 = strtok(NULL, " ");
            char * param9 = strtok(NULL, " ");
            char * param10 = strtok(NULL, " ");
            char * param11 = strtok(NULL, " ");
            char * param12 = strtok(NULL, " ");
            char * param13 = strtok(NULL, " ");
            char * param14 = strtok(NULL, " ");
            uint32 greytg = (uint32) strtoul(param1, NULL, 0);
            uint32 whitetg = (uint32) strtoul(param2, NULL, 0);
            uint32 greentg = (uint32) strtoul(param3, NULL, 0);
            uint32 bluetg = (uint32) strtoul(param4, NULL, 0);
            uint32 purpletg = (uint32) strtoul(param5, NULL, 0);
            uint32 orangetg = (uint32) strtoul(param6, NULL, 0);
            uint32 yellowtg = (uint32) strtoul(param7, NULL, 0);
            uint32 greyi = (uint32) strtoul(param8, NULL, 0);
            uint32 whitei = (uint32) strtoul(param9, NULL, 0);
            uint32 greeni = (uint32) strtoul(param10, NULL, 0);
            uint32 bluei = (uint32) strtoul(param11, NULL, 0);
            uint32 purplei = (uint32) strtoul(param12, NULL, 0);
            uint32 orangei = (uint32) strtoul(param13, NULL, 0);
            uint32 yellowi = (uint32) strtoul(param14, NULL, 0);

			SQLTransaction trans = WorldDatabase.BeginTransaction();
            trans->PAppend("UPDATE mod_auctionhousebot SET percentgreytradegoods = '%u' WHERE auctionhouse = '%u'", greytg, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentwhitetradegoods = '%u' WHERE auctionhouse = '%u'", whitetg, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentgreentradegoods = '%u' WHERE auctionhouse = '%u'", greentg, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentbluetradegoods = '%u' WHERE auctionhouse = '%u'", bluetg, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentpurpletradegoods = '%u' WHERE auctionhouse = '%u'", purpletg, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentorangetradegoods = '%u' WHERE auctionhouse = '%u'", orangetg, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentyellowtradegoods = '%u' WHERE auctionhouse = '%u'", yellowtg, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentgreyitems = '%u' WHERE auctionhouse = '%u'", greyi, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentwhiteitems = '%u' WHERE auctionhouse = '%u'", whitei, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentgreenitems = '%u' WHERE auctionhouse = '%u'", greeni, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentblueitems = '%u' WHERE auctionhouse = '%u'", bluei, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentpurpleitems = '%u' WHERE auctionhouse = '%u'", purplei, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentorangeitems = '%u' WHERE auctionhouse = '%u'", orangei, ahMapID);
            trans->PAppend("UPDATE mod_auctionhousebot SET percentyellowitems = '%u' WHERE auctionhouse = '%u'", yellowi, ahMapID);
			WorldDatabase.CommitTransaction(trans);
            config->SetPercentages(greytg, whitetg, greentg, bluetg, purpletg, orangetg, yellowtg, greyi, whitei, greeni, bluei, purplei, orangei, yellowi);
        }
        break;
    case 6:     //min prices
        {
            char * param1 = strtok(args, " ");
            uint32 minPrice = (uint32) strtoul(param1, NULL, 0);
			WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET minprice%s = '%u' WHERE auctionhouse = '%u'", color.c_str(), minPrice, ahMapID);
            config->SetMinPrice(col, minPrice);
        }
        break;
    case 7:     //max prices
        {
            char * param1 = strtok(args, " ");
            uint32 maxPrice = (uint32) strtoul(param1, NULL, 0);
			WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET maxprice%s = '%u' WHERE auctionhouse = '%u'", color.c_str(), maxPrice, ahMapID);
            config->SetMaxPrice(col, maxPrice);
        }
        break;
    case 8:     //min bid price
        {
            char * param1 = strtok(args, " ");
            uint32 minBidPrice = (uint32) strtoul(param1, NULL, 0);
			WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET minbidprice%s = '%u' WHERE auctionhouse = '%u'", color.c_str(), minBidPrice, ahMapID);
            config->SetMinBidPrice(col, minBidPrice);
        }
        break;
    case 9:     //max bid price
        {
            char * param1 = strtok(args, " ");
            uint32 maxBidPrice = (uint32) strtoul(param1, NULL, 0);
			WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET maxbidprice%s = '%u' WHERE auctionhouse = '%u'", color.c_str(), maxBidPrice, ahMapID);
            config->SetMaxBidPrice(col, maxBidPrice);
        }
        break;
    case 10:        //max stacks
        {
            char * param1 = strtok(args, " ");
            uint32 maxStack = (uint32) strtoul(param1, NULL, 0);
			WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET maxstack%s = '%u' WHERE auctionhouse = '%u'", color.c_str(), maxStack, ahMapID);
            config->SetMaxStack(col, maxStack);
        }
        break;
    case 11:        //buyer bid prices
        {
            char * param1 = strtok(args, " ");
            uint32 buyerPrice = (uint32) strtoul(param1, NULL, 0);
			WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET buyerprice%s = '%u' WHERE auctionhouse = '%u'", color.c_str(), buyerPrice, ahMapID);
            config->SetBuyerPrice(col, buyerPrice);
        }
        break;
    case 12:        //buyer bidding interval
        {
            char * param1 = strtok(args, " ");
            uint32 bidInterval = (uint32) strtoul(param1, NULL, 0);
			WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET buyerbiddinginterval = '%u' WHERE auctionhouse = '%u'", bidInterval, ahMapID);
            config->SetBiddingInterval(bidInterval);
        }
        break;
    case 13:        //buyer bids per interval
        {
            char * param1 = strtok(args, " ");
            uint32 bidsPerInterval = (uint32) strtoul(param1, NULL, 0);
			WorldDatabase.PExecute("UPDATE mod_auctionhousebot SET buyerbidsperinterval = '%u' WHERE auctionhouse = '%u'", bidsPerInterval, ahMapID);
            config->SetBidsPerInterval(bidsPerInterval);
        }
        break;
    default:
        break;
    }
}

void AuctionHouseBot::LoadValues(AHBConfig *config)
{
    if (debug_Out)
		sLog->outError( "Start Settings for %s Auctionhouses:", WorldDatabase.PQuery("SELECT name FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetCString());
    if (AHBSeller)
    {
        //load min and max items
		config->SetMinItems(WorldDatabase.PQuery("SELECT minitems FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxItems(WorldDatabase.PQuery("SELECT maxitems FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
        //load percentages
		uint32 greytg = WorldDatabase.PQuery("SELECT percentgreytradegoods FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 whitetg = WorldDatabase.PQuery("SELECT percentwhitetradegoods FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 greentg = WorldDatabase.PQuery("SELECT percentgreentradegoods FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 bluetg = WorldDatabase.PQuery("SELECT percentbluetradegoods FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 purpletg = WorldDatabase.PQuery("SELECT percentpurpletradegoods FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 orangetg = WorldDatabase.PQuery("SELECT percentorangetradegoods FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 yellowtg = WorldDatabase.PQuery("SELECT percentyellowtradegoods FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 greyi = WorldDatabase.PQuery("SELECT percentgreyitems FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 whitei = WorldDatabase.PQuery("SELECT percentwhiteitems FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 greeni = WorldDatabase.PQuery("SELECT percentgreenitems FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 bluei = WorldDatabase.PQuery("SELECT percentblueitems FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 purplei = WorldDatabase.PQuery("SELECT percentpurpleitems FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 orangei = WorldDatabase.PQuery("SELECT percentorangeitems FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
		uint32 yellowi = WorldDatabase.PQuery("SELECT percentyellowitems FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32();
        config->SetPercentages(greytg, whitetg, greentg, bluetg, purpletg, orangetg, yellowtg, greyi, whitei, greeni, bluei, purplei, orangei, yellowi);
        //load min and max prices
		config->SetMinPrice(AHB_GREY, WorldDatabase.PQuery("SELECT minpricegrey FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxPrice(AHB_GREY, WorldDatabase.PQuery("SELECT maxpricegrey FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinPrice(AHB_WHITE, WorldDatabase.PQuery("SELECT minpricewhite FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxPrice(AHB_WHITE, WorldDatabase.PQuery("SELECT maxpricewhite FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinPrice(AHB_GREEN, WorldDatabase.PQuery("SELECT minpricegreen FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxPrice(AHB_GREEN, WorldDatabase.PQuery("SELECT maxpricegreen FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinPrice(AHB_BLUE, WorldDatabase.PQuery("SELECT minpriceblue FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxPrice(AHB_BLUE, WorldDatabase.PQuery("SELECT maxpriceblue FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinPrice(AHB_PURPLE, WorldDatabase.PQuery("SELECT minpricepurple FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxPrice(AHB_PURPLE, WorldDatabase.PQuery("SELECT maxpricepurple FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinPrice(AHB_ORANGE, WorldDatabase.PQuery("SELECT minpriceorange FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxPrice(AHB_ORANGE, WorldDatabase.PQuery("SELECT maxpriceorange FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinPrice(AHB_YELLOW, WorldDatabase.PQuery("SELECT minpriceyellow FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxPrice(AHB_YELLOW, WorldDatabase.PQuery("SELECT maxpriceyellow FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
        //load min and max bid prices
		config->SetMinBidPrice(AHB_GREY, WorldDatabase.PQuery("SELECT minbidpricegrey FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxBidPrice(AHB_GREY, WorldDatabase.PQuery("SELECT maxbidpricegrey FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinBidPrice(AHB_WHITE, WorldDatabase.PQuery("SELECT minbidpricewhite FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxBidPrice(AHB_WHITE, WorldDatabase.PQuery("SELECT maxbidpricewhite FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinBidPrice(AHB_GREEN, WorldDatabase.PQuery("SELECT minbidpricegreen FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxBidPrice(AHB_GREEN, WorldDatabase.PQuery("SELECT maxbidpricegreen FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinBidPrice(AHB_BLUE, WorldDatabase.PQuery("SELECT minbidpriceblue FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxBidPrice(AHB_BLUE, WorldDatabase.PQuery("SELECT maxbidpriceblue FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinBidPrice(AHB_PURPLE, WorldDatabase.PQuery("SELECT minbidpricepurple FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxBidPrice(AHB_PURPLE, WorldDatabase.PQuery("SELECT maxbidpricepurple FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinBidPrice(AHB_ORANGE, WorldDatabase.PQuery("SELECT minbidpriceorange FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxBidPrice(AHB_ORANGE, WorldDatabase.PQuery("SELECT maxbidpriceorange FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMinBidPrice(AHB_YELLOW, WorldDatabase.PQuery("SELECT minbidpriceyellow FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxBidPrice(AHB_YELLOW, WorldDatabase.PQuery("SELECT maxbidpriceyellow FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
        //load max stacks
		config->SetMaxStack(AHB_GREY, WorldDatabase.PQuery("SELECT maxstackgrey FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxStack(AHB_WHITE, WorldDatabase.PQuery("SELECT maxstackwhite FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxStack(AHB_GREEN, WorldDatabase.PQuery("SELECT maxstackgreen FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxStack(AHB_BLUE, WorldDatabase.PQuery("SELECT maxstackblue FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxStack(AHB_PURPLE, WorldDatabase.PQuery("SELECT maxstackpurple FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxStack(AHB_ORANGE, WorldDatabase.PQuery("SELECT maxstackorange FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetMaxStack(AHB_YELLOW, WorldDatabase.PQuery("SELECT maxstackyellow FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
        if (debug_Out)
        {
            sLog->outError( "minItems                = %u", config->GetMinItems());
            sLog->outError( "maxItems                = %u", config->GetMaxItems());
            sLog->outError( "percentGreyTradeGoods   = %u", config->GetPercentages(AHB_GREY_TG));
            sLog->outError( "percentWhiteTradeGoods  = %u", config->GetPercentages(AHB_WHITE_TG));
            sLog->outError( "percentGreenTradeGoods  = %u", config->GetPercentages(AHB_GREEN_TG));
            sLog->outError( "percentBlueTradeGoods   = %u", config->GetPercentages(AHB_BLUE_TG));
            sLog->outError( "percentPurpleTradeGoods = %u", config->GetPercentages(AHB_PURPLE_TG));
            sLog->outError( "percentOrangeTradeGoods = %u", config->GetPercentages(AHB_ORANGE_TG));
            sLog->outError( "percentYellowTradeGoods = %u", config->GetPercentages(AHB_YELLOW_TG));
            sLog->outError( "percentGreyItems        = %u", config->GetPercentages(AHB_GREY_I));
            sLog->outError( "percentWhiteItems       = %u", config->GetPercentages(AHB_WHITE_I));
            sLog->outError( "percentGreenItems       = %u", config->GetPercentages(AHB_GREEN_I));
            sLog->outError( "percentBlueItems        = %u", config->GetPercentages(AHB_BLUE_I));
            sLog->outError( "percentPurpleItems      = %u", config->GetPercentages(AHB_PURPLE_I));
            sLog->outError( "percentOrangeItems      = %u", config->GetPercentages(AHB_ORANGE_I));
            sLog->outError( "percentYellowItems      = %u", config->GetPercentages(AHB_YELLOW_I));
            sLog->outError( "minPriceGrey            = %u", config->GetMinPrice(AHB_GREY));
            sLog->outError( "maxPriceGrey            = %u", config->GetMaxPrice(AHB_GREY));
            sLog->outError( "minPriceWhite           = %u", config->GetMinPrice(AHB_WHITE));
            sLog->outError( "maxPriceWhite           = %u", config->GetMaxPrice(AHB_WHITE));
            sLog->outError( "minPriceGreen           = %u", config->GetMinPrice(AHB_GREEN));
            sLog->outError( "maxPriceGreen           = %u", config->GetMaxPrice(AHB_GREEN));
            sLog->outError( "minPriceBlue            = %u", config->GetMinPrice(AHB_BLUE));
            sLog->outError( "maxPriceBlue            = %u", config->GetMaxPrice(AHB_BLUE));
            sLog->outError( "minPricePurple          = %u", config->GetMinPrice(AHB_PURPLE));
            sLog->outError( "maxPricePurple          = %u", config->GetMaxPrice(AHB_PURPLE));
            sLog->outError( "minPriceOrange          = %u", config->GetMinPrice(AHB_ORANGE));
            sLog->outError( "maxPriceOrange          = %u", config->GetMaxPrice(AHB_ORANGE));
            sLog->outError( "minPriceYellow          = %u", config->GetMinPrice(AHB_YELLOW));
            sLog->outError( "maxPriceYellow          = %u", config->GetMaxPrice(AHB_YELLOW));
            sLog->outError( "minBidPriceGrey         = %u", config->GetMinBidPrice(AHB_GREY));
            sLog->outError( "maxBidPriceGrey         = %u", config->GetMaxBidPrice(AHB_GREY));
            sLog->outError( "minBidPriceWhite        = %u", config->GetMinBidPrice(AHB_WHITE));
            sLog->outError( "maxBidPriceWhite        = %u", config->GetMaxBidPrice(AHB_WHITE));
            sLog->outError( "minBidPriceGreen        = %u", config->GetMinBidPrice(AHB_GREEN));
            sLog->outError( "maxBidPriceGreen        = %u", config->GetMaxBidPrice(AHB_GREEN));
            sLog->outError( "minBidPriceBlue         = %u", config->GetMinBidPrice(AHB_BLUE));
            sLog->outError( "maxBidPriceBlue         = %u", config->GetMinBidPrice(AHB_BLUE));
            sLog->outError( "minBidPricePurple       = %u", config->GetMinBidPrice(AHB_PURPLE));
            sLog->outError( "maxBidPricePurple       = %u", config->GetMaxBidPrice(AHB_PURPLE));
            sLog->outError( "minBidPriceOrange       = %u", config->GetMinBidPrice(AHB_ORANGE));
            sLog->outError( "maxBidPriceOrange       = %u", config->GetMaxBidPrice(AHB_ORANGE));
            sLog->outError( "minBidPriceYellow       = %u", config->GetMinBidPrice(AHB_YELLOW));
            sLog->outError( "maxBidPriceYellow       = %u", config->GetMaxBidPrice(AHB_YELLOW));
            sLog->outError( "maxStackGrey            = %u", config->GetMaxStack(AHB_GREY));
            sLog->outError( "maxStackWhite           = %u", config->GetMaxStack(AHB_WHITE));
            sLog->outError( "maxStackGreen           = %u", config->GetMaxStack(AHB_GREEN));
            sLog->outError( "maxStackBlue            = %u", config->GetMaxStack(AHB_BLUE));
            sLog->outError( "maxStackPurple          = %u", config->GetMaxStack(AHB_PURPLE));
            sLog->outError( "maxStackOrange          = %u", config->GetMaxStack(AHB_ORANGE));
            sLog->outError( "maxStackYellow          = %u", config->GetMaxStack(AHB_YELLOW));
        }
        //AuctionHouseEntry const* ahEntry =  sAuctionMgr->GetAuctionHouseEntry(config->GetAHFID());
        AuctionHouseObject* auctionHouse =  sAuctionMgr->GetAuctionsMap(config->GetAHFID());

        config->ResetItemCounts();
        uint32 auctions = auctionHouse->Getcount();

        if (auctions)
        {
            for (AuctionHouseObject::AuctionEntryMap::const_iterator itr = auctionHouse->GetAuctionsBegin(); itr != auctionHouse->GetAuctionsEnd(); ++itr)
            {
                AuctionEntry *Aentry = itr->second;
				Item *item = sAuctionMgr->GetAItem(Aentry->item_guidlow);
                if (item)
                {
                    ItemTemplate const *prototype = item->GetTemplate();
                    if (prototype)
                    {
                        switch (prototype->Quality)
                        {
                        case 0:
                            if (prototype->Class == ITEM_CLASS_TRADE_GOODS)
                                config->IncItemCounts(AHB_GREY_TG);
                            else
                                config->IncItemCounts(AHB_GREY_I);
                            break;
                        case 1:
                            if (prototype->Class == ITEM_CLASS_TRADE_GOODS)
                                config->IncItemCounts(AHB_WHITE_TG);
                            else
                                config->IncItemCounts(AHB_WHITE_I);
                            break;
                        case 2:
                            if (prototype->Class == ITEM_CLASS_TRADE_GOODS)
                                config->IncItemCounts(AHB_GREEN_TG);
                            else
                                config->IncItemCounts(AHB_GREEN_I);
                            break;
                        case 3:
                            if (prototype->Class == ITEM_CLASS_TRADE_GOODS)
                                config->IncItemCounts(AHB_BLUE_TG);
                            else
                                config->IncItemCounts(AHB_BLUE_I);
                            break;
                        case 4:
                            if (prototype->Class == ITEM_CLASS_TRADE_GOODS)
                                config->IncItemCounts(AHB_PURPLE_TG);
                            else
                                config->IncItemCounts(AHB_PURPLE_I);
                            break;
                        case 5:
                            if (prototype->Class == ITEM_CLASS_TRADE_GOODS)
                                config->IncItemCounts(AHB_ORANGE_TG);
                            else
                                config->IncItemCounts(AHB_ORANGE_I);
                            break;
                        case 6:
                            if (prototype->Class == ITEM_CLASS_TRADE_GOODS)
                                config->IncItemCounts(AHB_YELLOW_TG);
                            else
                                config->IncItemCounts(AHB_YELLOW_I);
                            break;
                        }
                    }
                }
            }
        }
        if (debug_Out)
        {
			sLog->outError( "Current Settings for %s Auctionhouses:", WorldDatabase.PQuery("SELECT name FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetCString());
            sLog->outError( "Grey Trade Goods\t%u\tGrey Items\t%u", config->GetItemCounts(AHB_GREY_TG), config->GetItemCounts(AHB_GREY_I));
            sLog->outError( "White Trade Goods\t%u\tWhite Items\t%u", config->GetItemCounts(AHB_WHITE_TG), config->GetItemCounts(AHB_WHITE_I));
            sLog->outError( "Green Trade Goods\t%u\tGreen Items\t%u", config->GetItemCounts(AHB_GREEN_TG), config->GetItemCounts(AHB_GREEN_I));
            sLog->outError( "Blue Trade Goods\t%u\tBlue Items\t%u", config->GetItemCounts(AHB_BLUE_TG), config->GetItemCounts(AHB_BLUE_I));
            sLog->outError( "Purple Trade Goods\t%u\tPurple Items\t%u", config->GetItemCounts(AHB_PURPLE_TG), config->GetItemCounts(AHB_PURPLE_I));
            sLog->outError( "Orange Trade Goods\t%u\tOrange Items\t%u", config->GetItemCounts(AHB_ORANGE_TG), config->GetItemCounts(AHB_ORANGE_I));
            sLog->outError( "Yellow Trade Goods\t%u\tYellow Items\t%u", config->GetItemCounts(AHB_YELLOW_TG), config->GetItemCounts(AHB_YELLOW_I));
        }
    }
    if (AHBBuyer)
    {
        //load buyer bid prices
		config->SetBuyerPrice(AHB_GREY, WorldDatabase.PQuery("SELECT buyerpricegrey FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetBuyerPrice(AHB_WHITE, WorldDatabase.PQuery("SELECT buyerpricewhite FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetBuyerPrice(AHB_GREEN, WorldDatabase.PQuery("SELECT buyerpricegreen FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetBuyerPrice(AHB_BLUE, WorldDatabase.PQuery("SELECT buyerpriceblue FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetBuyerPrice(AHB_PURPLE, WorldDatabase.PQuery("SELECT buyerpricepurple FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetBuyerPrice(AHB_ORANGE, WorldDatabase.PQuery("SELECT buyerpriceorange FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
		config->SetBuyerPrice(AHB_YELLOW, WorldDatabase.PQuery("SELECT buyerpriceyellow FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
        //load bidding interval
		config->SetBiddingInterval(WorldDatabase.PQuery("SELECT buyerbiddinginterval FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
        //load bids per interval
		config->SetBidsPerInterval(WorldDatabase.PQuery("SELECT buyerbidsperinterval FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetUInt32());
        if (debug_Out)
        {
            sLog->outError( "buyerPriceGrey          = %u", config->GetBuyerPrice(AHB_GREY));
            sLog->outError( "buyerPriceWhite         = %u", config->GetBuyerPrice(AHB_WHITE));
            sLog->outError( "buyerPriceGreen         = %u", config->GetBuyerPrice(AHB_GREEN));
            sLog->outError( "buyerPriceBlue          = %u", config->GetBuyerPrice(AHB_BLUE));
            sLog->outError( "buyerPricePurple        = %u", config->GetBuyerPrice(AHB_PURPLE));
            sLog->outError( "buyerPriceOrange        = %u", config->GetBuyerPrice(AHB_ORANGE));
            sLog->outError( "buyerPriceYellow        = %u", config->GetBuyerPrice(AHB_YELLOW));
            sLog->outError( "buyerBiddingInterval    = %u", config->GetBiddingInterval());
            sLog->outError( "buyerBidsPerInterval    = %u", config->GetBidsPerInterval());
        }
    }
	if (debug_Out) sLog->outError( "End Settings for %s Auctionhouses:", WorldDatabase.PQuery("SELECT name FROM mod_auctionhousebot WHERE auctionhouse = %u", config->GetAHID())->Fetch()->GetCString());
}
