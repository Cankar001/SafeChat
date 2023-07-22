#pragma once

#include <thread>
#include <functional>
#include <string>

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#include "Core/Core.h"

namespace SafeChat
{
	class Client
	{
	public:

		enum class ConnectionStatus
		{
			Disconnected = 0,
			Connecting,
			Connected,
			FailedToConnect
		};

	public:

		using DataReceivedCallback = std::function<void(const Byte*, uint32 size)>;
		using ServerConnectedCallback = std::function<void()>;
		using ServerDisconnectedCallback = std::function<void()>;

	public:

		Client() = default;
		~Client();

		void Connect(const std::string &address);
		void Disconnect();

		void SetDataReceivedCallback(const DataReceivedCallback &callback);
		void SetServerConnectedCallback(const ServerConnectedCallback &callback);
		void SetServerDisconnectedCallback(const ServerDisconnectedCallback &callback);

		void SendString(const std::string &text, bool reliable = true);

		template<typename T>
		void Send(const T &data, uint32 size, bool reliable = true)
		{
			EResult result = m_Sockets->SendMessageToConnection(m_Connection, data, size, reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable, nullptr);

		}

		bool IsRunning() const { return m_Running; }
		ConnectionStatus GetConnectionStatus() const { return m_ConnectionStatus; }
		const std::string &GetDebugConnectionMessage() const { return m_ConnectionDebugMessage; }

	private:

		void NetworkThreadFunc();
		void Shutdown();

	private:

		static void ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t *info);
		void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *info);

		void PollIncomingMessages();
		void PollConnectionStateChanges();

		void OnFatalError(const std::string &message);

	private:

		std::thread m_NetworkThread;
		std::string m_ConnectionDebugMessage;
		bool m_Running = false;
		std::string m_ServerAddress;

		DataReceivedCallback m_DataReceivedCallback;
		ServerConnectedCallback m_ServerConnectedCallback;
		ServerDisconnectedCallback m_ServerDisconnectedCallback;
		ConnectionStatus m_ConnectionStatus = ConnectionStatus::Disconnected;

		ISteamNetworkingSockets *m_Sockets = nullptr;
		HSteamNetConnection m_Connection = 0;
	};
}

