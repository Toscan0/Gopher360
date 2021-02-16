#include "ParseConfigFile.h"

void ParseConfigFile::RemoveComment(std::string &line) const
{
  if (line.find('#') != line.npos)
  {
    line.erase(line.find('#'));
  }
}

bool ParseConfigFile::OnlyWhitespace(const std::string &line) const
{
  return (line.find_first_not_of(' ') == line.npos);
}

bool ParseConfigFile::ValidLine(const std::string &line) const
{
  std::string temp = line;
  temp.erase(0, temp.find_first_not_of("\t "));
  if (temp[0] == '=')
  {
    return false;
  }

  for (size_t i = temp.find('=') + 1; i < temp.length(); i++)
  {
    if (temp[i] != ' ')
    {
      return true;
    }
  }

  return false;
}

void ParseConfigFile::ExtractKey(std::string &key, size_t const &sepPos, const std::string &line) const
{
  key = line.substr(0, sepPos);
  if (key.find('\t') != line.npos || key.find(' ') != line.npos)
  {
    key.erase(key.find_first_of("\t "));
  }
}
void ParseConfigFile::ExtractValue(std::string &value, size_t const &sepPos, const std::string &line) const
{
  value = line.substr(sepPos + 1);
  value.erase(0, value.find_first_not_of("\t "));
  value.erase(value.find_last_not_of("\t ") + 1);
}

void ParseConfigFile::ExtractContents(const std::string &line)
{
  std::string temp = line;
  temp.erase(0, temp.find_first_not_of("\t "));
  size_t sepPos = temp.find('=');

  std::string key, value;
  ExtractKey(key, sepPos, temp);
  ExtractValue(value, sepPos, temp);

  if (!keyExists(key))
  {
    contents.insert(std::pair<std::string, std::string>(key, value));
  }
  else
  {
	  ConfigFile::ExitWithError("CFG: Can only have unique key names!\n");
  }
}

void ParseConfigFile::ParseLine(const std::string &line, size_t const lineNo)
{
  if (line.find('=') == line.npos)
  {
	  ConfigFile::ExitWithError("CFG: Couldn't find separator on line: " + Convert::T_to_string(lineNo) + "\n");
  }

  if (!ValidLine(line))
  {
	  ConfigFile::ExitWithError("CFG: Bad format for line: " + Convert::T_to_string(lineNo) + "\n");
  }

  ExtractContents(line);
}

void ParseConfigFile::ExtractKeys()
{
  std::ifstream file;
  file.open(file_name.c_str());

  if (!file)
  {
    //TODO: Call generate config file
  }

  std::string line;
  size_t lineNo = 0;
  while (std::getline(file, line))
  {
    lineNo++;
    std::string temp = line;

    if (temp.empty())
    {
      continue;
    }

    RemoveComment(temp);
    if (OnlyWhitespace(temp))
    {
      continue;
    }

    ParseLine(temp, lineNo);
  }

  file.close();
}

ParseConfigFile::ParseConfigFile(const std::string &fName)
{
  this->file_name = fName;
  ExtractKeys();
}

bool ParseConfigFile::keyExists(const std::string &key) const
{
  return contents.find(key) != contents.end();
}