#pragma once

#include <functional>
#include "../lib/GBString/include/GBString.h"

namespace GenericBoson
{
	/*
	http���� GET, PUT, POST ���� ���� ���մϴ�.
	*/
	class GBMethod
	{
	public:
		const std::function<void(int)>& m_method;

		GBMethod(const std::function<void(int)>& method)
			: m_method(method) {}
	};
}