#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <windows.h>
#include "ConfigFile.h"
#include "Convert.h"

class ParseConfigFile : public ConfigFile
{
	private:
	  std::map<std::string, std::string> contents;
	  std::string file_name;

	  void RemoveComment(std::string &line) const;

	  bool OnlyWhitespace(const std::string &line) const;
	  bool ValidLine(const std::string &line) const;

	  void ExtractKey(std::string &key, size_t const &sepPos, const std::string &line) const;
	  void ExtractValue(std::string &value, size_t const &sepPos, const std::string &line) const;
	  void ExtractContents(const std::string &line);
	  void ExtractKeys();

	  void ParseLine(const std::string &line, size_t const lineNo);


	public:
		ParseConfigFile(const std::string &fName);

	  bool keyExists(const std::string &key) const;

	  template <typename ValueType>
	  ValueType GetValueOfKey(const std::string &key, ValueType const &defaultValue = ValueType()) const
	  {
		if (!keyExists(key))
		  return defaultValue;

		return Convert::String_to_T<ValueType>(contents.find(key)->second);
	  };
};