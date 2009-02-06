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

#ifndef __MATCH_MANAGER_H__
#define __MATCH_MANAGER_H__

#include "DisableMatchState.h"
#include "../player/MatchClan.h"

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

	class MatchManager
	{
	protected:
		/** The plugin instance */
		SimplePlugin * plugin;

		/** Current match state(e.g. kniferound, warmup, etc.) */
		BaseMatchState * state;

		/** Match's lignup */
		MatchLignup lignup;
	public:
		/** Prepare a match
		 * @param plugin The plugin instance
		 */
		MatchManager(SimplePlugin * plugin);

		~MatchManager();

		/** Get the match lignup */
		MatchLignup * getLignup();

		/** Set a new match state <br>
		 * Call the endState method of the previous state, and the startState of the new state
		 * @param newState The new match state
		 */
		void setMatchState(BaseMatchState * newState);
	};
}

#endif // __MATCH_MANAGER_H__
