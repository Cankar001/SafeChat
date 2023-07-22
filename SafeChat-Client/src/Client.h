#pragma once

#include <SafeChat.h>

namespace SafeChatClient
{
	class Client
	{
	public:

		Client();
		~Client();



	private:

		SafeChat::Client m_Client;
	};
}

