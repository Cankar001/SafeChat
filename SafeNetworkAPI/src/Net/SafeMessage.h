/*****************************************************************
 * \file   SafeMessage.h
 * \brief
 *
 * \author Can Karka
 * \date   January 2021
 *
 * Copyright (C) 2021 Can Karka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 *********************************************************************/

#pragma once

#include "DataTypes/SafeBaseTypes.h"
#include <vector>

template<typename T>
struct SafeMessageHeader
	{
	T ID{};
	uint32_t Size = 0;
	};

template<typename T>
struct SafeMessage
	{
	SafeMessageHeader<T> Header{};
	std::vector<uint8_t> Body;

	size_t Size() const
		{
		return Body.size();
		}

	template<typename DataType>
	friend SafeMessage<T> &operator<<(SafeMessage<T> &msg, const DataType &value)
		{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex");
		size_t i = msg.Body.size();

		msg.Body.resize(msg.Body.size() + sizeof(DataType));
		std::memcpy(msg.Body.data() + i, &value, sizeof(DataType));

		msg.Header.Size = (uint32_t)msg.Size();
		return msg;
		}

	template<typename DataType>
	friend SafeMessage<T> &operator>>(SafeMessage<T> &msg, DataType &value)
		{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex");
		size_t i = msg.Body.size() - sizeof(DataType);

		std::memcpy(&value, msg.Body.data() + i, sizeof(DataType));
		msg.Body.resize(i);

		msg.Header.Size = (uint32_t)msg.Size();
		return msg;
		}

	friend std::ostream &operator<<(std::ostream &stream, const SafeMessage<T> &msg)
		{
		stream << "ID: " << int(msg.Header.ID) << ", Size: " << msg.Header.Size;
		return stream;
		}
	};

template <typename T>
class SafeConnection;

template <typename T>
struct SafeOwnedMessage
	{
	std::shared_ptr<SafeConnection<T>> Remote = nullptr;
	SafeMessage<T> Message;

	friend std::ostream &operator<<(std::ostream &stream, const SafeOwnedMessage<T> &msg)
		{
		stream << msg.Message;
		return stream;
		}
	};

