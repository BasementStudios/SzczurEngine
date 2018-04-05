#include "FileDialog.hpp"

#include <iostream>
#include <algorithm>

#include "Szczur/Config.hpp"

#ifdef EDITOR
#include <Windows.h>
#endif

namespace rat
{

FileDialog::FileDialog(Mode mode, const std::string& caption, const std::string& directory, const std::string& filter)
	: _mode(mode), _caption(caption), _directory(directory), _filter(filter)
{
}

std::string FileDialog::getFile()
{
	char result[1024];

	show(result, false);

	return result;
}

std::vector<std::string> FileDialog::getFiles()
{
	std::vector<std::string> paths;

	char result[1024];

	if (!show(result, true))
		return std::vector<std::string>();

	char* pResult = result;

	std::string directory = pResult;
	
	pResult += directory.length() + 1;

	while (*pResult)
	{
		std::string file = pResult;

		pResult += file.length() + 1;

		paths.push_back(directory + DIRECTORY_SEPARATOR_CHAR + file);
	}
	
	return paths;
}

std::string FileDialog::getOpenFileName(const std::string& caption, const std::string& directory, const std::string& filter)
{
	FileDialog fileDialog(Open, caption, directory, filter);

	return fileDialog.getFile();
}

std::vector<std::string> FileDialog::getOpenFileNames(const std::string& caption, const std::string& directory, const std::string& filter)
{
	FileDialog fileDialog(Open, caption, directory, filter);
	
	return fileDialog.getFiles();
}

std::string FileDialog::getSaveFileName(const std::string& caption, const std::string& directory, const std::string& filter)
{
	FileDialog fileDialog(Save, caption, directory, filter);

	return fileDialog.getFile();
}

bool FileDialog::show(char *result, bool multiselect)
{
#ifdef EDITOR
	OPENFILENAME ofn = { 0 };
	char filePath[1024] = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = sizeof(filePath);

	std::replace(_filter.begin(), _filter.end(), '|', '\0');
	_filter += '\0';
	ofn.lpstrFilter = _filter.c_str();

	ofn.nFilterIndex = _filterIndex;
	ofn.lpstrTitle = _caption.c_str();
	ofn.lpstrInitialDir = _directory.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | (multiselect ? OFN_ALLOWMULTISELECT | OFN_EXPLORER : 0x0);

	bool accepted = false;

	if (_mode == Open)
		accepted = GetOpenFileName(&ofn);
	else if (_mode == Save)
		accepted = GetSaveFileName(&ofn);

	if (accepted)
	{
		memcpy(result, filePath, 1024);
	}

	return accepted;
#else
	return false;
#endif
}


}