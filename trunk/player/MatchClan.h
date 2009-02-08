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

#ifndef __MATCH_CLAN_H__
#define __MATCH_CLAN_H__

#include <string>
#include <sstream>
#include "ClanMember.h"

namespace cssmatch
{
	class SimplePlugin;

	/** Stats for a clan */
	struct ClanStats
	{
		/** Clan's score in Terrorist */
		int scoreT;

		/** Clan's score in Counter-Terrorist */
		int scoreCT;

		ClanStats();
	};

	/** A clan */
	class MatchClan
	{
	protected:
		/** Plugin instance which knows who all the players connected */
		SimplePlugin * plugin;

		/** Team where play the clan */
		TeamCode team;

		/** Clan's name */
		std::string name;

		/** Clan's stats */
		ClanStats stats;

		/** Determine if an automatically detected clan name can be considered as valid */
		bool isValidClanName(const std::string & newName) const;
	public:
		/**
		 * @param plugin The plugin which ables to give us the player list connected to the server
		 * @param team Current team of the clan
		 */
		MatchClan(SimplePlugin * plugin, TeamCode team);

		/** Get the clan name */
		const std::string * getName() const;

		/** Set the clan name */
		void setName(const std::string & newName);

		/** Get the team where the clan plays */
		TeamCode getTeamCode() const;

		/** Inform where the clan plays curretly */
		void setTeamCode(TeamCode code);

		/** Get the clan members */
		std::list<ClanMember *> getMembers();

		/** Get clan's stats */
		ClanStats * getStats();

		/** Try to automatically detect the clan name */
		void detectClanName();
	};
}

#endif // __MATCH_CLAN_H__
