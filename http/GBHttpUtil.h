#pragma once

namespace GenericBoson
{
	// \param urlCandidate
	// \param parsedPath
	// \param queryMap
	// \param fragment
	// \return
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string_view>& parsedPath, std::map<std::string_view, std::string_view>&queryMap, std::string_view fragment);

	// targetStringView�� ����Ű�� ���ڿ��� �������� �������� Ž���س����鼭 ���� ���� ������ separator�� �������� ���ʿ� �ִ� ���ڿ����� ���� ����Ű�� string_view�� output���� �������ϴ�.
	// \param targetStringView
	// \param separator
	// \param outputStringView
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, std::string_view& outputStringView);

	// targetStringView�� ����Ű�� ���ڿ��� �������� �������� Ž���س����鼭 ������ separator��� targetStringView�� �ɰ��� ���������� ���� ���ڿ��� ����� ��� ref out ���Ϳ� ����ְ� ������ ���ڿ��� string_view�� ��ȯ���ݴϴ�.
	// \param targetStringView
	// \param separator
	// \param outputArray
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string_view>& outputArray);

	// targetStringView�� ����Ű�� ���ڿ��� �������� �������� Ž���س����鼭 ������ separator��� targetStringView�� �ɰ��� ���������� ���� ���ڿ��� endCharacter�� ���� �� �ִ��� ����.
	// ���� �� �ִٸ�, ���� ó���� ���� endCharacter�� ������ �պκ��� ref out ���Ϳ� �ְ� �������� ��ȯ�Ѵ�.
	// ���� �� ���ٸ�, ���� ���ڿ� ��� ref out ���Ϳ� ���, �� string_view�� ��ȯ�Ѵ�.
	// \param targetStringView
	// \param separator
	// \param endCharacter
	// \param outputArray
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, char endCharacter, std::vector<std::string_view>& outputArray);
}