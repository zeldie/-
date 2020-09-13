#pragma once
#ifndef __CRunGameMgr_h__
#define __CRunGameMgr_h__

class CRunGameMgr
{
	DECLARE_SINGLETON(CRunGameMgr)
public:
	enum KEYDOWN { KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_END };
	enum LINEOPTION { LINE_NORMAL, LINE_SPEED, LINE_SLOW, LINE_FALL, LINEOPTION_END };
	typedef struct tagLineInfo
	{
		tagLineInfo()
		{
			ZeroMemory(&vStartPos, sizeof(_vec3));
			ZeroMemory(&vEndPos, sizeof(_vec3));
			ZeroMemory(&vDir, sizeof(_vec3));
			fLength = 0.f;
			eLineOption = LINE_NORMAL;
			dProgressStart = 0.;
			dProgressEnd = 0.;
			eSect = SECT_1;
		}
		_vec3		vStartPos;	// 시작좌표
		_vec3		vEndPos;	// 끝 좌표
		_vec3		vDir;		// 방향
		_float		fLength;	// 벡터의 크기
		LINEOPTION	eLineOption;	// 라인의 옵션, ex) 빨라진다, 느려진다, 바닥밑으로 떨어진다
		_double		dProgressStart;	// 구간의 진행도 시작수치
		_double		dProgressEnd;	// 구간의 진행도 끝수치
		RUN_LINE_SECT	eSect;
	}LINE_INFO;
private:
	CRunGameMgr();
	~CRunGameMgr();

public:
	HRESULT		Set_Device(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		Load_Line(wstring wstrPath, LINETYPE eType, RUN_LINE_SECT eSect);
	HRESULT		Create_Obstacle(_uint iSect);

	_bool		Change_Line(LINETYPE* eLineType, RUNPLAYERMOVESTATE* eState, KEYDOWN eKey, _vec3* pPos, _vec3* vUp, _vec3 * pAngle, _double* dProgress, _double* dLineAccel, const _double & dTimeDelta);	// 바꾸기 전 함수, 고쳐서 쓸거
	_bool		Follow_Line(LINETYPE eLineType, RUNPLAYERMOVESTATE* eState, _vec3* pPos, _vec3* vUp, _double* dProgress, _double* dLineAccel, const _double & dTimeDelta);

	void		NullCheck_ItemList();		// 리스트에 들어있는 객체가 죽었는지 확인해서 죽었으면 해당 포인터 제거하는 함수
	void		NullCheck_ObstacleList();	// 리스트에 들어있는 객체가 죽었는지 확인해서 죽었으면 해당 포인터 제거하는 함수
	void		NullCheck_ObstacleList_Dead();	// 리스트에 들어있는 객체가 죽었는지 확인해서 죽었으면 해당 포인터 제거하는 함수

	void		Clear_List();	// 리스트 클리어하는 함수, 만약 런게임에서 다른씬으로 갔다가 다시돌아올거면 반드시 이 함수 호출해야한다.

	list <Engine::CGameObject*>		Get_ObstacleList() { return m_listObstacle; }
	list <Engine::CGameObject*>		Get_ItemList() { return m_listItem; }
	_float							Get_LineLength(RUN_LINE_SECT eSect, LINETYPE eLineType) { return m_fLineLenth[eSect][eLineType]; }

public:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	vector<LINE_INFO>				m_vecLine[LINE_END];	// 플레이어가 타고 이동할 라인
	//_float						m_fLineLenth[LINE_END];	// 각 라인 벡터 길이의 합
	_float							m_fLineLenth[SECT_END][LINE_END];  // 각 라인 벡터 길이의 합
	list <Engine::CGameObject*>		m_listObstacle;	// 장애물 포인터
	list <Engine::CGameObject*>		m_listItem; // 아이템 포인터

};

#endif // !__CRunGameMgr_h__
