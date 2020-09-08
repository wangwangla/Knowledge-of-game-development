#include "BrowseDir.h"

BrowseDir* BrowseDir::browseDir = nullptr;

BrowseDir::BrowseDir()
{
}

BrowseDir* BrowseDir::getInstance(string rootName)
{
	if (BrowseDir::browseDir == nullptr)
	{
		BrowseDir::browseDir = new BrowseDir();
	}
	BrowseDir::browseDir->setRootPath(rootName);
	BrowseDir::browseDir->initFileInfo();
	return BrowseDir::browseDir;
}

void BrowseDir::setRootPath(string rootName)
{
	this->curRootName = rootName;
	char* curretPath = getcwd(nullptr, 0);
	this->curRootPath = string(curretPath) + "\\..\\resource\\" + this->curRootName;
}


bool BrowseDir::initFileInfo()
{
	filePath.clear();
	fileNames = vector<string>(6);
	WIN32_FIND_DATAA findData;
	HANDLE findRet;
	string researchPath = this->curRootPath + "\\*.*";
	findRet = FindFirstFile(researchPath.c_str(), &findData);

	if (INVALID_HANDLE_VALUE == findRet) {
		cout << "file research failed��" << endl;
		return false;
	}

	while ( FindNextFile(findRet, &findData) ) {
		//ȥ��. �� .. ȥ���ļ���
		//if ((strcmp(findData.cFileName, ".") == 0) || (strcmp(findData.cFileName, "..") == 0))
		if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}
		//����
		doSomethingWhenFind(findData.cFileName);

		cout << findData.cFileName << endl;
	};
}


void BrowseDir::doSomethingWhenFind(string fileName)
{
	string tempPath = this->curRootPath + "\\" + fileName;
	string tempKey = "";
	//fileName  �� �ң����ϣ��£�ǰ����
	//��
	if (fileName.find("right") != string::npos) {
		tempKey = "right";
		fileNames[0] = "right";
	}
	//��
	if (fileName.find("left") != string::npos) {
		tempKey = "left";
		fileNames[1] = "left";
	}
	//��
	if (fileName.find("top") != string::npos) {
		tempKey = "top";
		fileNames[2] = "top";
	}
	//��
	if (fileName.find("bottom") != string::npos) {
		tempKey = "bottom";
		fileNames[3] = "bottom";
	}
	//��
	if (fileName.find("back") != string::npos) {
		tempKey = "back";
		fileNames[4] = "back";
	}
	//ǰ
	if (fileName.find("front") != string::npos) {
		tempKey = "front";
		fileNames[5] = "front";
	}
	filePath[tempKey] = tempPath;
}

map<string, string> BrowseDir::getAllFilePath()
{
	return filePath;
}

vector<string> BrowseDir::getAllFileNames()
{
	return fileNames;
}

//
