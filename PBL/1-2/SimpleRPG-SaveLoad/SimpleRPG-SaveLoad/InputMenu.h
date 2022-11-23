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
	/// 숫자 입력을 통해 사용자와 상호작용할 수 있는 InputMenu 인스턴스를 만듭니다.
	/// </summary>
	/// <param name="lst">각 string은 하나의 메뉴가 됩니다.</param>
	InputMenu(std::initializer_list<std::string> lst);

	/// <summary>
	/// 사용자에게 메뉴를 보여주고, 입력을 받아 반환합니다.
	/// </summary>
	/// <returns>사용자가 선택한 메뉴에 대한 번호입니다. 최소 1부터 최대 메뉴의 개수까지 입력 받을 수 있습니다.</returns>
	int32_t	Ask();

	/// <summary>
	/// 가장 최근에 사용자가 선택한 메뉴에 대한 번호를 반환합니다.
	/// </summary>
	/// <returns>사용자가 선택한 메뉴 번호입니다. 한 번도 입력을 받은 적이 없다면 0을 반환합니다.</returns>
	int32_t	GetSelection() const;
};

