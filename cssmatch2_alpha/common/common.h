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
 * Portions of this code are also Copyright © 1996-2005 Valve Corporation, All rights reserved
 */

#ifndef __COMMON_H__
#define __COMMON_H__

// Visual C++ 2005 does not support the exception specification in the function declarations
#pragma warning(disable : 4290)

// Local variable not used (typically in the catch blocks)
#pragma warning(disable :4101)

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

#define GAME_DLL 1 // required by cbase.h
#include "cbase.h"
#include "baseentity.h"

#include <string>

#define PLUGIN_NAME "CSSMatch"

#define PLUGIN_VERSION_LIGHT "2.0.0 ALPHA"

#define PLUGIN_SITE "http://code.google.com/p/cssmatch-plugin-en/"

#define PLUGIN_VERSION "Nico's " \
		PLUGIN_NAME ", version " \
		PLUGIN_VERSION_LIGHT " (" __DATE__ "), " \
		PLUGIN_SITE

/** Entity's invalid index */
#define INVALID_ENTITY_INDEX 0

/** Entity's invalid userid */
#define INVALID_PLAYER_USERID -1

namespace cssmatch
{
	/** Team codes for the game */
	typedef enum TeamCode
	{
		INVALID_TEAM = TEAM_INVALID,
		UN_TEAM = TEAM_UNASSIGNED,
		SPEC_TEAM = TEAM_SPECTATOR,
		T_TEAM,
		CT_TEAM
	};

	/** Weapon's slot codes */
	typedef enum WeaponSlotCode
	{
		WEAPON_SLOT1 = 0,
		WEAPON_SLOT2,
		WEAPON_SLOT3,
		WEAPON_SLOT4,
		WEAPON_SLOT5
	};

	/** Print a message to the console
	 * @param message The message to display
	 */
	void print(const std::string & message);

	/** Make sure that an entity is valid
	 * @param entity The entity to check
	 * @return <code>true</code> if the entity is valid, <code>false</code> otherwise
	 */
	bool isValidEntity(edict_t * entity);

	/** Make sure that a player is valid
	 * @param pInfo The player to check
	 * @return <code>true</code> if the player is valid, or <code>false</code> if the player is invalid or SourceTV
	 */
	bool isValidPlayer(IPlayerInfo * pInfo);

	/** Make sure that a player index is valid
	 * @param index The player index to check
	 @ param maxClients The current max clients of the server
	 * @return <code>true</code> if the player index is valid, <code>false</code> otherwise
	 */
	bool isValidPlayerIndex(int index, int maxClients);

	/** Make sure that a player userid is valid (do not check the exitence of the player, just if the value is possible)
	 * @param userid The player userid to check
	 * @return <code>true</code> if the player userid is valid, <code>false</code> otherwise
	 */
	bool isValidPlayerUserid(int userid);

	/** Make sure that a server entity is valid
	 * @param sEntity The server entity to check
	 * @return <code>true</code> if the server entity is valid, <code>false</code> otherwise
	 */
	bool isValidServerEntity(IServerEntity * sEntity);

	/** Make sure that a base entity is valid
	 * @param bEntity The base entity to check
	 * @return <code>true</code> if the base entity is valid, <code>false</code> otherwise
	 */
	bool isValidBaseEntity(CBaseEntity * bEntity);

	/** Make sure that a server unknown instance is valid
	 * @param bEntity The server unknown to check
	 * @return <code>true</code> if the server unknown is valid, <code>false</code> otherwise
	 */
	bool isValidServerUnknown(IServerUnknown * sUnknown);

	/** Make sure that a base player instance is valid
	 * @param bEntity The base player to check
	 * @return <code>true</code> if the base player is valid, <code>false</code> otherwise
	 */
	bool isValidBasePlayer(CBasePlayer * bPlayer);

	/** Make sure that a base combat character instance is valid
	 * @param bEntity The base combat character to check
	 * @return <code>true</code> if the base combat character is valid, <code>false</code> otherwise
	 */
	bool isValidBaseCombatCharacter(CBaseCombatCharacter * bCombatCharacter);

	/** Get a server entity instance corresponding to this entity
	 * @param entity The entity
	 * @return The server entity instance corresponding to this entity, or NULL if something was invalid
	 */
	IServerEntity * getServerEntity(edict_t * entity);

	/** Get a base player entity instance corresponding to this entity
	 * @param entity The entity
	 * @return The base player entity instance corresponding to this entity, or NULL if something was invalid
	 */
	CBaseEntity * getBaseEntity(edict_t * entity);

	/** Get a server unknown pointer corresponding to this entity
	 * @param entity The entity
	 * @return The server unknown pointer corresponding to this entity, or NULL if something was invalid
	 */
	IServerUnknown * getServerUnknow(edict_t * entity);
}

#endif // __COMMON_H__
