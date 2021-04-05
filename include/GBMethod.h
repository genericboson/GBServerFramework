#pragma once

#include <functional>
#include "../lib/GBString/include/GBString.h"

namespace GenericBoson
{
	// http���� GET, PUT, POST ���� ���� ���Ѵ�.
	class GBMethod
	{
	public:
		// #ToDo
		// ���ø����� callable�� ���� ���ؼ� �ƽ���.
		// ���� ����� ����غ���.
		const std::function<void(const GBStringView)> m_method;
		const GBString m_methodName;

		GBMethod(GBStringView methodName, const std::function<void(const GBStringView)>& method)
			: m_method(method), m_methodName(methodName.data()){}
	};
}