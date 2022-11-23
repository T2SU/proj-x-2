#pragma once

#include <vector>
#include <string>
#include <cstdint>

class InputMenu
{
private:
	int32_t						_selection;
	std::vector<std::string>	_selections;

public:
	/// <summary>
	/// ���� �Է��� ���� ����ڿ� ��ȣ�ۿ��� �� �ִ� InputMenu �ν��Ͻ��� ����ϴ�.
	/// </summary>
	/// <param name="lst">�� string�� �ϳ��� �޴��� �˴ϴ�.</param>
	InputMenu(std::initializer_list<std::string> lst);

	/// <summary>
	/// ����ڿ��� �޴��� �����ְ�, �Է��� �޾� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>����ڰ� ������ �޴��� ���� ��ȣ�Դϴ�. �ּ� 1���� �ִ� �޴��� �������� �Է� ���� �� �ֽ��ϴ�.</returns>
	int32_t	Ask();

	/// <summary>
	/// ���� �ֱٿ� ����ڰ� ������ �޴��� ���� ��ȣ�� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>����ڰ� ������ �޴� ��ȣ�Դϴ�. �� ���� �Է��� ���� ���� ���ٸ� 0�� ��ȯ�մϴ�.</returns>
	int32_t	GetSelection() const;
};

