#pragma once

#include <functional>
#include "../lib/GBString/include/GBString.h"

namespace GenericBoson
{
	/*
	http���� GET, PUT, POST ���� ���� ���մϴ�.
	*/
	struct GBMethod
	{
		std::function<void(int)> m_method;

		GBMethod() = default;
		virtual ~GBMethod() = default;

		GBMethod(std::function<void(int)>& method)
			: m_method(method) {}

		virtual std::string GetName() = 0;
	};
}