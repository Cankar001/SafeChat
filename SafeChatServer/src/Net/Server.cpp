#include "SafePch.h"
#include "Server.h"

Server::Server(const std::string &ipAddr, int port)
    : IP(ipAddr), Port(port), Socket(Context)
    {
    asio::io_context::work idleWork(Context);
    Thread = std::thread([&]() { Context.run(); });

    ConnectionEndpoint = asio::ip::tcp::endpoint(asio::ip::make_address(ipAddr, ErrorCode), port);
    bool connected = Connect();
    if (connected)
        {
        std::cout << "Connected successfully!" << std::endl;
        }
    else
        {
        std::cerr << "Connection failed" << std::endl;
        }
    }

Server::~Server()
    {
    Disconnect();
    }

bool Server::Connect()
    {
    Socket.connect(ConnectionEndpoint, ErrorCode);
    if (!ErrorCode)
        return true;
    else
        return false;
    }

bool Server::IsConnected()
    {
    return Socket.is_open();
    }

void Server::Disconnect()
    {
    Socket.close();
    }

void Server::Accept()
    {
    std::vector<char> buffer(20 * 1024);

    if (IsConnected())
        {
        Socket.async_read_some(asio::buffer(buffer.data(), buffer.size()), [&](std::error_code errorCode, std::size_t length)
            {
            if (!errorCode)
                {
                // Grabbed data successfully
                std::cout << "\n\nRead " << length << " bytes.\n\n";

                for (int i = 0; i < length; i++)
                    std::cout << buffer[i];

                Accept();
                }
            else
                {
                // Error grabbing data
                }
            });
        }
    }

bool Server::Send(const std::string &buffer)
    {
    return Send(std::vector<char>(buffer.begin(), buffer.end()));
    }

bool Server::Send(const std::vector<char> &buffer)
    {
    Socket.write_some(asio::buffer(buffer.data(), buffer.size()), ErrorCode);
    if (!ErrorCode)
        return true;
    else
        return false;
    }

Server *Server::Connect(const std::string &ipAddr, int port)
    {
    return new Server(ipAddr, port);
    }

