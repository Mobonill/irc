#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

std::vector<std::string> splitString(const std::string& str, const char* delimiter)
{
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = str.find(delimiter);
    size_t sizeDelimiter = std::strlen(delimiter);

	while (end != std::string::npos) {
		result.push_back(str.substr(start, end - start));
		start = end + sizeDelimiter;
		end = str.find(delimiter, start);
	}
	result.push_back(str.substr(start));

	return result;
}

int main(void)
{
    std::string const msg = "my gosh fred, have you seen about my future car ";
    std::string summonExpressions = "divination,tarot,futur,42forecast,DE-CODER,Dcoder,decoder,read me,akinator";
    std::vector<std::string> summonBot = splitString(summonExpressions, ",");
	std::vector<std::string>::iterator vit;
	for (vit = summonBot.begin(); vit != summonBot.end(); ++vit)
	{
		if (msg.find(*vit) != std::string::npos)
            return true;
	}
    return false;
}
