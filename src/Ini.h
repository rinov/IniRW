#ifndef INI_H
#define INI_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>

namespace Ini {
#define UsingSTL std::map
	using Section = UsingSTL<std::string, std::string>;
	using IniFormat = UsingSTL<std::string, Section>;

	class IniBase {
	public:
		IniBase();

		void showIni();
		void setIni(IniBase& ini);

	protected:
		virtual ~IniBase();

		void resetValues();
		bool findSection(const std::string& section) const;

		IniFormat values;
	private:
		IniBase(const IniBase&) = delete;
		IniBase(const IniBase*) = delete;
		IniBase& operator=(const IniBase&) = delete;
	};

	class IniReader : public IniBase {
	public:
		using IniBase::IniBase;

		bool readIniFile(const std::string& filename);

		template<class T>
		T get(const std::string& section, const std::string& key, const T& default_value) const;

		const Section* const getSection(const std::string& section) const;
	private:
		template<class T>
		T parseValue(const std::string& value, const T& default_value) const;
		void parse(std::ifstream& s);
	};

	class IniWriter : public IniBase {
	public:
		using IniBase::IniBase;

		bool writeIniFile(const std::string& filename);

		bool setSection(const std::string& section);
		bool removeSection(const std::string& section);
		bool remove(const std::string& section, const std::string& key);

		template<class T = std::string>
		bool set(const std::string& section, const std::string& key, const T& value);

		bool sectionSpaceEnabled = true;
	};

	template<class T>
	inline T IniReader::get(const std::string & section, const std::string & key, const T& default_value) const
	{
		auto sec = this->getSection(section);

		if (sec != nullptr && this->findSection(section))

			if (sec->find(key) != sec->end())
				return parseValue<T>(sec->at(key), default_value);
		return default_value;
	}

	template<class T>
	inline T IniReader::parseValue(const std::string& value, const T& default_value) const
	{
		T ret;
		std::stringstream ss(value);
		if (ss >> ret)
			return std::move(ret);
		else
			return default_value;
	}

	template<class T>
	inline bool IniWriter::set(const std::string & section, const std::string & key, const T & value)
	{
		std::string ret;
		std::stringstream ss;
		ss << value;
		if (ss >> ret) {
			this->setSection(section);
			this->values[section].insert(std::make_pair(key, ret));
			return true;
		}
		else
			return false;
	}
}

#endif