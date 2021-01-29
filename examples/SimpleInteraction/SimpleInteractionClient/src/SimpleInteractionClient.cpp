#include "SimpleInteractionClient.h"

void SimpleInteractionClient::Ping()
	{
	// Create a message object
	SafeMessage<SimpleMessageType> msg;

	// set the message ID
	msg.Header.ID = SimpleMessageType::ServerPing;

	// Get the current time
	std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

	// Append the current time to the message
	msg << timeNow;

	// Send the message to the server
	Send(msg);
	}

