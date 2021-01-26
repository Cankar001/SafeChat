/*****************************************************************
 * \file   SafeConnection.h
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

#include "DataTypes/SafeBaseTypes.h"
#include "DataTypes/SafeThreadSafeQueue.h"

#include "Net/SafeMessage.h"

template<typename T>
class SafeConnection : public std::enable_shared_from_this<SafeConnection<T>>
	{
	public:
		enum class Owner
			{
			Server, Client
			};

	protected:
		asio::ip::tcp::socket Socket;
		asio::io_context &Context;

		uint32_t ID = 0;
		Owner MessageOwner = Owner::Server;
		SafeMessage<T> TempMessage;

		SafeThreadSafeQueue<SafeMessage<T>> MessagesOut;
		SafeThreadSafeQueue<SafeOwnedMessage<T>> &MessagesIn;

	private:

		void WriteHeader()
			{
			asio::async_write(Socket, asio::buffer(&MessagesOut.Front().Header, sizeof(SafeMessageHeader<T>)),
				[this](std::error_code ec, std::size_t length)
				{
				if (!ec)
					{
					if (MessagesOut.Front().Body.size() > 0)
						{
						WriteBody();
						}
					else
						{
						MessagesOut.Dequeue();

						if (!MessagesOut.IsEmpty())
							{
							WriteHeader();
							}
						}
					}
				else
					{
					std::cout << "[" << ID << "] Write Header Fail.\n";
					Socket.close();
					}
				});
			}

		void WriteBody()
			{
			asio::async_write(Socket, asio::buffer(MessagesOut.Front().Body.data(), MessagesOut.Front().Body.size()), [this](std::error_code ec, std::size_t length)
				{
				if (!ec)
					{
					MessagesOut.Dequeue();

					if (!MessagesOut.IsEmpty())
						{
						WriteHeader();
						}
					}
				else
					{
					std::cout << "[" << ID << "] Write Body Fail.\n";
					Socket.close();
					}
				});
			}

		void ReadHeader()
			{
			asio::async_read(Socket, asio::buffer(&TempMessage, sizeof(SafeMessageHeader<T>)),
				[this](std::error_code ec, std::size_t length)
				{
				if (!ec)
					{
					if (TempMessage.Header.Size > 0)
						{
						TempMessage.Body.resize(TempMessage.Header.Size);
						ReadBody();
						}
					else
						{
						AddToMessageQueue();
						}
					}
				else
					{
					std::cout << "[" << ID << "] Read Header Fail.\n";
					Socket.close();
					}
				});
			}

		void ReadBody()
			{
			asio::async_read(Socket, asio::buffer(TempMessage.Body.data(), TempMessage.Body.size()),
				[this](std::error_code ec, std::size_t length)
				{
				if (!ec)
					{
					AddToMessageQueue();
					}
				else
					{
					std::cout << "[" << ID << "] Read Body Fail.\n";
					Socket.close();
					}
				});
			}

		void AddToMessageQueue()
			{
			if (MessageOwner == Owner::Server)
				MessagesIn.Enqueue({ this->shared_from_this(), TempMessage });
			else
				MessagesIn.Enqueue({ nullptr, TempMessage });

			ReadHeader();
			}

	public:

		SafeConnection(Owner parent, asio::io_context &context, asio::ip::tcp::socket socket, SafeThreadSafeQueue<SafeOwnedMessage<T>> &messagesIn)
			: Context(context), Socket(std::move(socket)), MessagesIn(messagesIn), MessageOwner(parent)
			{
			}

		virtual ~SafeConnection()
			{
			}

		uint32_t GetID() const
			{
			return ID;
			}

		void ConnectToClient(uint32_t uid = 0)
			{
			if (MessageOwner == Owner::Server)
				{
				if (Socket.is_open())
					{
					ID = uid;
					ReadHeader();
					}
				}
			}

		void ConnectToServer(const asio::ip::tcp::resolver::results_type &endpoints)
			{
			if (MessageOwner == Owner::Client)
				{
				// Request asio attempts to connect to an endpoint
				asio::async_connect(Socket, endpoints, [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
					{
					if (!ec)
						{
						ReadHeader();
						}
					});
				}
			}

		void Disconnect()
			{
			if (IsConnected())
				asio::post(Context, [this]() { Socket.close(); });
			}

		bool IsConnected() const
			{
			return Socket.is_open();
			}

		/*
		void StartListening()
			{

			}
		*/

		void Send(const SafeMessage<T> &msg)
			{
			asio::post(Context, [this, msg]()
				{
				bool writingMsg = !MessagesOut.IsEmpty();
				MessagesOut.Enqueue(msg);
				if (!writingMsg)
					WriteHeader();
				});
			}
	};
