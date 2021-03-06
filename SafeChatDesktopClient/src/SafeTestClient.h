/*****************************************************************
 * \file   SafeTestClient.h
 * \brief
 *
 * \author Can Karka
 * \date   January 2021
 *
 * Copyright (C) 2021 Can Karka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 *********************************************************************/

#pragma once

#include <SafeNetworkAPI.h>

enum class CustomMsgTypes : uint32_t
	{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
	};

class SafeClientTest : public SafeClient<CustomMsgTypes>
	{
	public:

		void PingServer()
			{
			SafeMessage<CustomMsgTypes> msg;
			msg.Header.ID = CustomMsgTypes::ServerPing;

			// Caution with this...
			std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

			msg << timeNow;
			Send(msg);
			}

		void MessageAll()
			{
			SafeMessage<CustomMsgTypes> msg;
			msg.Header.ID = CustomMsgTypes::MessageAll;
			Send(msg);
			}


	};
