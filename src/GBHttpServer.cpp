#include "../include/stdafx.h"

#include "../include/GBHttpServer.h"
#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	GBHttpServer::GBHttpServer(uint16_t port)
	{
		// winsock2 �ʱ�ȭ
		if (WSAStartup(MAKEWORD(2, 0), &m_wsaData))
		{
			std::cout << "winsock startup failed\n";
			return;
		}

		// ���� �����
		 m_listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_listeningSocket == INVALID_SOCKET)
		{
			std::cout << "socket : " << WSAGetLastError() << '\n';
			return;
		}

		// ���� ����
		m_addr.sin_family = AF_INET;
		m_addr.sin_port = htons(port);
		m_addr.sin_addr.S_un.S_addr = INADDR_ANY;

		// ���� ���ε�
		if (bind(m_listeningSocket, (struct sockaddr *)&m_addr, sizeof(m_addr)) != 0)
		{
			std::cout << "bind : " << WSAGetLastError() << '\n';
			return;
		}

		// TCP Ŭ���̾�Ʈ�κ����� ���� �䱸�� ��ٸ��鼭 ����Ѵ�
		if (listen(m_listeningSocket, 5) != 0)
		{
			std::cout << "listen : " << WSAGetLastError() << '\n';
			return;
		}
	}

	GBHttpServer::~GBHttpServer()
	{
		// winsock2 ���� ó��
		closesocket(m_listeningSocket);
		WSACleanup();
	}

	bool GBHttpServer::Start()
	{
		// ���� ����
		while (1)
		{
			SOCKET acceptedSocket = accept(m_listeningSocket, (sockaddr*)&m_client, &m_addrSize);
			if (acceptedSocket == INVALID_SOCKET)
			{
				std::cout << "accept : " << WSAGetLastError() << '\n';
				return false;
			}

			// ����
			int recved = recvfrom(acceptedSocket, m_buffer, 1024, 0, (sockaddr*)&m_client, &m_addrSize);

			std::string_view bufString(m_buffer);
			m_buffer[recved - 1] = 0;
			if ('\0' == m_buffer[0])
			{
				strcpy(m_buffer, NULL);
			}

#if defined(_DEBUG)
			// ��� ǥ��
			GBCout << m_buffer << '\n';
#endif
			GenericBoson::GBHttpRequestLineReader requestLineReader(acceptedSocket);
			bool ret = requestLineReader.Read(m_buffer);
			if (false == ret)
			{
				std::cout << "An abnormal line exists in HTTP message.\n";
				return false;
			}

			//GenericBoson::GBHttpRouter router(acceptedSocket);
			//router.m_methodList.emplace_back("GET", [](const std::string_view path)
			//{
			//	std::cout << "GET : path = " << path.data() << std::endl;
			//});
			//router.m_methodList.emplace_back("PUT", [](const std::string_view path)
			//{
			//	std::cout << "PUT : path = " << path.data() << std::endl;
			//});
			//router.m_methodList.emplace_back("POST", [](const std::string_view path)
			//{
			//	std::cout << "POST : path = " << path.data() << std::endl;
			//});

			//// method
			//bool routingResult = router.Route(bufString);

			//if (false == routingResult)
			//{
			//	std::cout << "Routing failed." << '\n';
			//	return false;
			//}

			// ���� �ݱ�
			closesocket(acceptedSocket);
		}

		return true;
	}
}