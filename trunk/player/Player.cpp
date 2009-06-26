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

#include "Player.h"

using namespace cssmatch;

PlayerIdentity::PlayerIdentity() : 
	pEntity(NULL),
	index(INVALID_ENTITY_INDEX),
	userid(INVALID_PLAYER_USERID),
	steamid("")
{
}

Player::Player(IVEngineServer * engine, IPlayerInfoManager * pInfoManager, IServerPluginHelpers * help, int index) throw (PlayerException)
	: playerinfomanager(pInfoManager), helpers(help), cashHandler("CCSPlayer","m_iAccount"), lifeStateHandler("CBasePlayer","m_lifeState")
{
	identity.index = index;

	identity.pEntity = engine->PEntityOfEntIndex(index);
	if (! isValidEntity(identity.pEntity))
		throw PlayerException("The plugin was unable to construct a Player instance => player entity not found");

	identity.userid = engine->GetPlayerUserId(identity.pEntity);
	if (! isValidPlayerUserid(identity.userid))
		throw PlayerException("The plugin was unable to construct a Player instance => player userid not found");

	const char * tempSteamid = engine->GetPlayerNetworkIDString(identity.pEntity);
	if (tempSteamid == NULL)
		throw PlayerException("The plugin was unable to construct a Player instance => player steamid not found");
	else
		identity.steamid = tempSteamid;
}

PlayerIdentity * Player::getIdentity()
{
	return &identity;
}

TeamCode Player::getMyTeam() const
{
	TeamCode team = INVALID_TEAM;
	IPlayerInfo * pInfo = getPlayerInfo();

	if (isValidPlayer(pInfo))
		team = (TeamCode)pInfo->GetTeamIndex();

	if (team == INVALID_TEAM)
		print(__FILE__,__LINE__,"The plugin was unable to find the team of a Player");

	return team;
}

CBasePlayer * Player::getBasePlayer() const
{
	CBasePlayer * bPlayer = NULL;
	IServerUnknown * sUnknown = getServerUnknow(identity.pEntity);

	if (isValidServerUnknown(sUnknown))
		bPlayer = reinterpret_cast<CBasePlayer *>(sUnknown->GetBaseEntity());

	if (! isValidBasePlayer(bPlayer))
	{
		print(__FILE__,__LINE__,"The plugin was unable to find the base player pointer of a Player");
		bPlayer = NULL;
	}

	return bPlayer;
}

CBaseCombatCharacter * Player::getBaseCombatCharacter() const
{
	CBaseCombatCharacter * bCombatCharacter = NULL;
	CBaseEntity * bEntity = getBaseEntity(identity.pEntity);

	if (isValidBaseEntity(bEntity))
		bCombatCharacter = bEntity->MyCombatCharacterPointer();

	if (! isValidBaseCombatCharacter(bCombatCharacter))
	{
		print(__FILE__,__LINE__,"The plugin was unable to find the base combat character of a Player");
		bCombatCharacter = NULL;
	}

	return bCombatCharacter;
}

IPlayerInfo * Player::getPlayerInfo() const
{
	IPlayerInfo * pInfo = playerinfomanager->GetPlayerInfo(identity.pEntity);

	if (! isValidPlayer(pInfo))
	{
		print(__FILE__,__LINE__,"The plugin was unable to find the player's infos of a Player");
		pInfo = NULL;
	}

	return pInfo;
}

CBaseCombatWeapon * Player::getWeaponFromWeaponSlot(WeaponSlotCode slot) const
{
	CBaseCombatWeapon * bCombatWeapon = NULL;
	CBaseCombatCharacter * bCombatCharacter = getBaseCombatCharacter();

	if (isValidBaseCombatCharacter(bCombatCharacter))
		bCombatWeapon = bCombatCharacter->Weapon_GetSlot((int)slot);

	/*if (bCombatWeapon == NULL)
		print("");*/

	return bCombatWeapon;
}

void Player::removeWeapon(WeaponSlotCode slot)
{
	CBaseCombatWeapon * weapon = getWeaponFromWeaponSlot(slot);
	if (weapon != NULL)
		weapon->Kill();
}

void Player::use(const std::string & weapon) const
{
	helpers->ClientCommand(identity.pEntity,("use " + weapon).c_str());
}

void Player::setCash(unsigned int newCash)
{
	cashHandler.getProp<unsigned int>(identity.pEntity) = newCash;
}

void Player::setLifeState(unsigned int newState)
{
	lifeStateHandler.getProp<unsigned int>(identity.pEntity) = newState;
}
