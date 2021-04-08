#pragma once

#include <functional>
#include "../lib/GBString/include/GBString.h"

namespace GenericBoson
{
	/*
	http���� GET, PUT, POST ���� ���� ���մϴ�.
	*/
	template<typename METHOD>
	class GBMethod
	{
	public:
		const METHOD& m_method;

		GBMethod(const METHOD& method)
			: m_method(method) {}
	};
}