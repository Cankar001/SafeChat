#include "SimpleInteractionServer.h"

int main(int argc, char *argv[])
	{
	SAFE_LOG_TRACE("Starting server...");

	// creating server with port 60000 and enabling verbose console output
	SimpleInteractionServer server(60000, true);

	// Starting the server
	server.Start();

	// Wait for clients and respond to their messages
	server.StartListening();

	return 0;
	}
