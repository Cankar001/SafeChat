#include "SafeChatServer.h"

SafeServerInstance::SafeServerInstance(uint16_t port, bool verbose)
	: SafeServer<ServerMessageTypes>(port, verbose)
{
}

SafeServerInstance::~SafeServerInstance()
{
}

bool SafeServerInstance::OnClientConnected(const std::shared_ptr<SafeConnection<ServerMessageTypes>> &client)
{
	std::cout << "Client " << client->GetID() << " has connected, validating client..." << std::endl;
	return true;
}

void SafeServerInstance::OnClientDisconnected(const std::shared_ptr<SafeConnection<ServerMessageTypes>> &client)
{
	std::cout << "Removing client [" << client->GetID() << "]\n";
}

void SafeServerInstance::OnMessage(const std::shared_ptr<SafeConnection<ServerMessageTypes>> &client, SafeMessage<ServerMessageTypes> &msg)
{
	switch (msg.Header.ID)
	{
		case ServerMessageTypes::ServerPing:
		{
			std::cout << "[" << client->GetID() << "]: Server Ping\n";

			// Simply bounce message back to client
			client->Send(msg);
			break;
		}

		case ServerMessageTypes::MessageAll:
		{
			std::cout << "[" << client->GetID() << "]: Message All\n";

			// Construct a new message and send it to all clients
			SafeMessage<ServerMessageTypes> msg;
			msg.Header.ID = ServerMessageTypes::ServerMessage;
			msg << client->GetID();
			MessageAllClients(msg, client);
			break;
		}
	}
}

void SafeServerInstance::OnClientValidated(const std::shared_ptr<SafeConnection<ServerMessageTypes>> &client)
{
	std::cout << "Client " << client->GetID() << " has been validated!" << std::endl;
	SafeMessage<ServerMessageTypes> msg;
	msg.Header.ID = ServerMessageTypes::ServerAccept;
	client->Send(msg);
}

