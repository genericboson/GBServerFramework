#pragma once

#include "GBHttpLineReader.h"

namespace GenericBoson
{
	struct RequestLineInformation
	{
		std::string m_methodName;
		std::string m_targetPath;
		HttpVersion m_version = HttpVersion::None;
	};

	/*
	Read the first line of request.
	*/
	class GBHttpRequestLineReader : public GBHttpLineReader
	{
		const std::string_view m_requestLineCandidate;
		std::vector<std::string_view> m_tokens;

		// Read parsed tokens.
		// \return bool - succeeded? RequestLineInformation - The information of this request line.
		std::pair<bool, RequestLineInformation> Read();

		void ParseToken();
	public:
		GBHttpRequestLineReader(const std::string_view requestLineCandidate);
		virtual ~GBHttpRequestLineReader() = default;

		// \return bool - succeeded? RequestLineInformation - The information of this request line.
		std::pair<bool, RequestLineInformation> ParseAndRead();
	};
}