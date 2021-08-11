#pragma once

namespace GenericBoson
{
	// \param urlCandidate
	// \param parsedPath
	// \param queryMap
	// \return
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string_view>& parsedPath, std::map<std::string_view, std::string_view>&queryMap);

	// targetStringView�� ����Ű�� ���ڿ��� �������� �������� Ž���س����鼭 ���� ���� ������ separator�� �������� ���ʿ� �ִ� ���ڿ����� ���� ����Ű�� string_view�� output���� �������ϴ�.
	// \param targetStringView
	// \param separator
	// \param outputStringView
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, std::string_view& outputStringView);

	// targetStringView�� ����Ű�� ���ڿ��� �������� �������� Ž���س����鼭 ������ separator��� targetStringView�� �ɰ��� ���������� ���� ���ڿ��� ����� ��� ref ���Ϳ� ����ְ� ������ ���ڿ��� string_view�� ��ȯ���ݴϴ�.
	// \param targetStringView
	// \param separator
	// \param outputArray
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string_view>& outputArray);
}