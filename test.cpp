#define SPECIAL "[]\\`^{|}"
#define ALPHANUMSPE "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`^{|}"

#include <string>
#include <iostream>
#include <cctype>

bool isValid(const char &c, const std::string &valid)
{
	if (!c && valid.empty())
		return true;
	else if (valid.empty() || !c)
		return false;
	for (std::string ::const_iterator it = valid.begin(); it < valid.end(); it++)
	{
		if (c == *it)
			return true;
	}
	return false;
}

bool onlyValid(const std::string &s, const std::string &valid)
{
	if (s.empty() && valid.empty())
		return true;
	else if (s.empty() || valid.empty())
		return false;
	for (std::string ::const_iterator it = s.begin(); it < s.end(); it++)
	{
		if (!isValid(*it, valid))
			return false;
	}
	return true;
}

// c++ -Wall -Wextra -Werror -std=c++98 test.cpp

int main(void)
{
	std::string test = "hello";
	std::string tmp = &(*(test.begin() + 1));
	std::cout << "test name = [" << test << "]" << std::endl;
	std::cout << "- tmp = [" << tmp << "]" << std::endl;
	std::cout << "- test ref the plus = [" << *(&test+1) << "]" << std::endl;
	std::cout << "- test ref the crochet = [" << &test[1] << "]" << std::endl;
	std::cout << "- test with ref = [" << &(*(test.begin() + 1)) << "]" << std::endl;
	std::cout << "- test in c_str = [" << test.c_str() + 1 << "]" << std::endl;
	std::cout << "- test modif = [" << test.substr(1, test.size() - 1) << "]" << std::endl;
	std::cout << "- ALPHANUMSPE = [" << ALPHANUMSPE << "]" << std::endl;
	if (onlyValid(tmp, ALPHANUMSPE))
		std::cout << "-- tmp is valid - tmp = [" << tmp << "]" << std::endl;
	else
		std::cout << "-- tmp is false - tmp = [" << tmp << "]" << std::endl;
	if (onlyValid(test.substr(1, test.size() - 1), ALPHANUMSPE))
		std::cout << "-- test is valid - test = [" << test.substr(1, test.size() - 1) << "]" << std::endl;
	else
		std::cout << "-- test is false - test = [" << test.substr(1, test.size() - 1) << "]" << std::endl;
	return 0;
}
