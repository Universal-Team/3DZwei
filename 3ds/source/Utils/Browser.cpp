/*
*   This file is part of 3DZwei
*   Copyright (C) 2020-2023 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "Browser.hpp"
#include <cstring>
#include <dirent.h>
#include <functional>
#include <sys/stat.h>
#include <unistd.h>


/*
	Browser Initializer for a File / Directory type.

	const std::string &Path: The Start Path from where to load.
	const std::vector<std::string> &Extensions: The File Extensions which to filter (Optional).
*/
Browser::Browser(const std::string &Path, const std::vector<std::string> &Extensions) : ExtensionList(Extensions) {
	this->Type = Browser::BrowserType::File;

	chdir(Path.c_str());
	this->FetchDirectoryEntries();
}


/*
	Browser Initializer for a List type.

	const std::vector<std::string> &List: The list of Strings for the List type.
*/
Browser::Browser(const std::vector<std::string> &List) : ListEntries(List) { this->Type = Browser::BrowserType::List; }


/*
	Return, if a specific Name ends with an Extension.

	const std::string &Name: The Filename to check.
	const std::vector<std::string> &ExtList: The Extension list which to check.
*/
bool Browser::NameEndsWith(const std::string &Name, const std::vector<std::string> &ExtList) {
	if (Name.substr(0, 2) == "._") return false;
	if (Name.size() == 0) return false;
	if (ExtList.size() == 0) return true;

	for(size_t Idx = 0; Idx < ExtList.size(); Idx++) {
		const std::string Ext = ExtList.at(Idx);

		if (strcasecmp(Name.c_str() + Name.size() - Ext.size(), Ext.c_str()) == 0) return true;
	}

	return false;
}


/*
	Predict File Entries Alphabetically.

	const DirEntry &First: The first Entry which to compare.
	const DirEntry &Second: The second Entry which to compare.
*/
bool DirEntryPredicate(const Browser::DirEntry &First, const Browser::DirEntry &Second) {
	if (!First.IsDirectory && Second.IsDirectory) return false;
	if (First.IsDirectory && !Second.IsDirectory) return true;

	return strcasecmp(First.Name.c_str(), Second.Name.c_str()) < 0;
}


/*
	Fetches Directory Contents.

	const std::vector<std::string> &ExtList: The extensions which to filter (Optional).
*/
void Browser::FetchDirectoryEntries(const std::vector<std::string> &ExtList) {
	struct stat Stat;
	this->DirEntries.clear();
	DIR *PDir = opendir(".");

	if (PDir) {
		while(1) {
			DirEntry DirectoryEntry;
			struct dirent *Pent = readdir(PDir);
			if (!Pent) break;

			/* Fetch the file name and directory state. */
			stat(Pent->d_name, &Stat);
			DirectoryEntry.Name = Pent->d_name;
			DirectoryEntry.IsDirectory = (Stat.st_mode & S_IFDIR) ? true : false;

			if (DirectoryEntry.Name.compare(".") != 0 && (DirectoryEntry.IsDirectory || this->NameEndsWith(DirectoryEntry.Name, (ExtList.empty() ? this->ExtensionList : ExtList)))) {
				this->DirEntries.push_back(DirectoryEntry);
			}
		}

		closedir(PDir);
	}

	std::sort(this->DirEntries.begin(), this->DirEntries.end(), DirEntryPredicate); // Sort this alphabetically.
}


/* Returns the Directory Entry Files as a vector of strings. */
std::vector<std::string> Browser::GetFileList() {
	std::vector<std::string> TMP = { };

	if (!this->DirEntries.empty()) {
		for (size_t Idx = 0; Idx < this->DirEntries.size(); Idx++) TMP.push_back(this->DirEntries[Idx].Name);
	}

	return TMP;
}


/* Up, Down, Left and Right Callbacks. */
void Browser::Up() {
	switch(this->Type) {
		case Browser::BrowserType::File:
			if (this->DirEntries.empty()) break;
			if (this->Selected > 0) this->Selected--;
			else this->Selected = (int)this->DirEntries.size() - 1;
			break;

		case Browser::BrowserType::List:
			if (this->ListEntries.empty()) break;
			if (this->Selected > 0) this->Selected--;
			else this->Selected = (int)this->ListEntries.size() - 1;
			break;
	}
}


void Browser::Down() {
	switch(this->Type) {
		case Browser::BrowserType::File:
			if (this->DirEntries.empty()) break;
			if (this->Selected < (int)this->DirEntries.size() - 1) this->Selected++;
			else this->Selected = 0;
			break;

		case Browser::BrowserType::List:
			if (this->ListEntries.empty()) break;
			if (this->Selected < (int)this->ListEntries.size() - 1) this->Selected++;
			else this->Selected = 0;
			break;
	}
}


void Browser::Left(const int Amount) {
	if (this->Selected - Amount >= 0) this->Selected -= Amount;
	else this->Selected = 0;
}


void Browser::Right(const int Amount) {
	switch(this->Type) {
		case Browser::BrowserType::File:
			if (this->DirEntries.empty()) break;

			if (this->Selected + Amount < (int)this->DirEntries.size() - 1) this->Selected += Amount;
			else this->Selected = (int)this->DirEntries.size() - 1;
			break;

		case Browser::BrowserType::List:
			if (this->ListEntries.empty()) break;

			if (this->Selected + Amount < (int)this->ListEntries.size() - 1) this->Selected += Amount;
			else this->Selected = (int)this->ListEntries.size() - 1;
			break;
	}
}


/*
	Selected File Open Callback.

	Returns true, if it's a directory or false if not.
*/
bool Browser::OpenHandle() {
	switch(this->Type) {
		case Browser::BrowserType::File:
			if (this->DirEntries.empty()) return false;
			return this->DirEntries[this->Selected].IsDirectory;

		case Browser::BrowserType::List:
			return false;
	}

	return false;
}


/* Return, if you can go a directory back or not. */
bool Browser::CanDirBack() {
	if (this->Type == Browser::BrowserType::File) {
		char Path[PATH_MAX];
		getcwd(Path, PATH_MAX);

		/* Check for sdmc and /. */
		if (strcmp(Path, "sdmc:/") == 0 || strcmp(Path, "/") == 0) return false;
	}

	return true;
}


/* Goes a directory back. */
void Browser::GoDirBack() {
	if (this->Type == Browser::BrowserType::File) {
		if (!this->CanDirBack()) return;

		chdir("..");
		this->FetchDirectoryEntries();
		this->Selected = 0;
	}
}


/* Goes a directory up. */
void Browser::GoDirUp() {
	if (this->Type == Browser::BrowserType::File) {
		if (this->DirEntries.empty() || !this->DirEntries[this->Selected].IsDirectory) return; // Ensure it's a directory and not empty.

		const std::string Path = this->DirEntries[this->Selected].Name;
		chdir(Path.c_str());
		this->FetchDirectoryEntries();
		this->Selected = 0;
	}
}


/*
	Set the Selected Selection.

	const int Selection: The Selection to set.

	Returns true if success or false if not.
*/
bool Browser::SetSelection(const int Selection) {
	switch(this->Type) {
		case Browser::BrowserType::File:
			if (this->DirEntries.empty()) this->Selected = 0;

			if (Selection <= (int)this->DirEntries.size() - 1) {
				this->Selected = Selection;
				return true;
			}
			break;

		case Browser::BrowserType::List:
			if (this->ListEntries.empty()) this->Selected = 0;
			if (Selection <= (int)this->ListEntries.size() - 1) {
				this->Selected = Selection;
				return true;
			}
			break;
	}

	return false;
}


/* Returns the current working directory. */
std::string Browser::GetPath() const {
	char Path[PATH_MAX];
	getcwd(Path, PATH_MAX);

	return Path;
}