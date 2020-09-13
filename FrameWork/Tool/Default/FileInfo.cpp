#include "stdafx.h"
#include "FileInfo.h"

CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR szRelativePath[MAX_STR] = L"";
	TCHAR szCurrentPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurrentPath);

	// from���� TO�� ã�ư��� ����θ� ��ȯ���ش� ��𿡴�? szRelativePath
	// ���ǻ����� ���� ��ο� ��������! 
	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY, strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);


	return CString(szRelativePath);
}

// ����ó�� ������ ��� 
//D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain
//D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain\Tile\\*.*
void CFileInfo::DirInfoExtractionForMesh(const wstring & wstrFilePath, list<IMAGEPATH*>& rlstImagePath)
{
	// ���� ��� �� ������� Ŭ���� �̰� MFC���� ����. 
	CFileFind find;
	//D:\\�ں���\\103A\\4������-����\\DefaultWindow\\Texture\\Stage\\Terrain\\*.*
	wstring wstrPath = wstrFilePath + L"\\*.*";
	// FindFile - �־��� ��ο� ������ ������ Ȯ���ϴ� �Լ�. 
	// ���ڷ� *.*�� ���ٸ� �ش� ����� ���� ��� ������ ã�� ������ �ؼ��Ѵ�. 
	// ���� ���Ͽ� �����ϰ� �ʹٸ� FileNextFile�̶�� �Լ��� �����ϸ� �ȴ�. 
	BOOL bContinue = find.FindFile(wstrPath.c_str());

	// ������ �󸶳� Ÿ����ߵ��� �𸣴� ��Ȳ�̶� �ݺ��� ����. 
	while (bContinue)
	{
		// �ִ� ���� ���̻� ������ ���� ���ٸ� 0�� ��ȯ. 
		bContinue = find.FindNextFile();
		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
		{
			// ���� ������� ����Լ� ȣ��. 
			// D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain\\Tile
			DirInfoExtractionForMesh(find.GetFilePath().GetString(), rlstImagePath);
		}
		else
		{
			//���� �۾��� ����. ���� ���ϴ� ���ϱ��� ����. 
			if (find.IsDots())
				continue;
			IMAGEPATH* pImagePath = new IMAGEPATH;

			TCHAR szFilePath[MAX_STR] = L"";
			// D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain\\Tile
			lstrcpy(szFilePath, find.GetFilePath().GetString());

			PathRemoveFileSpec(szFilePath);
			// ���� ī��Ʈ ����. 
			//pImagePath->iCount = DirFileCount(szFilePath); 

			//wstring wstrTextureName = find.GetFileTitle().GetString();
			wstring wstrTextureName = PathFindFileName(szFilePath);

			//wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1);
			pImagePath->wstrFileName = wstrTextureName + L".X";
			pImagePath->wstrTextureName = L"Mesh_" + wstrTextureName;

			// Tile + L"%d.png" = Tile%d.png
			//szFilePath + wstrTextureName; 
			// D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain\\Tile\\Tile%d.png
			// PathCombine��� �̾��ִ� �Լ�. ���Լ��� ���ڿ� ���̿� �ڵ����� \\�� �������ش�. 
			//PathCombine(szFilePath, szFilePath, wstrTextureName.c_str());

			PathAddBackslash(szFilePath);
			pImagePath->wstrPath = CFileInfo::ConvertRelativePath(szFilePath);
			// D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain\\Tile
			PathRemoveFileSpec(szFilePath);
			PathRemoveFileSpec(szFilePath);
			pImagePath->wstrStageType = PathFindFileName(szFilePath);
		
			PathRemoveFileSpec(szFilePath);
			pImagePath->wstrMeshType = PathFindFileName(szFilePath);

			pImagePath->wstrResourceType = L"Static";
			//// D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain
			//PathRemoveFileSpec(szFilePath);
			//// statekey ����� ������ ObjectKey���� �ɰ�. 
			//pImagePath->wstrObjKey = PathFindFileName(szFilePath);

			rlstImagePath.emplace_back(pImagePath);
			bContinue = FALSE;

		}

	}


}

void CFileInfo::DirInfoExtractionForTexture(const wstring & wstrFilePath, list<IMAGEPATH*>& rlstImagePath)
{
	// ���� ��� �� ������� Ŭ���� �̰� MFC���� ����. 
	CFileFind find;
	//D:\\�ں���\\103A\\4������-����\\DefaultWindow\\Texture\\Stage\\Terrain\\*.*
	wstring wstrPath = wstrFilePath + L"\\*.*";
	// FindFile - �־��� ��ο� ������ ������ Ȯ���ϴ� �Լ�. 
	// ���ڷ� *.*�� ���ٸ� �ش� ����� ���� ��� ������ ã�� ������ �ؼ��Ѵ�. 
	// ���� ���Ͽ� �����ϰ� �ʹٸ� FileNextFile�̶�� �Լ��� �����ϸ� �ȴ�. 
	BOOL bContinue = find.FindFile(wstrPath.c_str());

	// ������ �󸶳� Ÿ����ߵ��� �𸣴� ��Ȳ�̶� �ݺ��� ����. 
	while (bContinue)
	{
		// �ִ� ���� ���̻� ������ ���� ���ٸ� 0�� ��ȯ. 
		bContinue = find.FindNextFile();
		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
		{
			DirInfoExtractionForTexture(find.GetFilePath().GetString(), rlstImagePath);
		}
		else
		{
			//���� �۾��� ����. ���� ���ϴ� ���ϱ��� ����. 
			if (find.IsDots())
				continue;
			IMAGEPATH* pImagePath = new IMAGEPATH;

			TCHAR szFilePath[MAX_STR] = L""; //�ڸ��Ű�
			TCHAR szOriginFilePath[MAX_STR] = L""; //�ڸ����� ����
			wstring wstrFileExtension = L"";

			lstrcpy(szFilePath, find.GetFilePath().GetString());
			lstrcpy(szOriginFilePath, find.GetFilePath().GetString());

			wstrFileExtension = PathFindExtension(szFilePath);

			PathRemoveFileSpec(szFilePath);			
			wstring wstrFolderName = PathFindFileName(szFilePath);

			if (lstrcmp(L"EffectSub", wstrFolderName.c_str())) //EffectSub �����ϰ� ��� ������ ����
			{
				PathRemoveFileSpec(szOriginFilePath);
			
				pImagePath->iCount = DirFileCount(szOriginFilePath);

				wstring wstrTextureName = find.GetFileTitle().GetString();
				wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1) + L"%d" + wstrFileExtension;

				PathCombine(szOriginFilePath, szOriginFilePath, wstrTextureName.c_str());


				pImagePath->wstrPath = CFileInfo::ConvertRelativePath(szOriginFilePath);
				PathRemoveFileSpec(szOriginFilePath);

				// ������ �̸��� �ᱹ statekey�� ��. 
				pImagePath->wstrFileName = PathFindFileName(szOriginFilePath);

				pImagePath->wstrResourceType = L"Static";

				rlstImagePath.emplace_back(pImagePath);
				bContinue = FALSE;
			}
			else //EffectSub ����
			{
				Engine::Safe_Delete(pImagePath);
			}
		}

	}
}

void CFileInfo::DirInfoExtractionForMeshTool(const wstring & _wstrFilePath, list<IMAGEPATH*>& _rlstImagePath)
{
	//MeshTool �غ�
	CFileFind find; //���ϰ��õ�Ŭ���� - mfc����
	wstring wstrPath = _wstrFilePath + L"\\*.*";

	BOOL bContinue = find.FindFile(wstrPath.c_str()); //���Ͼ����� 0��ȯ

	while (bContinue)
	{
		bContinue = find.FindNextFile(); //�������Ͼ����� 0��ȯ
		if (find.IsDots())
			continue;
		else if (find.IsDirectory()) //ã���� �����ΰ��
		{
			//����Լ�
			DirInfoExtractionForMeshTool(find.GetFilePath().GetString(), _rlstImagePath);
		}
		else //ã���� �����̸�
		{
			if (find.IsDots())
				continue;
			IMAGEPATH* pImagePath = new IMAGEPATH;

			TCHAR szFilePath[MAX_STR] = L"";
			lstrcpy(szFilePath, find.GetFilePath().GetString());

			TCHAR szExtension[MAX_STR] = L"";

			TCHAR szEx[MIN_STR] = L".X";
			lstrcpy(szExtension, PathFindExtension(szFilePath));

			if (!lstrcmp(szEx, szExtension))
			{
				// D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain\Tile\Tile0.png
				//C:\Users\ojon4\OneDrive\���� ȭ��\FrameWork\Texture\Mesh\DynamicMesh\Player\Player.X

				PathRemoveFileSpec(szFilePath); //����� ���ϴ��� �߶󳾰ž�

				pImagePath->iCount = DirFileCount(szFilePath); //�̰�� ���� ���ϼ��� ���ž�

															   //�����̸�
				pImagePath->wstrFileName = find.GetFileTitle().GetString(); //�����̸�������
																			//wstrTexturName = wstrTexturName.substr(0, wstrTexturName.length() - 1) + L"%d.png";
																			//-.substr : ���ۺ���(0) ������(����-1) ���ڿ��� ������ �Լ�

																			//PathCombine(szFilePath, szFilePath, wstrTex);
																			// PathCombine��� �̾��ִ� �Լ�. ���Լ��� ���ڿ� ���̿� �ڵ����� \\�� �������ش�. 
																			// D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain\\Tile\\Tile%d.png ->������

																			//pImagePath->wstrPath = CFileInfo::ConvertRelativePath(szFilePath);

																			//PathRemoveFileSpec(szFilePath);
																			//pImagePath->wstrChildFolder = PathFindFileName(szFilePath);

																			//�ؽ�ó�̸�
																			//pImagePath->wstrTextureName = L"Mesh_" + pImagePath->wstrFileName;//

				PathRemoveFileSpec(szFilePath);
				pImagePath->wstrTextureName = PathFindFileName(szFilePath);//

				PathRemoveFileSpec(szFilePath);//
				pImagePath->wstrMeshType = PathFindFileName(szFilePath);

				_rlstImagePath.emplace_back(pImagePath);
				bContinue = FALSE;
			}
			else
 				Engine::Safe_Delete(pImagePath);  ////�̰Ų��Ű澲�� -> else ���ǿ� �ɸ��°͵� �����Ҵ��������־���

		}
	}
}

void CFileInfo::DirInfoExtractionForEffectTool(const wstring & _wstrFilePath, list<IMAGEPATH*>& _rlstImagePath)
{
	//MeshTool �غ�
	CFileFind find; //���ϰ��õ�Ŭ���� - mfc����
	wstring wstrPath = _wstrFilePath + L"\\*.*";

	BOOL bContinue = find.FindFile(wstrPath.c_str()); //���Ͼ����� 0��ȯ

	while (bContinue)
	{
		bContinue = find.FindNextFile(); //�������Ͼ����� 0��ȯ
		if (find.IsDots())
			continue;
		else if (find.IsDirectory()) //ã���� �����ΰ��
		{
			//����Լ�
			DirInfoExtractionForEffectTool(find.GetFilePath().GetString(), _rlstImagePath);
		}
		else //ã���� �����̸�
		{
			if (find.IsDots())
				continue;
			IMAGEPATH* pImagePath = new IMAGEPATH;

			TCHAR szFilePath[MAX_STR] = L"";
			lstrcpy(szFilePath, find.GetFilePath().GetString());

			//PathRemoveFileSpec(szFilePath); //����� ���ϴ��� �߶󳾰ž�
			//pImagePath->iCount = DirFileCount(szFilePath); //�̰�� ���� ���ϼ��� ���ž�

			//pImagePath->wstrTextureName = PathFindFileName(szFilePath); //�����̸�


			TCHAR szExtension[MAX_STR] = L"";

			TCHAR szEx[MIN_STR] = L".tga";
			lstrcpy(szExtension, PathFindExtension(szFilePath));

			if (!lstrcmp(szEx, szExtension))
			{
				// D:\�ں���\103A\4������-����\DefaultWindow\Texture\Stage\Terrain\Tile\Tile0.png
				//C:\Users\ojon4\OneDrive\���� ȭ��\FrameWork\Texture\Mesh\DynamicMesh\Player\Player.X

				PathRemoveFileSpec(szFilePath); //����� ���ϴ��� �߶󳾰ž�

				pImagePath->iCount = DirFileCount(szFilePath); //�̰�� ���� ���ϼ��� ���ž�

				//pImagePath->wstrFileName = find.GetFileTitle().GetString(); //�ؽ��� �̸�

				
				pImagePath->wstrTextureName = PathFindFileName(szFilePath); //�����̸�

				//PathRemoveFileSpec(szFilePath);
				//pImagePath->wstrMeshType = PathFindFileName(szFilePath); //���� �����̸�

				_rlstImagePath.emplace_back(pImagePath);
				bContinue = FALSE;	
			}
			else
				Engine::Safe_Delete(pImagePath);  ////�̰Ų��Ű澲�� -> else ���ǿ� �ɸ��°͵� �����Ҵ��������־���

		}
	}
}

int CFileInfo::DirFileCount(const wstring & wstrFilePath)
{
	CFileFind find;
	wstring wstrPath = wstrFilePath + L"\\*.*";
	BOOL bContinue = find.FindFile(wstrPath.c_str());
	int iFileCount = 0;
	while (bContinue)
	{
		bContinue = find.FindNextFile();

		// .��Ŀ�� �ý��������̶�� �ǳʶٰ� ���� �־��� �����ϰ�츸 ������ ī��Ʈ �ؾ��Ѵ�. 
		if (find.IsDots() || find.IsSystem())
			continue;

		++iFileCount;
	}
	return iFileCount;
}

