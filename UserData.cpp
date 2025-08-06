#include "UserData.hpp"

// UserData::UserData() {}

UserData::UserData(const std::map<std::string, std::string> &newData) : _data(newData) {}

UserData::~UserData() {}

const std::map<std::string, std::string> &UserData::getAllData() const { return _data; }

std::string UserData::getData(const std::string &key) const
{
	if (_data.empty())
		return "";
	std::map<std::string, std::string>::const_iterator value = _data.find(key);
	if (value == _data.end())
		return "";
	else
		return value->second;
}

void UserData::setUserData(const std::map<std::string, std::string> &newData)
{
	if (_data.empty())
		_data = newData;
}
