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

#ifndef __BASE_TIMER_H__
#define __BASE_TIMER_H__

namespace cssmatch
{
	/** Permit to delay the execution of C++ functions <br>
	 * Any timer have to be derived from this "stub" class in order to use the timer in a generic way
	 */
	class BaseTimer
	{
	private:
		// Functors
		friend class TimerOutOfDate;
	protected:
		/** When (in server time seconds) the delayed code must be executed */
		float date;
	public:
		/** Prepare a timer
		 * @param executionDate When (in server time seconds) the delayed code must be executed
		 */
		BaseTimer(float executionDate);

		/** Get the execution date (in )
		 * @return The dte (server time seconds)
		 */
		float getExecutionDate() const;

		/** Execute the delayed code */
		virtual void execute() = 0;
	};

	/** Functor to quickly determine if a timer must be executed <br>
	 * If yes, the timer is executed and the operator () returns <code>true</code>
	 */
	class TimerOutOfDate
	{
	private:
		/** Reference date */
		float date;
	public:
		/** Initialize the functor
		 * @param currentDate The date to compare with the timer's execution date
		 */
		TimerOutOfDate(float currentDate);

		bool operator()(BaseTimer * timer);
	};
}

#endif // __BASE_TIMER_H__
