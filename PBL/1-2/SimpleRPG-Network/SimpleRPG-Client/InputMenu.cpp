#include "pch.h"
#include "InputMenu.h"

InputMenu::InputMenu(std::initializer_list<std::string> lst)
	: _selection()
	, _selections(lst)
{
}

int InputMenu::Ask()
{
	while (true)
	{
		int index = 1;
		for (const auto& menu : _selections)
		{
			std::cout << std::format("[{}]:  {}", index++, menu.c_str()) << std::endl;
		}
		std::cout << "무엇을 하시겠습니까?: ";

		std::string input;
		if (std::getline(std::cin, input))
		{
			try
			{
				_selection = std::stoi(input);
				if (_selection <= 0)
				{
					throw std::out_of_range("negative selection");
				}
				if (_selection > _selections.size())
				{
					throw std::out_of_range("over selections");
				}
				std::cout << std::endl;
				std::cout << std::endl;
				break;
			}
			catch (const std::exception&) {}
		}
		std::cout << "잘못된 입력입니다." << std::endl;
		_selection = 0;
	}
	return GetSelection();
}

int InputMenu::GetSelection() const
{
	return _selection;
}
