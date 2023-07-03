#include "SafeChatServer.h"

int main(int argc, char *argv[])
{
	SAFE_LOG_TRACE("Starting server...");
	
	SafeServerInstance server(60000, true);
	server.Start();

	while (1)
	{
		server.Listen(-1, true);
	}

	return 0;
}

