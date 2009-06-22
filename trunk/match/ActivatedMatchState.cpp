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

#include "ActivatedMatchState.h"

#include "MatchManager.h"
#include "../messages/I18nManager.h"
#include "../plugin/SimplePlugin.h"
#include "../player/ClanMember.h"

#include "igameevents.h" // IGameEventManager2, IGameEvent

#include <string>
#include <algorithm>

using namespace cssmatch;

ActivatedMatchState::ActivatedMatchState(MatchManager * match, IGameEventManager2 * eventManager)
	: BaseMatchState(match,eventManager)
{
}

void ActivatedMatchState::startState()
{
	// Register needed events
	gameeventmanager2->AddListener(this,"player_say",true);

}

void ActivatedMatchState::endState()
{
	gameeventmanager2->RemoveListener(this);
}

void ActivatedMatchState::FireGameEvent(IGameEvent * event)
{
	std::string eventName = event->GetName();

	/*if (eventName == "player_say")
		player_say(event);*/
}

void ActivatedMatchState::player_say(IGameEvent * event)
{
	SimplePlugin * plugin = SimplePlugin::getInstance();
	//ValveInterfaces * interfaces = plugin->getInterfaces();
	I18nManager * i18n = plugin->get18nManager();

	std::map<std::string, std::string> parameters;
	RecipientFilter recipients;
	std::list<ClanMember *> * playerlist = plugin->getPlayerlist();
	std::for_each(playerlist->begin(),playerlist->end(),PlayerToRecipient(&recipients));

	std::string command = event->GetString("text");

	if (command == "!score" || command == "!scores")
	{
		
	}

	else if (command == "!go" || command == "ready")
	{
		i18n->chatSay(recipients,"warmup_disable");
	}
	// Prob : player_say de warmup va appeler ce code...
}
