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

#ifndef __TRANSLATION_FILE_H__
#define __TRANSLATION_FILE_H__

#include "ConfigurationFile.h"
#include "../exceptions/BaseException.h"
#include <map>

namespace cssmatch
{
	class TranslationException : public ConfigurationFileException
	{
	public:
		TranslationException(const std::string & message) : ConfigurationFileException(message){};
	};

	class TranslationFile;
	namespace
	{
		/** Base parser state for the translation file engine */
		class BaseTranslationParserState
		{
		protected:
			/** The translation file to parse */
			TranslationFile * translationFile;
		public:
			/** Initialize the state handler
			 * @param translationFile The translation file to parse
			 */
			BaseTranslationParserState(TranslationFile * translationFile);

			/** Parse the translation file
			 * @param line The line to parse
			 * @throws TranslationException if an error occurs
			 */
			virtual void parse(std::string & line) = 0;
		};

		/** We are parsing the header for the translation file */
		class HeaderTranslationParserState : public BaseTranslationParserState
		{
		public:
			HeaderTranslationParserState(TranslationFile * translationFile);

			virtual void parse(std::string & line);
		};

		/** We are parsing the translation data for the translation file */
		class DataTranslationParserState : public BaseTranslationParserState
		{
		public:
			DataTranslationParserState(TranslationFile * translationFile);

			virtual void parse(std::string & line);
		};
	}

	/** Translation file <br>
	 * Rules : <br>
	 *	- // marks a commentary statement <br>
	 *	- A [header] must be placed before any translation, e.g. [French] <br>
	 *	- Each translation line must takes place on its proper line <br>
	 *	- Each translation line must be in this format :  <br>
	 *		keyword = translation <br>
	 *	- Both keyword and translation can between quotation mark, to delimit where the data ends <br>
	 *  Support the ANSI and UTF-8 encoding
	 */
	class TranslationFile : public ConfigurationFile
	{
	private:
		/** [header] (e.g. translation name)*/
		std::string header;

		/** {keyword => translation} map */
		std::map<std::string,std::string> translations;

		/** Translation parsing state */
		BaseTranslationParserState * parserState;

		/** Search and store the header and the translations 
		 * @throws TranslationException if not header was found
		 */
		void parse() throw(TranslationException);
		// I - Where is the header ?
		// II - Okay header found, now we'll try to get the translations
	public:
		/** Parse a new translation file
		 * @param filePath The path of the file to parse
		 * @throws TranslationException if not header was found (incorrect translations are ignored)
		 */
		TranslationFile(const std::string filePath) throw(ConfigurationFileException,TranslationException);
		virtual ~TranslationFile();

		/** Get the header content */
		std::string getHeader() const;

		/** Set the header content */
		void setHeader(const std::string & header);

		/** Set the translation parser state */
		void setParserState(BaseTranslationParserState * parserState);

		/** Add a translation 
		 * @param keyword Identifier for the translation
		 * @param translation The translation to add
		 */
		void addTranslation(const std::string & keyword, const std::string & translation);

		/** Check if a translation exists
		 * @param keyword The keyword corresponding to the translation
		 * @return <code>true</code> if the translation was found, <code>false</code> otherwise
		 */
		bool keywordExists(const std::string keyword) const;

		/** Get the translation corresponding to a keyword
		 * @param keyword The keyword corresponding to the translation
		 * @return The translation
		 * @throws TranslationException if the translation does not exist
		 */
		std::string operator [](const std::string keyword) throw (TranslationException);
	};
}

#endif // __TRANSLATION_FILE_H__