#pragma once

#include "GBHttpResponse.h"
#include "GBHttpMessageBody.h"
#include <functional>
#include <cstdarg>

#define GB_ACTION_METHOD_PARAMETERS const std::map<std::string_view, std::string_view>&, const std::map<std::string_view, std::string_view>&, const GBHttpMessageBody&

namespace GenericBoson
{
	// Explanation about std::functino template parameter :
	// Parameters
	//		1. const std::map<std::string, std::string>& : ���� Ű-�� ����Ʈ
	//		2. const std::map<std::string, std::string>& : ���-�ʵ� �� ����Ʈ
	//		3. const GBHttpMessageBody& : HTTP message body ��ü
	// Return Value
	//		1. GBHttpResponse : ���� response ��ü
	typedef std::function<GBHttpResponse(GB_ACTION_METHOD_PARAMETERS)> GB_ACTION_METHOD;

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