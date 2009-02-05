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

#include "EntityProp.h"

namespace cssmatch
{
	void EntityProp::getOffset(SendTable * table,  std::istringstream & propPathLeft)
	{
		std::string pathNextStep;
		std::getline(propPathLeft,pathNextStep,'.');
		//Msg("Scanning %s...\n",propPathLeft.c_str());

		int nbrProps = table->GetNumProps();
		for(int i=0;i<nbrProps;i++)
		{
			SendProp * sProp = table->GetProp(i);

			if (pathNextStep == sProp->GetName())
			{
				offset += sProp->GetOffset();
				switch(sProp->GetType())
				{
				case DPT_Vector:
				case DPT_String:
				case DPT_Array:
				case DPT_Int:
				case DPT_Float:
					i = nbrProps; // break
					break;
				case DPT_DataTable:
					getOffset(sProp->GetDataTable(),propPathLeft);
					break;
				default:
					offset = 0;
					i = nbrProps; // break
				}
			}
		}
	}

	void EntityProp::initialize()
	{
		// Get all server classes
		ServerClass * classes = SimplePlugin::getInstance()->getInterfaces()->serverGameDll->GetAllServerClasses();

		// Search the suitable class
		while (classes != NULL)
		{
			if (theClass == classes->GetName())
			{
				std::istringstream pathToProp(path);

				getOffset(classes->m_pTable,pathToProp);

				classes = NULL; // break
			}
			else
				classes = classes->m_pNext;
		}
	}

	EntityProp::EntityProp(const std::string & propClass, const std::string & propPath)
		: offset(0), theClass(propClass), path(propPath)
	{
		initialize();
	}
}
