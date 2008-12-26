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
 * Portions of this code are also Copyright � 1996-2005 Valve Corporation, All rights reserved
 */

#ifndef __RUNNABLE_CONFIGURATION_FILE_H__
#define __RUNNABLE_CONFIGURATION_FILE_H__

#include "ConfigurationFile.h"

#include "icvar.h"

#include <map>

namespace cssmatch
{
	/** Configuration file which is to be executed <br>
	 * CSSMatch wants to be able to restore as possible as the previous server configuration <br>
	 * at the end of the match. So it stores the value of the variables modified by the match configuration file <br>
	 * before execute it.
	 */
	class RunnableConfigurationFile : public ConfigurationFile
	{
	private:
		/** Valve's ICvar interface instance */
		ICvar * cvars;

		/** {Server variable pointer => value} map */
		std::map<ConVar *, std::string> data;

		/** Search data 
		 * @param line The line to parse
		 */
		std::pair<std::string,std::string> searchData(std::string line);

		/** Retrieve the server variables and their value  
		 * @return {variable name => value} map
		 */
		void getData();
	public:
		/** Prepare a runnable configuration file
		 * @param filePath The path of the file
		 * @param cvars The valve's ICvar interface instance
		 */
		RunnableConfigurationFile(const std::string filePath, ICvar * cvars);

		void execute();
	};
}

#endif // __RUNNABLE_CONFIGURATION_FILE_H__
