#include "../include/stdafx.h"

#include "../include/GBMethod.h"
#include "../include/GBRouter.h"

#pragma comment(lib, "ws2_32.lib")
 
WSADATA     wsaData;
struct      sockaddr_in addr;
struct      sockaddr_in client;
#define PORT_NUM 8000

// IP �ּ� ���
int getAddrHost(void)
{
	int i;
	HOSTENT *lpHost;       //  ȣ��Ʈ ������ �����ϴ� ����ü
	IN_ADDR inaddr;       // IP �ּҸ� �����ϴ� ����ü
	char szBuf[256], szIP[16];  // ȣ��Ʈ �̸�/IP �ּҸ� �����ϴ� �迭
 
	// ���� ó��
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		printf("WSAStartup Error\n");
		return -1;
	}
 
	// ���� �ӽ��� ȣ��Ʈ �̸��� ��´�
	gethostname(szBuf, (int)sizeof(szBuf));
 
	// ȣ��Ʈ ���� ���
	lpHost = gethostbyname(szBuf);
	// IP �ּ� ���
	for (i = 0; lpHost->h_addr_list[i]; i++)
	{
		memcpy(&inaddr, lpHost->h_addr_list[i], 4);
	}

	strcpy(szIP, inet_ntoa(inaddr));
	printf("build server: http://%s:%d\n", szIP,PORT_NUM);  
 
	WSACleanup();
 
	return 0;
}

int main()
{
	int len;
	int n;
	int sockaddr_in_size = sizeof(struct sockaddr_in);
	int recv_len = 0;
	char buf[1024];
 
	//IP ��巹�� ǥ��
	if (getAddrHost() != 0) {
		printf("get IP address failed");
		getch();
		return -1;
	}
 
	// winsock2 �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 0), &wsaData))
	{
		puts("reset winsock failed");
		getch();
		return -1;
	}
 
	// ���� �����
	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningSocket == INVALID_SOCKET)
	{
		printf("socket : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// ���� ����
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUM);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
 
	// ���� ���ε�
	if (bind(listeningSocket, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
		printf("bind : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// TCP Ŭ���̾�Ʈ�κ����� ���� �䱸�� ��ٸ��鼭 ����Ѵ�
	if (listen(listeningSocket, 5) != 0)
	{
		printf("listen : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// ���� ����
	while (1)
	{
		len = sizeof(client);
		SOCKET acceptedSocket = accept(listeningSocket, (struct sockaddr *)&client, &len);
		if (acceptedSocket == INVALID_SOCKET)
		{
			printf("accept : %d\n", WSAGetLastError());
			break;
		}
 
		// ����
		recv_len = recvfrom(acceptedSocket, buf, 1024, 0, (struct sockaddr *)&client, &sockaddr_in_size);

		std::string_view bufString(buf);
		buf[recv_len - 1] = 0;
		if ('\0' == buf[0])
		{
			strcpy(buf, NULL);
		}

		// ��� ǥ��
		std::cout << buf << '\n';

		GenericBoson::GBRouter router(acceptedSocket);
		router.m_methodList.emplace_back("GET", [](const std::string_view path)
		{
			std::cout << "GET : path = " << path.data() << std::endl;
		});
		router.m_methodList.emplace_back("PUT", [](const std::string_view path)
		{
			std::cout << "PUT : path = " << path.data() << std::endl;
		});
		router.m_methodList.emplace_back("POST", [](const std::string_view path)
		{
			std::cout << "POST : path = " << path.data() << std::endl;
		});
 
		// method
		bool routingResult = router.Route(bufString);

		if (false == routingResult)
		{
			std::cout << "Routing failed." << std::endl;
		}

		// ���� �ݱ�
		closesocket(acceptedSocket);
	}
 
	// winsock2 ���� ó��
	closesocket(listeningSocket);
	WSACleanup();
	return 0;
}