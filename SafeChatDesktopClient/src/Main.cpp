#include "SafeChatClient.h"

int main(int argc, char *arhv[])
	{
	SAFE_LOG_TRACE("Starting Client...");

	SafeClientTest c;
	c.Connect("127.0.0.1", 60000);

	// Temp keyboard input
	bool key[3] = { false, false, false };
	bool old_key[3] = { false, false, false };

	// Replace this with game loop ...
	bool quit = false;
	while (!quit)
		{
		if (GetForegroundWindow() == GetConsoleWindow())
			{
			key[0] = GetAsyncKeyState('1') & 0x8000;
			key[1] = GetAsyncKeyState('2') & 0x8000;
			key[2] = GetAsyncKeyState('3') & 0x8000;
			}

		if (key[0] == !old_key[0]) c.PingServer();
		if (key[1] == !old_key[1]) c.MessageAll();
		if (key[2] == !old_key[2]) quit = true;

		for (int i = 0; i < 3; ++i)
			old_key[i] = key[i];

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
						std::cout << "Server Accepted Connection\n";
						break;
						}

					case CustomMsgTypes::ServerPing:
						{
						std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
						std::chrono::system_clock::time_point timeThen;

						msg >> timeThen;
						std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
						break;
						}

					case CustomMsgTypes::ServerMessage:
						{
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

