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

#include "TranslationFile.h"

namespace cssmatch
{
	void TranslationFile::parse() throw(TranslationException)
	{
		std::list<std::string> lines = getLines();

		std::list<std::string>::iterator itLines = lines.begin();
		std::list<std::string>::iterator lastLines = lines.end();

		while(itLines != lastLines)
		{
			parserState->parse(*itLines);

			itLines++;
		}

		if (header.empty())
			throw TranslationException("The file " + filePath + " does not have a header");
	}

	TranslationFile::TranslationFile(const std::string filePath) throw(ConfigurationFileException,TranslationException) : ConfigurationFile(filePath)
	{
		parserState = new HeaderTranslationParserState(this);

		parse();
	}

	TranslationFile::~TranslationFile()
	{
		delete parserState;
	}

	std::string TranslationFile::getHeader() const
	{
		return header;
	}

	void TranslationFile::setHeader(const std::string & header)
	{
		this->header = header;
	}

	void TranslationFile::setParserState(BaseTranslationParserState * state)
	{
		delete parserState; // ends the old parser state

		parserState = state;
	}

	void TranslationFile::addTranslation(const std::string & keyword, const std::string & translation)
	{
		translations[keyword] = translation;
	}

	bool TranslationFile::keywordExists(const std::string keyword) const
	{
		return translations.find(keyword) != translations.end();
	}

	std::string TranslationFile::operator [](const std::string keyword) throw (TranslationException)
	{
		if (keywordExists(keyword))
			return translations[keyword];
		else
			throw TranslationException(keyword + " does not correspond to a known translation of the file " + filePath);
	}

	namespace
	{
		BaseTranslationParserState::BaseTranslationParserState(TranslationFile * transFile) : translationFile(transFile)
		{
		}

		HeaderTranslationParserState::HeaderTranslationParserState(TranslationFile * translationFile)
			: BaseTranslationParserState(translationFile)
		{
		}

		void HeaderTranslationParserState::parse(std::string & line)
		{
			// Search for [some header] statement
			size_t iHeaderBegins = line.find("[");
			if (iHeaderBegins != std::string::npos)
			{
				size_t iHeaderEnds = line.find("]");
				if (iHeaderEnds != std::string::npos)
				{
					translationFile->setHeader(line.substr(iHeaderBegins,iHeaderEnds - iHeaderBegins + 1));
					translationFile->setParserState(new DataTranslationParserState(translationFile));
					//Msg("Found header : '%s'\n",translationFile->getHeader().c_str());
				}
			}
			// Else we ignore this invalid line
		}

		DataTranslationParserState::DataTranslationParserState(TranslationFile * translationFile)
			: BaseTranslationParserState(translationFile)
		{
		}

		// BEURK !
		void DataTranslationParserState::parse(std::string & line)
		{
			// Search for "keyword = translation" statement 
			//	(quotation marks are optionnals, but delimit the string end if they exist)

			size_t iEqual = std::string::npos;
			size_t lineSize = line.size();

			bool betweenQuotes = false; // true if we are between quotation marks

			// Where is the "=" symbol ?
			std::string::const_iterator itChar = line.begin();
			std::string::const_iterator lastChar = line.end();
			bool equalFound = false;
			while((itChar != lastChar) && (! equalFound))
			{
				iEqual++;
				switch(*itChar) 
				{ 
				case '"': // Quoted string
					betweenQuotes = (! betweenQuotes);
					break;

				case '=':
						equalFound = (! betweenQuotes); // We found the equal symbol
					break;
				}

				itChar++;
			}

			// Is the equal symbol found or is the symbol is between quotation marks ?
			if (iEqual != lineSize-1)
			{ // No, we can get the data
				std::string dataName = line.substr(0,iEqual); //	
				ConfigurationFile::strip(dataName);

				// Stripping the quote if required
				size_t iLastCharDataName = dataName.size() - 1;
				if (iLastCharDataName>0)
				{
					if ((dataName[0] == '"') && (dataName[iLastCharDataName] == '"'))
						dataName = dataName.substr(1,iLastCharDataName - 1);

					std::string dataValue = line.substr(iEqual+1,lineSize); // +1 to ignore the equal symbol
					ConfigurationFile::strip(dataValue);

					// Stripping the quotes if required
					size_t iLastCharDataValue = dataValue.size()-1;
					if (iLastCharDataValue>0)
					{
						if ((dataValue[0] == '"') && (dataValue[iLastCharDataValue] == '"'))
							dataValue = dataValue.substr(1,iLastCharDataValue - 1);

						// \n are replaced by the corresponding escape sequence
						size_t iEndLine = dataValue.find("\\n");
						while (iEndLine != std::string::npos)
						{
							dataValue.replace(iEndLine,2,"\n",0,1);
							iEndLine = dataValue.find("\\n");
						}

						if ((dataName.size() > 0) && (dataValue.size() > 0))
						{
							translationFile->addTranslation(dataName,dataValue);
							//Msg("Found : '%s'='%s'\n",dataName.c_str(),dataValue.c_str());
						}
					}
				}
			}
			// Else we ignore this invalid line
		}
	}
}
