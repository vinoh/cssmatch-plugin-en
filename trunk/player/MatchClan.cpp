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

#include "MatchClan.h"

#include "../plugin/SimplePlugin.h"

namespace cssmatch
{
	bool MatchClan::isValidClanName(const std::string & newName) const
	{
		return newName.size() >= 3;
	}

	ClanStats::ClanStats()
	{
		scoreT = 0;
		scoreCT = 0;
	}

	MatchClan::MatchClan(SimplePlugin * plug, TeamCode t) : plugin(plug), name("Nobody"), team(t)
	{
	}

	const std::string * MatchClan::getName() const
	{
		return &name;
	}

	void MatchClan::setName(const std::string & newName)
	{
		name = newName;
	}

	TeamCode MatchClan::getTeamCode() const
	{
		return team;
	}

	void MatchClan::setTeamCode(TeamCode code)
	{
		team = code;
	}

	std::list<ClanMember *> MatchClan::getMembers()
	{
		// We have to construct a list of members who take part to this clan
		std::list<ClanMember *> clanPlayerList;

		std::list<ClanMember *> * playerlist = plugin->getPlayerlist();
		std::list<ClanMember *>::iterator itMembers = playerlist->begin();
		std::list<ClanMember *>::iterator endMembers = playerlist->end();
		while(itMembers != endMembers)
		{
			if ((*itMembers)->getMyTeam() == team)
				clanPlayerList.push_back((*itMembers));
			itMembers++;
		}

		return clanPlayerList;
	}

	ClanStats * MatchClan::getStats()
	{
		return &stats;
	}

	void MatchClan::detectClanName() // non testé/bugué
	{
		// 1. Get two members
		// 2. Get their names
		// 3. Search and accumulate consecutive/common characters until we find a clan name which contains at least 3 characters

		std::list<ClanMember *> members = getMembers();
		std::list<ClanMember *>::const_iterator itMembers = members.begin();
		std::list<ClanMember *>::const_iterator lastMembers = members.end();

		// 1.
		if (itMembers != lastMembers)
		{
			// Get the fist member
			ClanMember * member1 = *itMembers;

			// Try to get a second member
			itMembers++;
			if (itMembers != lastMembers)
			{
				// Get the second member
				ClanMember * member2 = *itMembers;


				// 2.
				IPlayerInfo * pInfo1 = member1->getPlayerInfo();
				IPlayerInfo * pInfo2 = member2->getPlayerInfo();
				if (isValidPlayer(pInfo1) && isValidPlayer(pInfo2))
				{
					std::string memberName1 = pInfo1->GetName();
					std::string memberName2 = pInfo2->GetName();


					// 3.
					std::string newName; // new clan name
					bool foundNewName = false; // true if a new clan name was found
					std::string::const_iterator itMemberName1 = memberName1.begin();
					std::string::const_iterator endMemberName1 = memberName1.end();
					std::string::const_iterator itMemberName2 = memberName2.begin();
					std::string::const_iterator endMemberName2 = memberName2.end();

					// For each character of member1
					//	Is a character is common with a character of member2 ?
					//		Try to accumalte all consecutive/common characters
					//		Is the clan name coherent ?
					//			Found a new clan name !
					//	Otherwise continue
					while(itMemberName1 != endMemberName1)
					{
						while((itMemberName1 != endMemberName1) && (itMemberName2 != endMemberName2))
						//	  (itMemberName1 may changes here)
						{
							if ((*itMemberName1) == (*itMemberName2))
							{ // Found a common character
								newName += *itMemberName1;

								itMemberName1++;
								//itMemberName2++; // see below
							}
							else if (isValidClanName(newName))
							{ // Found a coherent clan name
								setName(newName);
								foundNewName = true;

								// Halt !
								itMemberName1 = endMemberName1-1;
								itMemberName2 = endMemberName2-1;
							}
							else
								newName = "";

							itMemberName2++;
						}

						itMemberName1++;
					}

					// If no coherent name was found, we set a neutral name
					if (! foundNewName)
					{
						std::ostringstream buffer;
						buffer << "Clan " << team-1; // -1, so Terro's will be 1 and CT's 2 (see TeamCode enum)
						setName(buffer.str());
					}
				}
				else // Error :-(
				{
					print("An error occured detecting a clan name !");
					setName("Nobody");
				}
			}
			else // One member in this clan => clan name = member name
			{
				IPlayerInfo * pInfo = member1->getPlayerInfo();
				if (isValidPlayer(pInfo))
				{
					name = pInfo->GetName();
				}
				else // Error :-(
				{
					print("An error occured detecting a clan name !");
					setName("Nobody");
				}
			}
		}
		else // No member in this clan
		{
			setName("Nobody");
		}
	}
}
