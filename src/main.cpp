#include "../include/stdafx.h"

#include "../include/GBMethod.h"
#include "../include/GBRouter.h"

#pragma comment(lib, "ws2_32.lib")
 
WSADATA     wsaData;
SOCKET      sock0;
SOCKET      sockw;
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
	char html[1024];
 
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
	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock0 == INVALID_SOCKET)
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
	if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
		printf("bind : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// TCP Ŭ���̾�Ʈ�κ����� ���� �䱸�� ��ٸ��鼭 ����Ѵ�
	if (listen(sock0, 5) != 0)
	{
		printf("listen : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// ���� ����
	while (1)
	{
		len = sizeof(client);
		sockw = accept(sock0, (struct sockaddr *)&client, &len);
		if (sockw == INVALID_SOCKET)
		{
			printf("accept : %d\n", WSAGetLastError());
			break;
		}
 
		// ���� �ʱ�ȭ
		memset(html, 0, 1024);
 
		// ����
		recv_len = recvfrom(sockw, buf, 1024, 0, (struct sockaddr *)&client, &sockaddr_in_size);

		std::string_view bufString(buf);
		buf[recv_len - 1] = 0;
		if ('\0' == buf[0])
		{
			strcpy(buf, NULL);
		}

		// ��� ǥ��
		std::cout << buf << '\n';

		GenericBoson::GBRouter router;
		router.m_methodList.emplace_back("GET", []()
		{
			std::cout << "test1" << std::endl;
		});
		router.m_methodList.emplace_back("PUT", []()
		{
			std::cout << "test2" << std::endl;
		});
		router.m_methodList.emplace_back("POST", []()
		{
			std::cout << "test3" << std::endl;
		});
 
		// method
		std::string_view path = router.Route(bufString);

		std::cout << "request: " << path.data() << '\n';
 
		// HTTP
		char *header =  
			"HTTP/1.0 200 OK\n"
			"Content-type: text/html\n"
			"\n";
 
		send(sockw, header, strlen(header), 0);
 
		// ����� 
		if ("/page1" == path)
		{
			strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page1</h1>\n"
			"<a href=\"/page2\">->page2</a>\n"
			"</body>"
			"</html>");
		}
		else if ("/page2" == path)
		{
			strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page2</h1>\n"
			"<a href=\"/page1\">->page1</a>\n"
			"</body>"
			"</html>");
		}
		else
		{
			strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>404- Not Found</h1>\n"
			"</body>"
			"</html>");
		}
 
		// ���䣨HTML�� �����٣�
		if (send(sockw, html, strlen(html), 0) < 1)
		{
			printf("send : %d\n", WSAGetLastError());
			break;
		}
 
		// ���� �ݱ�
		closesocket(sockw);
	}
 
	// winsock2 ���� ó��
	closesocket(sock0);
	WSACleanup();
	return 0;
}