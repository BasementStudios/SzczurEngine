#pragma once

#include <vector>
#include <string>

/*
Filter format:
	| - separator
	Name - name of filter 
	Ext - extensions for filter

	Structure: Name|Ext[;Ext][|Name|Ext[;Ext]]

	Example: Images (png, jpg) and music (mp3)
		- Images (*.png, *.jpg)|*.png;*.jpg|Music (*.mp3)|*.mp3

*/

namespace rat
{

class FileDialog
{
public:
	enum Mode
	{
		Open,
		Save
	};

private:
	std::string _caption;

	std::string _directory;

	std::string _filter;

	int _filterIndex = 1;

	Mode _mode;

public:
	FileDialog(Mode mode, const std::string& caption = "", const std::string& directory = "", const std::string& filter = "");
	~FileDialog() = default;

	void setCaption(const std::string& caption) { _caption = caption; }

	void setDirectory(const std::string& directory) { _directory = directory; }
	
	void setFilter(const std::string& filter) { _filter = filter; }

	void setFilterIndex(int filterIndex) { _filterIndex = filterIndex; }

	std::string getFile();

	std::vector<std::string> getFiles();


	static std::string getOpenFileName(const std::string& caption = "", const std::string& directory = "", const std::string& filter = "");

	static std::vector<std::string> getOpenFileNames(const std::string& caption = "", const std::string& directory = "", const std::string& filter = "");

	static std::string getSaveFileName(const std::string& caption = "", const std::string& directory = "", const std::string& filter = "");

private:
	bool show(char *result, bool multiselect);
};

}