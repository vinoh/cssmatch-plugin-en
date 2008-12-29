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

#include "common.h"

namespace cssmatch
{
	void print(const std::string & message)
	{
		std::ostringstream buffer;
		buffer << PLUGIN_NAME << " : " << message << "\n";
		Msg(buffer.str().c_str());
	}

	bool isValidEntity(edict_t * entity)
	{
		return (entity != NULL) && (! entity->IsFree());
	}

	bool isValidPlayer(IPlayerInfo * pInfo)
	{
		return (pInfo != NULL) && pInfo->IsConnected() && pInfo->IsPlayer() && (! pInfo->IsHLTV());
	}

	bool isValidPlayerIndex(int index, int maxClients)
	{
		return (index > INVALID_ENTITY_INDEX) && index <= maxClients;
	}

	bool isValidPlayerUserid(int userid)
	{
		return userid > INVALID_PLAYER_USERID;
	}

	bool isValidServerEntity(IServerEntity * sEntity)
	{
		return sEntity != NULL;
	}

	bool isValidBaseEntity(CBaseEntity * bEntity)
	{
		return bEntity != NULL;
	}

	bool isValidServerUnknown(IServerUnknown * sUnknown)
	{
		return sUnknown != NULL;
	}

	bool isValidBasePlayer(CBasePlayer * bPlayer)
	{
		return bPlayer != NULL;
	}

	bool isValidBaseCombatCharacter(CBaseCombatCharacter * bCombatCharacter)
	{
		return bCombatCharacter != NULL;
	}
}