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

#include "I18nManager.h"

namespace cssmatch
{
	I18nManager::~I18nManager()
	{
		std::map<std::string,TranslationFile *>::iterator itLanguages = languages.begin();
		std::map<std::string,TranslationFile *>::iterator lastLanguages = languages.end();
		while(itLanguages != lastLanguages)
		{
			delete itLanguages->second;

			itLanguages++;
		}
	}

	void I18nManager::getTranslationFile(const std::string & language)
	{
		
	}

	std::string I18nManager::getTranslation(const std::string & lang,
											const std::string & keyword, 
											std::map<std::string, std::string> & parameters)
	{
		std::string message;

		// Is the translation already used ?
		if (languages.find(lang) == languages.end())
		{ 
			// No, we have to get the corresponding translations
			getTranslationFile(lang);
		}
		
		message = (*languages[lang])[keyword]; // copying it, because we will replace the parameters

		// Relace the parameters
		std::map<std::string,std::string>::const_iterator itParameters = parameters.begin();
		std::map<std::string,std::string>::const_iterator lastParameters = parameters.end();
		while(itParameters != lastParameters)
		{
			const std::string * parameter = &itParameters->first;
			size_t parameterSize = parameter->size();
			const std::string * value = &itParameters->second;
			size_t valueSize = value->size();

			// One option can be found multiple times
			size_t iParam = message.find(*parameter);
			while(iParam != std::string::npos)
			{
				message.replace(iParam,parameterSize,*value,0,valueSize);
				iParam = message.find(*parameter);
			}

			itParameters++;
		}

		return message;
	}
}
