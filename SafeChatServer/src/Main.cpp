#include "SafeChatServer.h"

int main(int argc, char *argv[])
	{
	SAFE_LOG_TRACE("Starting server...");
	
	SafeServerTest server(60000);
	server.Start();

	while (1)
		{
		server.Update(-1, true);
		}

	return 0;
	}

