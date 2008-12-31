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

#ifndef __I18N_MANAGER_H__
#define __I18N_MANAGER_H__

#include "../features/CannotBeCopied.h"
#include "UserMessagesManager.h"
#include "../configuration/TranslationFile.h"

#include "convar.h"

#include <map>

#define TRANSLATIONS_FOLDER "cstrike/cfg/cssmatch/languages/"

namespace cssmatch
{
	/** Support for internationalized messages <br>
	 * Messages can have parameters, prefixed by $. E.g. : "The attacker is $attackername" <br>
	 * These parameters have to be passed under the form of {parameter => value} map
	 */
	class I18nManager : public CannotBeCopied
	{
	private:
		/** What is the default language to use ? */
		ConVar * defaultLanguage;

		/** {language name => translation set} */
		std::map<std::string,TranslationFile *> languages;
	public:
		/** Empty map for messages which have no option to parse */
		static std::map<std::string, std::string> WITHOUT_PARAMETERS;

		/**
		 * @param defaultLanguage The default language to use if no translation is found
		 */
		I18nManager(ConVar * defaultLanguage);
		~I18nManager();

		/** Get the current default language */
		std::string getDefaultLanguage() const;

		/** Retrieve the TranslationFile instance corresponding to a language, then store/cache it if necessary 
		 * @param language The language which has to be used
		 * @return The corresponding translation file 
		 */
		TranslationFile * getTranslationFile(const std::string & language);

		/** Retrieve the translation of a message
		 * @param language The language of the translation
		 * @param keyword The identifier of the translation to retrieve
		 * @param parameters If specified, the message's parameters and their value
		 */
		std::string getTranslation(	const std::string & language,
									const std::string & keyword, 
									std::map<std::string, std::string> & parameters = WITHOUT_PARAMETERS);
	};
}

#endif // __I18N_MANAGER_H__
