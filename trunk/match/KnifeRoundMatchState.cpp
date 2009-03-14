/* 
 * Copyright 2008, 2009 Nicolas Maingot
 * 
 * This file is part of CSSMatch.
 * 
 * CSSMatch is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * CSSMatch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with CSSMatch; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Portions of this code are also Copyright © 1996-2005 Valve Corporation, All rights reserved
 */

#include "KnifeRoundMatchState.h"
#include "../plugin/SimplePlugin.h"

#include "WarmupMatchState.h"
#include "SetMatchState.h"

#include <algorithm>

namespace cssmatch
{
	KnifeRoundMatchState::KnifeRoundMatchState(MatchManager * match, IGameEventManager2 * eventManager) 
		: BaseMatchState(match,eventManager)
	{
	}

	void KnifeRoundMatchState::startState()
	{
		SimplePlugin * plugin = SimplePlugin::getInstance();
		I18nManager * i18n = plugin->get18nManager();

		// Global recipient list
		std::list<ClanMember *> * playerlist = plugin->getPlayerlist();
		RecipientFilter recipients;
		std::for_each(playerlist->begin(),playerlist->end(),PlayerToRecipient(&recipients));

		// Register needed events
		gameeventmanager2->AddListener(this,"round_start",true);
		gameeventmanager2->AddListener(this,"round_end",true);
		gameeventmanager2->AddListener(this,"item_pickup",true);
		gameeventmanager2->AddListener(this,"player_spawn",true);
		gameeventmanager2->AddListener(this,"bomb_beginplant",true);

		i18n->i18nChatSay(recipients,"kniferound_restarts");

		// Round number will indicates if some restarts have to be executed
		match->getInfos()->roundNumber = 0;

		// First restart
		plugin->queueCommand("mp_restartgame 2\n");
	}

	void KnifeRoundMatchState::endState()
	{
		// Remove all registered events
		gameeventmanager2->RemoveListener(this);
	}

	void KnifeRoundMatchState::FireGameEvent(IGameEvent * event)
	{
		std::string eventName = event->GetName();

		if (eventName == "item_pickup")
			item_pickup(event);
		else if (eventName == "player_spawn")
			player_spawn(event);
		else if (eventName == "round_start")
			round_start(event);
		else if (eventName == "bomb_beginplant")
			round_end(event);
		else if (eventName == "round_end")
			round_end(event);
	}

	void KnifeRoundMatchState::round_start(IGameEvent * event)
	{
		SimplePlugin * plugin = SimplePlugin::getInstance();
		I18nManager * i18n = plugin->get18nManager();

		switch(match->getInfos()->roundNumber++)
		{
		case 1:
			plugin->queueCommand("mp_restartgame 1\n");
			break;
		case 2:
			plugin->queueCommand("mp_restartgame 2\n");
			break;
		default:
			// Global recipient list
			std::list<ClanMember *> * playerlist = plugin->getPlayerlist();
			RecipientFilter recipients;
			std::for_each(playerlist->begin(),playerlist->end(),PlayerToRecipient(&recipients));
			
			i18n->i18nChatSay(recipients,"kniferound_announcement");
		}
	}

	void KnifeRoundMatchState::item_pickup(IGameEvent * event)
	{
		SimplePlugin * plugin = SimplePlugin::getInstance();

		std::string item = event->GetString("item");

		if (item != "knife")
		{
			std::list<ClanMember *> * playerlist = plugin->getPlayerlist();
			std::list<ClanMember *>::iterator invalidPlayer = playerlist->end();

			std::list<ClanMember *>::iterator itPlayer = 
				std::find_if(playerlist->begin(),invalidPlayer,PlayerHavingUserid(event->GetInt("userid")));
			if (itPlayer != invalidPlayer)
			{
				(*itPlayer)->removeWeapon(WEAPON_SLOT1);
				(*itPlayer)->removeWeapon(WEAPON_SLOT2);
				(*itPlayer)->removeWeapon(WEAPON_SLOT4);
			}
			else
				print(__FILE__,__LINE__,"Unable to find the player wich pickups an item");
		}
	}

	void KnifeRoundMatchState::player_spawn(IGameEvent * event)
	{
		SimplePlugin * plugin = SimplePlugin::getInstance();

		std::list<ClanMember *> * playerlist = plugin->getPlayerlist();
		std::list<ClanMember *>::iterator invalidPlayer = playerlist->end();

		std::list<ClanMember *>::iterator itPlayer = 
			std::find_if(playerlist->begin(),invalidPlayer,PlayerHavingUserid(event->GetInt("userid")));
		if (itPlayer != invalidPlayer)
		{
			try
			{
				(*itPlayer)->setCash(plugin->getConVar("cssmatch_kniferound_money")->GetInt());
			}
			catch(const BaseConvarsAccessorException & e)
			{
				plugin->printException(e,__FILE__,__LINE__);
			}
		}
	}

	void KnifeRoundMatchState::round_end(IGameEvent * event)
	{
		SimplePlugin * plugin = SimplePlugin::getInstance();
		ValveInterfaces * interfaces = plugin->getInterfaces();
		I18nManager * i18n = plugin->get18nManager();

		TeamCode winner = (TeamCode)event->GetInt("winner");
		if (winner > SPEC_TEAM)
		{
			try
			{
				if (plugin->getConVar("cssmatch_warmup_time")->GetInt() > 0)
				{
					match->setMatchState(new WarmupMatchState(match,interfaces->gameeventmanager2));
				}
				else if (plugin->getConVar("cssmatch_sets")->GetInt() > 0)
				{
					match->setMatchState(new SetMatchState(match,interfaces->gameeventmanager2));
				}
				else // Error case
				{
					std::list<ClanMember *> * playerlist = plugin->getPlayerlist();
					RecipientFilter recipients;
					std::for_each(playerlist->begin(),playerlist->end(),PlayerToRecipient(&recipients));

					i18n->i18nChatWarning(recipients,"match_config_error");
				}
			}
			catch(const BaseConvarsAccessorException & e)
			{
				plugin->printException(e,__FILE__,__LINE__);
			}
		}
	}

	void KnifeRoundMatchState::bomb_beginplant(IGameEvent * event)
	{
		SimplePlugin * plugin = SimplePlugin::getInstance();
		ValveInterfaces * interfaces = plugin->getInterfaces();
		I18nManager * i18n = plugin->get18nManager();

		try
		{
			if (plugin->getConVar("cssmatch_kniferound_allows_c4")->GetBool())
			{
				std::list<ClanMember *> * playerlist = plugin->getPlayerlist();
				std::list<ClanMember *>::iterator invalidPlayer = playerlist->end();

				std::list<ClanMember *>::iterator itPlayer = 
					std::find_if(playerlist->begin(),invalidPlayer,PlayerHavingUserid(event->GetInt("userid")));
				if (itPlayer != invalidPlayer)
				{
					PlayerIdentity * identity = (*itPlayer)->getIdentity();

					interfaces->helpers->ClientCommand(identity->pEntity,"use weapon_knife");

					RecipientFilter recipients;
					recipients.addRecipient(identity->index);
					i18n->i18nChatSay(recipients,"kniferound_c4");
				}
				else
					print(__FILE__,__LINE__,"Unable to find the player wich plants the bomb");
			}
		}
		catch(const BaseConvarsAccessorException & e)
		{
			plugin->printException(e,__FILE__,__LINE__);
		}
	}
}
