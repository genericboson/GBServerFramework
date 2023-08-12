#include "stdafx.h"

#include "GBHttpResponseWriter.h"

namespace GenericBoson
{
	GBHttpResponseWriter::GBHttpResponseWriter(GBExpandedOverlapped* pEol)
		: m_pEol(pEol)
	{
		pEol->m_sendOffset = 0;
	}

	GBHttpResponseWriter::~GBHttpResponseWriter()
	{
		int issueSendResult = IssueSend();
	}

	int GBHttpResponseWriter::IssueSend()
	{
		WSABUF bufToSend;
		DWORD sentBytes = 0;
		bufToSend.buf = m_pEol->m_sendBuffer;
		bufToSend.len = m_pEol->m_sendOffset;
		int sendResult = WSASend(m_pEol->m_socket, &bufToSend, 1, &sentBytes, 0, m_pEol, nullptr);

		return 0;
	}

	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	bool GBHttpResponseWriter::WriteStatusLine(const HttpVersion version, const GBHttpResponse & response, const std::string& reason)
	{
		float versionFloat = ((int)version) / 10.0f;

		int statusCodeInteger = (int)response.GetStatusCode();

		return WriteOneLineToBuffer("HTTP/%.2f %d %s\r\n", versionFloat, statusCodeInteger, Constant::g_cStatusCodeToReasonPhaseMap.at(statusCodeInteger).c_str());
	}

	bool GBHttpResponseWriter::WriteHeader(const std::vector<std::pair<std::string, std::string>>& headerList)
	{
		for(auto riter = headerList.rbegin(); riter != headerList.rend(); ++riter)
		{
			bool ret = WriteOneLineToBuffer("%s: %s\r\n", riter->first.c_str(), riter->second.c_str());
			if (false == ret)
			{
				return false;
			}
		}

		// RFC 7230�� HTTP �޼����� ������ ���� ���¶�� �����ִ�.
		/*
		HTTP-message = start-line
					  *( header-field CRLF )
					  CRLF
					  [ message-body ]
		*/
		// ���⿡�� ��û ������ ���� ����ʵ� ����Ʈ�� �޼��� �ٵ� ������ CRLF�̴�.
		// ��� ���-�ʵ� ���� CRLF�� �����µ� �׷��� �ֵ��� �������� CRLF�� �ѹ� �� �پ�� ����ε� �޼������ ���̴�.
		// �� ������ ���溸�� HTTP message writer�� �����Ѵٸ�,
		// ���-�ʵ�ֵ��� �� ���� �ٷ� �޼��� �ٵ� �̾ ����� ���� �ִ�.
		// �׷��� �ϸ� Http Client���� �������� �ɸ��ų� �߸��� �޼������ �������� �ȴ�. �� �׷����� ������ �˷����� �ʾƼ� ������� ���������.
		// �� ���ϰ� ���-�ʵ� ����Ʈ�� ������ ������ �ݵ�� "�ι�"�ǰ� �ٵ� �ٴ´ٰ� �����ϸ� ���ϴ�.
		// ���� CRLF�� 2���̸� �� ������ LF�� �ι��ص� ����� �����ϴ� ���ϴ�.
		WriteOneLineToBuffer("\r\n");

		return true;
	}

	bool GBHttpResponseWriter::WriteBody()
	{
		bool ret = WriteOneLineToBuffer(
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page1</h1>\n"
			"<a href=\"/page2\">->page2</a>\n"
			"</body>"
			"</html>"
		);

		if (false == ret)
		{
			return false;
		}

		return true;
	}

	bool GBHttpResponseWriter::WriteOneLineToBuffer(const char* format, ...)
	{
		char* pLineStartPosition = &m_pEol->m_sendBuffer[m_pEol->m_sendOffset];

		va_list argList;
		__crt_va_start(argList, format);
		int writtenCountOrErrorCode = _vsprintf_s_l(pLineStartPosition, BUFFER_SIZE - m_pEol->m_sendOffset, format, NULL, argList);
		__crt_va_end(argList);

		if (-1 == writtenCountOrErrorCode)
		{
			return false;
		}

		m_lines.emplace_back(pLineStartPosition, writtenCountOrErrorCode);

		m_pEol->m_sendOffset += writtenCountOrErrorCode;

		return true;
	}
}
