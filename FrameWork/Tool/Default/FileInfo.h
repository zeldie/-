#pragma once


class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();
public:
	static CString	ConvertRelativePath(CString strFullPath);
	static void		DirInfoExtractionForMesh(const wstring& wstrFilePath, list<IMAGEPATH*>& rlstImagePath); //경로를 추출해주는 함수야
	static void		DirInfoExtractionForTexture(const wstring& wstrFilePath, list<IMAGEPATH*>& rlstImagePath);
	static void		DirInfoExtractionForMeshTool(const wstring& _wstrFilePath, list<IMAGEPATH*>& _rlstImagePath);
	static void		DirInfoExtractionForEffectTool(const wstring& _wstrFilePath, list<IMAGEPATH*>& _rlstImagePath);
	static int		DirFileCount(const wstring& wstrFilePath);//파일이 몇개인지 세는함수
};


