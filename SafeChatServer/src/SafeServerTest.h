/*****************************************************************
 * \file   SafeServerTest.h
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

#include "SafeNetworkAPI.h"

enum class CustomMsgTypes : uint32_t
	{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
	};

class SafeServerTest : public SafeServer<CustomMsgTypes>
	{
	protected:
		virtual bool OnClientConnected(std::shared_ptr<SafeConnection<CustomMsgTypes>> client)
			{
			SafeMessage<CustomMsgTypes> msg;
			msg.Header.ID = CustomMsgTypes::ServerAccept;
			client->Send(msg);
			return true;
			}

		virtual void OnClientDisconnect(std::shared_ptr<SafeConnection<CustomMsgTypes>> client)
			{
			std::cout << "Removing client [" << client->GetID() << "]\n";
			}

		virtual void OnMessage(std::shared_ptr<SafeConnection<CustomMsgTypes>> client, SafeMessage<CustomMsgTypes> &msg)
			{
			switch (msg.Header.ID)
				{
				case CustomMsgTypes::ServerPing:
					{
					std::cout << "[" << client->GetID() << "]: Server Ping\n";

					// Simply bounce message back to client
					client->Send(msg);
					break;
					}

				case CustomMsgTypes::MessageAll:
					{
					std::cout << "[" << client->GetID() << "]: Message All\n";

					// Construct a new message and send it to all clients
					SafeMessage<CustomMsgTypes> msg;
					msg.Header.ID = CustomMsgTypes::ServerMessage;
					msg << client->GetID();
					MessageAllClients(msg, client);
					break;
					}
				}
			}

	public:

		virtual void OnClientValidated(std::shared_ptr<SafeConnection<CustomMsgTypes>> server) override
			{
			std::cout << "Client " << server->GetID() << " has been validated!" << std::endl;
			}

		SafeServerTest(uint16_t port)
			: SafeServer<CustomMsgTypes>(port)
			{
			}
	};

