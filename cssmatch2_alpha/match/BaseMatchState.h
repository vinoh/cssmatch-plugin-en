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

#ifndef __BASE_MATCH_STATE_H__
#define __BASE_MATCH_STATE_H__

#include "../common/common.h"
#include <map>

namespace cssmatch
{
	class SimpleMatchManager;

	/** Base match state (e.g. disabled, kniferound, warmup, set, etc.) */
	class BaseMatchState : public IGameEventListener2
	{
	protected:
		/** Match which is in this state */
		SimpleMatchManager * match;

		/** valve's IGameEventManager2 instance */
		IGameEventManager2 * gameeventmanager2;
	public:
		/** Prepare a new match state 
		 * @param match The match which will be in this state
		 * @param eventManager The valve's IGameEventManager2 instance
		 */
		BaseMatchState(SimpleMatchManager * m, IGameEventManager2 * eventManager);

		/** Apply this match state */
		virtual void startState() = 0;

		/** End this match state <br>
		 * Remove all event listeners we used here
		 */
		virtual void endState() = 0;

		// IGameEventListener2 method
		virtual void FireGameEvent(IGameEvent * event) = 0;
	};
}
#endif // __BASE_MATCH_STATE_H__
