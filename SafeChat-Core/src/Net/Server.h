#pragma once

#include <string>
#include <map>
#include <thread>
#include <functional>

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#include "Core/Core.h"

namespace SafeChat
{
	using ClientID = HSteamNetConnection;

	struct ClientInfo
	{
		ClientID ID;
		std::string ConnectionDesc;
	};

	class Server
	{
	public:

		using DataReceivedCallback = std::function<void(const ClientInfo &, const Byte*, uint32 size)>;
		using ClientConnectedCallback = std::function<void(const ClientInfo &)>;
		using ClientDisconnectedCallback = std::function<void(const ClientInfo &)>;

	public:

		Server(int32 port);
		~Server();

		void Start();
		void Stop();

		void SetDataReceivedCallback(const DataReceivedCallback &callback);
		void SetClientConnectedCallback(const ClientConnectedCallback &callback);
		void SetClientDisconnectedCallback(const ClientDisconnectedCallback &callback);

		void SendToClient(ClientID clientID, Byte *data, uint32 size, bool reliable = true);
		void SendToAllClients(Byte *data, uint32 size, ClientID excludeClientID = 0, bool reliable = true);

		void SendStringToClient(ClientID clientID, const std::string &text, bool reliable = true);
		void SendStringToAllClients(const std::string &text, ClientID excludeClientID = 0, bool reliable = true);

		template<typename T>
		void SendDataToClient(ClientID clientID, const T &data, bool reliable = true)
		{
			SendToClient(clientID, &data, sizeof(T), reliable);
		}

		template<typename T>
		void SendDataToAllClients(const T &data, ClientID excludeClientID = 0, bool reliable = true)
		{
			SendToAllClients(&data, sizeof(T), excludeClientID, reliable);
		}

		void KickClient(ClientID clientID);

		bool IsRunning() const { return m_Running; }
		const std::map<HSteamNetConnection, ClientInfo> &GetConnectedClients() const { return m_ConnectedClients; }

	private:

		void NetworkThreadFunc();

		static void ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t *info);
		void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *info);

		void PollIncomingMessages();
		void SetClientNick(HSteamNetConnection hConn, const char *nick);
		void PollConnectionStateChanges();

		void OnFatalError(const std::string &message);

	private:

		std::thread m_NetworkThread;
		DataReceivedCallback m_DataReceivedCallback;
		ClientConnectedCallback m_ClientConnectedCallback;
		ClientDisconnectedCallback m_ClientDisconnectedCallback;

		int32 m_Port = 0;
		bool m_Running = false;
		std::map<HSteamNetConnection, ClientInfo> m_ConnectedClients;

		ISteamNetworkingSockets *m_Sockets = nullptr;
		HSteamListenSocket m_ListenSocket = 0u;
		HSteamNetPollGroup m_PollGroup = 0u;
	};
}

