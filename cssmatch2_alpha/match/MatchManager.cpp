/* 
 * Copyright 2007, 2008 Nicolas Maingot
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
 * Portions of this code are also Copyright � 1996-2005 Valve Corporation, All rights reserved
 */

#include "MatchManager.h"
#include "../plugin/SimplePlugin.h"

namespace cssmatch
{
	MatchLignup::MatchLignup(SimplePlugin * plugin) : clan1(plugin,T_TEAM), clan2(plugin,CT_TEAM)
	{
	}

	MatchManager::MatchManager(SimplePlugin * pluginInstance) : plugin(pluginInstance), lignup(pluginInstance)
	{
		ValveInterfaces * interfaces = plugin->getInterfaces();

		state = new DisableMatchState(this,interfaces->gameeventmanager2);
		state->startState();
	}

	MatchManager::~MatchManager()
	{
		delete state;
	}

	MatchLignup * MatchManager::getLignup()
	{
		return &lignup;
	}

	void MatchManager::setMatchState(BaseMatchState * newState)
	{
		state->endState();
		delete state;

		state = newState;
		state->startState();
	}
}
