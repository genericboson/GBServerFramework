#include "../include/stdafx.h"

#include "../include/GBHttpUtil.h"

namespace GenericBoson
{
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string_view>& parsedPath, std::map<std::string_view, std::string_view>& queryMap)
	{
		// �Ʒ� if�� ���� false == urlCandidate.starts_with('/') �̾�����,
		// travis windows�� VS2017������ �����ؼ� �Ʒ�ó�� �ٲ�.
		if (true == urlCandidate.empty() || '/' != urlCandidate.at(0))
		{
			return false;
		}

		size_t offset = 0;
		std::string_view urlCandidateCopy = urlCandidate.substr(1);

		std::string_view leftStringView1 = Split(urlCandidateCopy, '/', parsedPath);

		if (0 < leftStringView1.size())
		{
			offset = leftStringView1.find_first_of('?', 0);

			if (std::string_view::npos == offset)
			{
				// ������ �����Ƿ� '/' �ڿ� ���� ������ ��� �������� �ִ´�.
				parsedPath.emplace_back(leftStringView1);
			}
			else
			{
				// ������ �����Ƿ� '/'�� '?' ������ �͸� ������ ��η� �ִ´�.
				std::string_view parsedSegment = leftStringView1.substr(0, offset);

				parsedPath.emplace_back(parsedSegment);

				leftStringView1 = leftStringView1.substr(offset + 1);

				std::vector<std::string_view> queryPairStringArray;
				std::string_view leftStringView2 = Split(leftStringView1, '&', queryPairStringArray);
				queryPairStringArray.push_back(leftStringView2);

				for(const auto& iQueryPairStr : queryPairStringArray)
				{
					std::string_view valueStr;
					std::string_view keyStr = Split(iQueryPairStr, '=', valueStr);
					queryMap.emplace(keyStr, valueStr);
				}
			}
		}

		return true;
	}

	std::string_view Split(const std::string_view targetStringView, char separator, std::string_view& outputStringView)
	{
		size_t offset = targetStringView.find_first_of(separator, 0);

		if (std::string_view::npos == offset)
		{
			return targetStringView;
		}

		outputStringView = targetStringView.substr(offset + 1);

		return targetStringView.substr(0, offset);
	}

	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string_view>& outputArray)
	{
		
		std::string_view stringViewCopy = targetStringView;

		while (true)
		{
			std::string_view leftPart;
			std::string_view parsedPart = Split(stringViewCopy, separator, leftPart);

			if (true == leftPart.empty())
			{
				return parsedPart;
			}

			stringViewCopy = leftPart;
			outputArray.push_back(parsedPart);
		}
	}
}