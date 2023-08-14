#include "stdafx.h"

#include "GBServer.h"

namespace GenericBoson
{
	

	std::pair<bool, std::string> GBServer::SetListeningSocket()
	{
#pragma region [1] Prepare and start listening port and IOCP
		// [1] - 1. WinSock 2.2 �ʱ�ȭ
		if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &m_wsaData))
		{
			return { false, "WSAStartup failed\n" };
		}

		// [1] - 2.  IOCP Ŀ�� ������Ʈ �����.
		g_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0);
		if (NULL == g_IOCP)
		{
			return { false, GetWSALastErrorString() };
		}

		m_threadPoolSize = 2 * std::thread::hardware_concurrency();
		for (int k = 0; k < m_threadPoolSize; ++k)
		{
			m_threadPool.emplace_back(&GBServer::ThreadFunction, this);
		}

		// [1] - 3.  ���� �����
		m_listeningSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == m_listeningSocket)
		{
			return { false, GetWSALastErrorString() };
		}

		// [1] - 4.  Associate the listening socket with the IOCP.
		HANDLE ret1 = CreateIoCompletionPort((HANDLE)m_listeningSocket, g_IOCP, (u_long)0, 0);

		if (NULL == ret1)
		{
			return { false, GetWSALastErrorString() };
		}

		// [1] - 5.  ���� ����
		m_addr.sin_family = AF_INET;
		m_addr.sin_port = htons(m_port);
		m_addr.sin_addr.S_un.S_addr = INADDR_ANY;

		// [1] - 6.  ���� ���ε�
		int ret2 = bind(m_listeningSocket, (struct sockaddr*)&m_addr, sizeof(m_addr));
		if (SOCKET_ERROR == ret2)
		{
			return { false, GetWSALastErrorString() };
		}

		// [1] - 7.  ������ ��Ʈ ����
		ret2 = listen(m_listeningSocket, SOMAXCONN);
		if (SOCKET_ERROR == ret2)
		{
			return { false, GetWSALastErrorString() };
		}
#pragma endregion [1] Prepare and start listening port and IOCP

#pragma region [2] Prepare AcceptEx and associate accept I/O requests to IOCP
		GUID GuidAcceptEx = WSAID_ACCEPTEX;
		DWORD returnedBytes;

		// [2] - 1. AcceptEx �Լ� ��������
		ret2 = WSAIoctl(m_listeningSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&GuidAcceptEx, sizeof(GuidAcceptEx),
			&m_lpfnAcceptEx, sizeof(m_lpfnAcceptEx),
			&returnedBytes, NULL, NULL);
		if (SOCKET_ERROR == ret2)
		{
			return { false, GetWSALastErrorString() };
		}
#pragma endregion [2] Prepare AcceptEx and associate accept I/O requests to IOCP

		return { true, {} };
	}

	int GBServer::IssueRecv(GBExpandedOverlapped* pEol, ULONG lengthToReceive)
	{
		pEol->m_type = IO_TYPE::RECEIVE;
		DWORD flag = 0;
		WSABUF wsaBuffer;
		wsaBuffer.len = lengthToReceive;			// packet length is 1 byte.
		wsaBuffer.buf = &pEol->m_recvBuffer[pEol->m_recvOffset];
		int recvResult = WSARecv(pEol->m_socket, &wsaBuffer, 1, nullptr, &flag, pEol, nullptr);

		return recvResult;
	}

	std::pair<bool, std::string> GBServer::Start()
	{
		bool result;
		std::string errorMsg;
		std::tie(result, errorMsg) = SetListeningSocket();

		m_sessions.resize(ISSUED_ACCEPTEX_COUNT);

		// AcceptEx �̽�
		for (int k = 0; k < ISSUED_ACCEPTEX_COUNT; ++k)
		{
			// AcceptEx ���ϸ����
			m_sessions[k].m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
			if (INVALID_SOCKET == m_sessions[k].m_socket)
			{
				return { false, GetWSALastErrorString() };
			}

			m_sessions[k].m_type = IO_TYPE::ACCEPT;

			// Posting an accept operation.
			DWORD returnedBytes;
			BOOL result = m_lpfnAcceptEx(m_listeningSocket, m_sessions[k].m_socket, m_listenBuffer, 0,
				sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
				&returnedBytes, &m_sessions[k]);
			int lastSocketError = WSAGetLastError();
			if (FALSE == result && ERROR_IO_PENDING != lastSocketError)
			{
				return { false, GetWSALastErrorString(lastSocketError) };
			}

			// Associate this accept socket withd IOCP.
			HANDLE associateAcceptSocketResult = CreateIoCompletionPort((HANDLE)m_sessions[k].m_socket, g_IOCP, (u_long)0, 0);
			if (NULL == associateAcceptSocketResult)
			{
				return { false, GetWSALastErrorString() };
			}
		}

		return { true, {} };
	}

	HANDLE GBServer::g_IOCP = INVALID_HANDLE_VALUE;
}