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
	InputMenu(std::initializer_list<std::string> lst);
	int32_t	Ask();
	int32_t	GetSelection() const;
};

