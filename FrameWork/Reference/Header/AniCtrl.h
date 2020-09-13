#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl(void);

public:
	//Setter
	void Set_Stop(_bool bBool) { m_bStop = bBool; }

public:
	LPD3DXANIMATIONCONTROLLER	Get_AniCtrl(void) { return m_pAniCtrl; }
	__forceinline _double&		Get_AnimationTrackTime() { return m_dbPeriod; }
	__forceinline _double&		Get_TrackTime();
	_uint						Get_CurrentTrack() { return m_iCurrentTrack; }
	_double						Get_AccTime() { return m_dAccTime; }

	_bool						Is_AnimationSetFinish(const _double dbRevisionTime);
	void						Set_AnimationSet(const _uint& iIndex);
	void						Set_AnimationSet_Change(const _uint& iIndex, _double dTrackTime, _double dAccTime);
	void						Set_TrackTime(_uint iCurrentTrack, _double dTrackTime);
	void						Set_AccTime(_double dAccTime) { m_dAccTime = dAccTime; }
	void						Play_AnimationSet(const _double& dTimeDelta);
	void						Reset_Animation();
public:
	HRESULT						Ready_AniCtrl(void);

private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;
	_uint							m_iCurrentTrack;	
	_uint							m_iNewTrack;
	_double							m_dAccTime;
	_uint							m_iOldAniIdx;

	_double							m_dbPeriod;
	_bool							m_bStop; //»Ò¡§ ->µπ∏Õ¿Ã ¥Ÿ¿Ã≥™πÕ¿Ã∆Â∆Æ ∏ÿ√ﬂ∞‘ «“∂Û∞Ì ∏∏µÎ

public:
	static CAniCtrl*		Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*		Clone (const CAniCtrl& rhs);
	virtual void			Free(void);
};

END
#endif // AniCtrl_h__
