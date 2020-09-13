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

	// from에서 TO로 찾아가는 상대경로를 반환해준다 어디에다? szRelativePath
	// 주의사항은 같은 경로에 있을때만! 
	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY, strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);


	return CString(szRelativePath);
}

// 제일처음 들어오는 경로 
//D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain
//D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain\Tile\\*.*
void CFileInfo::DirInfoExtractionForMesh(const wstring & wstrFilePath, list<IMAGEPATH*>& rlstImagePath)
{
	// 파일 경로 및 제어관련 클래스 이건 MFC에서 제공. 
	CFileFind find;
	//D:\\박병건\\103A\\4개월차-떨림\\DefaultWindow\\Texture\\Stage\\Terrain\\*.*
	wstring wstrPath = wstrFilePath + L"\\*.*";
	// FindFile - 주어진 경로에 파일의 유무를 확인하는 함수. 
	// 인자로 *.*이 들어간다면 해당 경로의 하위 모든 파일을 찾는 것으로 해석한다. 
	// 다음 파일에 접근하고 싶다면 FileNextFile이라는 함수로 접근하면 된다. 
	BOOL bContinue = find.FindFile(wstrPath.c_str());

	// 폴더를 얼마나 타고들어가야될지 모르는 상황이라 반복문 수행. 
	while (bContinue)
	{
		// 애는 만약 더이상 접글할 것이 없다면 0을 반환. 
		bContinue = find.FindNextFile();
		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
		{
			// 만약 폴더라면 재귀함수 호출. 
			// D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain\\Tile
			DirInfoExtractionForMesh(find.GetFilePath().GetString(), rlstImagePath);
		}
		else
		{
			//이제 작업을 수행. 내가 원하는 파일까지 나옴. 
			if (find.IsDots())
				continue;
			IMAGEPATH* pImagePath = new IMAGEPATH;

			TCHAR szFilePath[MAX_STR] = L"";
			// D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain\\Tile
			lstrcpy(szFilePath, find.GetFilePath().GetString());

			PathRemoveFileSpec(szFilePath);
			// 파일 카운트 구함. 
			//pImagePath->iCount = DirFileCount(szFilePath); 

			//wstring wstrTextureName = find.GetFileTitle().GetString();
			wstring wstrTextureName = PathFindFileName(szFilePath);

			//wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1);
			pImagePath->wstrFileName = wstrTextureName + L".X";
			pImagePath->wstrTextureName = L"Mesh_" + wstrTextureName;

			// Tile + L"%d.png" = Tile%d.png
			//szFilePath + wstrTextureName; 
			// D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain\\Tile\\Tile%d.png
			// PathCombine경로 이어주는 함수. 이함수는 문자열 사이에 자동으로 \\를 삽입해준다. 
			//PathCombine(szFilePath, szFilePath, wstrTextureName.c_str());

			PathAddBackslash(szFilePath);
			pImagePath->wstrPath = CFileInfo::ConvertRelativePath(szFilePath);
			// D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain\\Tile
			PathRemoveFileSpec(szFilePath);
			PathRemoveFileSpec(szFilePath);
			pImagePath->wstrStageType = PathFindFileName(szFilePath);
		
			PathRemoveFileSpec(szFilePath);
			pImagePath->wstrMeshType = PathFindFileName(szFilePath);

			pImagePath->wstrResourceType = L"Static";
			//// D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain
			//PathRemoveFileSpec(szFilePath);
			//// statekey 상단의 폴더는 ObjectKey값이 될것. 
			//pImagePath->wstrObjKey = PathFindFileName(szFilePath);

			rlstImagePath.emplace_back(pImagePath);
			bContinue = FALSE;

		}

	}


}

void CFileInfo::DirInfoExtractionForTexture(const wstring & wstrFilePath, list<IMAGEPATH*>& rlstImagePath)
{
	// 파일 경로 및 제어관련 클래스 이건 MFC에서 제공. 
	CFileFind find;
	//D:\\박병건\\103A\\4개월차-떨림\\DefaultWindow\\Texture\\Stage\\Terrain\\*.*
	wstring wstrPath = wstrFilePath + L"\\*.*";
	// FindFile - 주어진 경로에 파일의 유무를 확인하는 함수. 
	// 인자로 *.*이 들어간다면 해당 경로의 하위 모든 파일을 찾는 것으로 해석한다. 
	// 다음 파일에 접근하고 싶다면 FileNextFile이라는 함수로 접근하면 된다. 
	BOOL bContinue = find.FindFile(wstrPath.c_str());

	// 폴더를 얼마나 타고들어가야될지 모르는 상황이라 반복문 수행. 
	while (bContinue)
	{
		// 애는 만약 더이상 접글할 것이 없다면 0을 반환. 
		bContinue = find.FindNextFile();
		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
		{
			DirInfoExtractionForTexture(find.GetFilePath().GetString(), rlstImagePath);
		}
		else
		{
			//이제 작업을 수행. 내가 원하는 파일까지 나옴. 
			if (find.IsDots())
				continue;
			IMAGEPATH* pImagePath = new IMAGEPATH;

			TCHAR szFilePath[MAX_STR] = L""; //자를거고
			TCHAR szOriginFilePath[MAX_STR] = L""; //자르기전 원본
			wstring wstrFileExtension = L"";

			lstrcpy(szFilePath, find.GetFilePath().GetString());
			lstrcpy(szOriginFilePath, find.GetFilePath().GetString());

			wstrFileExtension = PathFindExtension(szFilePath);

			PathRemoveFileSpec(szFilePath);			
			wstring wstrFolderName = PathFindFileName(szFilePath);

			if (lstrcmp(L"EffectSub", wstrFolderName.c_str())) //EffectSub 제외하고 모든 폴더의 파일
			{
				PathRemoveFileSpec(szOriginFilePath);
			
				pImagePath->iCount = DirFileCount(szOriginFilePath);

				wstring wstrTextureName = find.GetFileTitle().GetString();
				wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1) + L"%d" + wstrFileExtension;

				PathCombine(szOriginFilePath, szOriginFilePath, wstrTextureName.c_str());


				pImagePath->wstrPath = CFileInfo::ConvertRelativePath(szOriginFilePath);
				PathRemoveFileSpec(szOriginFilePath);

				// 파일의 이름이 결국 statekey가 됨. 
				pImagePath->wstrFileName = PathFindFileName(szOriginFilePath);

				pImagePath->wstrResourceType = L"Static";

				rlstImagePath.emplace_back(pImagePath);
				bContinue = FALSE;
			}
			else //EffectSub 폴더
			{
				Engine::Safe_Delete(pImagePath);
			}
		}

	}
}

void CFileInfo::DirInfoExtractionForMeshTool(const wstring & _wstrFilePath, list<IMAGEPATH*>& _rlstImagePath)
{
	//MeshTool 준비
	CFileFind find; //파일관련된클래스 - mfc제공
	wstring wstrPath = _wstrFilePath + L"\\*.*";

	BOOL bContinue = find.FindFile(wstrPath.c_str()); //파일없으면 0반환

	while (bContinue)
	{
		bContinue = find.FindNextFile(); //다음파일없으면 0반환
		if (find.IsDots())
			continue;
		else if (find.IsDirectory()) //찾은게 폴더인경우
		{
			//재귀함수
			DirInfoExtractionForMeshTool(find.GetFilePath().GetString(), _rlstImagePath);
		}
		else //찾은게 파일이면
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
				// D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain\Tile\Tile0.png
				//C:\Users\ojon4\OneDrive\바탕 화면\FrameWork\Texture\Mesh\DynamicMesh\Player\Player.X

				PathRemoveFileSpec(szFilePath); //경로의 최하단을 잘라낼거야

				pImagePath->iCount = DirFileCount(szFilePath); //이경로 안의 파일수를 셀거야

															   //파일이름
				pImagePath->wstrFileName = find.GetFileTitle().GetString(); //파일이름가져옴
																			//wstrTexturName = wstrTexturName.substr(0, wstrTexturName.length() - 1) + L"%d.png";
																			//-.substr : 시작부터(0) 끝까지(길이-1) 문자열을 얻어오는 함수

																			//PathCombine(szFilePath, szFilePath, wstrTex);
																			// PathCombine경로 이어주는 함수. 이함수는 문자열 사이에 자동으로 \\를 삽입해준다. 
																			// D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain\\Tile\\Tile%d.png ->절대경로

																			//pImagePath->wstrPath = CFileInfo::ConvertRelativePath(szFilePath);

																			//PathRemoveFileSpec(szFilePath);
																			//pImagePath->wstrChildFolder = PathFindFileName(szFilePath);

																			//텍스처이름
																			//pImagePath->wstrTextureName = L"Mesh_" + pImagePath->wstrFileName;//

				PathRemoveFileSpec(szFilePath);
				pImagePath->wstrTextureName = PathFindFileName(szFilePath);//

				PathRemoveFileSpec(szFilePath);//
				pImagePath->wstrMeshType = PathFindFileName(szFilePath);

				_rlstImagePath.emplace_back(pImagePath);
				bContinue = FALSE;
			}
			else
 				Engine::Safe_Delete(pImagePath);  ////이거꼭신경쓰기 -> else 조건에 걸리는것도 동적할당해제해주야함

		}
	}
}

void CFileInfo::DirInfoExtractionForEffectTool(const wstring & _wstrFilePath, list<IMAGEPATH*>& _rlstImagePath)
{
	//MeshTool 준비
	CFileFind find; //파일관련된클래스 - mfc제공
	wstring wstrPath = _wstrFilePath + L"\\*.*";

	BOOL bContinue = find.FindFile(wstrPath.c_str()); //파일없으면 0반환

	while (bContinue)
	{
		bContinue = find.FindNextFile(); //다음파일없으면 0반환
		if (find.IsDots())
			continue;
		else if (find.IsDirectory()) //찾은게 폴더인경우
		{
			//재귀함수
			DirInfoExtractionForEffectTool(find.GetFilePath().GetString(), _rlstImagePath);
		}
		else //찾은게 파일이면
		{
			if (find.IsDots())
				continue;
			IMAGEPATH* pImagePath = new IMAGEPATH;

			TCHAR szFilePath[MAX_STR] = L"";
			lstrcpy(szFilePath, find.GetFilePath().GetString());

			//PathRemoveFileSpec(szFilePath); //경로의 최하단을 잘라낼거야
			//pImagePath->iCount = DirFileCount(szFilePath); //이경로 안의 파일수를 셀거야

			//pImagePath->wstrTextureName = PathFindFileName(szFilePath); //폴더이름


			TCHAR szExtension[MAX_STR] = L"";

			TCHAR szEx[MIN_STR] = L".tga";
			lstrcpy(szExtension, PathFindExtension(szFilePath));

			if (!lstrcmp(szEx, szExtension))
			{
				// D:\박병건\103A\4개월차-떨림\DefaultWindow\Texture\Stage\Terrain\Tile\Tile0.png
				//C:\Users\ojon4\OneDrive\바탕 화면\FrameWork\Texture\Mesh\DynamicMesh\Player\Player.X

				PathRemoveFileSpec(szFilePath); //경로의 최하단을 잘라낼거야

				pImagePath->iCount = DirFileCount(szFilePath); //이경로 안의 파일수를 셀거야

				//pImagePath->wstrFileName = find.GetFileTitle().GetString(); //텍스쳐 이름

				
				pImagePath->wstrTextureName = PathFindFileName(szFilePath); //폴더이름

				//PathRemoveFileSpec(szFilePath);
				//pImagePath->wstrMeshType = PathFindFileName(szFilePath); //상위 폴더이름

				_rlstImagePath.emplace_back(pImagePath);
				bContinue = FALSE;	
			}
			else
				Engine::Safe_Delete(pImagePath);  ////이거꼭신경쓰기 -> else 조건에 걸리는것도 동적할당해제해주야함

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

		// .마커와 시스템파일이라면 건너뛰고 내가 넣어준 파일일경우만 갯수를 카운트 해야한다. 
		if (find.IsDots() || find.IsSystem())
			continue;

		++iFileCount;
	}
	return iFileCount;
}

