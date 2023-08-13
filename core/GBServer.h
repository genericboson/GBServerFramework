#pragma once

#include "GBExpandedOverlapped.h"
#include "winsock2.h"
#include "MSWSock.h"

#include <thread>
#include <vector>
#include <string>

namespace GenericBoson
{
	const int ISSUED_ACCEPTEX_COUNT = 100;// SOMAXCONN / sizeof(ExpandedOverlapped) / 200;

	class GBServer
	{
	public:
		std::pair<bool, std::string> Start();

		GBServer() : m_port(8000) {};
		GBServer(uint16_t portNum) : m_port(portNum) {};
		virtual ~GBServer();

	private:
		std::string GetWSALastErrorString();
		std::string GetWSALastErrorString(int lastError);
		std::pair<bool, std::string> SetListeningSocket();

		void ThreadFunction();

		// \return true - all completed, false - not yet gathering completed.
		virtual bool OnReceived(GBExpandedOverlapped* pEol, DWORD receivedBytes) = 0;
		virtual bool OnSent(GBExpandedOverlapped* pEol, DWORD sentBytes) = 0;

		static int IssueRecv(GBExpandedOverlapped* pEol, ULONG lengthToReceive);
	private:
		int m_threadPoolSize = 0;
		std::vector<std::thread> m_threadPool;
		std::vector<GBExpandedOverlapped> m_sessions;

		WSADATA m_wsaData;
		sockaddr_in m_addr, m_client;

		SOCKET m_listeningSocket;
		
		// AcceptEx �Լ� ������
		LPFN_ACCEPTEX m_lpfnAcceptEx = NULL;

		uint16_t m_port = 0;

		int m_addrSize = sizeof(sockaddr_in);

		// ���� : ���� ����� �������� �־�� ����� �����Ѵ�.
		char m_listenBuffer[1024] = { 0, };

		static HANDLE g_IOCP;
		static volatile bool g_keepLooping;
	};
}