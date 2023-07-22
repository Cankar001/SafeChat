#pragma once

#include <string>

#include "Core/Core.h"

namespace SafeChat
{
	struct UserInfo
	{
		uint32 Color;
		std::string Username;
	};

	struct ChatMessage
	{
		std::string Username;
		std::string Message;

		ChatMessage() = default;

		ChatMessage(const std::string &username, const std::string &message)
			: Username(username), Message(message)
		{
		}

		const int32 MaxMessageLength = 4096;
		bool IsValidMessage(std::string &message);
	};
}

