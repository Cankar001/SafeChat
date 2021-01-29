/*****************************************************************
 * \file   SimpleInteractionServer.h
 * \brief  This simple Interaction Server just can accept a client/deny a client and respond to a client ping request.
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

enum class SimpleMessageType : uint32_t
	{
	ServerPing
	};

class SimpleInteractionServer : public SafeServer<SimpleMessageType>
	{
	public:

		/**
		 * 
		 * constructor which initializes the parent server instance with the port number to listen to.
		 * 
		 * @param port The port number where the server should listen on for new clients to connect.
		 */
		SimpleInteractionServer(uint16_t port, bool verbose = false);

		/**
		 * 
		 * standard destructor with nothing to do.
		 * 
		 */
		~SimpleInteractionServer();

		/**
		 * 
		 * If a client was validated to not be an enemy attacker this callback method is called.
		 * 
		 * @param server 
		 */
		virtual void OnClientValidated(std::shared_ptr<SafeConnection<SimpleMessageType>> server) override;

	protected:

		/**
		 * 
		 * .
		 * 
		 * @param client
		 * @return 
		 */
		virtual bool OnClientConnected(std::shared_ptr<SafeConnection<SimpleMessageType>> client) override;

		/**
		 * 
		 * .
		 * 
		 * @param client
		 */
		virtual void OnClientDisconnected(std::shared_ptr<SafeConnection<SimpleMessageType>> client) override;

		/**
		 * 
		 * .
		 * 
		 * @param client
		 * @param msg
		 */
		virtual void OnMessage(std::shared_ptr<SafeConnection<SimpleMessageType>> client, SafeMessage<SimpleMessageType> &msg) override;
	};
