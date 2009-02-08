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

#include "RunnableConfigurationFile.h"

#include <sstream>
#include <fstream>

namespace cssmatch
{
	std::pair<std::string,std::string> RunnableConfigurationFile::searchData(std::string line)
	{
		std::string varName;
		std::string varValue;

		size_t strSize = line.length();
		std::string::const_iterator itChar = line.begin();
		std::string::const_iterator lastChar = line.end();

		// Try to find the server variable name
		size_t iDataBegin = 0;
		bool allFound = false;
		while((itChar != lastChar) && (! allFound))
		{
			if ((*itChar != ' ') && (*itChar != '\t'))
				iDataBegin++;
			else
				allFound = true;

			itChar++;
		}
		if (iDataBegin > 0)
			varName = line.substr(0,iDataBegin);

		// Pass the blank separators
		size_t iDataLength = 0;
		while((itChar != lastChar) && (*itChar == ' ') || (*itChar == '\t'))
		{
			iDataBegin++;
			itChar++;
		}

		// Try to find the server variable value
		//	We don't care about quotes, these data have to be handle by the console to be correctly parsed
		if ((iDataBegin > 0) && (iDataBegin+1 < strSize))
			varValue = line.substr(iDataBegin+1,strSize-iDataBegin);

		return std::pair<std::string,std::string>(varName,varValue);
	}

	void RunnableConfigurationFile::getData()
	{	
		data.clear();

		std::list<std::string> lines = getLines();

		std::list<std::string>::iterator itLine = lines.begin();
		std::list<std::string>::iterator lastLine = lines.end();
		while(itLine != lastLine)
		{
			std::pair<std::string,std::string> lineData = searchData(*itLine);
			if ((lineData.first.size() > 0) && (lineData.second.size() > 0))
			{
				// We only keep the server variables, and its curent value
				//	The objective is to restore the value of these variables, as before the match
				ConVar * var = cvars->FindVar(lineData.first.c_str());
				if (var != NULL)
					data.insert(std::pair<ConVar *, std::string>(var,var->GetString()));
			}

			itLine++;
		}
	}

	RunnableConfigurationFile::RunnableConfigurationFile(const std::string filePath, ICvar * vars, IVEngineServer * eng)
		throw (ConfigurationFileException) : ConfigurationFile(filePath), cvars(vars), engine(eng)
	{
	}

	void RunnableConfigurationFile::execute() const
	{
		execute(engine,MATCH_CONFIGURATIONS_PATH + getFileName());
	}

	void RunnableConfigurationFile::execute(IVEngineServer * engine, const std::string & filePath)
	{
		std::ostringstream buffer;
		buffer << "exec " << filePath << "\n";

		// Add the command to the console queue
		engine->ServerCommand(buffer.str().c_str());

		// Execute the exec command
		engine->ServerExecute();

		// Execute the commands added by the file in the console queue
		engine->ServerExecute();
	}
}
