// Copyright (C) 2020 twyleg
#pragma once
#include "helper.h"

#include <fmt/core.h>

#include <boost/lexical_cast.hpp>

#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

namespace OpenDashboard::Common {

class CsvReader
{
public:

	explicit CsvReader(const std::filesystem::path& filepath, const std::string& charFilterString = "\n\r\"");

	void ReadHeader();
	bool IgnoreLine();
	bool ReadLine();

	template<class T>
	T GetValue(const std::string& columnName);

	template<class T>
	T GetValue(const int columnIndex);

private:

	const std::string mCharFilterString;
	std::ifstream mInputFile;
	std::unordered_map<std::string, int> mColumnNames;
	std::vector<std::string> mLastLineContent;
};

template<class T>
T CsvReader::GetValue(const std::string& columnName)
{
	const auto index = mColumnNames.find(columnName);
	THROW_IF(index == mColumnNames.end(), "Requested column \"{}\" does not exist", columnName);
	return GetValue<T>(index->second);
}

template<class T>
T CsvReader::GetValue(const int columnIndex)
{
	THROW_IF (columnIndex > mLastLineContent.size(), "Requested column (index={}) out of bounds", columnIndex);
	return boost::lexical_cast<T>(mLastLineContent[columnIndex]);
}

}
