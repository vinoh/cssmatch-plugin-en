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

#include "UserMessagesManager.h"
#include <algorithm>

namespace cssmatch
{
	UserMessagesManager::UserMessagesManager(IVEngineServer * eng) : engine(eng)
	{
	}

	void UserMessagesManager::chatSay(	RecipientFilter & recipients,
										const std::string & message,
										int playerIndex)
	{
		bf_write * pBitBuf = engine->UserMessageBegin(&recipients,MESSAGE_SAYTEXT);
		pBitBuf->WriteByte(playerIndex);
		pBitBuf->WriteString(message.c_str());
		pBitBuf->WriteByte(1);
		engine->MessageEnd();
	}

	void UserMessagesManager::chatWarning(	RecipientFilter & recipients,
											const std::string & message)
	{
		bf_write * pBitBuf = engine->UserMessageBegin(&recipients,MESSAGE_SAYTEXT);
		pBitBuf->WriteByte(0x02);
		pBitBuf->WriteString(message.c_str());
		pBitBuf->WriteByte(0x01);
		pBitBuf->WriteByte(1);
		engine->MessageEnd();
	}

	void UserMessagesManager::popupSay(	RecipientFilter & recipients,
										const std::string & message,
										int lifeTime,
										PopupSensitivityFlags flags)
	{
		int iBegin = 0;
		int popupSize = message.size();
		bool moreToSend = false;

		do
		{
			std::string toSend = message.substr(iBegin,POPUP_MAX_SIZE);
			iBegin += POPUP_MAX_SIZE;

			bf_write * pBuffer = engine->UserMessageBegin(&recipients,MESSAGE_SHOWMENU);

			pBuffer->WriteShort(flags);
			pBuffer->WriteChar(lifeTime);

			moreToSend = iBegin < popupSize;
			pBuffer->WriteByte(moreToSend); // Is the message completed ?
			
			pBuffer->WriteString(toSend.c_str());

			engine->MessageEnd();
		}
		while(moreToSend);
	}

	void UserMessagesManager::hintSay(	RecipientFilter & recipients,
										const std::string & message)
	{
		bf_write * pWrite = engine->UserMessageBegin(&recipients,MESSAGE_HINTTEXT);
		pWrite->WriteByte(1);
		pWrite->WriteString(message.c_str());
		pWrite->WriteByte(0);
		engine->MessageEnd();
	}

	void UserMessagesManager::centerSay(RecipientFilter & recipients,
										const std::string & message)
	{
		bf_write * pWrite = engine->UserMessageBegin(&recipients,MESSAGE_TEXTMSG);
		pWrite->WriteByte(4); 
		pWrite->WriteString(message.c_str()); 
		pWrite->WriteByte(0); 
		engine->MessageEnd();
	}

	void UserMessagesManager::consoleSay(	RecipientFilter & recipients,
											const std::string & message)
	{
		const std::vector<int> * playerList = recipients.getVector();
		
		std::vector<int>::const_iterator itPlayer = playerList->begin();
		std::vector<int>::const_iterator lastPlayer = playerList->end();
		while(itPlayer != lastPlayer)
		{
			consoleTell(*itPlayer,message);
			itPlayer++;
		}
	}

	void UserMessagesManager::consoleTell(int index, const std::string & message)
	{
		edict_t * pEntity = engine->PEntityOfEntIndex(index);

		if (isValidEntity(pEntity))
			engine->ClientPrintf(pEntity,message.c_str());
	}
}
