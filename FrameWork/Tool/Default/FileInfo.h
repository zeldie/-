#pragma once


class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();
public:
	static CString	ConvertRelativePath(CString strFullPath);
	static void		DirInfoExtractionForMesh(const wstring& wstrFilePath, list<IMAGEPATH*>& rlstImagePath); //��θ� �������ִ� �Լ���
	static void		DirInfoExtractionForTexture(const wstring& wstrFilePath, list<IMAGEPATH*>& rlstImagePath);
	static void		DirInfoExtractionForMeshTool(const wstring& _wstrFilePath, list<IMAGEPATH*>& _rlstImagePath);
	static void		DirInfoExtractionForEffectTool(const wstring& _wstrFilePath, list<IMAGEPATH*>& _rlstImagePath);
	static int		DirFileCount(const wstring& wstrFilePath);//������ ����� �����Լ�
};


