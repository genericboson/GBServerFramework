#include "../include/stdafx.h"

#include "../include/GBHttpHeaderReader.h"

namespace GenericBoson
{
	HTTP_STATUS_CODE GBHttpHeaderReader::Read(GBHttpInformation* pOutInfo)
	{
		return HTTP_STATUS_CODE::OK;
	}

	HTTP_STATUS_CODE GBHttpHeaderReader::ParseToken()
	{
		size_t findResult = std::string::npos;
		while (std::string::npos != (findResult = m_lines.front().find_first_of(':')))
		{
			std::string_view frontLine = m_lines.front();

			std::string_view headerName = frontLine.substr(0, findResult);
			std::string_view headerValue = frontLine.substr(findResult + 2); // ':'�� ' '�� �ǳʶڴ�.

			m_headerMap.emplace(headerName, headerValue);

			m_lines.pop();
		}

		return HTTP_STATUS_CODE::OK;
	}
}