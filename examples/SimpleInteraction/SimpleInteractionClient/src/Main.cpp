#include "SimpleInteractionClient.h"

int main(int argc, char *argv[])
	{
	SAFE_LOG_TRACE("Starting client...");

	// Create client instance
	SimpleInteractionClient client;

	// Connect to a server using a IP address and a port
	client.Connect("127.0.0.1", 60000);

	SAFE_LOG_TRACE("Command List:");
	SAFE_LOG_TRACE("- type ping to send a ping request to the sever");
	SAFE_LOG_TRACE("- type close to shut down the client");

	bool shouldQuit = false;
	while (!shouldQuit)
		{
		// Read console input
		std::string cmd;
		std::cout << "Your Command: ";
		std::cin >> cmd;

		// Check if the user wants to send a ping request
		if (cmd == "ping")
			{
			// Send the ping request to the server
			client.Ping();
			}
		// Check if the user wants to close the client
		else if (cmd == "close")
			{
			// mark the client to shut down in the next cycle
			shouldQuit = true;
			}

		// Check if the client is still connected
		if (client.IsConnected())
			{
			// Check if the message queue is not empty
			if (!client.GetAllIncomingMessages().IsEmpty())
				{
				// get the last message sent from the server
				auto msg = client.GetAllIncomingMessages().Front().Message;

				// remove the message from the message queue
				client.GetAllIncomingMessages().Dequeue();

				// Respond to the message
				switch (msg.Header.ID)
					{
					case SimpleMessageType::ServerPing:
						{
						// Get the current time again
						std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

						// create the time point for the ping which was sent by the server
						std::chrono::system_clock::time_point timeThen;

						// receive the ping sent by the server from the message object
						msg >> timeThen;

						// calculate the actual ping time
						double pingTime = std::chrono::duration<double>(timeNow - timeThen).count();

						// Print the pingTime to the console
						SAFE_LOG_TRACE("PING: %f", pingTime);
						break;
						}
					}
				}

			// Proceed with more client stuff ... (e.g. rendering the GUI)
			}
		else
			{
			// connection was interrupted
			SAFE_LOG_ERROR("CLIENT LOST CONNECTION");
			break;
			}
		}

	return 0;
	}
