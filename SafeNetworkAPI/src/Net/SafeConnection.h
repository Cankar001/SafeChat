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

#include <chrono>
#include <memory>
#include "SafeAsioInclude.h"

#include "DataTypes/SafeBaseTypes.h"
#include "DataTypes/SafeThreadSafeQueue.h"

#include "Net/SafeMessage.h"

template<typename T>
class SafeServer;

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

		uint64_t HandshakeOut = 0;
		uint64_t HandshakeIn = 0;
		uint64_t HandshakeCheck = 0;

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

		uint64_t Scramble(uint64_t input)
			{
			uint64_t out = input ^ 0xDEADBEEFC0DECAFE; // constants could be rotated by a dynamic degree
			out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0xF0F0F0F0F0F0F0) << 4;
			return out ^ 0xC0DEFACE12345678; // 12345678 could be a version number
			}

		void WriteValidation()
			{
			asio::async_write(Socket, asio::buffer(&HandshakeOut, sizeof(uint64_t)), [this](std::error_code ec, std::size_t length)
				{
				if (!ec)
					{
					if (MessageOwner == Owner::Client)
						ReadHeader();
					}
				else
					{
					Socket.close();
					}
				});
			}

		void ReadValidation(SafeServer<T> *server = nullptr)
			{
			asio::async_read(Socket, asio::buffer(&HandshakeIn, sizeof(uint64_t)), [this](std::error_code ec, std::size_t length)
				{
				if (!ec)
					{
					if (MessageOwner == Owner::Server)
						{
						if (HandshakeIn == HandshakeCheck) // Did the client really solve my hard puzzle?
							{
							std::cout << "Client validated!" << std::endl;
							// TODO: server OnClientValidated method call here

							ReadHeader();
							}
						else
							{
							// Client failed the puzzle! I knew my puzzles were too hard!
							std::cout << "Client disconnected (validation failed)" << std::endl;
							Socket.close();

							// Maybe we could increment a counter here and if it reaches a limit we could bann a client from the server?
							}
						}
					else
						{
						// Connection is a Client, so you have to solve the puzzle ... good luck ;-)
						HandshakeOut = Scramble(HandshakeIn);

						// yey, you solved the puzzle! let the server know that you are the hero! :D
						WriteValidation();
						}
					}
				else
					{
					std::cout << "Client Disconnected (ReadValidation)" << std::endl;
					Socket.close();
					}
				});
			}

	public:

		SafeConnection(Owner parent, asio::io_context &context, asio::ip::tcp::socket socket, SafeThreadSafeQueue<SafeOwnedMessage<T>> &messagesIn)
			: Context(context), Socket(std::move(socket)), MessagesIn(messagesIn), MessageOwner(parent)
			{

			if (parent == Owner::Server)
				{
				HandshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
				HandshakeCheck = Scramble(HandshakeOut);
				}
			else
				{
				HandshakeIn = 0;
				HandshakeOut = 0;
				}

			}

		virtual ~SafeConnection()
			{
			}

		uint32_t GetID() const
			{
			return ID;
			}

		void ConnectToClient(SafeServer<T> *server, uint32_t uid = 0)
			{
			if (MessageOwner == Owner::Server)
				{
				if (Socket.is_open())
					{
					ID = uid;
					WriteValidation();
					ReadValidation(server);
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
						ReadValidation();
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
