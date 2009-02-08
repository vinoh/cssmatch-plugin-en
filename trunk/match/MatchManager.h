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

#ifndef __MATCH_MANAGER_H__
#define __MATCH_MANAGER_H__

#include "DisableMatchState.h"
#include "../player/MatchClan.h"
#include "../configuration/RunnableConfigurationFile.h"

namespace cssmatch
{
	class SimplePlugin;

	/** A match lign-up */
	struct MatchLignup
	{
		/** First clan */
		MatchClan clan1;

		/** Second clan */
		MatchClan clan2;

		MatchLignup(SimplePlugin * plugin);
	};

	/** Information related to the match */
	struct MatchInfo
	{
		/** Set number */
		int setNumber;

		/** Round number */
		int roundNumber;

		MatchInfo();
	};

	class MatchManager
	{
	protected:
		/** The plugin instance */
		SimplePlugin * plugin;

		/** Current match state(e.g. kniferound, warmup, etc.) */
		BaseMatchState * state;

		/** Match's lignup */
		MatchLignup lignup;

		/** Match's info */
		MatchInfo infos;
	public:
		/** Prepare a match
		 * @param plugin The plugin instance
		 */
		MatchManager(SimplePlugin * plugin);

		~MatchManager();

		/** Get the match lignup */
		MatchLignup * getLignup();

		/** Get some informations about the match */
		MatchInfo * getInfos();

		/** Set a new match state <br>
		 * Call the endState method of the previous state, and the startState of the new state
		 * @param newState The new match state
		 */
		void setMatchState(BaseMatchState * newState);

		/** Start a new math
		 * @param config The configuration of the match
		 * @param umpire The player who starts the match
		 */
		void start(RunnableConfigurationFile & config, ClanMember * umpire = NULL);
	};
}

#endif // __MATCH_MANAGER_H__
