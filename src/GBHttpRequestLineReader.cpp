#include "../include/stdafx.h"

#include "../include/GBHttpRouter.h"
#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	std::pair<bool, HttpVersion> GBHttpRequestLineReader::Read(const std::string_view target, std::string& targetPath, std::string& methodName)
	{
		bool parseResult = Parse(target);

		if (false == parseResult)
		{
			// Parse �Լ����� ���� CRLF�� �� ���� ����. ��, gathering�� ���ؾ� �ȴ�.
			return { false, HttpVersion::None };
		}

		size_t parsedSize = m_parsed.size();

		if (2 == parsedSize)
		{
			methodName = m_parsed[0];
			targetPath = m_parsed[1];
			return { true, HttpVersion::Http09 };
		}
		else if (3 == parsedSize)
		{
			std::string_view httpStr("HTTP/");
			// �Ʒ� if�� ���� true == m_parsed[2].starts_with(httpStr) �̾�����,
			// travis windows�� VS2017������ �����ؼ� �Ʒ�ó�� �ٲ�.
			if (httpStr.size() < 5 || httpStr == m_parsed[2].substr(0, httpStr.size()))
			{
				// #ToDo
				// Invalid request-line.
				return { true, HttpVersion::None };
			}

			methodName = m_parsed[0];
			targetPath = m_parsed[1];
			std::string_view versionString(m_parsed[2]);
			std::string_view versionNumber = versionString.substr(httpStr.size());

			if ("0.9" == versionNumber)
			{
				return { true, HttpVersion::Http09 };
			}
			else if ("1.0" == versionNumber)
			{
				return { true, HttpVersion::Http10 };
			}
			else if ("1.1" == versionNumber)
			{
				return { true, HttpVersion::Http11 };
			}
		}

		// #ToDo
		// Invalid request-line.
		return { true, HttpVersion::None };
	}
}