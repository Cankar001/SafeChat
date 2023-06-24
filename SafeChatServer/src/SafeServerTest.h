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

#include <SafeNetworkAPI.h>

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
		virtual bool OnClientConnected(const std::shared_ptr<SafeConnection<CustomMsgTypes>> &client) override
			{
			std::cout << "Client " << client->GetID() << " has connected, validating client..." << std::endl;
			return true;
			}

		virtual void OnClientDisconnected(const std::shared_ptr<SafeConnection<CustomMsgTypes>> &client) override
			{
			std::cout << "Removing client [" << client->GetID() << "]\n";
			}

		virtual void OnMessage(const std::shared_ptr<SafeConnection<CustomMsgTypes>> &client, SafeMessage<CustomMsgTypes> &msg) override
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

		virtual void OnClientValidated(const std::shared_ptr<SafeConnection<CustomMsgTypes>> &client) override
			{
			std::cout << "Client " << client->GetID() << " has been validated!" << std::endl;
			SafeMessage<CustomMsgTypes> msg;
			msg.Header.ID = CustomMsgTypes::ServerAccept;
			client->Send(msg);
			}

		SafeServerTest(uint16_t port, bool verbose = false)
			: SafeServer<CustomMsgTypes>(port, verbose)
			{
			}
	};

