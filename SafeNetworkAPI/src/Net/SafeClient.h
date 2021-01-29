/*****************************************************************
 * \file   SafeClient.h
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

#include "SafeAsioInclude.h"
#include "Net/SafeConnection.h"

template<typename T>
class SafeClient
	{
	protected:
		asio::io_context Context;
		std::thread Thread;
		std::unique_ptr<SafeConnection<T>> Connection;

	private:
		SafeThreadSafeQueue<SafeOwnedMessage<T>> MessagesIn;
		bool PrintVerbose;

	public:

		SafeClient(bool verbose = false)
			: PrintVerbose(verbose)
			{
			}

		virtual ~SafeClient()
			{
			Disconnect();
			}

		bool Connect(const std::string &host, const uint16_t port)
			{
			try
				{
				// Resolve hostname/ip-address into tangiable physical address
				asio::ip::tcp::resolver resolver(Context);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

				Connection = std::make_unique<SafeConnection<T>>(SafeConnection<T>::Owner::Client, Context, asio::ip::tcp::socket(Context), MessagesIn);
				Connection->ConnectToServer(endpoints);
				Thread = std::thread([this]() { Context.run(); });
				}
			catch (std::exception &e)
				{
				if (PrintVerbose)
					SAFE_LOG_ERROR("Client Exception: %s", e.what());
				return false;
				}

			return true;
			}

		void Disconnect()
			{
			if (IsConnected())
				Connection->Disconnect();

			Context.stop();

			if (Thread.joinable())
				Thread.join();

			Connection.release();
			}

		bool IsConnected()
			{
			if (Connection)
				return Connection->IsConnected();

			return false;
			}

		void Send(const SafeMessage<T> &msg)
			{
			if (IsConnected())
				Connection->Send(msg);
			}

		SafeThreadSafeQueue<SafeOwnedMessage<T>> &GetAllIncomingMessages()
			{
			return MessagesIn;
			}
	};
