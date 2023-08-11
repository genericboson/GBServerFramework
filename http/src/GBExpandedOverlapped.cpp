#include "../include/stdafx.h"
#include "../include/GBExpandedOverlapped.h"

namespace GenericBoson
{
	bool GBExpandedOverlapped::GatherAndParseLines(DWORD receivedBytes)
	{
		int stringOffset = 0;
		// All Http message ( except for Entity-Body ) must be ended by CRLF or LF.
		int k = m_recvOffset;
		char* pLineStart = nullptr;
		for (; k < m_recvOffset + receivedBytes; ++k)
		{
			switch (m_recvBuffer[k])
			{
			case '\r':
				// carrage return�� �����Ѵ�.
				// RFC ������ ���ϸ� HTTP message�� request line�� header-value list���� ������ CRLF�� �ؾ� �ȴٰ� �����ִ�.
				// �׷���, 'HTTP �Ϻ� ���̵�'å�� ���ϸ�, ���� ���α׷� �� CR ���� LF������ �����ϴ� �͵��� ���ٰ� �Ѵ�.
				break;
			case '\n':
				m_lines.emplace(pLineStart, stringOffset);
				stringOffset = 0;
				break;
			default:
				if (0 == stringOffset)
				{
					pLineStart = &m_recvBuffer[k];
				}
				stringOffset++;
				break;
			}
		}

		return 0 == stringOffset;
	}
}