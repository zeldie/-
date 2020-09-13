// PathFindTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFindTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"

// CPathFindTool 대화 상자입니다.

CPathFindTool*	g_pPathFindTool;

IMPLEMENT_DYNAMIC(CPathFindTool, CDialog)

CPathFindTool::CPathFindTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFINDTOOL, pParent)
{

}

CPathFindTool::~CPathFindTool()
{
	for_each(m_listImagePath.begin(), m_listImagePath.end(), Engine::Safe_Delete<IMAGEPATH*>);
	m_listImagePath.clear();
}

void CPathFindTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[RADIO_MESH_DYNAMIC]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[RADIO_MESH_LOBBY]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[RADIO_MESH_CARTEL]);
	DDX_Control(pDX, IDC_RADIO4, m_Radio[RADIO_MESH_BOSS]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio[RADIO_MESH_RUN]);
	DDX_Control(pDX, IDC_RADIO6, m_Radio[RADIO_MESH_EFFECT_STATIC]);
	DDX_Control(pDX, IDC_RADIO7, m_Radio[RADIO_MESH_EFFECT_DYNAMIC]);
	DDX_Control(pDX, IDC_RADIO8, m_Radio[RADIO_TEXTURE]);
}


BEGIN_MESSAGE_MAP(CPathFindTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFindTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathFindTool::OnBnClickedLoad)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathFind 메시지 처리기입니다.


void CPathFindTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_Radio[RADIO_MESH_DYNAMIC].GetCheck())
	{
		wofstream fOut;
		fOut.open("../../Data/Mesh/Mesh_Dynamic.txt"); //~통로개방
		if (!fOut.fail())
		{
			for (auto& pImagePath : m_listImagePath)
			{
				if (!lstrcmp(L"All", pImagePath->wstrStageType.c_str()))
					fOut << pImagePath->wstrResourceType << L"|" << pImagePath->wstrMeshType << L"|" << pImagePath->wstrTextureName << L"|" << pImagePath->wstrPath << L"|" << pImagePath->wstrFileName << endl;
			}
		}
		fOut.close();

		WinExec("notepad.exe ../../Data/Mesh/Mesh_Dynamic.txt", SW_SHOW); //~메모장열어보는중

	}
	else if (m_Radio[RADIO_MESH_LOBBY].GetCheck())
	{
		wofstream fOut;
		fOut.open("../../Data/Mesh/Mesh_Lobby.txt"); //~통로개방
		if (!fOut.fail())
		{
			for (auto& pImagePath : m_listImagePath)
			{
				if (!lstrcmp(L"Lobby", pImagePath->wstrStageType.c_str()))
					fOut << pImagePath->wstrResourceType << L"|" << pImagePath->wstrMeshType << L"|" << pImagePath->wstrTextureName << L"|" << pImagePath->wstrPath << L"|" << pImagePath->wstrFileName << endl;
			}
		}
		fOut.close();

		WinExec("notepad.exe ../../Data/Mesh/Mesh_Lobby.txt", SW_SHOW); //~메모장열어보는중

	}
	else if (m_Radio[RADIO_MESH_CARTEL].GetCheck())
	{
		wofstream fOut;
		fOut.open("../../Data/Mesh/Mesh_Cartel.txt");
		if (!fOut.fail())
		{
			for (auto& pImagePath : m_listImagePath)
			{
				if (!lstrcmp(L"Cartel", pImagePath->wstrStageType.c_str()))
					fOut << pImagePath->wstrResourceType << L"|" << pImagePath->wstrMeshType << L"|" << pImagePath->wstrTextureName << L"|" << pImagePath->wstrPath << L"|" << pImagePath->wstrFileName << endl;
			}
		}
		fOut.close();

		WinExec("notepad.exe ../../Data/Mesh/Mesh_Cartel.txt", SW_SHOW);

	}
	else if (m_Radio[RADIO_MESH_BOSS].GetCheck())
	{
		wofstream fOut;
		fOut.open("../../Data/Mesh/Mesh_Boss.txt");
		if (!fOut.fail())
		{
			for (auto& pImagePath : m_listImagePath)
			{
				if (!lstrcmp(L"Boss", pImagePath->wstrStageType.c_str()))
					fOut << pImagePath->wstrResourceType << L"|" << pImagePath->wstrMeshType << L"|" << pImagePath->wstrTextureName << L"|" << pImagePath->wstrPath << L"|" << pImagePath->wstrFileName << endl;
			}
		}
		fOut.close();

		WinExec("notepad.exe ../../Data/Mesh/Mesh_Boss.txt", SW_SHOW);

	}
	else if (m_Radio[RADIO_MESH_RUN].GetCheck())
	{
		wofstream fOut;
		fOut.open("../../Data/Mesh/Mesh_Run.txt");
		if (!fOut.fail())
		{
			for (auto& pImagePath : m_listImagePath)
			{
				if (!lstrcmp(L"Run", pImagePath->wstrStageType.c_str()))
					fOut << pImagePath->wstrResourceType << L"|" << pImagePath->wstrMeshType << L"|" << pImagePath->wstrTextureName << L"|" << pImagePath->wstrPath << L"|" << pImagePath->wstrFileName << endl;
			}
		}
		fOut.close();

		WinExec("notepad.exe ../../Data/Mesh/Mesh_Run.txt", SW_SHOW);

	}
	else if (m_Radio[RADIO_MESH_EFFECT_STATIC].GetCheck())
	{
		wofstream fOut;
		fOut.open("../../Data/Mesh/Mesh_Effect_Static.txt");
		if (!fOut.fail())
		{
			for (auto& pImagePath : m_listImagePath)
			{
				if (!lstrcmp(L"StaticEffect", pImagePath->wstrStageType.c_str()))
					fOut << pImagePath->wstrResourceType << L"|" << pImagePath->wstrMeshType << L"|" << pImagePath->wstrTextureName << L"|" << pImagePath->wstrPath << L"|" << pImagePath->wstrFileName << endl;
			}
		}
		fOut.close();

		WinExec("notepad.exe ../../Data/Mesh/Mesh_Effect_Static.txt", SW_SHOW);

	}
	else if (m_Radio[RADIO_MESH_EFFECT_DYNAMIC].GetCheck())
	{
		wofstream fOut;
		fOut.open("../../Data/Mesh/Mesh_Effect_Dynamic.txt");
		if (!fOut.fail())
		{
			for (auto& pImagePath : m_listImagePath)
			{
				if (!lstrcmp(L"DynamicEffect", pImagePath->wstrStageType.c_str()))
					fOut << pImagePath->wstrResourceType << L"|" << pImagePath->wstrMeshType << L"|" << pImagePath->wstrTextureName << L"|" << pImagePath->wstrPath << L"|" << pImagePath->wstrFileName << endl;
			}
		}
		fOut.close();

		WinExec("notepad.exe ../../Data/Mesh/Mesh_Effect_Dynamic.txt", SW_SHOW);

	}
	else if (m_Radio[RADIO_TEXTURE].GetCheck())
	{
		wofstream fOut;
		fOut.open("../../Data/Texture/Texture.txt");
		if (!fOut.fail())
		{
			for (auto& pImagePath : m_listImagePath)
			{
				fOut << pImagePath->wstrResourceType << L"|" << pImagePath->wstrFileName << L"|" << pImagePath->wstrPath << L"|" << pImagePath->iCount << endl;
			}
		}
		fOut.close();

		WinExec("notepad.exe ../../Data/Texture/Texture.txt", SW_SHOW);

	}
	else
	{
		return;
	}

	HorizontalScroll();
}


void CPathFindTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_Radio[RADIO_MESH_DYNAMIC].GetCheck())
	{
		wifstream fIn;
		fIn.open("../../Data/Mesh/Mesh_Dynamic.txt");
		TCHAR szResourceType[MAX_STR] = L"";
		TCHAR szMeshType[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";
		TCHAR szFileName[MAX_STR] = L"";
		wstring wstrCombine = L""; //읽어온문자열들을 다 합쳐서 여기에 저장해주꾸야

		m_ListBox.ResetContent(); //원래있는문자열지워주고
		while (true)
		{
			fIn.getline(szResourceType, MAX_STR, '|');
			fIn.getline(szMeshType, MAX_STR, '|');
			fIn.getline(szTextureName, MAX_STR, '|');
			fIn.getline(szPath, MAX_STR, '|');
			fIn.getline(szFileName, MAX_STR);

			if (fIn.eof())
				break;

			wstrCombine = wstring(szResourceType) + L"|" + szMeshType + L"|" + szTextureName + L"|" + szPath + L"|" + szFileName;
			m_ListBox.AddString(wstrCombine.c_str());
		}
		fIn.close();

	}
	else if (m_Radio[RADIO_MESH_LOBBY].GetCheck())
	{
		wifstream fIn;
		fIn.open("../../Data/Mesh/Mesh_Lobby.txt");
		TCHAR szResourceType[MAX_STR] = L"";
		TCHAR szMeshType[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";
		TCHAR szFileName[MAX_STR] = L"";
		wstring wstrCombine = L""; //읽어온문자열들을 다 합쳐서 여기에 저장해주꾸야

		m_ListBox.ResetContent(); //원래있는문자열지워주고
		while (true)
		{
			fIn.getline(szResourceType, MAX_STR, '|');
			fIn.getline(szMeshType, MAX_STR, '|');
			fIn.getline(szTextureName, MAX_STR, '|');
			fIn.getline(szPath, MAX_STR, '|');
			fIn.getline(szFileName, MAX_STR);

			if (fIn.eof())
				break;

			wstrCombine = wstring(szResourceType) + L"|" + szMeshType + L"|" + szTextureName + L"|" + szPath + L"|" + szFileName;
			m_ListBox.AddString(wstrCombine.c_str());
		}
		fIn.close();

	}
	else if (m_Radio[RADIO_MESH_CARTEL].GetCheck())
	{
		wifstream fIn;
		fIn.open("../../Data/Mesh/Mesh_Cartel.txt");
		TCHAR szResourceType[MAX_STR] = L"";
		TCHAR szMeshType[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";
		TCHAR szFileName[MAX_STR] = L"";
		wstring wstrCombine = L""; 

		m_ListBox.ResetContent(); 
		while (true)
		{
			fIn.getline(szResourceType, MAX_STR, '|');
			fIn.getline(szMeshType, MAX_STR, '|');
			fIn.getline(szTextureName, MAX_STR, '|');
			fIn.getline(szPath, MAX_STR, '|');
			fIn.getline(szFileName, MAX_STR);

			if (fIn.eof())
				break;

			wstrCombine = wstring(szResourceType) + L"|" + szMeshType + L"|" + szTextureName + L"|" + szPath + L"|" + szFileName;
			m_ListBox.AddString(wstrCombine.c_str());
		}
		fIn.close();

	}
	else if (m_Radio[RADIO_MESH_BOSS].GetCheck())
	{
		wifstream fIn;
		fIn.open("../../Data/Mesh/Mesh_Boss.txt");
		TCHAR szResourceType[MAX_STR] = L"";
		TCHAR szMeshType[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";
		TCHAR szFileName[MAX_STR] = L"";
		wstring wstrCombine = L""; 

		m_ListBox.ResetContent(); 
		while (true)
		{
			fIn.getline(szResourceType, MAX_STR, '|');
			fIn.getline(szMeshType, MAX_STR, '|');
			fIn.getline(szTextureName, MAX_STR, '|');
			fIn.getline(szPath, MAX_STR, '|');
			fIn.getline(szFileName, MAX_STR);

			if (fIn.eof())
				break;

			wstrCombine = wstring(szResourceType) + L"|" + szMeshType + L"|" + szTextureName + L"|" + szPath + L"|" + szFileName;
			m_ListBox.AddString(wstrCombine.c_str());
		}
		fIn.close();

	}
	else if (m_Radio[RADIO_MESH_RUN].GetCheck())
	{
		wifstream fIn;
		fIn.open("../../Data/Mesh/Mesh_Run.txt");
		TCHAR szResourceType[MAX_STR] = L"";
		TCHAR szMeshType[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";
		TCHAR szFileName[MAX_STR] = L"";
		wstring wstrCombine = L"";

		m_ListBox.ResetContent();
		while (true)
		{
			fIn.getline(szResourceType, MAX_STR, '|');
			fIn.getline(szMeshType, MAX_STR, '|');
			fIn.getline(szTextureName, MAX_STR, '|');
			fIn.getline(szPath, MAX_STR, '|');
			fIn.getline(szFileName, MAX_STR);

			if (fIn.eof())
				break;

			wstrCombine = wstring(szResourceType) + L"|" + szMeshType + L"|" + szTextureName + L"|" + szPath + L"|" + szFileName;
			m_ListBox.AddString(wstrCombine.c_str());
		}
		fIn.close();

	}
	else if (m_Radio[RADIO_MESH_EFFECT_STATIC].GetCheck())
	{
		wifstream fIn;
		fIn.open("../../Data/Mesh/Mesh_Effect_Static.txt");
		TCHAR szResourceType[MAX_STR] = L"";
		TCHAR szMeshType[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";
		TCHAR szFileName[MAX_STR] = L"";
		wstring wstrCombine = L"";

		m_ListBox.ResetContent();
		while (true)
		{
			fIn.getline(szResourceType, MAX_STR, '|');
			fIn.getline(szMeshType, MAX_STR, '|');
			fIn.getline(szTextureName, MAX_STR, '|');
			fIn.getline(szPath, MAX_STR, '|');
			fIn.getline(szFileName, MAX_STR);

			if (fIn.eof())
				break;

			wstrCombine = wstring(szResourceType) + L"|" + szMeshType + L"|" + szTextureName + L"|" + szPath + L"|" + szFileName;
			m_ListBox.AddString(wstrCombine.c_str());
		}
		fIn.close();

	}
	else if (m_Radio[RADIO_MESH_EFFECT_DYNAMIC].GetCheck())
	{
		wifstream fIn;
		fIn.open("../../Data/Mesh/Mesh_Effect_Dynamic.txt");
		TCHAR szResourceType[MAX_STR] = L"";
		TCHAR szMeshType[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";
		TCHAR szFileName[MAX_STR] = L"";
		wstring wstrCombine = L"";

		m_ListBox.ResetContent();
		while (true)
		{
			fIn.getline(szResourceType, MAX_STR, '|');
			fIn.getline(szMeshType, MAX_STR, '|');
			fIn.getline(szTextureName, MAX_STR, '|');
			fIn.getline(szPath, MAX_STR, '|');
			fIn.getline(szFileName, MAX_STR);

			if (fIn.eof())
				break;

			wstrCombine = wstring(szResourceType) + L"|" + szMeshType + L"|" + szTextureName + L"|" + szPath + L"|" + szFileName;
			m_ListBox.AddString(wstrCombine.c_str());
		}
		fIn.close();

	}
	else if (m_Radio[RADIO_TEXTURE].GetCheck())
	{
		wifstream fIn;
		fIn.open("../../Data/Texture/Texture.txt");
		TCHAR szResourceType[MAX_STR] = L"";
		TCHAR szFileName[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";
		TCHAR szCount[MAX_STR] = L"";
		wstring wstrCombine = L""; 

		m_ListBox.ResetContent(); 
		while (true)
		{
			fIn.getline(szResourceType, MAX_STR, '|');
			fIn.getline(szFileName, MAX_STR, '|');
			fIn.getline(szPath, MAX_STR, '|');
			fIn.getline(szCount, MAX_STR);

			if (fIn.eof())
				break;

			wstrCombine = wstring(szResourceType) + L"|" + szFileName + L"|" + szPath + L"|" + szCount;
			m_ListBox.AddString(wstrCombine.c_str());
		}
		fIn.close();
	}
	else
	{
		return;
	}

	HorizontalScroll();
	UpdateData(FALSE);
}

void CPathFindTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//여기선 파일을 끌어다 놓고 그 끌어놓은 것들 갯수를 셀거야
	TCHAR szFilePath[MAX_STR] = L"";
	int iFileCount = DragQueryFile(hDropInfo, -1, szFilePath, MAX_STR);

	if (m_Radio[RADIO_MESH_DYNAMIC].GetCheck()
		|| m_Radio[RADIO_MESH_LOBBY].GetCheck()
		|| m_Radio[RADIO_MESH_CARTEL].GetCheck() 
		|| m_Radio[RADIO_MESH_BOSS].GetCheck()
		|| m_Radio[RADIO_MESH_RUN].GetCheck()
		|| m_Radio[RADIO_MESH_EFFECT_STATIC].GetCheck()
		|| m_Radio[RADIO_MESH_EFFECT_DYNAMIC].GetCheck()) 
	{
		for (int i = 0; i < iFileCount; ++i)
		{
			DragQueryFile(hDropInfo, i, szFilePath, MAX_STR); //해당인덱스의 파일명을 불러와
															  //그리고 이제추출을할거야 밑에함수를통해서
			CFileInfo::DirInfoExtractionForMesh(szFilePath, m_listImagePath);
		}

		m_ListBox.ResetContent(); //이전에있던리스트박스내용을지워줘

		wstring wstrCombine = L"";

		for (auto& pImagePath : m_listImagePath)
		{
			wstrCombine = pImagePath->wstrResourceType + L"|" + pImagePath->wstrMeshType + L"|" + pImagePath->wstrTextureName + L"|" + pImagePath->wstrPath + L"|" + pImagePath->wstrFileName;

			m_ListBox.AddString(wstrCombine.c_str());
		}

	}
	else if (m_Radio[RADIO_TEXTURE].GetCheck())
	{
		TCHAR szFilePath[MAX_STR] = L"";
		int iFileCount = DragQueryFile(hDropInfo, -1, szFilePath, MAX_STR);

		for (int i = 0; i < iFileCount; ++i)
		{
			DragQueryFile(hDropInfo, i, szFilePath, MAX_STR); //해당인덱스의 파일명을 불러와
															  //그리고 이제추출을할거야 밑에함수를통해서
			CFileInfo::DirInfoExtractionForTexture(szFilePath, m_listImagePath);
		}

		m_ListBox.ResetContent(); //이전에있던리스트박스내용을지워줘

		wstring wstrCombine = L"";
		TCHAR szBuf[MIN_STR] = L"";

		for (auto& pImagePath : m_listImagePath)
		{
			_itow_s(pImagePath->iCount, szBuf, 10); //숫자->문자열
			wstrCombine = pImagePath->wstrResourceType + L"|" + pImagePath->wstrFileName + L"|" + pImagePath->wstrPath + L"|" + szBuf;

			m_ListBox.AddString(wstrCombine.c_str());
		}

	}
	else
	{
		return;
	}

	HorizontalScroll();

	CDialog::OnDropFiles(hDropInfo);
}

void CPathFindTool::HorizontalScroll()
{
	CString strName;
	CSize size;
	int iCX = 0;

	// 리스트 박스도 DC로 출력하고 있다. 그러므로 디씨얻어옴. 
	CDC* pDC = m_ListBox.GetDC();

	// 문자열 갯수.,
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		// 인덱스번째 문자열을 얻어오는 함수. 
		m_ListBox.GetText(i, strName);

		// 문자열의 길이를 픽셀단위로 변환. 
		size = pDC->GetTextExtent(strName);

		if (size.cx > iCX)
			iCX = size.cx;
	}
	ReleaseDC(pDC);

	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX);

}
