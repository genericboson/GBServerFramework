#include "../include/stdafx.h"

#include "../include/GBHttpRequestReader.h"

namespace GenericBoson
{
	bool GBHttpRequestReader::Read(std::queue<std::string>& lines)
	{
		assert(0 < lines.size());

		// ExtendedOverlapped.GatherAndParseLines���� �������Դٴ� ���� �ּ� 1���� �о��ٴ� ���̴�.
		GBHttpRequestLineReader requestLineReader(lines);

		bool succeeded = requestLineReader.ParseAndRead(&m_requestLineInfo);

		GBHttpHeaderInformation headerInfo;
		if (1 < lines.size())
		{
			// ����б�
			GBHttpHeaderReader headerReader(lines);

			succeeded = headerReader.ParseAndRead(&m_headerInfo);
		}

		return succeeded;
	}
}