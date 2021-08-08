#include "../include/stdafx.h"

#include "../include/GBHttpUtil.h"

namespace GenericBoson
{
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string>& parsedPath, std::map<std::string, std::string>& queryMap)
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
				parsedPath.emplace_back(leftStringView1);
			}
			else
			{
				std::string_view parsedSegment = leftStringView1.substr(0, offset);

				parsedPath.emplace_back(parsedSegment);

				leftStringView1 = leftStringView1.substr(offset + 1);

				std::vector<std::string> queryPairStringArray;
				std::string_view leftStringView2 = Split(leftStringView1, '&', queryPairStringArray);
			}
		}

		return true;
	}

	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string>& outputArray)
	{
		size_t offset = 0;
		std::string_view stringViewCopy = targetStringView;

		while(true)
		{
			offset = stringViewCopy.find_first_of('/', 0);

			if (std::string_view::npos == offset)
			{
				return stringViewCopy;
			}

			std::string_view parsedSegment = stringViewCopy.substr(0, offset);

			outputArray.emplace_back(parsedSegment);

			stringViewCopy = stringViewCopy.substr(offset + 1);
		}
	}
}