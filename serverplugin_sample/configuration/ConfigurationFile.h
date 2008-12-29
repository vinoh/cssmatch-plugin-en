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

#ifndef __CONFIGURATION_FILE_H__
#define __CONFIGURATION_FILE_H__

#include <string>
#include <list>

namespace cssmatch
{
	/** Configuration file <br>
	 * Commented line statements begin by //
	 */
	class ConfigurationFile
	{
	protected:
		/** File's path */
		std::string filePath;

		/** Make sure that the string doesn't contains a winsows or linux end line character 
		 * @param line The line to parse
		 */
		void removeEndLine(std::string & line);

		/** Remove the commented part of a string
		 * @param line The line to parse
		 */
		void removeComments(std::string & line);

		/** Strip the line
		 * @param line The line to parse
		 */
		void strip(std::string & line);
	public:
		/** Construct a handler for a configuration file 
		 * @param filePath The path of the file
		 */
		ConfigurationFile(const std::string filePath);

		/** Get the file's name */
		std::string getFileName() const;

		/** Get a list of all the file data lines (stripped, without comments) */
		std::list<std::string> getLines();
	};
}

#endif // __CONFIGURATION_FILE_H__
