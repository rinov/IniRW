#include "Ini.h"
#include <fstream>
#include <regex>
#include <sstream>

using namespace Ini;

//_/_/_/_/_/_/ IniBase _/_/_/_/_/_/

IniBase::IniBase()  {}
IniBase::~IniBase() {}

void IniBase::showIni()
{
	std::cout << "---show_Ini---" << std::endl;
	for (auto section : this->values) {
		std::cout << "[" << section.first << "]" << std::endl;
		for (auto parameter : section.second) 
			std::cout << parameter.first << "=" << parameter.second << std::endl;
	}
	std::cout << std::endl;
}

void IniBase::setIni(IniBase& ini)
{
	this->values = ini.values;
}

void IniBase::resetValues()
{
	for (auto& i : this->values) 
		i.second.clear();
	this->values.clear();
}

bool IniBase::findSection(const std::string & section) const
{
	return this->values.find(section) != this->values.end();
}

//_/_/_/_/_/_/ IniReader _/_/_/_/_/_/


bool IniReader::readIniFile(const std::string& filename)
{
	std::ifstream ifs(filename);
	
	if (!ifs.is_open()) {
		ifs.close();
		return false;
	}
	resetValues();
	this->parse(ifs);

	ifs.close();
	return true;
}

const Section* const IniReader::getSection(const std::string & section) const
{
	if (!this->findSection(section)) 
		return nullptr;
	else 
		return &values.at(section);
}

void IniReader::parse(std::ifstream& s)
{
	std::string line;
	std::string target;
	std::smatch m;
	std::regex clear_characters_section(R"(\s|\[|\])");
	std::regex clear_characters_parameter(R"(\s)");
	std::regex check_section(R"(^\s*\[[a-zA-Z0-9]+\]\s*$)");
	std::regex check_parameter(R"(^\s*\S+\s*=\s*\S+\s*$)");
	std::regex check_comment(R"(#|:)");

	while (std::getline(s, line)) {
		std::string data;
		for (char c : line) {
			if (c == '#' || c == ':') break;
			else data += c;
		}

		if (!std::regex_match(data, m, check_section)){
			if(!target.empty() && std::regex_match(data, m, check_parameter)) {
				data = std::regex_replace(m.str(), clear_characters_parameter, "");
				std::stringstream ss(data);
				std::string key_token,value_token;
				std::getline(ss, key_token, '=');
				std::getline(ss, value_token);
				this->values[target].insert(std::make_pair(key_token,value_token));
			}
		}
		else {
			data = std::regex_replace(m.str(), clear_characters_section, "");
			this->values.insert(std::make_pair(data, Section()));
			target = data;
		}
	}
}

//_/_/_/_/_/_/ IniWriter _/_/_/_/_/_/

bool IniWriter::writeIniFile(const std::string & filename)
{
	std::ofstream ofs(filename,std::ios::out);
	if (!ofs.is_open()) {
		ofs.close();
		return false;
	}

	for (auto section : this->values) {
		ofs << "[" <<section.first << "]" << std::endl;
		for (auto parameter : section.second)
			ofs << parameter.first << "=" << parameter.second << std::endl;
		if(this->sectionSpaceEnabled)
			ofs << std::endl;
	}
	ofs.close();
	return true;
}

bool IniWriter::setSection(const std::string & section)
{
	if (!this->findSection(section)) {
		this->values.insert(std::make_pair(section, Section()));
		return true;
	}
	else
		return false;
}

bool IniWriter::removeSection(const std::string & section)
{
	return this->values.erase(section) > 0;
}

bool IniWriter::remove(const std::string & section, const std::string & key)
{
	if (this->findSection(section)) {
		return this->values[section].erase(key) > 0;
	}else
		return false;
}
