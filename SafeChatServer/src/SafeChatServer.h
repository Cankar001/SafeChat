/*****************************************************************
 * \file   SafeChatServer.h
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

enum class ServerMessageTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

class SafeServerInstance : public SafeServer<ServerMessageTypes >
{
public:

	SafeServerInstance(uint16_t port, bool verbose = false);
	~SafeServerInstance();

	virtual void OnClientValidated(const std::shared_ptr<SafeConnection<ServerMessageTypes >> &client) override;

protected:
	
	virtual bool OnClientConnected(const std::shared_ptr<SafeConnection<ServerMessageTypes >> &client) override;
	virtual void OnClientDisconnected(const std::shared_ptr<SafeConnection<ServerMessageTypes >> &client) override;
	virtual void OnMessage(const std::shared_ptr<SafeConnection<ServerMessageTypes >> &client, SafeMessage<ServerMessageTypes> &msg) override;

private:


};

