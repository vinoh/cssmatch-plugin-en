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

#include "ConCommandCallbacks.h"

#include "../match/DisabledMatchState.h"
#include "../match/MatchManager.h"
#include "../plugin/SimplePlugin.h"
#include "../configuration/RunnableConfigurationFile.h"

using namespace cssmatch;

void cssmatch::cssm_help()
{
	SimplePlugin * plugin = SimplePlugin::getInstance();

	const std::list<ConCommand *> * pluginConCommands = plugin->getPluginConCommands();
	std::list<ConCommand *>::const_iterator itConCommand = pluginConCommands->begin();
	std::list<ConCommand *>::const_iterator lastConCommand = pluginConCommands->end();
	while (itConCommand != lastConCommand)
	{
		ConCommand * command = *itConCommand;
		plugin->log(std::string(command->GetName()) + " : " + command->GetHelpText());

		itConCommand++;
	}
}

void cssmatch::cssm_start()
{
	SimplePlugin * plugin = SimplePlugin::getInstance();
	ValveInterfaces * interfaces = plugin->getInterfaces();
	MatchManager * match = plugin->getMatch();

	bool kniferound = true;
	std::string configurationFile = DEFAULT_CONFIGURATION_FILE;
	switch(interfaces->engine->Cmd_Argc())
	{
	case 3:
		kniferound = std::string(interfaces->engine->Cmd_Argv(2)) != "-cutround";
		// break;
	case 2:
		configurationFile = interfaces->engine->Cmd_Argv(1);
		// break;
	case 1:
		match->start(RunnableConfigurationFile(CFG_FOLDER_PATH MATCH_CONFIGURATIONS_PATH + configurationFile,interfaces->convars->getConVarAccessor(),interfaces->engine),kniferound);
		break;
	default:
		plugin->log(std::string(interfaces->engine->Cmd_Argv(0)) + " [configuration file from cstrike/cfg/cssmatch/configurations] [-cutround]"); 
	}
}

void cssmatch::cssm_stop()
{
	SimplePlugin * plugin = SimplePlugin::getInstance();
	MatchManager * match = plugin->getMatch();
	ValveInterfaces * interfaces = plugin->getInterfaces();

	match->setMatchState(new DisabledMatchState(match));
}
