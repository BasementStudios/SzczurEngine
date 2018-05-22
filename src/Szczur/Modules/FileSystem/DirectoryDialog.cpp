#include "DirectoryDialog.hpp"

namespace rat
{

DirectoryDialog::DirectoryDialog()
{
}

DirectoryDialog::~DirectoryDialog()
{
}

std::string DirectoryDialog::getDirectory()
{
#ifdef OS_WINDOWS
	char path[512] = { 0 };

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = _caption.c_str();
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	if (_editBox)
		bi.ulFlags += BIF_EDITBOX;

	if (!_newFolderButton)
		bi.ulFlags += BIF_NONEWFOLDERBUTTON;

	bi.lpfn = DirectoryDialog::BrowseCallbackProc;
	bi.lParam = reinterpret_cast<LPARAM>(_directory.c_str());

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		SHGetPathFromIDList(pidl, path);

		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}

		return path;
	}

	return std::string(path);
#elif
	return std::string();
#endif
}

std::string DirectoryDialog::getExistingDirectory(const std::string& caption, const std::string& dir)
{
	DirectoryDialog directoryDialog;
	directoryDialog.setCaption(caption);
	directoryDialog.setDirectory(dir);

	return directoryDialog.getDirectory();
}

#ifdef OS_WINDOWS
static BOOL CALLBACK EnumCallback(HWND hWndChild, LPARAM lParam)
{
	char szClass[MAX_PATH];
	HTREEITEM hNode;
	if (GetClassName(hWndChild, szClass, sizeof(szClass)) && strcmp(szClass, "SysTreeView32") == 0)
	{

		hNode = TreeView_GetSelection(hWndChild);    // found the tree view window
		TreeView_EnsureVisible(hWndChild, hNode);   // ensure its selection is visible
		return false;
	}
	return true;
}

int DirectoryDialog::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
		case BFFM_INITIALIZED:
			SendMessage(hwnd, BFFM_SETEXPANDED, true, lpData);    // expand the tree view
			SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);
			break;
		case BFFM_SELCHANGED:
			EnumChildWindows(hwnd, EnumCallback, 0);
			break;
	}

	return 0;
}
#endif

}
