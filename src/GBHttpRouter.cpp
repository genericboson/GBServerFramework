#include "../include/stdafx.h"
#include "../include/GBHttpRouter.h"

namespace GenericBoson
{
	std::pair<bool, GBHttpResponse> GBHttpRouterBase::Route(PathSegment& rootPath, const std::string_view targetPath, const std::string_view methodName)
	{
		std::vector<std::string> pathSegmentArray;
		bool parseResult = ParseUrlString(targetPath, pathSegmentArray);

		if (false == parseResult)
		{
			// #ToDo
			// The path in request line does not start with '/'.
			return { false, {} };
		}

		// ���� true == m_supportMethodMap.contains(methodName.data()) ���µ�,
		// travis�� VS2017������ �����ؼ� �Ʒ��� ���� �ٲ�.
		if (m_supportMethodMap.end() == m_supportMethodMap.find(methodName.data()))
		{
			// #ToDo
			// Invalid request : The method name is not matched with the version.
			return { false, {} };
		}

		PathSegment* pNode = &rootPath;
		for (auto& iPathSegment : pathSegmentArray)
		{
			// ���� true == pNode->m_subTreeMap.contains(iPathSegment) ���µ�,
			// travis�� VS2017������ �����ؼ� �Ʒ��� ���� �ٲ�.
			if (pNode->m_subTreeMap.end() == pNode->m_subTreeMap.find(iPathSegment))
			{
				// #ToDo ActionMethod not found.
				return { false, {} };
			}

			pNode = pNode->m_subTreeMap[iPathSegment].get();
		}

		if ("GET" == methodName)
		{
			if (nullptr == pNode->m_pGetMethod)
			{
				// #ToDo
				// The method not exist.
				return { false, {} };
			}

			auto response = pNode->m_pGetMethod->m_method(0);

			return { true, response };
		}
		else if ("HEAD" == methodName)
		{
			if (nullptr == pNode->m_pHeadMethod)
			{
				// #ToDo
				// The method not exist.
				return { false, {} };
			}

			auto response = pNode->m_pHeadMethod->m_method(0);

			return { true, response };
		}
		else if ("POST" == methodName)
		{
			if (nullptr == pNode->m_pPostMethod)
			{
				// #ToDo
				// The method not exist.
				return { false, {} };
			}

			auto response = pNode->m_pPostMethod->m_method(0);

			return { true, response };
		}

		// #ToDo
		// Internal logic error
		return { false, {} };
	}
}