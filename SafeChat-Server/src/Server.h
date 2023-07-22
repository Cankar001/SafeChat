#pragma once

#include <SafeChat.h>

namespace SafeChatServer
{
	class Server
	{
	public:
		Server();
		~Server();

	private:

		SafeChat::Server m_Server;
	};
}