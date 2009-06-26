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

#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__

#include "igameevents.h"

#include <map>
#include <list>

namespace cssmatch
{
	/** Event listener template, which can be used as a class member */
	template<typename T>
	class EventListener : public IGameEventListener2
	{
	protected:
		/** Callback's object */
		T * object;

		/** valve's IGameEventManager2 instance */
		IGameEventManager2 * gameeventmanager2;
	private:
		/** { event name => callback } / one event = one callback */
		std::map<std::string, void (T::*)(IGameEvent *)> callbacks;
	public:
		EventListener(T * callbackObject, IGameEventManager2 * eventManager)
			: object(callbackObject), gameeventmanager2(eventManager){}
		~EventListener()
		{
			gameeventmanager2->RemoveListener(this);
		}

		// IGameEventListener2 method(s)
		virtual void FireGameEvent(IGameEvent * event)
		{
			const char * c_eventName = event->GetName();
			std::string eventName = c_eventName;

			if (callbacks.find(eventName) != callbacks.end())
			{
				// Call all the callback corresponding to this event
				((object)->*(callbacks[eventName]))(event);
			}
		}

		/** Add a callback for an event
		 * @param eventName The event name to listen
		 * @param object The object which listens the event
		 * @param callback The object's function to call when the event occur
		 * @param serverSide Is the event server side ?
		 */
		bool addCallback(const std::string & eventName, void (T::* callback)(IGameEvent * event), bool serverSide = true)
		{
			bool success = false;
			const char * c_eventName = eventName.c_str();

			if (callbacks.find(eventName) != callbacks.end())
			{
				callbacks[eventName] = callback;
				success = true;
			}
			else
			{
				if (gameeventmanager2->AddListener(this,c_eventName,serverSide))
				{
					callbacks[eventName] = callback;
					success = true;				
				}
			}

			return success;
		}

		/** Remove any callback for an event
		 * @param eventName The event name
		 */
		bool removeCallback(const std::string & eventName)
		{
			bool success = false;
			const char * c_eventName = eventName.c_str();

			if (callbacks.find(eventName) != callbacks.end())
			{
				success = true;
				callbacks.erase(eventName);
			}

			return success;
		}
	};
}

#endif // __EVENT_LISTENER_H__
