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

#include "I18nPluginConVar.h"

namespace cssmatch
{
	I18nPluginConVar::I18nPluginConVar(SimplePlugin * plug, char const * pName, char const * pDefaultValue, int flags)
		: ConVar(pName,pDefaultValue,flags), plugin(plug)
	{
		AddFlags(FCVAR_PLUGIN);
	}

	I18nPluginConVar::I18nPluginConVar(SimplePlugin * plug, char const * pName, char const * pDefaultValue, int flags, char const * pHelpString)
		: ConVar(pName,pDefaultValue,flags,pHelpString), plugin(plug)
	{
		AddFlags(FCVAR_PLUGIN);
	}

	I18nPluginConVar::I18nPluginConVar(	SimplePlugin * plug, 
							char const * pName,
							char const * pDefaultValue,
							int flags,
							char const * pHelpString,
							bool bMin,
							float fMin,
							bool bMax,
							float fMax)
		: ConVar(pName,pDefaultValue,flags,pHelpString,bMin,fMin,bMax,fMax), plugin(plug)
	{
		AddFlags(FCVAR_PLUGIN);
	}

	I18nPluginConVar::I18nPluginConVar(	SimplePlugin * plug,
							char const * pName,
							char const * pDefaultValue,
							int flags,
							char const * pHelpString,
							FnChangeCallback callback)
		: ConVar(pName,pDefaultValue,flags,pHelpString,callback), plugin(plug)
	{
		AddFlags(FCVAR_PLUGIN);
	}

	I18nPluginConVar::I18nPluginConVar(	SimplePlugin * plug, 
							char const * pName,
							char const *pDefaultValue,
							int flags,
							char const * pHelpString,
							bool bMin,
							float fMin,
							bool bMax,
							float fMax,
							FnChangeCallback callback)
		: ConVar(pName,pDefaultValue,flags,pHelpString,bMin,fMin,bMax,fMax,callback), plugin(plug)
	{
		AddFlags(FCVAR_PLUGIN);
	}

	char const * I18nPluginConVar::GetHelpText(void) const
	{
		ValveInterfaces * interfaces = plugin->getInterfaces();
		I18nManager * i18n = plugin->get18nManager();

		// What player ask the description ?
		int playerIndex = plugin->GetCommandIndex();

		// What is the language used by this player
		std::string language = interfaces->engine->GetClientConVarValue(playerIndex,"cl_language");

		// Return the translation
		return strdup(i18n->getTranslation(language,ConVar::GetHelpText()).c_str());
	}
}
