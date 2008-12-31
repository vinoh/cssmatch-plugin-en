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

#ifndef __SIMPLE_PLUGIN_H__
#define __SIMPLE_PLUGIN_H__

#include "../common/common.h"

#include "../messages/RecipientFilter.h"

#include "../features/BaseSingleton.h"
#include "../convars/ConvarsAccessor.h"
#include "../match/MatchManager.h"
#include "../timer/BaseTimer.h"
#include "../messages/I18nManager.h"

#include <list>

namespace cssmatch
{
	class TeamMember;

	/** Group of the instances of the Valve's interfaces  */
	struct ValveInterfaces
	{
		IVEngineServer * engine; // helper functions (messaging clients, loading content, making entities, running commands, etc)
		IFileSystem * filesystem; // file I/O 
		IGameEventManager2 * gameeventmanager2; // game events interface
		IPlayerInfoManager * playerinfomanager; // game dll interface to interact with players
		IServerPluginHelpers * helpers; // special 3rd party plugin helpers from the engine
		CGlobalVars * gpGlobals; // global vars
		BaseConvarsAccessor * convars; // console vars

		ValveInterfaces();
	};

	/** Simple implementation of the Source plugin call back interface */
	class SimplePlugin : public BaseSingleton<SimplePlugin>, public IServerPluginCallbacks
	{
	private:
		// The parent must be able to instanciate his child
		friend class BaseSingleton<SimplePlugin>;
	protected:
		/** Valve's interfaces accessor */
		ValveInterfaces interfaces;

		/** Last client index who used a client command */
		int clientCommandIndex;

		/** Player List */
		std::list<TeamMember> playerlist;

		/** Match manager */
		MatchManager * match;

		/** Timer list */
		std::list<BaseTimer *> timers;

		/** Plugin ConVars list */
		std::list<ConVar *> pluginConVars;

		/** Internationalization tool */
		I18nManager * i18n;

		SimplePlugin();
		virtual ~SimplePlugin();

		/** Get a named interface from an interface factory
		 * @param factory The factory provide by the game when the plugin is loaded
		 * @param interfaceVersion The name of the interface to get
		 */
		template<class T>
		static bool getInterface(CreateInterfaceFn factory, T * & toInitialize, const std::string & interfaceVersion)
		{
			bool success = false;

			toInitialize = (T *)factory(interfaceVersion.c_str(),NULL);

			if (toInitialize == NULL)
				Msg(std::string("CSSMatch : Unable to get the \"" + interfaceVersion + "\" interface !\n").c_str());
			else
				success = true;

			return success;
		}
	public:
		/** Valve's interface accessor */
		ValveInterfaces * getInterfaces();

		/** Get the last player index wich use a rcon command */
		int GetCommandIndex() const;

		void SetCommandClient(int index);

		/** Get the connected player list */
		std::list<TeamMember> * getPlayerList();

		/** Get the match manager */
		MatchManager * getMatchManager();

		/** Add a plugin console variable */
		void addPluginConVar(ConVar * variable);

		/** Get the plugin convars list */
		const std::list<ConVar *> * getPluginConVars() const;

		/** Get the internationalization tool */
		I18nManager * get18nManager();

		/** Add a timer */
		void addTimer(BaseTimer * timer);

		/** Set the console variable accessor <br>
		 * The accessor will be automatically delete when the plugin is unloaded
		 * @param convarsAccessor The console variable accessor to set
		 */
		void setConvarsAccessor(BaseConvarsAccessor * convarsAccessor);

		// IServerPluginCallbacks methods
		virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
		virtual void Unload();
		virtual void Pause();
		virtual void UnPause();
		virtual const char * GetPluginDescription();
		virtual void LevelInit(char const * pMapName);
		virtual void ServerActivate(edict_t * pEdictList, int edictCount, int clientMax);
		virtual void GameFrame(bool simulating);
		virtual void LevelShutdown();
		virtual void ClientActive(edict_t * pEntity);
		virtual void ClientDisconnect(edict_t * pEntity);
		virtual void ClientPutInServer(edict_t * pEntity, char const * playername);
		virtual void ClientSettingsChanged(edict_t * pEdict);
		virtual PLUGIN_RESULT ClientConnect(bool * bAllowConnect, edict_t * pEntity, const char *pszName, const char * pszAddress, char * reject, int maxrejectlen);
		virtual PLUGIN_RESULT ClientCommand(edict_t * pEntity);
		virtual PLUGIN_RESULT NetworkIDValidated(const char * pszUserName, const char * pszNetworkID);

		// Tools

		/** Print a message in the logs
		 * @param message The message to display
		 */
		void log(const std::string & message) const;

		/** Print debug informations relating to an exception
		 * @param e The exception to debug
		 */
		void SimplePlugin::printException(const BaseException & e, const std::string & fileName, int line) const;

		/** Appends a command to the server command queue
		 * @param command The command to append
		 */
		void queueCommand(const std::string & command) const;

		/** Check if SourceTV is connected to the server (tv_enable is not checked)
		 * @return <code>true</code> if SourceTV was found, <code>false</code> otherwise
		 */
		bool hltvConnected() const;

		/** Get the current player count 
		 * @param team If specified, the count will be limited to a particular team
		 * @return The player count
		 */
		int getPlayerCount(TeamCode team = INVALID_TEAM) const;
	};
}

/* Loop over all players excluding the 0 index 
#define FOREACH_PLAYER(indexVar) \
	int maxplayers = SimplePlugin::getInstance()->getInterfaces()->gpGlobals->maxClients; \
	for (int indexVar=1;indexVar <= maxplayers;indexVar++)*/

#endif // __SIMPLE_PLUGIN_H__
