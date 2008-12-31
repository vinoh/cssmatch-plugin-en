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

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include "../common/common.h"
#include "../exceptions/BaseException.h"
#include "../messages/RecipientFilter.h"

namespace cssmatch
{
	class PlayerException : public BaseException
	{
	public:
		PlayerException(const std::string & message) : BaseException(message){};
	};

	struct PlayerIdentity
	{
		/** Pointer to the player entity instance corresponding to this player */
		edict_t * pEntity;

		/** Index of this player */
		int index;

		/** Userid of this player */
		int userid;

		/** Steamid of this player */
		std::string steamid;

		PlayerIdentity();
	};

	/** Base class for player informations encapsulation */
	class Player
	{
	private:
		// Functors (friends to accelerates the lookups)
		friend class PlayerHavingPEntity;
		friend class PlayerHavingIndex;
		friend class PlayerHavingUserid;
		friend class PlayerHavingTeam;
		friend struct PlayerIsHltv;
		friend class PlayerToRecipient;
	protected:
		/** IPlayerInfoManager instance to dynamically query informations about the player */
		IPlayerInfoManager * playerinfomanager;

		/** Player's identity */
		PlayerIdentity identity;

	public:
		/** Initialize the informations relating to the player 
		 * @param engine The valve's IVEngineServer interface (required to query the player's informations)
		 * @param playerinfomanager The IPlayerInfoManager instance (required to query the player's informations)
		 * @param index The player index corresponding to the player (must be valid !)
		 */
		Player(IVEngineServer * engine, IPlayerInfoManager * playerinfomanager, int index) throw(PlayerException);

		/** Get the idneity informations about this player 
		 * @return A PlayerIdentity structure pointer
		 */
		PlayerIdentity * getIdentity();

		/** Get the current team of this player
		 * @return The TeamCode corresponding to the player's team, or INVALID_TEAM if something was invalid
		 */
		TeamCode getMyTeam() const;

		/** Get a server entity instance corresponding to this player
		 * @return The server entity instance corresponding to this player, or NULL if something was invalid
		 */
		IServerEntity * getServerEntity() const;

		/** Get a base player entity instance corresponding to this player
		 * @return The base player entity instance corresponding to this player, or NULL if something was invalid
		 */
		CBaseEntity * getBaseEntity() const;

		/** Get a server unknown pointer corresponding to this player
		 * @return The server unknown pointer corresponding to this player, or NULL if something was invalid
		 */
		IServerUnknown * getServerUnknow() const;

		/** Get a base player instance corresponding to this player
		 * @return The base player instance corresponding to this player, or NULL if something was invalid
		 */
		CBasePlayer * getBasePlayer() const;

		/** Get a base combat character instance corresponding to this player
		 * @return The base combat character instance corresponding to this player, or NULL if something was invalid
		 */
		CBaseCombatCharacter * getBaseCombatCharacter() const;

		/** Get an IPlayerInfo instance corresponding to this player
		 * @return The IPlayerInfo instance corresponding to this player, or NULL if something was invalid
		 */
		IPlayerInfo * getPlayerInfo() const;

		/** Get the CBaseCombatWeapon instance corresponding to a player entity's weapon slot 
		 * @param slot The slot code where is weapon can be found
		 * @return The CBaseCombatWeapon instance found, or NULL if something was invalid
		 */
		CBaseCombatWeapon * getWeaponFromWeaponSlot(WeaponSlotCode slot) const;
	};

	/** Functor to quickly find a Player instance by his pEntity instance */
	class PlayerHavingPEntity
	{
	private:
		edict_t * pEntity;
	public:
		PlayerHavingPEntity(edict_t * pEntity);
		bool operator ()(const Player & player);
	};

	/** Functor to quickly find a Player instance by his index */
	class PlayerHavingIndex
	{
	private:
		int index;
	public:
		PlayerHavingIndex(int index);
		bool operator ()(const Player & player);
	};

	/** Functor to quickly find a Player instance by his userid */
	class PlayerHavingUserid
	{
	private:
		int userid;
	public:
		PlayerHavingUserid(int userid);
		bool operator ()(const Player & player);
	};

	/** Functor to quickly find a Player instance by his team */
	class PlayerHavingTeam
	{
	private:
		TeamCode team;
	public:
		PlayerHavingTeam(TeamCode team);
		bool operator ()(const Player & player);
	};

	/** Functor to quickly find SourceTV */
	struct PlayerIsHltv
	{
		bool operator ()(const Player & player);
	};

	/** Functor to add a player in a recipient (index list) */
	class PlayerToRecipient
	{
	private:
		RecipientFilter * recipientFilter;
	public:
		PlayerToRecipient(RecipientFilter * recipientFilter);
		void operator ()(const Player & player);
	};
}

#endif // __PLAYER_H__
