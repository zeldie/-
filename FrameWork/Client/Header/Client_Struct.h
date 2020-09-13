#ifndef Struct_h__
#define Struct_h__

typedef struct tagBaseInfo
{
	tagBaseInfo()
		:eObjectID(OBJID::OBJECT_END)
		,eEffectID(EFFECTID::EFFECT_END)
		,iAtt(0)
		,iMaxHp(0)
		,iHp(0)
		,iPrevHp(0)
		,iSuperAmmor(0)
		,iMaxSuperAmmor(0)
		,fEffectHitTime(0.f)
		,fHalfAngle(0.f)
	{
	}
	OBJID		eObjectID;		// ������Ʈ�� Ÿ��
	EFFECTID	eEffectID;		// ����Ʈ ID
	COLLISIONID	eCollisionID;	// �浹���
	_int		iAtt;			// ���ݷ�
	_int		iMaxHp;			// �ִ� ü��
	_int		iHp;			// ���� ü��
	_int		iPrevHp;		// ���� ü��
	_int		iSuperAmmor;	// ���۾Ƹ�
	_int		iMaxSuperAmmor;		// �ִ뽴�۾Ƹ�
	_int		iCritical;			// ũ��Ƽ��
	_int		iCheatATK;			// Cheat
	_float		fEffectHitTime;		// ����Ʈ �ǰ��� ���� �ð�.
	_float		fHalfAngle;			// ��ä�� ����Ʈ ���� ����
	_vec3		vFanLook;			// ��ä�� ����Ʈ �躤��
	_vec3		vObjectPos;			// Object Position
}BASE_INFO;

typedef struct tagEffectInfo
{
	tagEffectInfo()
		:wstrType(L"")
		, wstrName(L"")
		, fSpeed(0.f)
		, fDeadTime(0.f)
		, iPass(0)
		, iAlpha(0)
		, vScale(1.f, 1.f, 1.f)
	{
		ZeroMemory(&vPos, sizeof(_vec3));
		ZeroMemory(&vRot, sizeof(_vec3));
		ZeroMemory(&vMovePos, sizeof(_vec3));
		ZeroMemory(&vMoveRot, sizeof(_vec3));
		ZeroMemory(&vMoveScale, sizeof(_vec3));
	}
	wstring		wstrType;	//��->Ŭ��� �����ö� ���º���, ���̳������� ����ƽ���� �ؽ�������
	wstring		wstrName;	//X�����̸�
	_vec3		vPos;
	_vec3		vRot;
	_vec3		vScale;
	_vec3		vMovePos;
	_vec3		vMoveRot;
	_vec3		vMoveScale;
	_float		fSpeed;
	_float		fDeadTime;
	_uint		iPass;
	_uint		iAlpha;
	vector <Engine::EFFECT_UV_INFO> vectorUVInfo;
}EFFECT_INFO;

typedef struct tagFlagResult //-->���� UIMgr���� ��
{
	_tchar szName[MIN_STR];
	_uint iMainWeapon;
	_uint iSubWeapon;
	_uint iEarnedPoints;
	_uint iKill;
	_uint iDamage;
	_uint iBreak;

}FLAG_RESULT;

typedef struct tagTCAHRFlagResult //-->���� UIMgr���� ��
{
	_tchar szName[MIN_STR];
	_tchar szEarnedPoints[MIN_STR];
	_tchar szKill[MIN_STR];
	_tchar szDamage[MIN_STR];
	_tchar szBreak[MIN_STR];

}TCHAR_FLAGRESULT;

typedef struct tagFlagDeath
{
	_tchar	szKillerName[MIN_STR];
	TEAM	eKillerTeam;
	_tchar	szDeadName[MIN_STR];
	TEAM	eDeadTeam;

}FLAG_DEATH;

typedef struct tagTexture
{
	tagTexture()
		:fDeadTime(1.f),
		bIsBillboard(true)
	{
		ZeroMemory(&vScale, sizeof(_vec3));
		ZeroMemory(&vAnimationU, sizeof(_vec2));
		ZeroMemory(&vAnimationV, sizeof(_vec2));
	}
	_vec3 vScale;
	_float fDeadTime;
	_vec2 vAnimationU;
	_vec2 vAnimationV;
	_bool bIsBillboard;
}TEXTUREEFFECT_INFO;

typedef struct tagQuest //-->���� QuestMgr���� ��
{
	tagQuest()
		:wstrQuest(L""),
		bClear(false)
	{
	}

	wstring wstrQuest;
	_bool	bClear;

}QUEST_INFO;
#endif // Struct_h__
