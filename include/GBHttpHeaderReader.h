#pragma once

#include "GBHttpReader.h"

namespace GenericBoson
{
	struct GBHttpHeaderInformation final : public GBHttpInformation
	{
		std::map<std::string, std::string> m_headerMap;
	};

	class GBHttpHeaderReader : public GBHttpReader
	{
		std::queue<std::string>& m_lines;

		// Inherited via GBHttpReader
		virtual HTTP_STATUS_CODE Read(GBHttpInformation* pOutInfo) override;

		virtual HTTP_STATUS_CODE ParseToken() override;

	public:
		std::map<std::string, std::string> m_headerMap;

		GBHttpHeaderReader(std::queue<std::string>& lines)
			: m_lines(lines), GBHttpReader(lines) {}
	};
}