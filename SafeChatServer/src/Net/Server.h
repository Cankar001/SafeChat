/*****************************************************************
 * \file   Server.h
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

#include <AsioInclude.h>

/**
 * 
 * .
 * 
 */
class Server
	{
	private:
		std::thread Thread;
		std::string IP;
		int Port;

		asio::error_code ErrorCode;
		asio::io_context Context;
		asio::ip::tcp::endpoint ConnectionEndpoint;
		asio::ip::tcp::socket Socket;

	public:

		Server(const std::string &ipAddr, int port);
		~Server();

		bool Connect();
		bool IsConnected();
		void Disconnect();
		void Accept();
		bool Send(const std::string &buffer);
		bool Send(const std::vector<char> &buffer);

		static Server *Connect(const std::string &ipAddr, int port);
	};
