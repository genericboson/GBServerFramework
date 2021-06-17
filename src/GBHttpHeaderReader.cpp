#include "../include/stdafx.h"

#include "../include/GBHttpHeaderReader.h"

namespace GenericBoson
{
	bool GBHttpHeaderReader::Read(GBHttpInformation* pOutInfo)
	{

		return true;
	}

	void GBHttpHeaderReader::ParseToken()
	{
		size_t findResult = std::string::npos;
		while (std::string::npos != (findResult = m_lines.front().find_first_of(':')))
		{
			std::string_view frontLine = m_lines.front();

			// RFC ���� ':' ������ �ݵ�� ��ĭ��ĭ�� �;��Ѵ�.
			assert(' ' == frontLine[findResult + 1]);

			std::string_view headerName = frontLine.substr(0, findResult);
			std::string_view headerValue = frontLine.substr(findResult + 2); // ':'�� ' '�� �ǳʶڴ�.

			m_headerMap.emplace(headerName, headerValue);

			m_lines.pop();
		}
	}
}