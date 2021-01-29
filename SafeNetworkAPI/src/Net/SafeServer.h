/*****************************************************************
 * \file   SafeServer.h
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

#include <memory>
#include <deque>
#include "SafeAsioInclude.h"

#include "DataTypes/SafeThreadSafeQueue.h"

#include "Net/SafeMessage.h"
#include "Net/SafeConnection.h"

template<typename T>
class SafeServer
	{
	protected:
		SafeThreadSafeQueue<SafeOwnedMessage<T>> MessagesIn;
		std::deque<std::shared_ptr<SafeConnection<T>>> Connections;
		asio::io_context Context;
		std::thread Thread;

		asio::ip::tcp::acceptor AsioAcceptor; // Handles new incoming connection attempts...
		uint32_t nIDCounter = 10000; // Clients will be identified in the "wider system" via an ID

		bool PrintVerbose;

		virtual bool OnClientConnected(std::shared_ptr<SafeConnection<T>> client)
			{
			return false;
			}

		virtual void OnClientDisconnected(std::shared_ptr<SafeConnection<T>> client)
			{
			}

		virtual void OnMessage(std::shared_ptr<SafeConnection<T>> client, SafeMessage<T> &msg)
			{
			}

	public:

		virtual void OnClientValidated(std::shared_ptr<SafeConnection<T>> client)
			{
			}

	public:

		SafeServer(uint16_t port, bool verbose = false)
			: AsioAcceptor(Context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), PrintVerbose(verbose)
			{
			}

		virtual ~SafeServer()
			{
			Stop();
			}

		bool Start()
			{
			try
				{
				WaitForClientConnection();
				Thread = std::thread([this]() { Context.run(); });
				}
			catch (std::exception &e)
				{
				if (PrintVerbose)
					SAFE_LOG_ERROR("Exception: %s", e.what());

				return false;
				}

			if (PrintVerbose)
				SAFE_LOG_TRACE("Server Started!");
			return true;
			}

		void Stop()
			{
			Context.stop();

			if (Thread.joinable())
				Thread.join();

			if (PrintVerbose)
				SAFE_LOG_TRACE("Server Stopped!");
			}

		void WaitForClientConnection()
			{
			// Prime context with an instruction to wait until a socket connects. This
			// is the purpose of an "acceptor" object. It will provide a unique socket
			// for each incoming connection attempt
			AsioAcceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket)
				{
				if (!ec)
					{
					//std::cout << "Server New Connection: " << socket.remote_endpoint() << "\n";
					if (PrintVerbose)
						SAFE_LOG_TRACE("[SERVER] New Connection: %s:%d", socket.remote_endpoint().address().to_string().c_str(), socket.remote_endpoint().port());

					std::shared_ptr<SafeConnection<T>> newconn =
						std::make_shared<SafeConnection<T>>(SafeConnection<T>::Owner::Server,
							Context, std::move(socket), MessagesIn);

					if (OnClientConnected(newconn))
						{
						Connections.push_back(std::move(newconn));
						Connections.back()->ConnectToClient(this, nIDCounter++);

						if (PrintVerbose)
							SAFE_LOG_TRACE("[%d] Connection Approved!", Connections.back()->GetID());
						}
					else
						{
						if (PrintVerbose)
							SAFE_LOG_WARN("[-----] Connection Denied");
						}
					}
				else
					{
					if (PrintVerbose)
						SAFE_LOG_ERROR("[SERVER] New Connection Error: %s", ec.message().c_str());
					}

				WaitForClientConnection();
				});
			}

		void MessageClient(std::shared_ptr<SafeConnection<T>> client, const SafeMessage<T> &msg)
			{
			if (client && client->IsConnected())
				{
				client->Send(msg);
				}
			else
				{
				OnClientDisconnected(client);
				client.reset();

				Connections.erase(
					std::remove(Connections.begin(), Connections.end(), client), Connections.end());
				}
			}

		void MessageAllClients(const SafeMessage<T> &msg, std::shared_ptr<SafeConnection<T>> ignoreClient = nullptr)
			{
			bool invalidClientExists = false;

			for (auto &client : Connections)
				{
				if (client && client->IsConnected())
					{
					if (client != ignoreClient)
						client->Send(msg);
					}
				else
					{
					OnClientDisconnected(client);
					client.reset();
					invalidClientExists = true;
					}
				}

			if (invalidClientExists)
				Connections.erase(
					std::remove(Connections.begin(), Connections.end(), nullptr), Connections.end());
			}

		void StartListening()
			{
			while (1)
				Listen(-1, true);
			}

		void Listen(size_t maxMessages = -1, bool wait = false)
			{
			if (wait) MessagesIn.Wait();

			size_t messageCount = 0;
			while (messageCount < maxMessages && !MessagesIn.IsEmpty())
				{
				auto msg = MessagesIn.Front();
				MessagesIn.Dequeue();

				OnMessage(msg.Remote, msg.Message);
				messageCount++;
				}
			}
	};

