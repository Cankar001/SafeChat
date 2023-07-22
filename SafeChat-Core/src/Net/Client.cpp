#include "Client.h"

#include <iostream>
#include <sstream>

namespace SafeChat
{
    // Can only have one server instance per-process
    static Client *s_Instance = nullptr;

    Client::~Client()
    {
        Disconnect();
    }

    void Client::Connect(const std::string &address)
    {
        if (m_Running)
            return;

        if (m_NetworkThread.joinable())
            m_NetworkThread.join();

        m_ServerAddress = address;
        m_NetworkThread = std::thread([this]() { NetworkThreadFunc(); });
    }
    
    void Client::Disconnect()
    {
        m_Running = false;

        if (m_NetworkThread.joinable())
            m_NetworkThread.join();
    }
    
    void Client::SetDataReceivedCallback(const DataReceivedCallback &callback)
    {
        m_DataReceivedCallback = callback;
    }
    
    void Client::SetServerConnectedCallback(const ServerConnectedCallback &callback)
    {
        m_ServerConnectedCallback = callback;
    }
    
    void Client::SetServerDisconnectedCallback(const ServerDisconnectedCallback &callback)
    {
        m_ServerDisconnectedCallback = callback;
    }
    
    void Client::SendString(const std::string &text, bool reliable)
    {
        Send(text.data(), (uint32)text.size(), reliable);
    }
    
    void Client::NetworkThreadFunc()
    {
        s_Instance = this;

        // Reset connection status
        m_ConnectionStatus = ConnectionStatus::Connecting;

        SteamDatagramErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg))
        {
            m_ConnectionDebugMessage = "Could not initialize GameNetworkingSockets";
            m_ConnectionStatus = ConnectionStatus::FailedToConnect;
            return;
        }

        // Select instance to use.  For now we'll always use the default.
        m_Sockets = SteamNetworkingSockets();

        // Start connecting
        SteamNetworkingIPAddr address;
        if (!address.ParseString(m_ServerAddress.c_str()))
        {
            std::stringstream ss;
            ss << "Invalid IP address - could not parse " << m_ServerAddress;

            OnFatalError(ss.str());
            m_ConnectionDebugMessage = "Invalid IP address";
            m_ConnectionStatus = ConnectionStatus::FailedToConnect;
            return;
        }

        SteamNetworkingConfigValue_t options;
        options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void *)ConnectionStatusChangedCallback);
        m_Connection = m_Sockets->ConnectByIPAddress(address, 1, &options);
        if (m_Connection == k_HSteamNetConnection_Invalid)
        {
            m_ConnectionDebugMessage = "Failed to create connection";
            m_ConnectionStatus = ConnectionStatus::FailedToConnect;
            return;
        }

        m_Running = true;
        while (m_Running)
        {
            PollIncomingMessages();
            PollConnectionStateChanges();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        m_Sockets->CloseConnection(m_Connection, 0, nullptr, false);
        m_ConnectionStatus = ConnectionStatus::Disconnected;
        GameNetworkingSockets_Kill();
    }
    
    void Client::Shutdown()
    {
        m_Running = false;
    }
    
    void Client::ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t *info)
    {
        s_Instance->OnConnectionStatusChanged(info);
    }
    
    void Client::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *info)
    {
        switch (info->m_info.m_eState)
        {
        case k_ESteamNetworkingConnectionState_None:
            // NOTE: We will get callbacks here when we destroy connections. You can ignore these.
            break;

        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            m_Running = false;
            m_ConnectionStatus = ConnectionStatus::FailedToConnect;
            m_ConnectionDebugMessage = info->m_info.m_szEndDebug;

            // Print an appropriate message
            if (info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
            {
                // Note: we could distinguish between a timeout, a rejected connection,
                // or some other transport problem.
                std::cout << "Could not connect to remove host. " << info->m_info.m_szEndDebug << std::endl;
            }
            else if (info->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
            {
                std::cout << "Lost connection with remote host. " << info->m_info.m_szEndDebug << std::endl;
            }
            else
            {
                // NOTE: We could check the reason code for a normal disconnection
                std::cout << "Disconnected from host. " << info->m_info.m_szEndDebug << std::endl;
            }

            // Clean up the connection.  This is important!
            // The connection is "closed" in the network sense, but
            // it has not been destroyed.  We must close it on our end, too
            // to finish up.  The reason information do not matter in this case,
            // and we cannot linger because it's already closed on the other end,
            // so we just pass 0s.
            m_Sockets->CloseConnection(info->m_hConn, 0, nullptr, false);
            m_Connection = k_HSteamNetConnection_Invalid;
            m_ConnectionStatus = ConnectionStatus::Disconnected;
            break;
        }

        case k_ESteamNetworkingConnectionState_Connecting:
            // We will get this callback when we start connecting.
            m_ConnectionStatus = ConnectionStatus::Connecting;
            break;

        case k_ESteamNetworkingConnectionState_Connected:
            m_ConnectionStatus = ConnectionStatus::Connected;
            m_ServerConnectedCallback();
            break;

        default:
            break;
        }
    }
    
    void Client::PollIncomingMessages()
    {
        // Process all messages
        while (m_Running)
        {
            ISteamNetworkingMessage *incomingMessage = nullptr;
            int messageCount = m_Sockets->ReceiveMessagesOnConnection(m_Connection, &incomingMessage, 1);
            if (messageCount == 0)
                break;

            if (messageCount < 0)
            {
                // messageCount < 0 means critical error?
                m_Running = false;
                return;
            }

            m_DataReceivedCallback((Byte*)incomingMessage->m_pData, incomingMessage->m_cbSize);

            // Release when done
            incomingMessage->Release();
        }
    }
    
    void Client::PollConnectionStateChanges()
    {
        m_Sockets->RunCallbacks();
    }
    
    void Client::OnFatalError(const std::string &message)
    {
        std::cout << message << std::endl;
        m_Running = false;
    }
}

