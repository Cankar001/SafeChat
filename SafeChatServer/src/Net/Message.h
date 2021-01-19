#pragma once

#include "AsioInclude.h"

struct Header
	{
	int32_t ID = 0;
	uint32_t Size = 0;
	};

struct Message
	{
	Header MessageHeader;
	std::vector<uint8_t> MessageBody;

	size_t Size() const
		{
		return sizeof(Header) + MessageBody.size();
		}

	template<typename DataType>
	friend Message &operator<<(Message &msg, DataType &data)
		{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex");

		size_t cachedSize = msg.MessageBody.size();
		msg.MessageBody.resize(msg.MessageBody.size() + sizeof(DataType));
		std::memcpy(msg.MessageBody.data() + cachedSize, &data, sizeof(DataType));

		msg.MessageHeader.Size = msg.Size();
		return msg;
		}

	template<typename DataType>
	friend Message &operator>>(Message &msg, DataType &data)
		{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex");

		size_t cachedSize = msg.MessageBody.size() - sizeof(DataType);
		std::memcpy(&data, msg.MessageBody.size() + cachedSize, sizeof(DataType));
		msg.MessageBody.resize(cachedSize);

		msg.MessageHeader.Size = msg.Size();
		return msg;
		}

	friend std::ostream &operator<<(std::ostream &stream, const Message &msg)
		{
		stream << "ID: " << msg.MessageHeader.ID << ", Size: " << msg.MessageHeader.Size;
		return stream;
		}
	};
