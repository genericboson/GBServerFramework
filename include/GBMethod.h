#pragma once

#include "GBHttpResponse.h"
#include "GBHttpMessageBody.h"
#include <functional>
#include <cstdarg>

namespace GenericBoson
{
	// Explanation about std::functino template parameter :
	// Parameters
	//		1. const std::map<std::string, std::string>& : ���� Ű-�� ����Ʈ
	//		2. const std::map<std::string, std::string>& : ���-�ʵ� �� ����Ʈ
	//		3. const GBHttpMessageBody& : HTTP message body ��ü
	// Return Value
	//		1. GBHttpResponse : ���� response ��ü
	typedef std::function<GBHttpResponse(const std::map<std::string, std::string>&, const std::map<std::string, std::string>&, const GBHttpMessageBody&)> GB_ACTION_METHOD;

	/*
	http���� GET, PUT, POST ���� ���� ���մϴ�.
	*/
	struct GBMethod
	{
		GB_ACTION_METHOD m_method;

		GBMethod() = default;
		virtual ~GBMethod() = default;

		GBMethod(GB_ACTION_METHOD& method)
			: m_method(method) {}

		virtual std::string GetName() = 0;
	};
}