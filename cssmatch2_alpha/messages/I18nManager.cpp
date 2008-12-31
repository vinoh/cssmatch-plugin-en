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
	std::map<std::string, std::string> I18nManager::WITHOUT_PARAMETERS;

	I18nManager::I18nManager(ConVar * defaultLang) : defaultLanguage(defaultLang)
	{
	}

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

	std::string I18nManager::getDefaultLanguage() const
	{
		return defaultLanguage->GetString();
	}

	TranslationFile * I18nManager::getTranslationFile(const std::string & language)
	{
		TranslationFile * translationSet = NULL;

		// Is the language never used (in the cache) ?
		std::map<std::string,TranslationFile *>::iterator itLanguages = languages.find(language);
		std::map<std::string,TranslationFile *>::iterator lastLanguages = languages.end();
		if (itLanguages == lastLanguages)
		{
			try
			{
				translationSet = new TranslationFile(TRANSLATIONS_FOLDER + language + ".txt");

				// Put these translations in the cache
				languages[language] = translationSet;
			}
			catch(const ConfigurationFileException & e)
			{
				// If the file was not found, we'll use the default language instead
				std::string defaultLanguageName = defaultLanguage->GetString();
				std::map<std::string,TranslationFile *>::iterator itDefault = languages.find(defaultLanguageName);
				if (itDefault == lastLanguages)
				{
					try
					{
						translationSet = new TranslationFile(TRANSLATIONS_FOLDER + defaultLanguageName + ".txt");
						languages[defaultLanguageName] = translationSet;
					}
					catch(const ConfigurationFileException & e)
					{
						print("ERROR ! Default translation file not found !");
					}
				}
				else
					translationSet = itDefault->second;
			}
		}

		return translationSet;
	}

	std::string I18nManager::getTranslation(const std::string & lang,
											const std::string & keyword, 
											std::map<std::string, std::string> & parameters)
	{
		std::string message;

		// No, we have to get the corresponding translations
		TranslationFile * translation = getTranslationFile(lang);
		
		if (translation != NULL)
		{
			message = (*translation)[keyword]; // copying it, because we will replace the parameters

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
		}

		return message;
	}
}
