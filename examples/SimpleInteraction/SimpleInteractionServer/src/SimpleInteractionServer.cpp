#include "SimpleInteractionServer.h"

SimpleInteractionServer::SimpleInteractionServer(uint16_t port, bool verbose)
	: SafeServer<SimpleMessageType>(port, verbose)
	{
	}

SimpleInteractionServer::~SimpleInteractionServer()
	{
	}

void SimpleInteractionServer::OnClientValidated(std::shared_ptr<SafeConnection<SimpleMessageType>> client)
	{
	SAFE_LOG_TRACE("CLIENT {%d} VALIDATED SUCCESSFULLY!", client->GetID());
	}

bool SimpleInteractionServer::OnClientConnected(std::shared_ptr<SafeConnection<SimpleMessageType>> client)
	{
	SAFE_LOG_TRACE("CLIENT {%d} CONNECTED TO SERVER!", client->GetID());
	return true;
	}

void SimpleInteractionServer::OnClientDisconnected(std::shared_ptr<SafeConnection<SimpleMessageType>> client)
	{
	SAFE_LOG_TRACE("CLIENT {%d} DISCONNECTED FROM SERVER!", client->GetID());
	}

void SimpleInteractionServer::OnMessage(std::shared_ptr<SafeConnection<SimpleMessageType>> client, SafeMessage<SimpleMessageType> &msg)
	{
	switch (msg.Header.ID)
		{
		case SimpleMessageType::ServerPing:
			{
			SAFE_LOG_TRACE("CLIENT {%d} REQUESTED PING!", client->GetID());
			client->Send(msg);
			break;
			}
		}
	}

