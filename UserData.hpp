#pragma once
#include <map>
#include <string>

class UserData
{
	private:
		std::map<std::string, std::string> _data;
		UserData();
	public:
		UserData(const std::map<std::string, std::string> &newData);
		~UserData();
		const std::map<std::string, std::string> &getAllData() const;
		std::string getData(const std::string &key) const;
		void setUserData(const std::map<std::string, std::string> &newData);
};

