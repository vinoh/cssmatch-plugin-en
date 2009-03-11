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

#include "MatchManager.h"

#include "DisableMatchState.h"
#include "KnifeRoundMatchState.h"
#include "WarmupMatchState.h"
#include "SetMatchState.h"

#include "../plugin/SimplePlugin.h"
#include "../messages/Countdown.h"

#include <algorithm>

namespace cssmatch
{
	MatchLignup::MatchLignup() : clan1(T_TEAM), clan2(CT_TEAM)
	{
	}

	MatchInfo::MatchInfo() : setNumber(1), roundNumber(1), startTime(getLocalTime())
	{
	}

	MatchManager::MatchManager()
	{
		SimplePlugin * plugin = SimplePlugin::getInstance();
		ValveInterfaces * interfaces = plugin->getInterfaces();

		state = new DisableMatchState(this,interfaces->gameeventmanager2);
		state->startState();
	}


	MatchManager::~MatchManager()
	{
		delete state;

		Countdown::getInstance()->stop();
	}

	MatchLignup * MatchManager::getLignup()
	{
		return &lignup;
	}

	MatchInfo * MatchManager::getInfos()
	{
		return &infos;
	}

	void MatchManager::setMatchState(BaseMatchState * newState)
	{
		state->endState();
		delete state;

		state = newState;
		state->startState();
	}

	void MatchManager::start(RunnableConfigurationFile & config, bool kniferound, ClanMember * umpire)
	{
		SimplePlugin * plugin = SimplePlugin::getInstance();
		ValveInterfaces * interfaces = plugin->getInterfaces();
		I18nManager * i18n = plugin->get18nManager();

		// Global recipient list
		std::list<ClanMember *> * playerlist = plugin->getPlayerlist();
		RecipientFilter recipients;
		std::for_each(playerlist->begin(),playerlist->end(),PlayerToRecipient(&recipients));

		// Update match infos
		infos.setNumber = 1;
		infos.roundNumber = 1;

		// Annulation de d'éventuel timers en court
		plugin->removeTimers();

		// Execute the configuration file
		config.execute();

		// Print the plugin list to the server log
		plugin->queueCommand("plugin_print\n");

		// Save the current date
		infos.startTime = getLocalTime();

		// Try to find the clan names
		lignup.clan1.detectClanName();
		lignup.clan2.detectClanName();

		// Set the new server password
		try
		{
			std::string password = plugin->getConVar("cssmatch_password")->GetString();
			plugin->getConVar("sv_password")->SetValue(password.c_str());

			std::map<std::string, std::string> parameters;
			parameters["$password"] = password;
			plugin->addTimer(
				new TimerI18nPopupSay(
					i18n,interfaces->gpGlobals->curtime+5.0f,recipients,"match_password_popup",5,OPTION_ALL,parameters));

			// Set the suitable match state
			if (plugin->getConVar("cssmatch_kniferound")->GetBool() && kniferound)
			{
				this->setMatchState(new KnifeRoundMatchState(this,interfaces->gameeventmanager2));
			}
			else if (plugin->getConVar("cssmatch_warmup_time")->GetInt() > 0)
			{
				this->setMatchState(new WarmupMatchState(this,interfaces->gameeventmanager2));
			}
			else if (plugin->getConVar("cssmatch_sets")->GetInt() > 0)
			{
				this->setMatchState(new SetMatchState(this,interfaces->gameeventmanager2));
			}
			else // Error case
			{
				i18n->i18nChatWarning(recipients,"match_config_error");
			}
		}
		catch(const BaseConvarsAccessorException & e)
		{
			plugin->printException(e,__FILE__,__LINE__);
		}

		// Announcement
		if (umpire != NULL)
		{
			std::map<std::string,std::string> parameters;
			parameters["$admin"] = umpire->getPlayerInfo()->GetName();
			i18n->i18nChatSay(recipients,"match_started_by",umpire->getIdentity()->index,parameters);
		}
		else
			i18n->i18nChatSay(recipients,"match_started");
	}
}
