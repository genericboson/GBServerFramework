#include "GBHttpServer.h"

namespace GenericBoson
{
	//bool GBHttpServer::TraversePathTree(const std::vector<std::string_view>& pathTree, const std::map<std::string_view, std::string_view> queryMap, PathSegment*& pTargetPath)
	//{
	//	for (const auto& iPathSegment : pathTree)
	//	{
	//		// ���� true == pTargetPath->m_subTreeMap.contains(iPathSegment) ���µ�,
	//		// travis�� VS2017������ �����ؼ� �Ʒ��� ���� �ٲ�.
	//		if (pTargetPath->m_subTreeMap.end() == pTargetPath->m_subTreeMap.find(iPathSegment))
	//		{
	//			pTargetPath->m_subTreeMap.emplace(iPathSegment, std::make_unique<PathSegment>());
	//		}

	//		pTargetPath = (pTargetPath->m_subTreeMap[iPathSegment]).get();
	//	}

	//	if (nullptr != pTargetPath->m_pGetMethod)
	//	{
	//		// #ToDo
	//		// The action method Already Exists at the path.
	//		return false;
	//	}

	//	return true;
	//}

	bool GBServer::OnSent(GBExpandedOverlapped* pEol, DWORD sentBytes)
	{
		return true;
	}

	bool GBServer::OnReceived(GBExpandedOverlapped* pEol, DWORD receivedBytes)
	{
		bool succeeded = false;

		//bool parseResult = pEol->GatherAndParseLines(receivedBytes);
		pEol->m_recvOffset += receivedBytes;

		bool gatheringNotFinished = false;
		bool gatheringFinishedButNothing = false;

		/*if (false == parseResult)
		{
			gatheringNotFinished = true;
		}
		else if (0 == pEol->m_recvOffset)
		{
			gatheringFinishedButNothing = true;
		}*/

		// �������� ������ �ʾҰų�, ����� ������ �������ٸ�, �� �������� �Ѵ�.
		if (true == gatheringNotFinished || true == gatheringFinishedButNothing)
		{
			int issueRecvResult = IssueRecv(pEol, BUFFER_SIZE - pEol->m_recvOffset);
			int lastError = WSAGetLastError();

			if (SOCKET_ERROR == issueRecvResult && WSA_IO_PENDING != lastError)
			{
				// #ToDo
				// Issue receiving failed.
			}

			return false;
		}

		GBHttpRequestReader requestReader(pEol);
		HTTP_STATUS_CODE readResult = requestReader.Read();


#if defined(_DEBUG)
		// ��� ǥ��
		std::cout << pEol->m_recvBuffer << '\n';
#endif

		GBHttpResponse response;
		{
			std::lock_guard<std::mutex> lock(g_mainCriticalsection);

			pEol->m_httpVersion = requestReader.m_pRequestLineInformation->m_version;

			//g_pRouter->m_methodList.emplace_back("GET", [](const std::string_view path)
			//{
			//	std::cout << "GET : path = " << path.data() << std::endl;
			//});
			//g_pRouter->m_methodList.emplace_back("PUT", [](const std::string_view path)
			//{
			//	std::cout << "PUT : path = " << path.data() << std::endl;
			//});
			//g_pRouter->m_methodList.emplace_back("POST", [](const std::string_view path)
			//{
			//	std::cout << "POST : path = " << path.data() << std::endl;
			//});

			//succeeded = g_pRouter->Route(g_rootPath, requestReader, response);

			if (false == succeeded)
			{
				// #ToDo �α����� �ٲ���
				//return { false, "Routing failed." };
			}
		}

		GBHttpResponseWriter responseWriter(pEol);
		responseWriter.WriteStatusLine(HttpVersion::Http10, response, "none");//requestReader.m_pRequestLineInformation->m_version

		std::vector<std::pair<std::string, std::string>> headerList;

		headerList.emplace_back("Host", "localhost:8000");
		headerList.emplace_back("Connection", "keep-alive");
		headerList.emplace_back("Accept", "*/*");

		//headerList.emplace_back("Content-type", "text/html");

		responseWriter.WriteHeader(headerList);

		responseWriter.WriteBody();

		return true;
	}

	//bool GBExpandedOverlapped::GatherAndParseLines(DWORD receivedBytes)
	//{
	//	int stringOffset = 0;
	//	// All Http message ( except for Entity-Body ) must be ended by CRLF or LF.
	//	int k = m_recvOffset;
	//	char* pLineStart = nullptr;
	//	for (; k < m_recvOffset + receivedBytes; ++k)
	//	{
	//		switch (m_recvBuffer[k])
	//		{
	//		case '\r':
	//			// carrage return�� �����Ѵ�.
	//			// RFC ������ ���ϸ� HTTP message�� request line�� header-value list���� ������ CRLF�� �ؾ� �ȴٰ� �����ִ�.
	//			// �׷���, 'HTTP �Ϻ� ���̵�'å�� ���ϸ�, ���� ���α׷� �� CR ���� LF������ �����ϴ� �͵��� ���ٰ� �Ѵ�.
	//			break;
	//		case '\n':
	//			m_lines.emplace(pLineStart, stringOffset);
	//			stringOffset = 0;
	//			break;
	//		default:
	//			if (0 == stringOffset)
	//			{
	//				pLineStart = &m_recvBuffer[k];
	//			}
	//			stringOffset++;
	//			break;
	//		}
	//	}

	//	return 0 == stringOffset;
	//}
}