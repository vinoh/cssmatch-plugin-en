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
		gameeventmanager2->AddListener(this,"item_pickup",true);

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

		if (eventName == "round_start")
			round_start(event);
		if (eventName == "item_pickup")
			item_pickup(event);
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
				print(__FILE__,__LINE__,"Unable to find the player wich pickup an item");
		}
	}
}
