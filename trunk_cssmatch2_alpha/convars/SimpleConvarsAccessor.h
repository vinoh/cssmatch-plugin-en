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

#ifndef __SIMPLE_CONVARS_ACCESSOR_H__
#define __SIMPLE_CONVARS_ACCESSOR_H__

#include "BaseConvarsAccessor.h"

namespace cssmatch
{
	/** A simple implementation of a console variables accessor */
	class SimpleConvarsAccessor : public BaseConvarsAccessor
	{
	public:
		/** Initialize the interface
		 *
		 * @param cvarFactory The valve's console variable factory
		 * @throws If the console variable interface cannot be initialized
		 *
		 */
		virtual void initializeInterface(CreateInterfaceFn cvarFactory) throw (BaseConvarsAccessorException);

		/** Retrieve the plugin's console variables accessor
		 *
		 * @return A console variable accessor
		 */
		virtual ICvar * getConVarAccessor();

		// IConCommandBaseAccessor method
		virtual bool RegisterConCommandBase(ConCommandBase * variable);
	};
}

#endif // __SIMPLE_CONVARS_ACCESSOR_H__
