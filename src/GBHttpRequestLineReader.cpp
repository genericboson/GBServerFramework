#include "../include/stdafx.h"

#include "../include/GBHttpRouter.h"
#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	GBHttpRequestLineReader::GBHttpRequestLineReader(const std::string_view requestLineCandidate)
	: m_requestLineCandidate(requestLineCandidate){}

	std::pair<bool, RequestLineInformation> GBHttpRequestLineReader::ParseAndRead()
	{

	}

	std::pair<bool, RequestLineInformation> GBHttpRequestLineReader::Read(const std::string& targetPath)
	{
		std::string methodName;
		size_t parsedSize = m_tokens.size();

		if (2 == parsedSize)
		{
			methodName = m_tokens[0];
			targetPath = m_tokens[1];
			return { HttpVersion::Http09, methodName };
		}
		else if (3 == parsedSize)
		{
			std::string_view httpStr("HTTP/");
			// �Ʒ� if�� ���� true == m_tokens[2].starts_with(httpStr) �̾�����,
			// travis windows�� VS2017������ �����ؼ� �Ʒ�ó�� �ٲ�.
			if (httpStr.size() < 5 || httpStr == m_tokens[2].substr(0, httpStr.size()))
			{
				// #ToDo
				// Invalid request-line.
				return { HttpVersion::None, {} };
			}

			methodName = m_tokens[0];
			targetPath = m_tokens[1];
			std::string_view versionString(m_tokens[2]);
			std::string_view versionNumber = versionString.substr(httpStr.size());

			if ("0.9" == versionNumber)
			{
				return { HttpVersion::Http09, methodName };
			}
			else if ("1.0" == versionNumber)
			{
				return { HttpVersion::Http10, methodName };
			}
			else if ("1.1" == versionNumber)
			{
				return { HttpVersion::Http11, methodName };
			}
		}

		// #ToDo
		// Invalid request-line.
		return { HttpVersion::None, {} };
	}
}