#include "SafeChatClient.h"

int main(int argc, char *arhv[])
	{
	SAFE_LOG_TRACE("Starting Client...");

	SafeClientTest c;
	c.Connect("127.0.0.1", 60000);

	// Replace this with game loop ...
	while (1)
		{
		if (c.IsConnected())
			{
			if (!c.GetAllIncomingMessages().IsEmpty())
				{
				auto msg = c.GetAllIncomingMessages().Front().Message;
				c.GetAllIncomingMessages().Dequeue();

				switch (msg.Header.ID)
					{
					case CustomMsgTypes::ServerAccept:
						{
						// Server has responded to a ping request				
						std::cout << "Server Accepted Connection\n";
						break;
						}

					case CustomMsgTypes::ServerPing:
						{
						// Server has responded to a ping request
						std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
						std::chrono::system_clock::time_point timeThen;

						msg >> timeThen;
						std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
						break;
						}

					case CustomMsgTypes::ServerMessage:
						{
						// Server has responded to a ping request	
						uint32_t clientID;
						msg >> clientID;
						std::cout << "Hello from [" << clientID << "]\n";
						break;
						}
					}
				}
			}
		else
			{
			std::cout << "Server Down\n";
			break;
			}
		}

	return 0;
	}

