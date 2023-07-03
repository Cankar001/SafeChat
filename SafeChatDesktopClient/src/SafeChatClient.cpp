#include "SafeChatClient.h"

SafeClientInstance::SafeClientInstance()
{
}

SafeClientInstance::~SafeClientInstance()
{
}

void SafeClientInstance::PingServer()
{
	SafeMessage<ClientMessageTypes> msg;
	msg.Header.ID = ClientMessageTypes::ServerPing;

	// Caution with this...
	std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

	msg << timeNow;
	Send(msg);
}

void SafeClientInstance::MessageAll()
{
	SafeMessage<ClientMessageTypes> msg;
	msg.Header.ID = ClientMessageTypes::MessageAll;
	Send(msg);
}

