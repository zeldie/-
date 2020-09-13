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
	OBJID		eObjectID;		// 오브젝트의 타입
	EFFECTID	eEffectID;		// 이펙트 ID
	COLLISIONID	eCollisionID;	// 충돌방식
	_int		iAtt;			// 공격력
	_int		iMaxHp;			// 최대 체력
	_int		iHp;			// 현재 체력
	_int		iPrevHp;		// 이전 체력
	_int		iSuperAmmor;	// 슈퍼아머
	_int		iMaxSuperAmmor;		// 최대슈퍼아머
	_int		iCritical;			// 크리티컬
	_int		iCheatATK;			// Cheat
	_float		fEffectHitTime;		// 이펙트 피격후 무적 시간.
	_float		fHalfAngle;			// 부채꼴 이펙트 범위 각도
	_vec3		vFanLook;			// 부채꼴 이펙트 룩벡터
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
	wstring		wstrType;	//툴->클라로 가져올때 쓰는변수, 다이나믹인지 스태틱인지 텍스쳐인지
	wstring		wstrName;	//X파일이름
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

typedef struct tagFlagResult //-->희정 UIMgr에서 씀
{
	_tchar szName[MIN_STR];
	_uint iMainWeapon;
	_uint iSubWeapon;
	_uint iEarnedPoints;
	_uint iKill;
	_uint iDamage;
	_uint iBreak;

}FLAG_RESULT;

typedef struct tagTCAHRFlagResult //-->희정 UIMgr에서 씀
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

typedef struct tagQuest //-->희정 QuestMgr에서 씀
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
