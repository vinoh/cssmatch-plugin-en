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

#ifndef __ACTIVATED_MATCH_STATE_H__
#define __ACTIVATED_MATCH_STATE_H__

#include "BaseMatchState.h" // BaseMatchState

class IGameEventManager2;
class IGameEvent;

namespace cssmatch
{
	class MatchManager;

	/** Base class for "match in progress" state */
	class ActivatedMatchState : public BaseMatchState
	{
	public:
		ActivatedMatchState(MatchManager * match, IGameEventManager2 * eventManager);

		// BaseMatchState methods
		//	Please override these methods in the derivated classes
		virtual void startState();
		virtual void endState();
		virtual void FireGameEvent(IGameEvent * event);

		// Game event callbacks
		void player_say(IGameEvent * event);
	};
}

#endif // __ACTIVATED_MATCH_STATE_H__
