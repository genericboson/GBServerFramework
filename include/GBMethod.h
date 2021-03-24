#pragma once

#include <functional>

namespace GenericBoson
{
	// http���� GET, PUT, POST ���� ���� ���Ѵ�.
	class GBMethod
	{
	public:
		// #ToDo
		// ���ø����� callable�� ���� ���ؼ� �ƽ���.
		// ���� ����� ����غ���.
		const std::function<void()> m_method;
		const std::string m_methodName;

		GBMethod(std::string_view methodName, const std::function<void()>& method)
			: m_method(method), m_methodName(methodName.data()){}
	};
}