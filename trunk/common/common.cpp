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

#include "common.h"

#include "interface.h"

/**
 * @see http://developer.valvesoftware.com/wiki/KeyValues_class#Important_Notes
 */
#include "filesystem.h"
#include "engine/iserverplugin.h"
#include "dlls/iplayerinfo.h"
#include "eiface.h"
#include "igameevents.h"
#include "convar.h"
#include "icvar.h"
#include "bitbuf.h"
#include "baseentity.h"

#include <sstream>
#include <time.h>

#include "../plugin/SimplePlugin.h"

using namespace cssmatch;

tm * cssmatch::getLocalTime()
{
	time_t date = time(NULL);
	return gmtime(&date);
}

void cssmatch::print(const std::string & fileName, int line, const std::string & message)
{
	std::ostringstream buffer;
	buffer << PLUGIN_NAME << " (" << fileName << " l." << line << ") : " << message << "\n";
	//Msg(buffer.str().c_str());
	SimplePlugin::getInstance()->log(buffer.str());
}

void cssmatch::printException(const BaseException & e, const std::string & fileName, int line)
{
	std::ostringstream buffer;
	buffer << e.what() << " (" << fileName << ", l." << line << ")";
	SimplePlugin::getInstance()->log(buffer.str());
}

bool cssmatch::isValidEntity(edict_t * entity)
{
	return (entity != NULL) && (! entity->IsFree());
}

bool cssmatch::isValidPlayer(IPlayerInfo * pInfo)
{
	return (pInfo != NULL) && pInfo->IsConnected() && pInfo->IsPlayer() && (! pInfo->IsHLTV());
}

bool cssmatch::isValidPlayerIndex(int index, int maxClients)
{
	return (index > INVALID_ENTITY_INDEX) && index <= maxClients;
}

bool cssmatch::isValidPlayerUserid(int userid)
{
	return userid > INVALID_PLAYER_USERID;
}

bool cssmatch::isValidServerEntity(IServerEntity * sEntity)
{
	return sEntity != NULL;
}

bool cssmatch::isValidBaseEntity(CBaseEntity * bEntity)
{
	return bEntity != NULL;
}

bool cssmatch::isValidServerUnknown(IServerUnknown * sUnknown)
{
	return sUnknown != NULL;
}

bool cssmatch::isValidBasePlayer(CBasePlayer * bPlayer)
{
	return bPlayer != NULL;
}

bool cssmatch::isValidBaseCombatCharacter(CBaseCombatCharacter * bCombatCharacter)
{
	return bCombatCharacter != NULL;
}

IServerEntity * cssmatch::getServerEntity(edict_t * entity)
{
	IServerEntity * sEntity = entity->GetIServerEntity();

	if (! isValidServerEntity(sEntity))
	{
		print(__FILE__,__LINE__,"The plugin was unable to find the server entity of an entity");
		sEntity = NULL;
	}

	return sEntity;
}

CBaseEntity * cssmatch::getBaseEntity(edict_t * entity)
{
	CBaseEntity * bEntity = NULL;
	IServerEntity * sEntity = getServerEntity(entity);

	if (isValidServerEntity(sEntity))
		bEntity = sEntity->GetBaseEntity();

	if (! isValidBaseEntity(bEntity))
	{
		print(__FILE__,__LINE__,"The plugin was unable to find the base entity of an entity");
		bEntity = NULL;
	}

	return bEntity;
}

IServerUnknown * cssmatch::getServerUnknow(edict_t * entity)
{
	IServerUnknown * sUnknown = entity->GetUnknown();

	if (! isValidServerUnknown(sUnknown))
	{
		print(__FILE__,__LINE__,"The plugin was unable to find the server unknown pointer of an entity");
		sUnknown = NULL;
	}

	return sUnknown;
}
