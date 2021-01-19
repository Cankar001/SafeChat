/*****************************************************************
 * \file   Main.cpp
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

#include "SafePch.h"

#include "SafeChat.h"
#include "DataTypes/DataTypes.h"

#include "Net/Server.h"

int main(int argc, char *argv[])
	{
	SAFE_LOG_TRACE("Starting server...");

	std::string request =
		"GET /index.php HTTP/1.1\r\n"
		"Host: cankarka.com\r\n"
		"Connection: close\r\n\r\n";

	Server *server = Server::Connect("85.13.162.217", 80);
	server->Accept();

	bool sent = server->Send(request);
	if (!sent)
		{
		std::cerr << "Could not send data!" << std::endl;
		}

	return 0;
	}

