#pragma once

#include "Szczur/Config.hpp"

#ifdef OS_WINDOWS
#include <Shlobj.h>
#endif

namespace rat
{

class DirectoryDialog
{
private:
	std::string _caption;

	std::string _directory;

	bool _editBox = false;

	bool _newFolderButton = true;

public:
	DirectoryDialog();
	~DirectoryDialog();

	void setCaption(const std::string& caption) { _caption = caption; }

	void setDirectory(const std::string& directory) { _directory = directory; }

	void setEditBox(bool editBox) { _editBox = editBox; }

	void setNewFolderButton(bool newFolderButton) { _newFolderButton = newFolderButton; }

	std::string getDirectory();


	static std::string getExistingDirectory(const std::string& caption = "", const std::string& dir = "");

#ifdef OS_WINDOWS
private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
#endif
};

}
