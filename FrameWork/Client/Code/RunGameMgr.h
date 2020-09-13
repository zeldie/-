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
		_vec3		vStartPos;	// ������ǥ
		_vec3		vEndPos;	// �� ��ǥ
		_vec3		vDir;		// ����
		_float		fLength;	// ������ ũ��
		LINEOPTION	eLineOption;	// ������ �ɼ�, ex) ��������, ��������, �ٴڹ����� ��������
		_double		dProgressStart;	// ������ ���൵ ���ۼ�ġ
		_double		dProgressEnd;	// ������ ���൵ ����ġ
		RUN_LINE_SECT	eSect;
	}LINE_INFO;
private:
	CRunGameMgr();
	~CRunGameMgr();

public:
	HRESULT		Set_Device(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		Load_Line(wstring wstrPath, LINETYPE eType, RUN_LINE_SECT eSect);
	HRESULT		Create_Obstacle(_uint iSect);

	_bool		Change_Line(LINETYPE* eLineType, RUNPLAYERMOVESTATE* eState, KEYDOWN eKey, _vec3* pPos, _vec3* vUp, _vec3 * pAngle, _double* dProgress, _double* dLineAccel, const _double & dTimeDelta);	// �ٲٱ� �� �Լ�, ���ļ� ����
	_bool		Follow_Line(LINETYPE eLineType, RUNPLAYERMOVESTATE* eState, _vec3* pPos, _vec3* vUp, _double* dProgress, _double* dLineAccel, const _double & dTimeDelta);

	void		NullCheck_ItemList();		// ����Ʈ�� ����ִ� ��ü�� �׾����� Ȯ���ؼ� �׾����� �ش� ������ �����ϴ� �Լ�
	void		NullCheck_ObstacleList();	// ����Ʈ�� ����ִ� ��ü�� �׾����� Ȯ���ؼ� �׾����� �ش� ������ �����ϴ� �Լ�
	void		NullCheck_ObstacleList_Dead();	// ����Ʈ�� ����ִ� ��ü�� �׾����� Ȯ���ؼ� �׾����� �ش� ������ �����ϴ� �Լ�

	void		Clear_List();	// ����Ʈ Ŭ�����ϴ� �Լ�, ���� �����ӿ��� �ٸ������� ���ٰ� �ٽõ��ƿðŸ� �ݵ�� �� �Լ� ȣ���ؾ��Ѵ�.

	list <Engine::CGameObject*>		Get_ObstacleList() { return m_listObstacle; }
	list <Engine::CGameObject*>		Get_ItemList() { return m_listItem; }
	_float							Get_LineLength(RUN_LINE_SECT eSect, LINETYPE eLineType) { return m_fLineLenth[eSect][eLineType]; }

public:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	vector<LINE_INFO>				m_vecLine[LINE_END];	// �÷��̾ Ÿ�� �̵��� ����
	//_float						m_fLineLenth[LINE_END];	// �� ���� ���� ������ ��
	_float							m_fLineLenth[SECT_END][LINE_END];  // �� ���� ���� ������ ��
	list <Engine::CGameObject*>		m_listObstacle;	// ��ֹ� ������
	list <Engine::CGameObject*>		m_listItem; // ������ ������

};

#endif // !__CRunGameMgr_h__
