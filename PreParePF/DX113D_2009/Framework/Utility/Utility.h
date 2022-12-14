#pragma once

namespace Utility
{
	string ToString(wstring value);
	wstring ToWString(string value);

	vector<string> SplitString(string origin, string tok);
	void Replace(string* str, string comp, string rep);

	wstring GetExtension(wstring path);	
	string GetFileName(string path);
	string GetFileNameWithoutExtension(string path);
	string GetDirectoryName(string path);

	void CreateFolders(string path);

	bool ExistDirectory(string path);
	bool ExistFile(string path);
}